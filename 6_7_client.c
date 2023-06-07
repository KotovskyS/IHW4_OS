#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void encrypt(const char *input, char *output, int fragment_size) {
    for (int i = 0; i < fragment_size; i++) {
        output[i] = input[i] + 3;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Запускайте так: %s <Server IP Address> <Server Port>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in server_addr;
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    char init_msg = 'i';
    socklen_t server_addr_len = sizeof(server_addr);
    sendto(client_socket, &init_msg, sizeof(init_msg), 0, (struct sockaddr *) &server_addr, server_addr_len);
    int fragment_start, fragment_size;
    char input[4096], output[4096];
    recvfrom(client_socket, &fragment_start, sizeof(fragment_start), 0, (struct sockaddr *) &server_addr,
             &server_addr_len);
    recvfrom(client_socket, &fragment_size, sizeof(fragment_size), 0, (struct sockaddr *) &server_addr,
             &server_addr_len);
    recvfrom(client_socket, input, fragment_size, 0, (struct sockaddr *) &server_addr, &server_addr_len);
    encrypt(input, output, fragment_size);
    sendto(client_socket, output, fragment_size, 0, (struct sockaddr *) &server_addr, server_addr_len);
    close(client_socket);
    return 0;
}

