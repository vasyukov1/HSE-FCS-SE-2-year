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
        printf("Usage: %s <SERVER_IP> <SERVER_PORT>\n", argv[0]);
        exit(1);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(client_socket);
        exit(1);
    }

    send(client_socket, "client1", 7, 0);

    printf("Connected as Client1. Enter messages:\n");

    char buffer[MAX_BUFFER];

    while (1) {
        if (fgets(buffer, MAX_BUFFER, stdin) == NULL) {
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);

        if (strstr(buffer, "The End") != NULL) {
            break;
        }
    }

    close(client_socket);
    return 0;
}