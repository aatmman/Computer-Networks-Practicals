import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class HelloClient {
    public static final int PORT = 9001;

    public static void main(String[] args) {
        String serverIP = args.length > 0 ? args[0] : "127.0.0.1";

        System.out.println("Connecting to TCP Hello Server at " + serverIP + ":" + PORT + "...");
        try (Socket socket = new Socket(serverIP, PORT)) {
            System.out.println("Connected successfully!");

            BufferedReader in = new BufferedReader(
                new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
            PrintWriter out = new PrintWriter(
                new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8), true);

            String message = "Hello from TCP Client! Greetings.";
            out.println(message);
            System.out.println("Sent message: \"" + message + "\"");

            String response = in.readLine();
            System.out.println("Server replied: \"" + response + "\"");

            System.out.println("Client closed socket.");
        } catch (IOException e) {
            System.err.println("Client error: " + e.getMessage());
        }
    }
}

