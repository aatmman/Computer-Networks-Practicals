#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Perform modulo-2 (XOR) division and return the remainder string of length (n - 1)
string mod2Divide(string dividend, const string &generator) {
    int n = generator.size();
    string rem = dividend;
    for (int i = 0; i <= (int)dividend.size() - n; i++) {
        if (rem[i] == '1') {
            for (int j = 0; j < n; j++) {
                rem[i + j] = (rem[i + j] == generator[j]) ? '0' : '1';
            }
        }
    }
    return rem.substr(rem.size() - (n - 1)); // last (n - 1) bits = remainder
}

// Convert an ASCII string into an 8-bit binary representation per character
string asciiToBinary(const string &text) {
    string bin = "";
    for (unsigned char c : text) {
        for (int i = 7; i >= 0; i--) {
            bin += ((c >> i) & 1) ? '1' : '0';
        }
    }
    return bin;
}

int main() {
    cout << "========================================================\n";
    cout << "  CYCLIC REDUNDANCY CHECK (CRC) ERROR DETECTION - C++  \n";
    cout << "========================================================\n";

    string message, generator;
    cout << "Enter ASCII message: ";
    getline(cin, message);

    if (message.empty()) {
        message = "Hi";
        cout << "Empty message entered. Defaulting to: \"" << message << "\"\n";
    }

    cout << "Enter generator polynomial bits (e.g. 1101 or 10001000000100001): ";
    cin >> generator;

    // Validate generator polynomial
    if (generator.size() < 2) {
        cerr << "Error: Generator polynomial must have at least 2 bits.\n";
        return 1;
    }
    for (char c : generator) {
        if (c != '0' && c != '1') {
            cerr << "Error: Generator must contain only binary digits (0 and 1).\n";
            return 1;
        }
    }
    if (generator.front() != '1' || generator.back() != '1') {
        cout << "Note: Standard CRC generators usually begin and end with '1'.\n";
    }

    string data = asciiToBinary(message);
    int n = generator.size();

    // --- SENDER SIDE ---
    // Append (n - 1) zero bits to the data
    string augmented = data + string(n - 1, '0');
    string crc = mod2Divide(augmented, generator);
    string frame = data + crc; // Transmitted codeword

    cout << "\n--- SENDER SIDE ---" << endl;
    cout << "Original Message     : \"" << message << "\"" << endl;
    cout << "Data Bits (" << data.size() << " bits) : " << data << endl;
    cout << "Generator (" << n << " bits) : " << generator << endl;
    cout << "Appended Zeros       : " << string(n - 1, '0') << endl;
    cout << "Computed CRC (FCS)   : " << crc << endl;
    cout << "Transmitted Frame    : " << frame << endl;

    // --- CHANNEL TRANSMISSION / ERROR SIMULATION ---
    char choice;
    cout << "\n--- TRANSMISSION CHANNEL ---" << endl;
    cout << "Simulate a transmission bit error? (y/n): ";
    cin >> choice;

    string received = frame;
    if (choice == 'y' || choice == 'Y') {
        int pos = -1;
        cout << "Enter bit position to flip (0 to " << frame.size() - 1 << "): ";
        cin >> pos;
        if (pos >= 0 && pos < (int)frame.size()) {
            char orig = received[pos];
            received[pos] = (received[pos] == '0') ? '1' : '0';
            cout << "Flipped bit at index " << pos << " from '" << orig << "' to '" << received[pos] << "'." << endl;
            cout << "Corrupted Frame Received : " << received << endl;
        } else {
            cout << "Invalid index! Transmitting frame without error." << endl;
        }
    } else {
        cout << "Transmitting frame over clean channel without errors." << endl;
    }

    // --- RECEIVER SIDE ---
    cout << "\n--- RECEIVER SIDE ---" << endl;
    string remainder = mod2Divide(received, generator);
    bool hasError = remainder.find('1') != string::npos;

    cout << "Received Frame          : " << received << endl;
    cout << "Receiver Remainder      : " << remainder << endl;

    if (hasError) {
        cout << "Verification Status     : [REJECTED] Error detected in received frame!" << endl;
        cout << "Action                  : Discard frame and request retransmission (ARQ)." << endl;
    } else {
        cout << "Verification Status     : [ACCEPTED] Remainder is all zeros. No error detected." << endl;
        string receivedData = received.substr(0, data.size());
        cout << "Extracted Data Bits     : " << receivedData << endl;
    }

    cout << "========================================================\n";
    return 0;
}

