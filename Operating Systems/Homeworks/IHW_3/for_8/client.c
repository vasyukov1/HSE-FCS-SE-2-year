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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2]))
    };

    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    send(sock, "PLAYER\n", 7, 0);
    
    char buffer[BUFFER_SIZE];
    char accum[BUFFER_SIZE * 2] = {0};
    size_t accum_len = 0;
    int score = 0;

    while (1) {
        int bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        strncat(accum, buffer, sizeof(accum) - accum_len - 1);
        accum_len += bytes;

        char *line_end;
        while ((line_end = strchr(accum, '\n')) != NULL) {
            *line_end = '\0';

            if (strncmp(accum, "MATCH", 5) == 0) {
                int opp_id, points;
                char your_choice[10], opp_choice[10], result[10];
                sscanf(accum, "MATCH %d %s %s %s %d", &opp_id, your_choice, opp_choice, result, &points);
                printf("vs %d: %s vs %s - %s (+%d)\n", opp_id, your_choice, opp_choice, result, points);
                score += points;
            } else if (strncmp(accum, "RANKING", 7) == 0) {
                printf("Final Ranking:%s\n", accum + 8);
            } else if (strncmp(accum, "END", 3) == 0) {
                sscanf(accum, "END %d", &score);
                printf("Final score: %d\n", score);
                close(sock);
                return 0;
            }

            size_t line_length = line_end - accum + 1;
            memmove(accum, line_end + 1, accum_len - line_length);
            accum_len -= line_length;
            accum[accum_len] = '\0';
        }
    }

    close(sock);
    return 0;
}