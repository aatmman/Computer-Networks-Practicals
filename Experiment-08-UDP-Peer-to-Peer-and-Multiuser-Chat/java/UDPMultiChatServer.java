import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class UDPMultiChatServer {
    public static final int PORT = 9300;

    // Track registered clients by SocketAddress
    private static final Map<SocketAddress, String> clients = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        int port = args.length > 0 ? Integer.parseInt(args[0]) : PORT;

        System.out.println("========================================================");
        System.out.println("  UDP MULTIUSER CHAT RELAY SERVER LISTENING ON PORT " + port + " (JAVA)");
        System.out.println("========================================================");
        System.out.println("Waiting for client JOIN datagrams...");

        try (DatagramSocket socket = new DatagramSocket(port)) {
            byte[] buffer = new byte[1024];

            while (true) {
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);

                SocketAddress senderAddr = packet.getSocketAddress();
                String msg = new String(packet.getData(), 0, packet.getLength(), StandardCharsets.UTF_8).trim();

                if (msg.startsWith("JOIN")) {
                    String username = "Anonymous";
                    if (msg.contains(":")) {
                        username = msg.substring(msg.indexOf(":") + 1).trim();
                    }

                    if (!clients.containsKey(senderAddr)) {
                        clients.put(senderAddr, username);
                        System.out.println("[Registration] " + username + " registered from " + senderAddr
                                           + " (Total: " + clients.size() + ")");

                        String welcome = "[Server]: Welcome " + username + " to UDP Multiuser Chat!";
                        sendPacket(socket, welcome, senderAddr);

                        String joinNotice = "[Server Notice]: " + username + " joined the chat.";
                        broadcast(socket, joinNotice, senderAddr);
                    }
                    continue;
                }

                if (msg.equalsIgnoreCase("exit") || msg.equalsIgnoreCase("LEAVE") || msg.equalsIgnoreCase("/quit")) {
                    String username = clients.remove(senderAddr);
                    if (username != null) {
                        System.out.println("[Departure] " + username + " has disconnected.");
                        String leaveNotice = "[Server Notice]: " + username + " left the chat.";
                        broadcast(socket, leaveNotice, senderAddr);
                    }
                    continue;
                }

                // Chat message
                String senderName = clients.getOrDefault(senderAddr, "Guest");
                String broadcastMsg = "[" + senderName + "]: " + msg;
                System.out.println(broadcastMsg);
                broadcast(socket, broadcastMsg, senderAddr);
            }

        } catch (IOException e) {
            System.err.println("Server error: " + e.getMessage());
        }
    }

    private static void broadcast(DatagramSocket socket, String message, SocketAddress excludeAddr) {
        for (SocketAddress clientAddr : clients.keySet()) {
            if (!clientAddr.equals(excludeAddr)) {
                sendPacket(socket, message, clientAddr);
            }
        }
    }

    private static void sendPacket(DatagramSocket socket, String message, SocketAddress destAddr) {
        try {
            byte[] bytes = message.getBytes(StandardCharsets.UTF_8);
            DatagramPacket packet = new DatagramPacket(bytes, bytes.length, destAddr);
            socket.send(packet);
        } catch (IOException e) {
            System.err.println("Failed to send packet to " + destAddr + ": " + e.getMessage());
        }
    }
}

