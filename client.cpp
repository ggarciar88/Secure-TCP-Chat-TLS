#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes = recv(sock, buffer, 1024, 0);
        if (bytes <= 0) break;
        std::cout << buffer << std::endl;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    std::thread(receive_messages, sock).detach();

    std::string message;
    while (std::getline(std::cin, message)) {
        send(sock, message.c_str(), message.size(), 0);
    }
    close(sock);
    return 0;
}
