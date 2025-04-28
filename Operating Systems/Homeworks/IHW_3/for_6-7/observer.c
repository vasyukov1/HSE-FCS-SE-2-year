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
    
    send(sock, "OBSERVER\n", 9, 0);

    char accum[BUFFER_SIZE * 2] = {0};
    size_t accum_len = 0;

    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;
        
        buffer[bytes] = '\0';
        
        if (accum_len + bytes >= sizeof(accum)) {
            fprintf(stderr, "Buffer overflow detected!\n");
            break;
        }

        strncat(accum, buffer, sizeof(accum) - accum_len - 1);
        accum_len += bytes;

        char *line;
        while ((line = strchr(accum, '\n')) != NULL) {
            *line = '\0';

            if (strncmp(accum, "PLAYER_CONNECTED", 16) == 0) {
                int id;
                sscanf(accum, "PLAYER_CONNECTED %d", &id);
                printf("[System] Player %d connected\n", id);
            } 
            else if (strncmp(accum, "MATCH_START", 11) == 0) {
                int p1, p2;
                sscanf(accum, "MATCH_START %d %d", &p1, &p2);
                printf("[Match] Player %d vs Player %d started\n", p1, p2);
            } 
            else if (strncmp(accum, "MATCH_RESULT", 12) == 0) {
                int p1, p2;
                char choice1[10], choice2[10], result[10];
                sscanf(accum, "MATCH_RESULT %d %s %d %s %s", 
                      &p1, choice1, &p2, choice2, result);
                printf("[Result] %d (%s) vs %d (%s) → %s\n", 
                      p1, choice1, p2, choice2, result);
            } 
            else if (strncmp(accum, "RANKING", 7) == 0) {
                printf("[Ranking]%s\n", accum + 8);
            }

            size_t processed = line - accum + 1;
            memmove(accum, line + 1, accum_len - processed);
            accum_len -= processed;
            accum[accum_len] = '\0';
        }
    }

    close(sock);
    return 0;
}