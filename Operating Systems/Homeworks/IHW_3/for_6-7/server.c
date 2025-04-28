#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    int id;
    int score;
} Client;

typedef struct {
    int socket;
} Observer;

Observer observers[MAX_CLIENTS];
int num_observers = 0;

Client clients[MAX_CLIENTS];
int num_clients = 0;

void generate_choices(int *p1, int *p2) {
    *p1 = rand() % 3;
    *p2 = rand() % 3;
}

int determine_result(int p1, int p2) {
    if ((p1 + 1) % 3 == p2) return 1;
    if ((p2 + 1) % 3 == p1) return 2;
    return 0;
}

void send_match_result(int client_sock, int opponent_id, const char *your_choice, const char *opponent_choice, const char *result, int points) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "MATCH %d %s %s %s %d\n", opponent_id, your_choice, opponent_choice, result, points);
    send(client_sock, buffer, strlen(buffer), 0);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <port> <num_students>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int num_students = atoi(argv[3]);
    srand(time(NULL));

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(ip),
        .sin_port = htons(port)
    };

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, num_students);

    printf("Server listening on ip %s port %d...\n", ip, port);

    int connected_players = 0;
    while (connected_players < num_students) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
        
        char type[10];
        recv(client_sock, type, sizeof(type), 0);
        type[strcspn(type, "\n")] = '\0';

        if (strcmp(type, "PLAYER") == 0) {
            clients[num_clients].socket = client_sock;
            clients[num_clients].id = num_clients + 1;
            clients[num_clients].score = 0;
            num_clients++;
            connected_players++;
            printf("Player %d connected\n", num_clients);

            char notify[BUFFER_SIZE];
            snprintf(notify, BUFFER_SIZE, "PLAYER_CONNECTED %d\n", num_clients);
            for (int j = 0; j < num_observers; j++) {
                send(observers[j].socket, notify, strlen(notify), 0);
            }
        } else if (strcmp(type, "OBSERVER") == 0) {
            observers[num_observers].socket = client_sock;
            num_observers++;
            printf("Observer connected\n");
        } else {
            close(client_sock);
        }
    }

    printf("Starting tournament...\n");
    const char *choices[] = {"Rock", "Scissors", "Paper"};

    for (int i = 0; i < num_clients; i++) {
        for (int j = i + 1; j < num_clients; j++) {
            char match_start[BUFFER_SIZE];
            snprintf(match_start, BUFFER_SIZE, "MATCH_START %d %d\n", clients[i].id, clients[j].id);
            for (int k = 0; k < num_observers; k++) {
                send(observers[k].socket, match_start, strlen(match_start), 0);
            }

            int p1, p2;
            generate_choices(&p1, &p2);
            int res = determine_result(p1, p2);

            if (res == 1) {
                clients[i].score += 2;
            } else if (res == 2) {
                clients[j].score += 2;
            } else {
                clients[i].score++;
                clients[j].score++;
            }

            send_match_result(clients[i].socket, clients[j].id, choices[p1], choices[p2], 
                             res == 1 ? "WIN" : res == 2 ? "LOSE" : "DRAW", 
                             res == 1 ? 2 : res == 0 ? 1 : 0);
            send_match_result(clients[j].socket, clients[i].id, choices[p2], choices[p1], 
                             res == 2 ? "WIN" : res == 1 ? "LOSE" : "DRAW", 
                             res == 2 ? 2 : res == 0 ? 1 : 0);

            char match_result[BUFFER_SIZE];
            snprintf(match_result, BUFFER_SIZE, "MATCH_RESULT %d %s %d %s %s\n", 
                    clients[i].id, choices[p1], clients[j].id, choices[p2], 
                    res == 1 ? "WIN1" : res == 2 ? "WIN2" : "DRAW");
            for (int k = 0; k < num_observers; k++) {
                send(observers[k].socket, match_result, strlen(match_result), 0);
                usleep(1000);
            }
        }
    }

    for (int i = 0; i < num_clients - 1; i++) {
        for (int j = 0; j < num_clients - i - 1; j++) {
            if (clients[j].score < clients[j+1].score) {
                Client temp = clients[j];
                clients[j] = clients[j+1];
                clients[j+1] = temp;
            }
        }
    }

    char ranking_buffer[BUFFER_SIZE];
    snprintf(ranking_buffer, BUFFER_SIZE, "RANKING");
    for (int i = 0; i < num_clients; i++) {
        char entry[20];
        snprintf(entry, 20, " %d:%d", clients[i].id, clients[i].score);
        strncat(ranking_buffer, entry, BUFFER_SIZE - strlen(ranking_buffer) - 1);
    }
    strncat(ranking_buffer, "\n", BUFFER_SIZE - strlen(ranking_buffer) - 1);

    for (int i = 0; i < num_clients; i++) {
        send(clients[i].socket, ranking_buffer, strlen(ranking_buffer), 0);
    }

    for (int k = 0; k < num_observers; k++) {
        send(observers[k].socket, ranking_buffer, strlen(ranking_buffer), 0);
    }

    printf("Waiting for observers...\n");
    sleep(1);

    printf("Closing connections...\n");

    for (int k = 0; k < num_observers; k++) {
        shutdown(observers[k].socket, SHUT_RDWR);
        close(observers[k].socket);
    }

    for (int i = 0; i < num_clients; i++) {
        shutdown(clients[i].socket, SHUT_RDWR);
        close(clients[i].socket);
    }

    close(server_sock);
    printf("Tournament ended.\n");
    return 0;
}