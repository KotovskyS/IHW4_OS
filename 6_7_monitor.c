#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <порт>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in server_addr;
    int monitor_socket;
    if ((monitor_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }
    int broadcastEnable = 1;
    setsockopt(monitor_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(monitor_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    char buffer[4096];
    while (true) {
        int bytesReceived = recv(monitor_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            perror("Recv failed");
            break;
        }
        buffer[bytesReceived] = '\0';
        printf("%s\n", buffer);
    }
    close(monitor_socket);
    return 0;
}

