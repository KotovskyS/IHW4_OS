#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Использование: %s <IP-адрес> <порт> <текст> <количество клиентов>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in server_addr, client_addr;
    int server_socket;
    int num_clients = atoi(argv[4]);
    char encrypted_text[4096];
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    socklen_t client_addr_len = sizeof(client_addr);
    for (int i = 0; i < num_clients; i++) {
        char init_msg;
        recvfrom(server_socket, &init_msg, sizeof(init_msg), 0, (struct sockaddr *) &client_addr, &client_addr_len);
        int fragment_start = (strlen(argv[3]) / num_clients) * i;
        int fragment_size = (i == num_clients - 1) ? (strlen(argv[3]) - fragment_start) : (strlen(argv[3]) /
                                                                                           num_clients);
        sendto(server_socket, &fragment_start, sizeof(fragment_start), 0, (struct sockaddr *) &client_addr,
               client_addr_len);
        sendto(server_socket, &fragment_size, sizeof(fragment_size), 0, (struct sockaddr *) &client_addr,
               client_addr_len);
        sendto(server_socket, argv[3] + fragment_start, fragment_size, 0, (struct sockaddr *) &client_addr,
               client_addr_len);

        recvfrom(server_socket, encrypted_text + fragment_start, fragment_size, 0, (struct sockaddr *) &client_addr,
                 &client_addr_len);
    }
    close(server_socket);
    encrypted_text[strlen(argv[3])] = '\0';
    printf("Зашифрованный текст: %s\n", encrypted_text);
    return 0;
}

