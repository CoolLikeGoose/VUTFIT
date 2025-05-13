# ipk24chat-client

  * [Summary](#summary)
  * [Theory](#theory)
  * [Design Overview](#design-overview)
  * [Implementation Details](#implementation-details)

[//]: # (  * [Testing]&#40;#testing&#41;)

[//]: # (  * [Bibliography]&#40;#bibliography&#41;)

## Summary
The client-side application is designed to facilitate communication with a server
using both TCP and UDP protocols. Operating within a multithreaded environment, 
it efficiently manages data reading and writing tasks. Key features include robust 
authentication mechanisms and seamless channel entry for effective communication.

#### Key Features:
- **TCP and UDP Communication:** Utilizes both TCP and UDP protocols to establish reliable and efficient communication channels with the server.
- **Multithreaded Operation:** Implements multithreading to handle concurrent tasks such as data reading and writing, ensuring smooth and responsive operation.
- **Authentication Mechanisms:** Provides robust authentication mechanisms to ensure secure access to server resources.
- **Channel Management:** Facilitates seamless entry into communication channels, enabling users to engage in conversations effortlessly.


## Theory

### TCP (Transmission Control Protocol)
TCP is a reliable, connection-oriented protocol used for communication between client and server applications. Here's how it works in the context of our messenger application:

- **Connection Establishment:** When a client wants to communicate with the server, it 
initiates a TCP connection using a three-way handshake. This ensures a reliable and ordered 
exchange of data.
- **Reliable Messaging:** TCP guarantees the delivery of messages in the correct order and 
handles retransmissions and acknowledgments to ensure reliability.
- **User Interaction:** In our messenger application, TCP is used for critical operations
like authentication, joining chat channels, and sending messages. It provides a stable and 
reliable channel for these interactions.

### UDP (User Datagram Protocol)
UDP is a lightweight, connectionless protocol suitable for real-time communication and broadcasting. Here's how it's utilized in our messenger application:

- **Message Broadcasting:** UDP is used for broadcasting messages in real-time chat channels, 
where immediate delivery is prioritized over reliability.
- **Unreliable Messaging:** Unlike TCP, UDP does not guarantee delivery or order of messages. 
This is acceptable for non-critical messages like chat messages in our application.
- **User Interaction:** Users can send and receive messages in real-time chat channels using UDP.
While it lacks the reliability of TCP, it offers low latency and minimal overhead.

### Protocol on top of TCP and UDP
**IPK24-CHAT Protocol:** Our application utilizes the IPK24-CHAT protocol, defining message 
types like CONFIRM, REPLY, AUTH, JOIN, MSG, ERR, and BYE. These messages play a pivotal role 
in facilitating key interactions between client and server. They enable functionalities such
as user authentication, management of chat channels, seamless exchange of messages, and 
effective error handling.


## Design Overview
### Modular Structure
The application is organized into modules to enhance maintainability and scalability. Each module encapsulates specific functionalities, promoting code organization and reusability.

### Protocol Agnosticism
The design minimizes dependency on specific network protocols. Components adapt seamlessly to different protocols, ensuring efficient communication.
### Message Handling
Messages efficiently store and process data, adapting encoding and decoding based on the protocol.
### Networking Abstraction
The networking layer abstracts support for multiple protocols (UDP, TCP) with protocol-specific logic encapsulated.
### Asynchronous Task Management
An asynchronous task manager enables concurrent execution of network operations, enhancing performance.



## Implementation Details
The application is developed using C# .NET 8 framework. Standard SDK tools were utilized, with the addition of the Mono.Options library for streamlined argument parsing.
### Libraries Used
 - Mono.Options: Employed for enhanced argument parsing, facilitating smooth configuration management.

### Networking Module
#### IProtocolHandler.cs: 
Defines the interface for protocol-specific handlers, allowing for easy integration of different protocols.

#### NetworkManager.cs: 
This central component orchestrates network operations, providing a unified interface for sending and receiving data over the network. It abstracts the underlying protocol details and delegates protocol-specific operations to the appropriate handlers.

#### TcpHandler.cs: 
Implements the protocol handler for TCP communication. It encapsulates logic for establishing and managing TCP connections, sending and receiving data over TCP sockets.

#### UdpHandler.cs: 
Implements the protocol handler for UDP communication. It manages UDP sockets, enabling efficient transmission of datagrams over the network.

### Other Modules
#### ArgParse.cs: 
This module is responsible for parsing command-line arguments, providing a convenient way to configure the application's behavior through command-line inputs.

#### AsyncManager.cs: 
The asynchronous task manager facilitates concurrent execution of tasks, allowing for parallel processing of network operations without blocking the main thread.

#### InputManager.cs: 
Manages user input, ensuring seamless interaction with the application by handling input validation and processing.

#### ResponseManager.cs: 

This component handles incoming responses from the network, processing and presenting them to the user in a user-friendly format.
### Message Models
Within the Messages module, various message models are implemented, all inheriting from the abstract class Message. Each model is equipped with constructors that accommodate different scenarios and data types, such as byte arrays for UDP communication and strings for TCP communication. Additionally, encoding and decoding functions are provided for each model, tailored to specific protocols. These functions ensure efficient data exchange by adapting to the unique requirements of each protocol.
The design of the Messages module promotes flexibility and interoperability, enabling seamless communication across diverse network protocols.

[//]: # (## Testing)
[//]: # (The testing section outlines the approach taken to validate and verify the implemented functionality. It includes details on test cases, test scenarios, and the results of testing to ensure the reliability and robustness of the application.)

[//]: # (## Bibliography)
[//]: # (The bibliography provides a list of all sources used during the development of the application. It includes references to relevant textbooks, articles, and online resources consulted for theoretical background, implementation techniques, or troubleshooting.)
https://git.fit.vutbr.cz/xsmirn02/IPK-CHAT
