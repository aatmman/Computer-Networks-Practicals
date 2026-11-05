import java.util.Scanner;

public class GoBackN {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("========================================================");
        System.out.println("    GO-BACK-N (GBN) SLIDING WINDOW ARQ SIMULATION - JAVA");
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

        int base = 0;
        int retransmissions = 0;
        int passNumber = 1;

        System.out.println("\nStarting Go-Back-N Simulation...");
        System.out.println("Window Size (N) = " + windowSize + ", Total Frames = " + totalFrames);

        while (base < totalFrames) {
            int windowEnd = Math.min(base + windowSize, totalFrames);
            System.out.println("\n[Pass " + (passNumber++) + "] Active Window: [" + base + " .. " + (windowEnd - 1) + "]");
            System.out.println("  Sender transmitting frames " + base + " through " + (windowEnd - 1) + "...");

            boolean lossOccurred = false;
            int firstLostInThisPass = -1;

            for (int i = base; i < windowEnd; i++) {
                if (i == lostFrame) {
                    System.out.println("    -> Frame " + i + " : [DROPPED / LOST IN TRANSIT]");
                    lossOccurred = true;
                    firstLostInThisPass = i;
                    // In Go-Back-N, receiver window is 1; it discards all subsequent out-of-order frames
                    for (int j = i + 1; j < windowEnd; j++) {
                        System.out.println("    -> Frame " + j + " : [ARRIVED OUT-OF-ORDER - DISCARDED BY RECEIVER]");
                    }
                    break;
                } else {
                    System.out.println("    -> Frame " + i + " : [RECEIVED & CUMULATIVE ACK SENT]");
                }
            }

            if (lossOccurred) {
                System.out.println("  [TIMEOUT] Timer expired for unacknowledged Frame " + firstLostInThisPass + "!");
                System.out.println("  [GO-BACK-N ACTION] Receiver discarded out-of-order frames.");
                System.out.println("  Sender retransmitting all frames from Frame " + firstLostInThisPass + " to " + (windowEnd - 1) + ":");
                int framesToRetransmit = windowEnd - firstLostInThisPass;
                for (int r = firstLostInThisPass; r < windowEnd; r++) {
                    System.out.println("    -> Frame " + r + " : [RETRANSMITTED SUCCESSFULLY -> ACK RECEIVED]");
                }
                retransmissions += framesToRetransmit;

                // Reset lostFrame so it is not dropped again
                lostFrame = -1;
            }

            // Window slides forward past all successfully acknowledged frames
            System.out.println("  Receiver delivered in-order frames up to " + (windowEnd - 1) + ".");
            System.out.println("  Window slid forward by " + (windowEnd - base) + " position(s). New base = " + windowEnd);
            base = windowEnd;
        }

        System.out.println("\n========================================================");
        System.out.println("SIMULATION COMPLETE");
        System.out.println("Total Frames Successfully Transmitted : " + totalFrames);
        System.out.println("Total Retransmissions in Go-Back-N     : " + retransmissions);
        System.out.println("Total Frame Transmissions (Original+Retx): " + (totalFrames + retransmissions));
        System.out.println("========================================================");

        scanner.close();
    }
}
