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
                // In Go-Back-N, receiver discards all subsequent out-of-order frames
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
            cout << "  [GO-BACK-N ACTION] Sender rolls window back to Frame " << firstLostInThisPass << ".\n";
            int framesToRetransmit = windowEnd - firstLostInThisPass;
            cout << "  Retransmitting " << framesToRetransmit << " frame(s): ";
            for (int r = firstLostInThisPass; r < windowEnd; r++) {
                cout << r << (r == windowEnd - 1 ? "" : ", ");
            }
            cout << "\n";
            retransmissions += framesToRetransmit;

            // Reset lostFrame so it successfully delivers upon retransmission
            lostFrame = -1;
            // Roll back sender base to the lost frame
            base = firstLostInThisPass;
        } else {
            cout << "  All frames in window [" << base << " .. " << windowEnd - 1 << "] acknowledged successfully.\n";
            base = windowEnd; // Slide window forward
        }
    }

    cout << "\n========================================================\n";
    cout << "SIMULATION COMPLETE\n";
    cout << "Total Frames Successfully Transmitted : " << totalFrames << "\n";
    cout << "Total Retransmissions in Go-Back-N     : " << retransmissions << "\n";
    cout << "Total Frame Transmissions (Original+Retx): " << totalFrames + retransmissions << "\n";
    cout << "========================================================\n";

    return 0;
}
