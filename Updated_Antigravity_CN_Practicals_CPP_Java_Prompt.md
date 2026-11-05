# UPDATED ANTIGRAVITY MASTER PROMPT — COMPUTER NETWORKS PRACTICALS (C++ + JAVA)

You are working on my **Computer Networks (CN) Practical Repository**. Update/create the repository using the uploaded **CN Lab Manual (1).docx** as the primary source, and preserve the Java code that we have already completed.

## Main requirement — TWO SEPARATE TEXT GUIDES

For every experiment that involves programming, create **two separate `.txt` files**:

1. `CPP_GUIDE.txt` — complete C++ implementation and explanation.
2. `JAVA_GUIDE.txt` — complete Java implementation and explanation.

Do **not** combine C++ and Java into one file. Do not overwrite one language with the other. The repository must clearly show both implementations side by side.

We have already worked on the Java implementations. First inspect the existing project files and preserve/reuse the Java code wherever it exists. Do not discard, replace, or unnecessarily rewrite working Java code. Compare it with the experiment requirements and make only necessary corrections. Also provide the C++ implementation required by the lab manual.

If a particular programming experiment has no meaningful implementation in one of the languages, do not invent one. Instead, create the relevant guide file with a clear note explaining that the experiment is configuration/tool-based and does not require a C++/Java program. For tool-based experiments, create a separate `EXPERIMENT_GUIDE.txt` if needed.

## Source of truth and language policy

- Read the uploaded CN Lab Manual fully before making changes.
- Follow the experiment titles, aims, requirements, and expected tasks in the manual.
- The manual specifies C++ for its programming experiments in the relevant sections. Keep those C++ versions.
- Java versions are an additional parallel implementation because we have already completed the code in Java / need to retain it for our practical preparation.
- Do not claim that the manual itself specifies Java if it does not.
- Do not silently translate or remove any existing implementation.
- Where the manual's sample code has a bug, incomplete logic, or a limitation, explain it in a clearly labelled **Manual Code Note / Correction** section. Do not silently change the stated experiment objective.
- Keep the C++ and Java versions functionally equivalent where the task allows it. Explain language-specific differences such as `ServerSocket`/`Socket` and `DatagramSocket` in Java versus POSIX socket APIs in C++.

## Required repository structure

Create or update this structure:

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
│   └── java/
│
├── Experiment-04-Sliding-Window-GBN-SR/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   └── java/
│
├── Experiment-05-TCP-Socket-Programming/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   └── java/
│
├── Experiment-06-UDP-File-Transfer/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   └── java/
│
├── Experiment-07-TCP-Peer-to-Peer-and-Multiuser-Chat/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   └── java/
│
├── Experiment-08-UDP-Peer-to-Peer-and-Multiuser-Chat/
│   ├── CPP_GUIDE.txt
│   ├── JAVA_GUIDE.txt
│   ├── cpp/
│   └── java/
│
├── Experiment-09-Apache-Web-Server/
│   └── EXPERIMENT_GUIDE.txt
│
└── Experiment-10-Wireshark-Packet-Capture/
    └── EXPERIMENT_GUIDE.txt
```

You may add helper files, sample input/output files, diagrams, or scripts, but keep the two language guides separate. If the existing repository already has a different sensible structure, adapt it without deleting useful work.

## Experiments and expected coverage

Use the exact experiment wording from the lab manual. The expected list is:

1. Setting up a Small Network (2PC/4PC) and Configuration for Sharing Resources.
2. Setup a WAN containing Wired as well as Wireless LAN using Packet Tracer.
3. Program for Error Detection and Correction for ASCII Codes using CRC.
4. Program to Simulate Go-back-N and Selective Repeat Modes of Sliding Window Protocol.
5. Program using TCP Socket: Say Hello to Each Other, File Transfer, Calculator (Arithmetic).
6. Program using UDP Sockets to Enable File Transfer between Two Machines.
7. Program using TCP Sockets for Wired Network: Peer to Peer Chat and Multiuser Chat.
8. Program using UDP Sockets for Wired Network: Peer to Peer Chat and Multiuser Chat.
9. Installation and Configuration of Web Server (Client-Server based).
10. Capture Packets using Wireshark with Filter Expressions (TCP/HTTP to Facebook, SYN/PSH/RST flags).

### Experiment 3 — CRC
Both language guides must contain:
- CRC sender and receiver/error-checking logic.
- ASCII/binary data handling as required by the manual.
- Generator polynomial input, appending `n-1` zero bits, modulo-2 XOR division, remainder, transmitted codeword, and receiver-side verification.
- Complete source code, sample input/output, compile/run instructions, and line-by-line or block-wise explanation.
- Clearly explain that CRC detects errors; it does not itself correct the corrupted data.
- Keep C++ and Java programs functionally equivalent.

### Experiment 4 — Go-Back-N and Selective Repeat
Both guides must contain:
- Separate simulations for Go-Back-N ARQ and Selective Repeat ARQ.
- Sender window, receiver window, ACKs, timeout, lost-frame scenario, retransmission behaviour.
- Explain cumulative ACK and discarding out-of-order frames in Go-Back-N.
- Explain individual ACKs and buffering of out-of-order frames in Selective Repeat.
- Explain sequence number/window size constraints.
- Include a sample run showing the difference between the two protocols.
- Make the simulation deterministic and easy to demonstrate in a practical exam.

### Experiment 5 — TCP sockets
Both guides must cover all three tasks from the manual:
- Say Hello to Each Other.
- File Transfer.
- Arithmetic Calculator.

Provide separate server and client source files for each task. Use clear filenames, for example:
- `hello_server` / `hello_client`
- `file_server` / `file_client`
- `calculator_server` / `calculator_client`

For Java, use appropriate Java networking classes such as `ServerSocket` and `Socket`. For C++, use the appropriate POSIX socket API on Linux/macOS. Explain TCP connection establishment, server/client call sequence, IP address, port, and message exchange.

### Experiment 6 — UDP file transfer
Both guides must include:
- Separate sender and receiver programs.
- File reading/writing in binary mode so text and binary files are handled.
- Chunked transfer, sender/receiver IP and port, and clear run order.
- Explain `DatagramSocket`/`DatagramPacket` for Java and `SOCK_DGRAM`, `sendto`, and `recvfrom` for C++.
- Explain UDP's lack of guaranteed delivery, ordering, and retransmission.
- If implementing sequence numbers, acknowledgements, timeout, or retransmission beyond the manual's sample, label these as enhancements. Do not imply the manual's simple sample already provides reliability.
- Avoid relying only on a potentially losable end-marker without explaining the limitation.

### Experiment 7 — TCP chat
Both guides must cover:
- TCP peer-to-peer chat as specified in the manual.
- TCP multiuser chat with a server and multiple clients.
- Separate source files and instructions to run server before clients.
- Explain threading/concurrency, client handling, message broadcasting, ports, and graceful disconnect.
- Ensure code is runnable and not merely pseudocode.

### Experiment 8 — UDP chat
Both guides must cover:
- UDP peer-to-peer chat.
- UDP multiuser chat.
- Separate sender/receiver or server/client files as appropriate.
- Explain datagrams, IP/port addressing, `DatagramSocket`/`DatagramPacket` in Java, and `sendto`/`recvfrom` in C++.
- Explain the limitations of UDP and how the program handles client addresses.
- Ensure code is runnable and not merely pseudocode.

### Experiments 1 and 2 — network configuration
These are configuration/Packet Tracer experiments. Create `EXPERIMENT_GUIDE.txt` with:
- Aim, requirements, theory, topology diagram (ASCII or Mermaid), addressing table.
- Step-by-step physical/logical setup.
- Static IP configuration, subnet mask, gateway, routing where relevant.
- Windows resource sharing for Experiment 1.
- Wired and wireless LAN setup, router/WAN configuration, SSID and WPA2 security for Experiment 2.
- Ping/testing commands, expected results, troubleshooting, viva questions, examiner explanation, and conclusion.
- Do not fabricate Packet Tracer screenshots.

### Experiment 9 — Apache web server
Create an exceptionally beginner-friendly `EXPERIMENT_GUIDE.txt`. Assume I am new to Apache and Linux terminal.

Include:
1. What a web server is and how client-server HTTP works.
2. Apache installation on Ubuntu:
   - `sudo apt update`
   - `sudo apt install apache2`
3. Service control and verification:
   - `sudo systemctl status apache2`
   - `sudo systemctl start apache2`
   - `sudo systemctl enable apache2`
4. Default document root: `/var/www/html`.
5. How to create/edit `index.html` using `nano`.
6. How to verify locally using `http://localhost` and `curl`.
7. How to access from another machine on the same LAN using the server's IP address.
8. How to find the IP using `ip addr` or `hostname -I`.
9. Firewall instructions using UFW, with a note to expose only what is necessary:
   - `sudo ufw allow 'Apache'`
   - `sudo ufw status`
10. Important Apache paths, logs, configuration, and useful commands.
11. Troubleshooting: service not starting, port 80 occupied, firewall, wrong document root, permission problems.
12. WSL2 notes and an optional XAMPP alternative, clearly distinguished from the Ubuntu Apache procedure.
13. Expected browser output, viva questions, examiner explanation, and conclusion.

### Experiment 10 — Wireshark
Create `EXPERIMENT_GUIDE.txt` with:
- Installation and interface selection.
- Starting/stopping a capture.
- Display filters and capture filters, clearly distinguished.
- Useful filters: `tcp`, `http`, `ip.addr == x.x.x.x`, `tcp.flags.syn == 1`, `tcp.flags.push == 1`, `tcp.flags.reset == 1`.
- Explain source/destination IP, source/destination port, protocol, packet length, TCP flags, and packet details.
- Explain how to inspect TCP handshake.
- Explain that Facebook and most modern websites use HTTPS, so packet capture generally reveals metadata and TLS traffic, not readable encrypted page contents. Do not promise readable HTTP payloads for HTTPS sites.
- Viva questions, examiner explanation, troubleshooting, and conclusion.

## Mandatory format inside EACH language guide

Every `CPP_GUIDE.txt` and `JAVA_GUIDE.txt` must contain the following sections in this order:

1. Experiment title and number.
2. Aim.
3. Requirements (software, hardware, compiler/runtime).
4. Learning objectives.
5. Theory and important terminology.
6. Working principle / algorithm.
7. Network topology or flow diagram (ASCII/Mermaid where relevant).
8. Complete source code — include the full code in the guide and also save it as separate `.cpp` or `.java` files in the corresponding language folder.
9. Explanation of the code, including important functions/classes and statements.
10. Setup and installation instructions.
11. Exact compile/run commands.
12. Execution order (which terminal/machine runs first).
13. Sample input and expected output.
14. How to test and verify the result.
15. Common errors and troubleshooting.
16. Manual Code Note / Correction / Enhancement (if applicable).
17. Viva questions with concise, correct answers — at least 15 per programming experiment.
18. A short 30–60 second explanation I can speak to the examiner.
19. Quick revision points.
20. Result / conclusion.

For non-programming experiments, use `EXPERIMENT_GUIDE.txt` and include the equivalent relevant sections.

## Code quality and execution requirements

- All programs must be complete, compilable/runnable, and suitable for a college practical demonstration.
- Use clear variable names and beginner-friendly comments.
- Do not leave TODOs, placeholders, ellipses, or pseudocode in place of required logic.
- Handle invalid input and basic runtime errors.
- Use sensible ports and document them consistently.
- Avoid hardcoding a machine-specific IP address. Explain localhost testing and how to replace it with the server's LAN IP for two-machine testing.
- For socket programs, clearly state whether the code is for Linux/macOS POSIX sockets, Windows Winsock, or Java. Do not mix platform-specific APIs.
- For C++ socket code on Linux/macOS, include required headers and compile commands. Mention any platform differences accurately.
- For Java, include the required JDK version, `javac` and `java` commands, and class/file naming rules.
- Do not include external libraries unless necessary. Prefer standard libraries.
- Never claim a program was tested unless it was actually compiled/run. Add a `TEST STATUS` line: `Tested successfully`, `Not tested`, or `Tested with limitation`, and state the environment.
- If testing tools are unavailable, state exactly what remains unverified.
- Keep the examples consistent across both languages so I can compare them.

## Root README and index

`README.md` should explain:
- What this repository contains.
- Why there are separate C++ and Java guides.
- Which experiments are tool/configuration based.
- How to navigate the folders.
- How to compile/run each language at a high level.
- That the uploaded lab manual is the source of the official experiment requirements.

`EXPERIMENT_INDEX.txt` should list all 10 experiments and the exact guide filenames available in each folder.

`QUICK_START.txt` should give a short practical-day checklist:
- Which programs to compile first.
- Which server to start first.
- How to run clients.
- How to test with localhost and two machines.
- How to revise viva questions quickly.

## Final validation before finishing

1. Inspect the existing repository and preserve all useful files, especially existing Java programs.
2. Compare every experiment against the uploaded lab manual.
3. Verify that each programming experiment has separate C++ and Java guide files.
4. Verify that the source files match the code shown in their guides.
5. Compile every C++ program where the required compiler/environment is available.
6. Compile every Java program with `javac` where the JDK is available.
7. Run basic demonstrations where feasible.
8. Check that client and server ports match.
9. Check filenames and Java public class names match.
10. Fix broken commands, missing imports/headers, and inconsistent filenames.
11. Do not push to GitHub, publish, or make a remote repository change unless I explicitly authorize it.
12. At the end, provide a concise report listing:
    - Files created/updated.
    - Existing Java code preserved.
    - C++ programs added.
    - Experiments not applicable to either language.
    - Programs actually compiled/tested and environment.
    - Any remaining limitations.

**Important:** The goal is a clean, GitHub-ready educational repository with two independent, complete learning guides for each programming experiment: one for C++ and one for Java. Preserve the work already done in Java, and add the C++ version based on the lab manual.
