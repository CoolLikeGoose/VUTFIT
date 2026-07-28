#include <iostream>
#include <string>
// #include <sys/socket.h>
// #include <netinet/in.h>
#include <unistd.h>

int main() {
    // int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_fd < 0) {
    //     perror("socket");
    //     return 1;
    // }

    // sockaddr_in addr{};
    // addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = INADDR_ANY;
    // addr.sin_port = htons(4318);

    // if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
    //     perror("bind");
    //     return 1;
    // }

    // listen(server_fd, 5);
    // std::cout << "Fake OTEL endpoint running on port 4318..." << std::endl;

    // while (true) {
    //     int client = accept(server_fd, nullptr, nullptr);
    //     if (client < 0) continue;

    //     char buffer[4096];
    //     ssize_t n = read(client, buffer, sizeof(buffer) - 1);
    //     if (n > 0) {
    //         buffer[n] = '\0';
    //         std::cout << "\n--- Received OTEL data ---\n"
    //                   << buffer
    //                   << "\n---------------------------\n";
    //     }

    //     const char *response =
    //         "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    //     write(client, response, strlen(response));
    //     close(client);
    // }

    // return 0;
}
