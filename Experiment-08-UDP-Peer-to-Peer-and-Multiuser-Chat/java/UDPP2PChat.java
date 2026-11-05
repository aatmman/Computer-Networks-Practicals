import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class UDPP2PChat {

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: java UDPP2PChat <localPort> <remotePort> [remoteIP]");
            System.out.println("Example (Terminal 1): java UDPP2PChat 9250 9251 127.0.0.1");
            System.out.println("Example (Terminal 2): java UDPP2PChat 9251 9250 127.0.0.1");
            return;
        }

        int localPort = Integer.parseInt(args[0]);
        int remotePort = Integer.parseInt(args[1]);
        String remoteIP = args.length > 2 ? args[2] : "127.0.0.1";

        System.out.println("========================================================");
        System.out.println("         UDP PEER-TO-PEER (P2P) CHAT - JAVA             ");
        System.out.println("========================================================");
        System.out.println("Bound to local port " + localPort + ".");
        System.out.println("Target remote peer: " + remoteIP + ":" + remotePort);
        System.out.println("Type messages and press Enter. Type 'exit' to quit.\n");

        try (DatagramSocket socket = new DatagramSocket(localPort);
             Scanner scanner = new Scanner(System.in)) {

            InetAddress remoteAddress = InetAddress.getByName(remoteIP);

            // Background receiver thread
            Thread receiver = new Thread(() -> {
                byte[] buffer = new byte[1024];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                while (!socket.isClosed()) {
                    try {
                        socket.receive(packet);
                        String msg = new String(packet.getData(), 0, packet.getLength(), StandardCharsets.UTF_8);
                        if (msg.equals("exit")) {
                            System.out.println("\n[Notice] Peer has left the chat. Press Enter to exit.");
                            break;
                        }
                        System.out.print("\r[Peer " + packet.getAddress().getHostAddress() + ":" + packet.getPort() + "]: "
                                         + msg + "\n[You]: ");
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
                String line = scanner.nextLine();
                if (line.equalsIgnoreCase("exit") || line.equalsIgnoreCase("/quit")) {
                    byte[] exitBytes = "exit".getBytes(StandardCharsets.UTF_8);
                    DatagramPacket exitPacket = new DatagramPacket(exitBytes, exitBytes.length, remoteAddress, remotePort);
                    socket.send(exitPacket);
                    break;
                }
                if (!line.trim().isEmpty()) {
                    byte[] data = line.getBytes(StandardCharsets.UTF_8);
                    DatagramPacket packet = new DatagramPacket(data, data.length, remoteAddress, remotePort);
                    socket.send(packet);
                }
            }

            System.out.println("P2P chat ended.");
        } catch (IOException e) {
            System.err.println("Socket error: " + e.getMessage());
        }
    }
}
