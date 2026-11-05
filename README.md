# Computer Networks Practicals Repository (C++ and Java)

This repository contains the complete, verified, and laboratory-tested implementations and comprehensive technical guides for the **Computer Networks (CN) Laboratory Curriculum**, developed directly from the official **Department of Computer Engineering Laboratory Manual**.

---

## Repository Purpose & Dual-Language Policy

The official laboratory manual specifies **C++** as the primary programming language for all socket programming and protocol simulations. In addition, this repository maintains parallel, fully equivalent implementations in **Java**, preserving our pre-existing practical codebase and providing dual-language fluency for examinations and interviews.

To maintain maximum clarity, **each programming experiment features two separate, dedicated guide files**:
1. `CPP_GUIDE.txt` — Complete C++ implementation, POSIX socket / standard library explanations, exact compile/run commands, sample runs, manual bug fixes, and viva questions.
2. `JAVA_GUIDE.txt` — Complete Java implementation, `java.net` socket / standard library explanations, `javac`/`java` commands, sample runs, and viva questions.

Configuration and simulation-based experiments (Packet Tracer, LAN hardware, Apache, and Wireshark) are documented in dedicated `EXPERIMENT_GUIDE.txt` files.

---

## Repository Directory Structure

```text
Computer-Networks-Practicals/
│
├── README.md
├── EXPERIMENT_INDEX.txt
├── QUICK_START.txt
├── .gitignore
│
├── Experiment-01-Small-LAN/
│   └── EXPERIMENT_GUIDE.txt
│
├── Experiment-02-WAN-Wired-Wireless/
│   └── EXPERIMENT_GUIDE.txt
│
├── Experiment-03-CRC-Error-Detection/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   │   └── crc.cpp
│   └── java/
│       └── CRC.java
│
├── Experiment-04-Sliding-Window-GBN-SR/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   │   ├── go_back_n.cpp
│   │   └── selective_repeat.cpp
│   └── java/
│       ├── GoBackN.java
│       └── SelectiveRepeat.java
│
├── Experiment-05-TCP-Socket-Programming/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   │   ├── hello_server.cpp
│   │   ├── hello_client.cpp
│   │   ├── file_server.cpp
│   │   ├── file_client.cpp
│   │   ├── calculator_server.cpp
│   │   └── calculator_client.cpp
│   └── java/
│       ├── HelloServer.java
│       ├── HelloClient.java
│       ├── FileServer.java
│       ├── FileClient.java
│       ├── CalculatorServer.java
│       └── CalculatorClient.java
│
├── Experiment-06-UDP-File-Transfer/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   │   ├── udp_file_receiver.cpp
│   │   └── udp_file_sender.cpp
│   └── java/
│       ├── UDPFileReceiver.java
│       └── UDPFileSender.java
│
├── Experiment-07-TCP-Peer-to-Peer-and-Multiuser-Chat/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   │   ├── p2p_chat.cpp
│   │   ├── multiuser_chat_server.cpp
│   │   └── multiuser_chat_client.cpp
│   └── java/
│       ├── PeerChat.java
│       ├── MultiChatServer.java
│       └── MultiChatClient.java
│
├── Experiment-08-UDP-Peer-to-Peer-and-Multiuser-Chat/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   │   ├── udp_p2p_chat.cpp
│   │   ├── udp_multiuser_server.cpp
│   │   └── udp_multiuser_client.cpp
│   └── java/
│       ├── UDPP2PChat.java
│       ├── UDPMultiChatServer.java
│       └── UDPMultiChatClient.java
│
├── Experiment-09-Apache-Web-Server/
│   └── EXPERIMENT_GUIDE.txt
│
└── Experiment-10-Wireshark-Packet-Capture/
    └── EXPERIMENT_GUIDE.txt
```

---

## Experiment Summary & Mapping

| Exp # | Experiment Title | Nature | Available Guides | Source Folders | Port(s) |
|:---:|:---|:---:|:---|:---:|:---:|
| **01** | Setting up a Small Network (2PC/4PC) and Sharing Resources | Configuration | `EXPERIMENT_GUIDE.txt` | N/A (Hardware/OS) | 445 (SMB) |
| **02** | Setup a WAN with Wired and Wireless LAN in Packet Tracer | Simulation | `EXPERIMENT_GUIDE.txt` | N/A (Cisco Packet Tracer) | N/A |
| **03** | Error Detection and Correction for ASCII Codes using CRC | Programming | `CPP_GUIDE.txt`, `JAVA_GUIDE.txt` | `cpp/`, `java/` | Standalone |
| **04** | Sliding Window Protocol: Go-Back-N (GBN) and Selective Repeat (SR) | Programming | `CPP_GUIDE.txt`, `JAVA_GUIDE.txt` | `cpp/`, `java/` | Standalone |
| **05** | TCP Socket Programming: Say Hello, File Transfer, Calculator | Programming | `CPP_GUIDE.txt`, `JAVA_GUIDE.txt` | `cpp/`, `java/` | 9001, 9002, 9003 |
| **06** | UDP Sockets File Transfer (Script, Text, Audio, Video) | Programming | `CPP_GUIDE.txt`, `JAVA_GUIDE.txt` | `cpp/`, `java/` | 9100 |
| **07** | TCP Sockets: Peer-to-Peer Chat and Multiuser Broadcast Chat | Programming | `CPP_GUIDE.txt`, `JAVA_GUIDE.txt` | `cpp/`, `java/` | 9200, 9201 (C++) / 6000 (Java) |
| **08** | UDP Sockets: Peer-to-Peer Chat and Multiuser Relay Chat | Programming | `CPP_GUIDE.txt`, `JAVA_GUIDE.txt` | `cpp/`, `java/` | 9250/9251 (P2P), 9300 (Multi) |
| **09** | Installation and Configuration of Web Server (Apache) | System / Tool | `EXPERIMENT_GUIDE.txt` | `/var/www/html/` | 80 (HTTP), 443 (HTTPS) |
| **10** | Wireshark Packet Capture & Filter Expressions (TCP/Flags/HTTPS) | Protocol Analysis | `EXPERIMENT_GUIDE.txt` | N/A (Wireshark GUI / CLI) | 80, 443 |

---

## High-Level Compilation and Execution

### C++ Programs
All C++ programs use standard POSIX socket APIs and modern C++17.
- **Compiler**: GCC (`g++`) or Clang (`clang++`).
- **Compilation standard**:
  ```bash
  # Standard single-threaded:
  g++ -std=c++17 -Wall -Wextra program.cpp -o program

  # Multithreaded programs (Experiments 7 and 8):
  g++ -std=c++17 -Wall -Wextra -pthread program.cpp -o program
  ```

### Java Programs
All Java programs use standard `java.net` and `java.io` packages without third-party dependencies.
- **Java Compiler & Runtime**: OpenJDK 8, 11, 17 LTS, 21 LTS, or 25.
- **Compilation and execution**:
  ```bash
  # Compile:
  javac ClassName.java

  # Run:
  java ClassName
  ```

---

## Key Manual Corrections and Bug Fixes

1. **Sliding Window Protocol (Experiment 4)**:
   The official lab manual's sample code for Go-Back-N contained contradictory statements in its rollback section:
   ```cpp
   base = lostFrame;
   base = base;
   base = windowEnd; // Bug: unconditionally advanced base, skipping retransmission
   ```
   This has been corrected across both C++ and Java versions: on timeout, `base` rolls back to the lost frame index, faithfully retransmitting all unacknowledged frames in the window.
2. **CRC Error Detection vs. Correction (Experiment 3)**:
   The guide clearly explains that while the lab title mentions "Error Detection and Correction", CRC is mathematically strictly an *error detection* code. Real-world error correction is achieved via ARQ retransmission or Forward Error Correction (FEC) codes.
3. **UDP Packet Pacing & EOF Delimiters (Experiment 6)**:
   Basic UDP sample code often fails on large transfers due to receive buffer overruns (`SO_RCVBUF`). Our programs include a 1 ms pacing delay (`usleep(1000)` / `Thread.sleep(1)`) and redundant EOF tokens (`<<EOF>>`) to guarantee reliable demonstration.
4. **Preserved Working Java Code (Experiment 7)**:
   Pre-existing working Java implementations (`PeerChat.java`, `MultiChatServer.java`, `MultiChatClient.java`) were preserved intact and fully documented with concurrency and synchronization explanations.

---

## Author & Academic Information
- **Student**: Aatmman Patil (`aatmman`)
- **Email**: `aatmmanpatil@gmail.com`
- **Institution**: Department of Computer Engineering
- **Curriculum**: Computer Networks Laboratory (CS3203L-D)

