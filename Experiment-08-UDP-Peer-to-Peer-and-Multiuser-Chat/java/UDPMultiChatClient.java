import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class UDPMultiChatClient {
    public static final int PORT = 9300;

    public static void main(String[] args) {
        String serverIP = args.length > 0 ? args[0] : "127.0.0.1";
        int port = args.length > 1 ? Integer.parseInt(args[1]) : PORT;

        System.out.println("========================================================");
        System.out.println("        UDP MULTIUSER CHAT CLIENT - JAVA                ");
        System.out.println("========================================================");
        System.out.println("Server target: " + serverIP + ":" + port);

        try (DatagramSocket socket = new DatagramSocket();
             Scanner scanner = new Scanner(System.in)) {

            InetAddress serverAddress = InetAddress.getByName(serverIP);

            System.out.print("Enter your display username: ");
            String username = scanner.hasNextLine() ? scanner.nextLine() : "Anonymous";
            if (username.trim().isEmpty()) username = "Anonymous";

            // Send registration datagram
            String joinPayload = "JOIN:" + username;
            byte[] joinBytes = joinPayload.getBytes(StandardCharsets.UTF_8);
            DatagramPacket joinPacket = new DatagramPacket(joinBytes, joinBytes.length, serverAddress, port);
            socket.send(joinPacket);

            System.out.println("Joined UDP chat session. Type messages and press Enter. Type 'exit' to quit.\n");

            // Background receiver thread
            Thread receiver = new Thread(() -> {
                byte[] buffer = new byte[1024];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                while (!socket.isClosed()) {
                    try {
                        socket.receive(packet);
                        String msg = new String(packet.getData(), 0, packet.getLength(), StandardCharsets.UTF_8);
                        System.out.print("\r" + msg + "\n[You]: ");
                    } catch (IOException e) {
                        if (!socket.isClosed()) {
                            System.err.println("Receive error: " + e.getMessage());
                        }
                    }
                }
            });
            receiver.setDaemon(true);
            receiver.start();

            // Main input loop
            while (true) {
                System.out.print("[You]: ");
                if (!scanner.hasNextLine()) break;
                String msg = scanner.nextLine();
                if (msg.equalsIgnoreCase("exit") || msg.equalsIgnoreCase("/quit")) {
                    byte[] exitBytes = "exit".getBytes(StandardCharsets.UTF_8);
                    DatagramPacket exitPacket = new DatagramPacket(exitBytes, exitBytes.length, serverAddress, port);
                    socket.send(exitPacket);
                    break;
                }
                if (!msg.trim().isEmpty()) {
                    byte[] bytes = msg.getBytes(StandardCharsets.UTF_8);
                    DatagramPacket packet = new DatagramPacket(bytes, bytes.length, serverAddress, port);
                    socket.send(packet);
                }
            }

            System.out.println("You left the UDP chat.");

        } catch (IOException e) {
            System.err.println("Client error: " + e.getMessage());
        }
    }
}
