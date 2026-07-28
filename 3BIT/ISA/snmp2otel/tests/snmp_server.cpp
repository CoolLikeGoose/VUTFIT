#include <iostream>
#include <cstring>
// #include <arpa/inet.h>
#include <unistd.h>
// #include <net

int main() {
    // int sock = socket(AF_INET, SOCK_DGRAM, 0);
    // if (sock < 0) {
    //     perror("socket");
    //     return 1;
    // }

    // sockaddr_in server{};
    // server.sin_family = AF_INET;
    // server.sin_port = htons(16100);
    // server.sin_addr.s_addr = INADDR_ANY;

    // if (bind(sock, (sockaddr*)&server, sizeof(server)) < 0) {
    //     perror("bind");
    //     return 1;
    // }

    // std::cout << "Fake SNMP server running on UDP/16100..." << std::endl;

    // char buffer[1024];
    // sockaddr_in client{};
    // socklen_t len = sizeof(client);

    // while (true) {
    //     ssize_t n = recvfrom(sock, buffer, sizeof(buffer), 0,
    //                          (sockaddr*)&client, &len);
    //     if (n > 0) {
    //         std::cout << "[Received SNMP request] " << n << " bytes" << std::endl;
    //         // Fake response (echo)
    //         sendto(sock, buffer, n, 0, (sockaddr*)&client, len);
    //     }
    // }

    // close(sock);
    // return 0;
}
