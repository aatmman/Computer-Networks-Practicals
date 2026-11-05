import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class HelloServer {
    public static final int PORT = 9001;

    public static void main(String[] args) {
        System.out.println("========================================================");
        System.out.println("  TCP HELLO SERVER LISTENING ON PORT " + PORT + " (JAVA)");
        System.out.println("========================================================");
        System.out.println("Waiting for incoming client connection...");

        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            try (Socket clientSocket = serverSocket.accept()) {
                System.out.println("Client connected from " + clientSocket.getRemoteSocketAddress());

                BufferedReader in = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream(), StandardCharsets.UTF_8));
                PrintWriter out = new PrintWriter(
                    new OutputStreamWriter(clientSocket.getOutputStream(), StandardCharsets.UTF_8), true);

                String clientMessage = in.readLine();
                System.out.println("Client says: \"" + clientMessage + "\"");

                String reply = "Hello from TCP Server! Connection verified.";
                out.println(reply);
                System.out.println("Sent reply: \"" + reply + "\"");
            }
            System.out.println("Server closed connection gracefully.");
        } catch (IOException e) {
            System.err.println("Server error: " + e.getMessage());
        }
    }
}

