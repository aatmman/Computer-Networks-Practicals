import java.util.Scanner;

public class CRC {

    /**
     * Performs modulo-2 (XOR) binary division and returns the remainder
     * of length (generator.length() - 1).
     */
    public static String mod2Divide(String dividend, String generator) {
        int n = generator.length();
        char[] rem = dividend.toCharArray();

        for (int i = 0; i <= rem.length - n; i++) {
            if (rem[i] == '1') {
                for (int j = 0; j < n; j++) {
                    rem[i + j] = (rem[i + j] == generator.charAt(j)) ? '0' : '1';
                }
            }
        }
        return new String(rem, rem.length - (n - 1), n - 1);
    }

    /**
     * Converts an ASCII text string to an 8-bit binary string per character.
     */
    public static String asciiToBinary(String text) {
        StringBuilder bin = new StringBuilder();
        for (char c : text.toCharArray()) {
            for (int i = 7; i >= 0; i--) {
                bin.append(((c >> i) & 1) == 1 ? '1' : '0');
            }
        }
        return bin.toString();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("========================================================");
        System.out.println("   CYCLIC REDUNDANCY CHECK (CRC) ERROR DETECTION - JAVA ");
        System.out.println("========================================================");

        System.out.print("Enter ASCII message: ");
        String message = scanner.nextLine();
        if (message.isEmpty()) {
            message = "Hi";
            System.out.println("Empty message entered. Defaulting to: \"" + message + "\"");
        }

        System.out.print("Enter generator polynomial bits (e.g. 1101 or 10001000000100001): ");
        String generator = scanner.next();

        if (generator.length() < 2) {
            System.err.println("Error: Generator polynomial must have at least 2 bits.");
            scanner.close();
            return;
        }
        for (char c : generator.toCharArray()) {
            if (c != '0' && c != '1') {
                System.err.println("Error: Generator must contain only binary digits (0 and 1).");
                scanner.close();
                return;
            }
        }

        String data = asciiToBinary(message);
        int n = generator.length();

        // --- SENDER SIDE ---
        String zeros = "0".repeat(n - 1);
        String augmented = data + zeros;
        String crc = mod2Divide(augmented, generator);
        String frame = data + crc; // Transmitted codeword

        System.out.println("\n--- SENDER SIDE ---");
        System.out.println("Original Message     : \"" + message + "\"");
        System.out.println("Data Bits (" + data.length() + " bits) : " + data);
        System.out.println("Generator (" + n + " bits) : " + generator);
        System.out.println("Appended Zeros       : " + zeros);
        System.out.println("Computed CRC (FCS)   : " + crc);
        System.out.println("Transmitted Frame    : " + frame);

        // --- CHANNEL TRANSMISSION / ERROR SIMULATION ---
        System.out.println("\n--- TRANSMISSION CHANNEL ---");
        System.out.print("Simulate a transmission bit error? (y/n): ");
        char choice = scanner.next().charAt(0);

        String received = frame;
        if (choice == 'y' || choice == 'Y') {
            System.out.print("Enter bit position to flip (0 to " + (frame.length() - 1) + "): ");
            int pos = scanner.nextInt();
            if (pos >= 0 && pos < frame.length()) {
                char orig = received.charAt(pos);
                char flipped = (orig == '0') ? '1' : '0';
                received = frame.substring(0, pos) + flipped + frame.substring(pos + 1);
                System.out.println("Flipped bit at index " + pos + " from '" + orig + "' to '" + flipped + "'.");
                System.out.println("Corrupted Frame Received : " + received);
            } else {
                System.out.println("Invalid index! Transmitting frame without error.");
            }
        } else {
            System.out.println("Transmitting frame over clean channel without errors.");
        }

        // --- RECEIVER SIDE ---
        System.out.println("\n--- RECEIVER SIDE ---");
        String remainder = mod2Divide(received, generator);
        boolean hasError = remainder.contains("1");

        System.out.println("Received Frame          : " + received);
        System.out.println("Receiver Remainder      : " + remainder);

        if (hasError) {
            System.out.println("Verification Status     : [REJECTED] Error detected in received frame!");
            System.out.println("Action                  : Discard frame and request retransmission (ARQ).");
        } else {
            System.out.println("Verification Status     : [ACCEPTED] Remainder is all zeros. No error detected.");
            String receivedData = received.substring(0, data.length());
            System.out.println("Extracted Data Bits     : " + receivedData);
        }

        System.out.println("========================================================");
        scanner.close();
    }
}
