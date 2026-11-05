#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    cout << "========================================================\n";
    cout << "  SELECTIVE REPEAT (SR) SLIDING WINDOW ARQ SIMULATION - C++\n";
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

    vector<bool> acked(totalFrames, false);
    vector<bool> receiverBuffered(totalFrames, false);
    int retransmissions = 0;
    int base = 0;
    int passNumber = 1;

    cout << "\nStarting Selective Repeat Simulation...\n";
    cout << "Window Size (N) = " << windowSize << ", Total Frames = " << totalFrames << "\n";

    while (base < totalFrames) {
        int windowEnd = min(base + windowSize, totalFrames);
        cout << "\n[Pass " << passNumber++ << "] Active Window: [" << base << " .. " << windowEnd - 1 << "]\n";
        cout << "  Sender transmitting unacknowledged frames in window...\n";

        bool lossOccurred = false;

        for (int i = base; i < windowEnd; i++) {
            if (acked[i]) {
                cout << "    -> Frame " << i << " : [ALREADY ACKNOWLEDGED - SKIPPED]\n";
                continue;
            }

            if (i == lostFrame) {
                cout << "    -> Frame " << i << " : [DROPPED / LOST IN TRANSIT]\n";
                lossOccurred = true;
            } else {
                acked[i] = true;
                receiverBuffered[i] = true;
                cout << "    -> Frame " << i << " : [RECEIVED & BUFFERED OUT-OF-ORDER BY RECEIVER -> INDIVIDUAL ACK SENT]\n";
            }
        }

        if (lossOccurred) {
            cout << "  [TIMEOUT] Timer expired ONLY for unacknowledged Frame " << lostFrame << "!\n";
            cout << "  [SELECTIVE REPEAT ACTION] Retransmitting ONLY Frame " << lostFrame << "...\n";
            cout << "    -> Frame " << lostFrame << " : [RETRANSMITTED SUCCESSFULLY -> ACK RECEIVED]\n";
            acked[lostFrame] = true;
            receiverBuffered[lostFrame] = true;
            retransmissions++;
            lostFrame = -1; // Loss handled
        }

        // Slide the window forward past all consecutively acknowledged frames
        int oldBase = base;
        while (base < totalFrames && acked[base]) {
            base++;
        }
        cout << "  Receiver delivered in-order frames up to " << base - 1 << ".\n";
        cout << "  Window slid forward by " << (base - oldBase) << " position(s). New base = " << base << "\n";
    }

    cout << "\n========================================================\n";
    cout << "SIMULATION COMPLETE\n";
    cout << "Total Frames Successfully Transmitted : " << totalFrames << "\n";
    cout << "Total Retransmissions in Selective Repeat : " << retransmissions << "\n";
    cout << "Total Frame Transmissions (Original+Retx): " << totalFrames + retransmissions << "\n";
    cout << "========================================================\n";

    return 0;
}

