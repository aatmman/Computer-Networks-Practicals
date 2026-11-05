import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class CalculatorServer {
    public static final int PORT = 9003;

    public static void main(String[] args) {
        System.out.println("========================================================");
        System.out.println("  TCP CALCULATOR SERVER LISTENING ON PORT " + PORT + " (JAVA)");
        System.out.println("========================================================");
        System.out.println("Waiting for calculation requests...");

        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            try (Socket clientSocket = serverSocket.accept()) {
                System.out.println("Client connected from " + clientSocket.getRemoteSocketAddress());

                BufferedReader in = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream(), StandardCharsets.UTF_8));
                PrintWriter out = new PrintWriter(
                    new OutputStreamWriter(clientSocket.getOutputStream(), StandardCharsets.UTF_8), true);

                String expr = in.readLine();
                if (expr != null) {
                    System.out.println("Received arithmetic expression: \"" + expr + "\"");

                    String reply = evaluateExpression(expr.trim());
                    System.out.println("Computed reply: \"" + reply + "\"");
                    out.println(reply);
                }
            }
            System.out.println("Calculation finished. Server closed connection.");
        } catch (IOException e) {
            System.err.println("Calculator server error: " + e.getMessage());
        }
    }

    private static String evaluateExpression(String expr) {
        String[] tokens = expr.split("\\s+");
        if (tokens.length != 3) {
            return "Error: Invalid expression format. Expected '<num1> <op> <num2>' (e.g. 15 + 4)";
        }

        try {
            double a = Double.parseDouble(tokens[0]);
            String op = tokens[1];
            double b = Double.parseDouble(tokens[2]);

            switch (op) {
                case "+":
                    return "Result: " + (a + b);
                case "-":
                    return "Result: " + (a - b);
                case "*":
                    return "Result: " + (a * b);
                case "/":
                    if (b == 0) {
                        return "Error: Division by zero is undefined.";
                    }
                    return "Result: " + (a / b);
                default:
                    return "Error: Unsupported operator '" + op + "'. Supported: +, -, *, /";
            }
        } catch (NumberFormatException e) {
            return "Error: Operands must be valid numbers.";
        }
    }
}

