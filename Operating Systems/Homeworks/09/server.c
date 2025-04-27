#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 2) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on %s:%d\n", server_ip, server_port);

    int client1_socket = -1;
    int client2_socket = -1;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (client1_socket == -1 || client2_socket == -1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        char client_type[10];
        int bytes_received = recv(client_socket, client_type, sizeof(client_type) - 1, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            continue;
        }
        client_type[bytes_received] = '\0';

        if (strcmp(client_type, "client1") == 0) {
            if (client1_socket == -1) {
                client1_socket = client_socket;
                printf("Client1 connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            } else {
                printf("Client1 already connected. Rejecting.\n");
                close(client_socket);
            }
        } else if (strcmp(client_type, "client2") == 0) {
            if (client2_socket == -1) {
                client2_socket = client_socket;
                printf("Client2 connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            } else {
                printf("Client2 already connected. Rejecting.\n");
                close(client_socket);
            }
        } else {
            printf("Unknown client type: %s\n", client_type);
            close(client_socket);
        }
    }

    printf("Both clients connected. Starting message relay.\n");

    char buffer[MAX_BUFFER];
    int read_size;

    while ((read_size = recv(client1_socket, buffer, MAX_BUFFER, 0)) > 0) {
        buffer[read_size] = '\0';
        send(client2_socket, buffer, read_size, 0);

        if (strstr(buffer, "The End") != NULL) {
            printf("Termination message received. Exiting.\n");
            break;
        }
    }

    close(client1_socket);
    close(client2_socket);
    close(server_socket);
    return 0;
}