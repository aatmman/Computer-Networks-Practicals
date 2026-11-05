import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class CalculatorClient {
    public static final int PORT = 9003;

    public static void main(String[] args) {
        String serverIP = args.length > 0 ? args[0] : "127.0.0.1";

        System.out.println("Connecting to TCP Calculator Server at " + serverIP + ":" + PORT + "...");
        try (Socket socket = new Socket(serverIP, PORT);
             Scanner scanner = new Scanner(System.in)) {

            System.out.println("Connected!");

            BufferedReader in = new BufferedReader(
                new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
            PrintWriter out = new PrintWriter(
                new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8), true);

            System.out.print("Enter arithmetic expression (e.g., 25 + 17 or 100 / 4): ");
            String expr = scanner.hasNextLine() ? scanner.nextLine() : "12 + 7";
            if (expr.trim().isEmpty()) {
                expr = "12 + 7";
            }

            out.println(expr);
            System.out.println("Sent: \"" + expr + "\"");

            String response = in.readLine();
            System.out.println("Server Response: " + response);

        } catch (IOException e) {
            System.err.println("Calculator client error: " + e.getMessage());
        }
    }
}
