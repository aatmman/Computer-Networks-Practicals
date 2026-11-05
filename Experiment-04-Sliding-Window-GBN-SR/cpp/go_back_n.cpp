#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    cout << "========================================================\n";
    cout << "    GO-BACK-N (GBN) SLIDING WINDOW ARQ SIMULATION - C++ \n";
    cout << "========================================================\n";

    int totalFrames, windowSize, lostFrame;
    cout << "Enter total number of frames to send: ";
    if (!(cin >> totalFrames) || totalFrames <= 0) {
        cerr << "Error: Total frames must be a positive integer.\n";
        return 1;
    }

    cout << "Enter sliding window size (N): ";
    if (!(cin >> windowSize) || windowSize <= 0) {
        cerr << "Error: Window size must be a positive integer.\n";
        return 1;
    }

    cout << "Enter frame index to simulate as LOST (0-indexed, or -1 for none): ";
    cin >> lostFrame;

    int base = 0;
    int retransmissions = 0;
    int passNumber = 1;

    cout << "\nStarting Go-Back-N Simulation...\n";
    cout << "Window Size (N) = " << windowSize << ", Total Frames = " << totalFrames << "\n";

    while (base < totalFrames) {
        int windowEnd = min(base + windowSize, totalFrames);
        cout << "\n[Pass " << passNumber++ << "] Active Window: [" << base << " .. " << windowEnd - 1 << "]\n";
        cout << "  Sender transmitting frames " << base << " through " << windowEnd - 1 << "...\n";

        bool lossOccurred = false;
        int firstLostInThisPass = -1;

        for (int i = base; i < windowEnd; i++) {
            if (i == lostFrame) {
                cout << "    -> Frame " << i << " : [DROPPED / LOST IN TRANSIT]\n";
                lossOccurred = true;
                firstLostInThisPass = i;
                // In Go-Back-N, receiver window is 1; it discards all subsequent out-of-order frames
                for (int j = i + 1; j < windowEnd; j++) {
                    cout << "    -> Frame " << j << " : [ARRIVED OUT-OF-ORDER - DISCARDED BY RECEIVER]\n";
                }
                break;
            } else {
                cout << "    -> Frame " << i << " : [RECEIVED & CUMULATIVE ACK SENT]\n";
            }
        }

        if (lossOccurred) {
            cout << "  [TIMEOUT] Timer expired for unacknowledged Frame " << firstLostInThisPass << "!\n";
            cout << "  [GO-BACK-N ACTION] Receiver discarded out-of-order frames.\n";
            cout << "  Sender retransmitting all frames from Frame " << firstLostInThisPass << " to " << windowEnd - 1 << ":\n";
            int framesToRetransmit = windowEnd - firstLostInThisPass;
            for (int r = firstLostInThisPass; r < windowEnd; r++) {
                cout << "    -> Frame " << r << " : [RETRANSMITTED SUCCESSFULLY -> ACK RECEIVED]\n";
            }
            retransmissions += framesToRetransmit;

            // Reset lostFrame so it is not dropped again
            lostFrame = -1;
        }

        // Window slides forward past all successfully acknowledged frames
        cout << "  Receiver delivered in-order frames up to " << windowEnd - 1 << ".\n";
        cout << "  Window slid forward by " << (windowEnd - base) << " position(s). New base = " << windowEnd << "\n";
        base = windowEnd;
    }

    cout << "\n========================================================\n";
    cout << "SIMULATION COMPLETE\n";
    cout << "Total Frames Successfully Transmitted : " << totalFrames << "\n";
    cout << "Total Retransmissions in Go-Back-N     : " << retransmissions << "\n";
    cout << "Total Frame Transmissions (Original+Retx): " << totalFrames + retransmissions << "\n";
    cout << "========================================================\n";

    return 0;
}
