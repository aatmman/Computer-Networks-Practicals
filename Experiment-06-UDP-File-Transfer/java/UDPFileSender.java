import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class UDPFileSender {
    public static final int PORT = 9100;
    public static final int CHUNK_SIZE = 1024;
    public static final String END_MARKER = "<<EOF>>";

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java UDPFileSender <filename> [receiver_ip]");
            return;
        }

        String fileName = args[0];
        String receiverIP = args.length > 1 ? args[1] : "127.0.0.1";

        File file = new File(fileName);
        if (!file.exists()) {
            try (FileWriter fw = new FileWriter(file)) {
                fw.write("#!/usr/bin/env python3\n# Transferred over Java UDP DatagramSocket\nprint('UDP transfer successful!')\n");
            } catch (IOException e) {
                System.err.println("Could not create sample file: " + e.getMessage());
            }
            System.out.println("Created default sample file: \"" + fileName + "\"");
        }

        System.out.println("========================================================");
        System.out.println("  UDP FILE SENDER -> " + receiverIP + ":" + PORT + " (JAVA)");
        System.out.println("========================================================");
        System.out.println("Sending file: \"" + fileName + "\" (" + file.length() + " bytes)...");

        try (DatagramSocket socket = new DatagramSocket();
             FileInputStream fileIn = new FileInputStream(file)) {

            InetAddress receiverAddress = InetAddress.getByName(receiverIP);
            byte[] buffer = new byte[CHUNK_SIZE];
            int bytesRead;
            long totalBytes = 0;
            int chunkCount = 0;

            while ((bytesRead = fileIn.read(buffer)) != -1) {
                DatagramPacket packet = new DatagramPacket(buffer, bytesRead, receiverAddress, PORT);
                socket.send(packet);
                totalBytes += bytesRead;
                chunkCount++;

                // Pacing delay to prevent buffer overruns on fast loops
                try {
                    Thread.sleep(1);
                } catch (InterruptedException ignored) {}

                if (chunkCount % 50 == 0) {
                    System.out.print(".");
                }
            }

            // Send EOF marker (repeat 3 times with brief pauses)
            byte[] eofBytes = END_MARKER.getBytes(StandardCharsets.UTF_8);
            for (int k = 0; k < 3; k++) {
                DatagramPacket eofPacket = new DatagramPacket(eofBytes, eofBytes.length, receiverAddress, PORT);
                socket.send(eofPacket);
                try { Thread.sleep(5); } catch (InterruptedException ignored) {}
            }

            System.out.println("\nFile transfer finished successfully!");
            System.out.println("Total Chunks Sent : " + chunkCount);
            System.out.println("Total Bytes Sent  : " + totalBytes + " bytes");

        } catch (IOException e) {
            System.err.println("Sender error: " + e.getMessage());
        }
    }
}
