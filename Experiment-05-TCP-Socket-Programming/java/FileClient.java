import java.io.*;
import java.net.*;

public class FileClient {
    public static final int PORT = 9002;
    public static final int BUFFER_SIZE = 4096;

    public static void main(String[] args) {
        String fileName = args.length > 0 ? args[0] : "send_file.txt";
        String serverIP = args.length > 1 ? args[1] : "127.0.0.1";

        File file = new File(fileName);
        if (!file.exists()) {
            try (FileWriter writer = new FileWriter(file)) {
                writer.write("This is sample text transmitted over a reliable TCP socket in Java.\n");
                writer.write("TCP stream ensures all bytes are received in order without data loss.\n");
            } catch (IOException e) {
                System.err.println("Could not create sample file: " + e.getMessage());
            }
            System.out.println("Created default sample file: \"" + fileName + "\"");
        }

        System.out.println("Connecting to TCP File Server at " + serverIP + ":" + PORT + "...");
        try (Socket socket = new Socket(serverIP, PORT)) {
            System.out.println("Connected! Sending file: \"" + fileName + "\" (" + file.length() + " bytes)...");

            try (FileInputStream fileIn = new FileInputStream(file);
                 OutputStream out = socket.getOutputStream()) {

                byte[] buffer = new byte[BUFFER_SIZE];
                int bytesRead;
                long totalSent = 0;

                while ((bytesRead = fileIn.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                    totalSent += bytesRead;
                }
                out.flush();
                System.out.println("File transfer complete! Successfully sent " + totalSent + " bytes.");
            }
        } catch (IOException e) {
            System.err.println("File client error: " + e.getMessage());
        }
    }
}

