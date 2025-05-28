#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>

#define MAX_CLIENTS 100
#define MAX_MONITORS 10
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;
    int id;
    int score;
} Client;

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;
    int active;
} Monitor;

Client clients[MAX_CLIENTS];
Monitor monitors[MAX_MONITORS];
int num_clients = 0;
int num_monitors = 0;
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

void remove_monitor(int index) {
    printf("Monitor %d disconnected. Removing...\n", index + 1);
    for (int i = index; i < num_monitors - 1; i++) {
        monitors[i] = monitors[i + 1];
    }
    num_monitors--;
}

void send_to_monitors(const char *msg) {
    for (int i = 0; i < num_monitors; i++) {
        ssize_t sent = sendto(server_sock, msg, strlen(msg), 0, 
                             (struct sockaddr*)&monitors[i].addr, monitors[i].addr_len);
        
        if (sent < 0) {
            if (errno == ECONNREFUSED || errno == EHOSTUNREACH || errno == ENETUNREACH) {
                remove_monitor(i);
                i--;
            } else {
                perror("sendto failed for monitor");
            }
        }
    }
}

void send_match_result(Client *client, int opp_id, const char *your_choice, 
                      const char *opp_choice, const char *result, int points) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "MATCH %d %s %s %s %d", 
            opp_id, your_choice, opp_choice, result, points);
    
    ssize_t sent = sendto(server_sock, buffer, strlen(buffer), 0, 
          (struct sockaddr*)&client->addr, client->addr_len);
    
    if (sent < 0) {
        perror("sendto failed for client");
    }
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
    if (server_sock < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(ip),
        .sin_port = htons(port)
    };

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", ip, port);
    printf("Waiting for %d students and monitors...\n", num_students);

    while (num_clients < num_students) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        char buffer[BUFFER_SIZE];
        int bytes = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, 
                            (struct sockaddr*)&client_addr, &addr_len);
        if (bytes <= 0) continue;
        buffer[bytes] = '\0';

        if (strcmp(buffer, "REGISTER") == 0) {
            int exists = 0;
            for (int i = 0; i < num_clients; i++) {
                if (clients[i].addr.sin_port == client_addr.sin_port &&
                    clients[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                clients[num_clients].addr = client_addr;
                clients[num_clients].addr_len = addr_len;
                clients[num_clients].id = num_clients + 1;
                clients[num_clients].score = 0;
                num_clients++;
                printf("Student %d registered\n", num_clients);
            }
        } else if (strcmp(buffer, "MONITOR") == 0) {
            int exists = 0;
            for (int i = 0; i < num_monitors; i++) {
                if (monitors[i].addr.sin_port == client_addr.sin_port &&
                    monitors[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                if (num_monitors < MAX_MONITORS) {
                    monitors[num_monitors].addr = client_addr;
                    monitors[num_monitors].addr_len = addr_len;
                    monitors[num_monitors].active = 1;
                    num_monitors++;
                    printf("Monitor %d connected\n", num_monitors);
                } else {
                    printf("Max monitors reached. Ignoring new monitor.\n");
                }
            }
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

            const char *result1 = res == 1 ? "WIN" : res == 2 ? "LOSE" : "DRAW";
            const char *result2 = res == 2 ? "WIN" : res == 1 ? "LOSE" : "DRAW";
            send_match_result(&clients[i], clients[j].id, choices[p1], choices[p2], 
                             result1, res == 1 ? 2 : res == 0 ? 1 : 0);
            send_match_result(&clients[j], clients[i].id, choices[p2], choices[p1], 
                             result2, res == 2 ? 2 : res == 0 ? 1 : 0);

            char monitor_msg[BUFFER_SIZE];
            const char *full_result = res == 1 ? "WIN1" : res == 2 ? "WIN2" : "DRAW";
            snprintf(monitor_msg, BUFFER_SIZE, "FULLMATCH %d %d %s %s %s",
                    clients[i].id, clients[j].id, choices[p1], choices[p2], full_result);
            send_to_monitors(monitor_msg);
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

    for (int i = 0; i < num_clients; i++) {
        sendto(server_sock, ranking, strlen(ranking), 0,
              (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
    }
    send_to_monitors(ranking);

    for (int i = 0; i < num_clients; i++) {
        char end_msg[BUFFER_SIZE];
        snprintf(end_msg, BUFFER_SIZE, "END %d", clients[i].score);
        sendto(server_sock, end_msg, strlen(end_msg), 0,
              (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
    }
    
    for (int attempt = 0; attempt < 3; attempt++) {
        send_to_monitors("END");
        usleep(50000);
    }

    close(server_sock);
    printf("Tournament ended. Total monitors: %d\n", num_monitors);
    return 0;
}