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

    sendto(sock, "REGISTER", 8, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[BUFFER_SIZE];
    int score = 0;

    while (1) {
        struct sockaddr_in from_addr;
        socklen_t from_len = sizeof(from_addr);
        int bytes = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, 
                            (struct sockaddr*)&from_addr, &from_len);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        if (strncmp(buffer, "MATCH", 5) == 0) {
            int opp_id, points;
            char your_choice[10], opp_choice[10], result[10];
            sscanf(buffer, "MATCH %d %s %s %s %d", &opp_id, your_choice, 
                  opp_choice, result, &points);
            printf("vs %d: %s vs %s - %s (+%d)\n", opp_id, your_choice, 
                  opp_choice, result, points);
            score += points;
        } else if (strncmp(buffer, "RANKING", 7) == 0) {
            printf("Final Ranking:%s\n", buffer + 7);
        } else if (strncmp(buffer, "END", 3) == 0) {
            sscanf(buffer, "END %d", &score);
            printf("Final score: %d\n", score);
            break;
        }
    }

    close(sock);
    return 0;
}