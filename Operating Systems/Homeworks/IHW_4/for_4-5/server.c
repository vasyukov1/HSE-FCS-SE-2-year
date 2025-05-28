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
    struct sockaddr_in addr;
    socklen_t addr_len;
    int id;
    int score;
} Client;

Client clients[MAX_CLIENTS];
int num_clients = 0;
int server_sock;

void generate_choices(int *p1, int *p2) {
    *p1 = rand() % 3;
    *p2 = rand() % 3;
}

int determine_result(int p1, int p2) {
    if ((p1 + 1) % 3 == p2) return 1;
    if ((p2 + 1) % 3 == p1) return 2;
    return 0;
}

void send_match_result(Client *client, int opponent_id, const char *your_choice, const char *opp_choice, const char *result, int points) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "MATCH %d %s %s %s %d\n", opponent_id, your_choice, opp_choice, result, points);
    sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr*)&client->addr, client->addr_len);
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

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(ip),
        .sin_port = htons(port)
    };

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Server listening on %s:%d\n", ip, port);
    printf("Waiting for %d clients...\n", num_students);

    while (num_clients < num_students) {
        Client new_client;
        new_client.addr_len = sizeof(new_client.addr);
        char buffer[BUFFER_SIZE];
        int bytes = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&new_client.addr, &new_client.addr_len);
        if (bytes < 0) continue;

        int registered = 0;
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].addr.sin_addr.s_addr == new_client.addr.sin_addr.s_addr &&
                clients[i].addr.sin_port == new_client.addr.sin_port) {
                registered = 1;
                break;
            }
        }

        if (!registered) {
            new_client.id = num_clients + 1;
            new_client.score = 0;
            clients[num_clients++] = new_client;
            printf("Client %d connected\n", num_clients);
        }
    }

    printf("Starting tournament...\n");
    const char *choices[] = {"Rock", "Scissors", "Paper"};

    for (int i = 0; i < num_clients; i++) {
        for (int j = i + 1; j < num_clients; j++) {
            int p1, p2;
            generate_choices(&p1, &p2);
            int res = determine_result(p1, p2);

            if (res == 1) clients[i].score += 2;
            else if (res == 2) clients[j].score += 2;
            else { clients[i].score++; clients[j].score++; }

            send_match_result(&clients[i], clients[j].id, choices[p1], choices[p2], res == 1 ? "WIN" : res == 2 ? "LOSE" : "DRAW", res == 1 ? 2 : res == 0 ? 1 : 0);
            send_match_result(&clients[j], clients[i].id, choices[p2], choices[p1], res == 2 ? "WIN" : res == 1 ? "LOSE" : "DRAW", res == 2 ? 2 : res == 0 ? 1 : 0);
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

    char ranking[BUFFER_SIZE] = "RANKING";
    for (int i = 0; i < num_clients; i++) {
        char entry[20];
        snprintf(entry, 20, " %d:%d", clients[i].id, clients[i].score);
        strncat(ranking, entry, BUFFER_SIZE - strlen(ranking) - 1);
    }
    strncat(ranking, "\n", BUFFER_SIZE - strlen(ranking) - 1);

    for (int i = 0; i < num_clients; i++) {
        sendto(server_sock, ranking, strlen(ranking), 0, (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
        char end_msg[BUFFER_SIZE];
        snprintf(end_msg, BUFFER_SIZE, "END %d\n", clients[i].score);
        sendto(server_sock, end_msg, strlen(end_msg), 0, (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
    }

    close(server_sock);
    printf("Tournament ended\n");
    return 0;
}