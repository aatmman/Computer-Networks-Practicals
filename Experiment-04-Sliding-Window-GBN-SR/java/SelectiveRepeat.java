import java.util.Scanner;

public class SelectiveRepeat {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("========================================================");
        System.out.println("  SELECTIVE REPEAT (SR) SLIDING WINDOW ARQ SIMULATION - JAVA");
        System.out.println("========================================================");

        System.out.print("Enter total number of frames to send: ");
        int totalFrames = scanner.nextInt();
        if (totalFrames <= 0) {
            System.err.println("Error: Total frames must be a positive integer.");
            scanner.close();
            return;
        }

        System.out.print("Enter sliding window size (N): ");
        int windowSize = scanner.nextInt();
        if (windowSize <= 0) {
            System.err.println("Error: Window size must be a positive integer.");
            scanner.close();
            return;
        }

        System.out.print("Enter frame index to simulate as LOST (0-indexed, or -1 for none): ");
        int lostFrame = scanner.nextInt();

        boolean[] acked = new boolean[totalFrames];
        boolean[] receiverBuffered = new boolean[totalFrames];
        int retransmissions = 0;
        int base = 0;
        int passNumber = 1;

        System.out.println("\nStarting Selective Repeat Simulation...");
        System.out.println("Window Size (N) = " + windowSize + ", Total Frames = " + totalFrames);

        while (base < totalFrames) {
            int windowEnd = Math.min(base + windowSize, totalFrames);
            System.out.println("\n[Pass " + (passNumber++) + "] Active Window: [" + base + " .. " + (windowEnd - 1) + "]");
            System.out.println("  Sender transmitting unacknowledged frames in window...");

            boolean lossOccurred = false;

            for (int i = base; i < windowEnd; i++) {
                if (acked[i]) {
                    System.out.println("    -> Frame " + i + " : [ALREADY ACKNOWLEDGED - SKIPPED]");
                    continue;
                }

                if (i == lostFrame) {
                    System.out.println("    -> Frame " + i + " : [DROPPED / LOST IN TRANSIT]");
                    lossOccurred = true;
                } else {
                    acked[i] = true;
                    receiverBuffered[i] = true;
                    System.out.println("    -> Frame " + i + " : [RECEIVED & BUFFERED OUT-OF-ORDER BY RECEIVER -> INDIVIDUAL ACK SENT]");
                }
            }

            if (lossOccurred) {
                System.out.println("  [TIMEOUT] Timer expired ONLY for unacknowledged Frame " + lostFrame + "!");
                System.out.println("  [SELECTIVE REPEAT ACTION] Retransmitting ONLY Frame " + lostFrame + "...");
                System.out.println("    -> Frame " + lostFrame + " : [RETRANSMITTED SUCCESSFULLY -> ACK RECEIVED]");
                acked[lostFrame] = true;
                receiverBuffered[lostFrame] = true;
                retransmissions++;
                lostFrame = -1; // Loss handled
            }

            // Slide window forward past all consecutively acknowledged frames
            int oldBase = base;
            while (base < totalFrames && acked[base]) {
                base++;
            }
            System.out.println("  Receiver delivered in-order frames up to " + (base - 1) + ".");
            System.out.println("  Window slid forward by " + (base - oldBase) + " position(s). New base = " + base);
        }

        System.out.println("\n========================================================");
        System.out.println("SIMULATION COMPLETE");
        System.out.println("Total Frames Successfully Transmitted : " + totalFrames);
        System.out.println("Total Retransmissions in Selective Repeat : " + retransmissions);
        System.out.println("Total Frame Transmissions (Original+Retx): " + (totalFrames + retransmissions));
        System.out.println("========================================================");

        scanner.close();
    }
}

