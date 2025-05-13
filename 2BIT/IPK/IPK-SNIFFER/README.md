# IPK-SNIFFER

* [Summary](#summary)
* [Theory](#theory)
* [Design Overview](#design-overview)
* [Implementation Details](#implementation-details)
* [Testing](#testing)
* [Bibliography](#bibliography)

## Summary 
IPK Sniffer is a lightweight network packet sniffer implemented in C# using SharpPCAP. It allows you to capture and analyze network traffic on your machine.

#### Key Features:

- **Sniffing around the different interfaces:** show the available interfaces with `-i`

- **Filtering by port:** specify the desired port with `-port|--port`, `--port-destination` or `--port-source`

- **Sniffing a certain number of packets:** specify the desired number of packets using `-n`(default is 1)

- **Support and filter for different types of packets:** currently available packages include: `TCP`, `UDP`, `NDP`, `ARP`, `ICMPv4`, `ICMPv6`, `MLD`, `IGMP`. 


## Theory
### TCP (Transmission Control Protocol)
- TCP is a connection-oriented protocol used for reliable, ordered, and error-checked delivery of data packets over a network.
- Uses `IP addresses` and `ports` for communication.
- Provides reliable, ordered delivery of a stream of bytes.

### UDP (User Datagram Protocol)
- UDP is a connectionless protocol that sends datagrams (packets) of data without requiring a connection.
- Uses `IP addresses` and `ports` for communication.
- Provides fast, lightweight communication but does not guarantee delivery or order.

### NDP (Neighbor Discovery Protocol)
- NDP is part of the ICMPv6 protocol suite and is used by IPv6 nodes to discover link-layer addresses (MAC addresses) of neighboring nodes.
- Uses `IPv6 addresses` and `MAC addresses` for communication.

### ARP (Address Resolution Protocol)
- ARP is used to map IP addresses to MAC addresses on a local network.
- Uses `IP addresses` and `MAC addresses` for communication.

### ICMPv4 (Internet Control Message Protocol version 4)
- ICMPv4 is used for diagnostic and control purposes in IP networks.
- Uses `IP addresses` for communication.

### ICMPv6 (Internet Control Message Protocol version 6)
- ICMPv6 is the IPv6 version of ICMP and serves similar purposes as ICMPv4.
- Uses `IPv6 addresses` for communication.

### MLD (Multicast Listener Discovery)
- MLD is used by IPv6 routers to discover multicast listeners on a directly attached link.
- Uses `IPv6 addresses` for communication.

### IGMP (Internet Group Management Protocol)
- IGMP is used by IPv4 hosts and adjacent multicast routers to establish multicast group memberships.
- Uses `IP addresses` for communication.


## Design Overview

### Modular Structure

The application is organized in modules to improve maintainability and scalability. The program can be divided into important modules such as:

- **ArgParse:** responsible for parsing command-line arguments passed to the program. It extracts relevant options and parameters provided by the user, such as interface selection, protocol and port filters.

- **PacketSniffer:** is the core component responsible for capturing network packets from the specified interface. It utilizes low-level network APIs to intercept incoming packets and forwards them to the PacketParser for further processing.

- **PacketParser:** responsible for parsing the raw packet data received from the PacketSniffer. It analyzes the packet headers and extracts relevant information such as source and destination addresses, protocol type, and payload data.

- **Packets Implementation(PacketObject files):** PacketHandlers module contains implementations for handling different types of network packets. Each packet type (e.g., TCP, UDP, ICMP) has its handler responsible for customizing the output format for that specific packet type. 

This modular approach allows for easy customization and extension of the program's functionality.

### Protocol Agnosticism

The `BaseClass` class contains a `ParsePacket` method that implements the factory pattern in some way.
So methods that work at higher levels of abstraction don't have to worry about what package 
they are working with.  Except for the `PacketParser` class, which is responsible for 
processing `RawCapture`.

The design minimizes dependency on specific network protocols. 
Components adapt seamlessly to different protocols, ensuring efficient communication.
As a result, the application can be easily extended with other protocols.

### Asynchronous Task Management
Asynchronous operation is the responsibility of the AsyncManager which implements the singleton pattern.

Originally the program was supposed to have asynchronous methods, 
which however was not required in the task, but the program can still be used asynchronously 
if you extend the functionality to listen to multiple interfaces or ports simultaneously with
output to a single thread. 

Since the `Console.WriteLine()` method is a thread-safe method in C#, 
no errors should occur because the different instances of the `PacketSniffer` class do not use the
same data except for the `ArgParse` class. 

A possible solution is to send a copy of the `ArgParse
instance` to each `PacketSniffer instance`.




## Implementation Details

The application is developed using C# .NET Core using the Rider IDE.
Development was performed on Windows platform and testing was mainly done in WSL.

### Libraries Used
Standard SDK tools were utilized, with the addition of:
- **Mono.Options:** library for enhanced argument parsing, facilitating smooth configuration management.
- **SharpPcap:** library for capturing and processing network packets.

### Additional tools used in the development
- **Python:** scapy library has been used to test various packages.
- **NetCat:** was used at the beginning of testing to check TCP and UDP packets.
- **WireShark:** was used to track packets and to verify the correctness of the written tests.
- **tcpdump:** has been used to track packages in linux.

### Modules
As mentioned above the code is divided into several modules among which we can highlight such as:

- **ArgParse** Provides basic argument parsing and a place to store arguments for further 
processing.  If protocols are added, should be updated:
  - `OptionSet` 
  - `Help information`
  
- **PacketSniffer**
  - Initializes the interface to receive packets
  - Implements a function to display all available interfaces
  - Implements a function to search for an interface among the available ones by name

- **PacketParser**
  - Provides a function for `RawCapture` parsing
  - Uses an internal `ExtractPacket()` function that extracts from a packet its `IPv4/IPv6` protocol
  information and a packet with protocol-specific information
  - Filters packet output based on the filters given by the user

- **BasePacket and (Specific PacketObjects)**
  - Provides a `ParsePacket()` function that returns one of the instantiations of the BasePacket child classes.
  - BasePacket child classes implement `override string ToString()` which allows converting
  BasePacket classes to String for output, also thanks to this output for different packages
  output can be customized on needs.

## Testing
### Testing environment
The `WSL` environment was used for testing.

It is assumed that the program is run with sufficient permissions to listen to the network interface in mode Promiscuous

#### Manual testing
Manual testing can be done by sending packages manually using tools such as netcat
or using python cli.
#### Automatic testing
Attempts were made to write fully automated tests however, when using Python,
the program running in a separate thread unfortunately couldn't always capture the packet.
The reason for this was not determined, and attempts to write fully automated testing
were abandoned, as they are not a priority due to time constraints.
#### Semi-automatic testing
For semi-automatic testing, you must run the ipk-sniffer in different terminal/window.

The tests described in tests.py and those that will be further used in testing.
When launched without arguments, sends packets of all types in a row. 
Also, you can specify an argument, as a result of which only a packet of a specific type will be sent.


### Test cases:

#### Arguments tests
**Objective:** Make sure that all arguments are parsed correctly
**Testing Format:**
- Line with sniffer console starting
- Sniffer output
- Expected output

**No arguments**

> Sniffer starting - `sudo ./ipk-sniffer`

>Output: 
```
eth0
any
lo
bluetooth-monitor
nflog
nfqueue
dbus-system
dbus-session
```

>Expected output:
```
{List of avaible devices}
```

**Only -i without interface name**

> Sniffer starting - `sudo ./ipk-sniffer -i`

>Output:
```
eth0
any
lo
bluetooth-monitor
nflog
nfqueue
dbus-system
dbus-session
```

>Expected output:
```
{List of avaible devices}
```

**Only -i with interface name**

> Sniffer starting - `sudo ./ipk-sniffer -i any`

>Output:
```
timestamp: 2024-04-22T20:02:15.436+00:00
frame length: 89
src IP: 172.30.224.1
dst IP: 224.0.0.251
src port: 5353
dst port: 5353

0x0000: 00 02 00 01 00 06 00 15 5D E3 FD 31 00 00 08 00         ........]ãý1....
0x0010: 45 00 00 49 8A F5 00 00 FF 11 C3 92 AC 1E E0 01         E..I.õ..ÿ.Ã.¬.à.
0x0020: E0 00 00 FB 14 E9 14 E9 00 35 41 57 00 00 00 00         à..û.é.é.5AW....
0x0030: 00 01 00 00 00 00 00 00 10 5F 73 70 6F 74 69 66         ........._spotif
0x0040: 79 2D 63 6F 6E 6E 65 63 74 04 5F 74 63 70 05 6C         y-connect._tcp.l
0x0050: 6F 63 61 6C 00 00 0C 00 01                              ocal.....
```

>Expected output:
```
{any packet}
```

**Port argument**

> Sniffer starting - `sudo ./ipk-sniffer -i any -p 3333`

> Test - `nc -u localhost 3333` with value `TESTING`

>Output:
```
timestamp: 2024-04-22T20:04:41.921+00:00
frame length: 52
src IP: 127.0.0.1
dst IP: 127.0.0.1
src port: 38077
dst port: 3333

0x0000: 00 00 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 24 3B 2F 40 00 40 11 01 98 7F 00 00 01         E..$;/@.@.......
0x0020: 7F 00 00 01 94 BD 0D 05 00 10 FE 23 54 45 53 54         .....½....þ#TEST
0x0030: 49 4E 47 0A                                             ING.
```
>Expected output:
```
{any packet} with DATA contains "TESTING" AND port=3333
```

> Test - `nc -u localhost 3433` with value `TESTING`

>Output:
```
```
>Expected output:
```
NONE or {any packet} with port=3333
```

#### Packets tests
**Objective:** Ensure that all packets are received and processed correctly depending on their types.
**Testing Format:**
- Line with sniffer console starting
- Line with test console starting
- Sniffer output
- Expected output

**TCP**

> Sniffer starting - `sudo ./ipk-sniffer -i any --tcp` 

> Tests starting - `sudo python3 ./tests.py tcp` 

>Output:
```
timestamp: 2024-04-22T19:27:32.490+00:00
frame length: 91
src IP: 127.0.0.1
dst IP: 127.0.0.1
src port: 20
dst port: 4567
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 4B 00 01 00 00 40 06 7C AA 7F 00 00 01         E..K....@.|ª....
0x0020: 7F 00 00 01 00 14 11 D7 00 00 00 00 00 00 00 00         .......×........
0x0030: 50 02 20 00 84 70 00 00 47 45 54 20 2F 20 48 54         P. ..p..GET / HT
0x0040: 54 50 2F 31 2E 31 0D 0A 48 6F 73 74 3A 20 6C 6F         TP/1.1..Host: lo
0x0050: 63 61 6C 68 6F 73 74 0D 0A 0D 0A                        calhost....
```
>Expected output:
```
timestamp: {any}
frame length: 91
src IP: 127.0.0.1
dst IP: 127.0.0.1
src port: 20
dst port: 4567
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 4B 00 01 00 00 40 06 7C AA 7F 00 00 01         E..K....@.|ª....
0x0020: 7F 00 00 01 00 14 11 D7 00 00 00 00 00 00 00 00         .......×........
0x0030: 50 02 20 00 84 70 00 00 47 45 54 20 2F 20 48 54         P. ..p..GET / HT
0x0040: 54 50 2F 31 2E 31 0D 0A 48 6F 73 74 3A 20 6C 6F         TP/1.1..Host: lo
0x0050: 63 61 6C 68 6F 73 74 0D 0A 0D 0A                        calhost....
```

**UDP**

> Sniffer starting - `sudo ./ipk-sniffer -i any --udp`

> Tests starting - `sudo python3 ./tests.py udp`

>Output:
```
timestamp: 2024-04-22T19:36:06.967+00:00
frame length: 52
src IP: 127.0.0.1
dst IP: 127.0.0.1
src port: 53
dst port: 12345
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 24 00 01 00 00 40 11 7C C6 7F 00 00 01         E..$....@.|Æ....
0x0020: 7F 00 00 01 00 35 30 39 00 10 A4 DE 54 65 73 74         .....509..¤ÞTest
0x0030: 20 55 44 50                                             .UDP
```
>Expected output:
```
timestamp: {any}
frame length: 52
src IP: 127.0.0.1
dst IP: 127.0.0.1
src port: 53
dst port: 12345
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 24 00 01 00 00 40 11 7C C6 7F 00 00 01         E..$....@.|Æ....
0x0020: 7F 00 00 01 00 35 30 39 00 10 A4 DE 54 65 73 74         .....509..¤ÞTest
0x0030: 20 55 44 50                                             .UDP
```

**NDP**

> Sniffer starting - `sudo ./ipk-sniffer -i any --ndp`

> Tests starting - `sudo python3 ./tests.py ndp`

>Output:
```
timestamp: 2024-04-22T19:37:16.657+00:00
frame length: 80
src IP: ::1
dst IP: ::1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 86 DD         ...............Ý
0x0010: 60 00 00 00 00 18 3A FF 00 00 00 00 00 00 00 00         `.....:ÿ........
0x0020: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00         ................
0x0030: 00 00 00 00 00 00 00 01 87 00 78 AA 00 00 00 00         ..........xª....
0x0040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01         ................
```
>Expected output:
```
timestamp: {any}
frame length: 80
src IP: ::1
dst IP: ::1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 86 DD         ...............Ý
0x0010: 60 00 00 00 00 18 3A FF 00 00 00 00 00 00 00 00         `.....:ÿ........
0x0020: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00         ................
0x0030: 00 00 00 00 00 00 00 01 87 00 78 AA 00 00 00 00         ..........xª....
0x0040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01         ................
```

**ARP**

> Sniffer starting - `sudo ./ipk-sniffer -i any --arp`

> Tests starting - `sudo python3 ./tests.py arp`

>Output:
```
timestamp: 2024-04-22T19:38:37.167+00:00
src MAC: 00:00:00:00:00:00
dst MAC: 00:00:00:00:00:00
frame length: 44
src IP: 127.0.0.1
dst IP: 127.0.0.1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 06         ................
0x0010: 00 01 08 00 06 04 00 01 00 00 00 00 00 00 7F 00         ................
0x0020: 00 01 00 00 00 00 00 00 7F 00 00 01                     ............
```
>Expected output:
```
timestamp: {any}
src MAC: 00:00:00:00:00:00
dst MAC: 00:00:00:00:00:00
frame length: 44
src IP: 127.0.0.1
dst IP: 127.0.0.1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 06         ................
0x0010: 00 01 08 00 06 04 00 01 00 00 00 00 00 00 7F 00         ................
0x0020: 00 01 00 00 00 00 00 00 7F 00 00 01   
```

**ICMPv6**

> Sniffer starting - `sudo ./ipk-sniffer -i any --icmp6`

> Tests starting - `sudo python3 ./tests.py icmp6`

>Output:
```
timestamp: 2024-04-22T19:39:31.068+00:00
frame length: 64
src IP: ::1
dst IP: ::1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 86 DD         ...............Ý
0x0010: 60 00 00 00 00 08 3A 40 00 00 00 00 00 00 00 00         `.....:@........
0x0020: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00         ................
0x0030: 00 00 00 00 00 00 00 01 80 00 7F BB 00 00 00 00         ...........»....
```
>Expected output:
```
timestamp: {any}
frame length: 64
src IP: ::1
dst IP: ::1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 86 DD         ...............Ý
0x0010: 60 00 00 00 00 08 3A 40 00 00 00 00 00 00 00 00         `.....:@........
0x0020: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00         ................
0x0030: 00 00 00 00 00 00 00 01 80 00 7F BB 00 00 00 00         ...........»....
```

**ICMPv4**

> Sniffer starting - `sudo ./ipk-sniffer -i any --icmp4`

> Tests starting - `sudo python3 ./tests.py icmp4`

>Output:
```
timestamp: 2024-04-22T19:40:13.007+00:00
frame length: 44
src IP: 127.0.0.1
dst IP: 127.0.0.1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 1C 00 01 00 00 40 01 7C DE 7F 00 00 01         E.......@.|Þ....
0x0020: 7F 00 00 01 08 00 F7 FF 00 00 00 00                     ......÷ÿ....
```
>Expected output:
```
timestamp: {any}
frame length: 44
src IP: 127.0.0.1
dst IP: 127.0.0.1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 1C 00 01 00 00 40 01 7C DE 7F 00 00 01         E.......@.|Þ....
0x0020: 7F 00 00 01 08 00 F7 FF 00 00 00 00                     ......÷ÿ....
```

**MLD**

> Sniffer starting - `sudo ./ipk-sniffer -i any --mld`

> Tests starting - `sudo python3 ./tests.py mld`

>Output:
```
timestamp: 2024-04-22T19:41:13.007+00:00
frame length: 84
src IP: ::1
dst IP: ::1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 86 DD         ...............Ý
0x0010: 60 00 00 00 00 1C 3A 01 00 00 00 00 00 00 00 00         `.....:.........
0x0020: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00         ................
0x0030: 00 00 00 00 00 00 00 01 8F 00 6D A3 00 00 00 00         ..........m£....
0x0040: 04 00 00 00 FF 02 00 00 00 00 00 00 00 00 00 00         ....ÿ...........
0x0050: 00 00 00 01                                             ....
```
>Expected output:
```
timestamp: {any}
frame length: 84
src IP: ::1
dst IP: ::1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 86 DD         ...............Ý
0x0010: 60 00 00 00 00 1C 3A 01 00 00 00 00 00 00 00 00         `.....:.........
0x0020: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00         ................
0x0030: 00 00 00 00 00 00 00 01 8F 00 6D A3 00 00 00 00         ..........m£....
0x0040: 04 00 00 00 FF 02 00 00 00 00 00 00 00 00 00 00         ....ÿ...........
0x0050: 00 00 00 01   
```

**IGMP**

> Sniffer starting - `sudo ./ipk-sniffer -i any --igmp`

> Tests starting - `sudo python3 ./tests.py igmp`

>Output:
```
timestamp: 2024-04-22T19:43:43.997+00:00
frame length: 44
src port: 127.0.0.1
dst port: 127.0.0.1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 1C 00 01 00 00 01 02 BB DD 7F 00 00 01         E.........»Ý....
0x0020: 7F 00 00 01 11 14 EE EB 00 00 00 00                     ......îë....
```
>Expected output:
```
timestamp: {any}
frame length: 44
src port: 127.0.0.1
dst port: 127.0.0.1
0x0000: 00 02 03 04 00 06 00 00 00 00 00 00 00 00 08 00         ................
0x0010: 45 00 00 1C 00 01 00 00 01 02 BB DD 7F 00 00 01         E.........»Ý....
0x0020: 7F 00 00 01 11 14 EE EB 00 00 00 00                     ......îë....
```

## Bibliography
Github. Official repository of SharpPcap [online]. [cited 2024-04-22]. Available at: https://github.com/dotpcap/sharppcap

Github. TCPDUMP/LIBPCAP public repository [online]. [cited 2024-04-22]. Available at: http://www.tcpdump.org/

G. Klyne. Date and Time on the Internet: Timestamps [online]. July 2002. [cited 2024-04-22]. RFC 3339. Available at: https://www.rfc-editor.org/rfc/rfc3339