#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2]))
    };
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    sendto(sock, "MONITOR", 7, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Monitor connected. Waiting for data...\n");
    char buffer[BUFFER_SIZE];

    while (1) {
        struct sockaddr_in from_addr;
        socklen_t from_len = sizeof(from_addr);
        int bytes = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, 
                            (struct sockaddr*)&from_addr, &from_len);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        if (strncmp(buffer, "FULLMATCH", 9) == 0) {
            int p1, p2;
            char c1[10], c2[10], res[10];
            sscanf(buffer, "FULLMATCH %d %d %s %s %s", &p1, &p2, c1, c2, res);
            printf("Match: %d (%s) vs %d (%s) -> %s\n", p1, c1, p2, c2, res);
        } else if (strncmp(buffer, "RANKING", 7) == 0) {
            printf("Current Ranking:%s\n", buffer + 7);
        } else if (strncmp(buffer, "END", 3) == 0) {
            printf("Tournament ended\n");
            break;
        }
    }

    close(sock);
    return 0;
}