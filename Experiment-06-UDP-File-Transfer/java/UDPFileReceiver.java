import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class UDPFileReceiver {
    public static final int PORT = 9100;
    public static final int CHUNK_SIZE = 1024;
    public static final String END_MARKER = "<<EOF>>";

    public static void main(String[] args) {
        String outputFilename = args.length > 0 ? args[0] : "received_output";

        System.out.println("========================================================");
        System.out.println("  UDP FILE RECEIVER LISTENING ON PORT " + PORT + " (JAVA)");
        System.out.println("========================================================");
        System.out.println("Output will be saved to: \"" + outputFilename + "\"");
        System.out.println("Waiting for incoming datagrams...");

        try (DatagramSocket socket = new DatagramSocket(PORT);
             FileOutputStream fileOut = new FileOutputStream(outputFilename)) {

            byte[] buffer = new byte[CHUNK_SIZE];
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            long totalBytes = 0;
            int chunkCount = 0;

            while (true) {
                socket.receive(packet);
                int length = packet.getLength();

                // Check for EOF marker
                String markerCheck = new String(packet.getData(), 0, length, StandardCharsets.UTF_8);
                if (markerCheck.equals(END_MARKER)) {
                    System.out.println("\n[EOF Marker Received] File transfer complete.");
                    break;
                }

                fileOut.write(packet.getData(), 0, length);
                totalBytes += length;
                chunkCount++;

                if (chunkCount % 50 == 0) {
                    System.out.print(".");
                }
            }

            System.out.println("Received from " + packet.getAddress() + ":" + packet.getPort());
            System.out.println("Total Chunks Received: " + chunkCount);
            System.out.println("Total Bytes Saved    : " + totalBytes + " bytes");

        } catch (IOException e) {
            System.err.println("Receiver error: " + e.getMessage());
        }
    }
}
