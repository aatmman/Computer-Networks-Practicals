
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class MultiChatServer {
    private static final Set<PrintWriter> clients =
        ConcurrentHashMap.newKeySet();

    public static void main(String[] args) throws IOException {
        int port = args.length == 0 ? 6000 : Integer.parseInt(args[0]);

        try (ServerSocket server = new ServerSocket(port)) {
            System.out.println("Chat server started on port " + port);

            while (true) {
                Socket socket = server.accept();
                Thread handler = new Thread(() -> handleClient(socket));
                handler.start();
            }
        }
    }

    private static void handleClient(Socket socket) {
        PrintWriter out = null;
        String name = null;

        try (Socket connection = socket) {
            BufferedReader in = new BufferedReader(
                new InputStreamReader(
                    connection.getInputStream(), StandardCharsets.UTF_8));

            out = new PrintWriter(
                new OutputStreamWriter(
                    connection.getOutputStream(), StandardCharsets.UTF_8), true);

            out.println("Enter your name:");
            name = in.readLine();

            if (name == null || name.trim().isEmpty()) {
                return;
            }

            name = name.trim();
            clients.add(out);

            broadcast("[Server] " + name + " joined the chat.");

            String message;

            while ((message = in.readLine()) != null) {
                broadcast(name + ": " + message);
            }
        } catch (IOException e) {
            System.out.println("A client connection ended unexpectedly.");
        } finally {
            if (out != null && clients.remove(out)) {
                broadcast("[Server] " + name + " left the chat.");
            }
        }
    }

    private static void broadcast(String message) {
        for (PrintWriter writer : clients) {
            synchronized (writer) {
                writer.println(message);
            }
        }
    }
}
