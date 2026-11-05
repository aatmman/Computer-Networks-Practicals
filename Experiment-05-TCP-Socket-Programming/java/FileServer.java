import java.io.*;
import java.net.*;

public class FileServer {
    public static final int PORT = 9002;
    public static final int BUFFER_SIZE = 4096;

    public static void main(String[] args) {
        System.out.println("========================================================");
        System.out.println("  TCP FILE SERVER LISTENING ON PORT " + PORT + " (JAVA)");
        System.out.println("========================================================");
        System.out.println("Waiting for file upload from client...");

        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            try (Socket clientSocket = serverSocket.accept()) {
                System.out.println("Client connected from " + clientSocket.getRemoteSocketAddress());

                String outputFileName = "received_file.dat";
                try (InputStream in = clientSocket.getInputStream();
                     FileOutputStream fileOut = new FileOutputStream(outputFileName)) {

                    byte[] buffer = new byte[BUFFER_SIZE];
                    int bytesRead;
                    long totalBytes = 0;

                    System.out.println("Receiving file stream...");
                    while ((bytesRead = in.read(buffer)) != -1) {
                        fileOut.write(buffer, 0, bytesRead);
                        totalBytes += bytesRead;
                    }

                    System.out.println("File reception complete! Saved as \"" + outputFileName + "\" (" + totalBytes + " bytes).");
                }
            }
        } catch (IOException e) {
            System.err.println("File server error: " + e.getMessage());
        }
    }
}
