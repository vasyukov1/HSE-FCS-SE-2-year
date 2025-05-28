#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

#define MAX_CLIENTS 100
#define MAX_MONITORS 10
#define BUFFER_SIZE 1024
#define PING_INTERVAL 5
#define MAX_MISSED_PINGS 3

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;
    int id;
    int score;
    int active;
    int missed_pings;
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
int next_client_id = 1;
int active_clients = 0;
int active_monitors = 0;
int max_students_allowed = 0;

void generate_choices(int *p1, int *p2) {
    *p1 = rand() % 3;
    *p2 = rand() % 3;
}

int determine_result(int p1, int p2) {
    if ((p1 + 1) % 3 == p2) return 1;
    if ((p2 + 1) % 3 == p1) return 2;
    return 0;
}

void send_to_monitors(const char *msg) {
    for (int i = 0; i < num_monitors; i++) {
        if (monitors[i].active) {
            ssize_t sent = sendto(server_sock, msg, strlen(msg), 0, 
                                 (struct sockaddr*)&monitors[i].addr, 
                                 monitors[i].addr_len);
            
            if (sent < 0) {
                if (errno == ECONNREFUSED || errno == EHOSTUNREACH || 
                    errno == ENETUNREACH) {
                    printf("Monitor %d disconnected\n", i+1);
                    monitors[i].active = 0;
                    active_monitors--;
                }
            }
        }
    }
}

void send_match_result(Client *client, int opp_id, const char *your_choice, 
                      const char *opp_choice, const char *result, int points) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "MATCH %d %s %s %s %d", 
            opp_id, your_choice, opp_choice, result, points);
    
    if (client->active) {
        ssize_t sent = sendto(server_sock, buffer, strlen(buffer), 0, 
                             (struct sockaddr*)&client->addr, client->addr_len);
        
        if (sent < 0) {
            if (errno == ECONNREFUSED || errno == EHOSTUNREACH || 
                errno == ENETUNREACH) {
                printf("Client %d disconnected\n", client->id);
                client->active = 0;
                active_clients--;
            }
        }
    }
}

void remove_inactive_clients() {
    for (int i = 0; i < num_clients; i++) {
        if (!clients[i].active) {
            printf("Removing inactive client %d\n", clients[i].id);
            for (int j = i; j < num_clients - 1; j++) {
                clients[j] = clients[j + 1];
            }
            num_clients--;
            i--;
        }
    }
}

void remove_inactive_monitors() {
    for (int i = 0; i < num_monitors; i++) {
        if (!monitors[i].active) {
            printf("Removing inactive monitor %d\n", i+1);
            for (int j = i; j < num_monitors - 1; j++) {
                monitors[j] = monitors[j + 1];
            }
            num_monitors--;
            i--;
        }
    }
}

void check_activity() {
    const char *ping_msg = "PING";
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].active) {
            ssize_t sent = sendto(server_sock, ping_msg, strlen(ping_msg), 0,
                                 (struct sockaddr*)&clients[i].addr, 
                                 clients[i].addr_len);
            
            if (sent < 0) {
                if (errno == ECONNREFUSED || errno == EHOSTUNREACH || 
                    errno == ENETUNREACH) {
                    printf("Client %d disconnected\n", clients[i].id);
                    clients[i].active = 0;
                    active_clients--;
                }
            } else {
                clients[i].missed_pings++;
                
                if (clients[i].missed_pings > MAX_MISSED_PINGS) {
                    printf("Client %d missed %d pings. Marking as inactive\n", 
                          clients[i].id, clients[i].missed_pings);
                    clients[i].active = 0;
                    active_clients--;
                }
            }
        }
    }
    
    for (int i = 0; i < num_monitors; i++) {
        if (monitors[i].active) {
            ssize_t sent = sendto(server_sock, ping_msg, strlen(ping_msg), 0,
                                 (struct sockaddr*)&monitors[i].addr, 
                                 monitors[i].addr_len);
            if (sent < 0) {
                if (errno == ECONNREFUSED || errno == EHOSTUNREACH || 
                    errno == ENETUNREACH) {
                    printf("Monitor %d disconnected\n", i+1);
                    monitors[i].active = 0;
                    active_monitors--;
                }
            }
        }
    }
    
    remove_inactive_clients();
    remove_inactive_monitors();
}

void shutdown_server() {
    printf("Shutting down server...\n");
    
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].active) {
            sendto(server_sock, "SHUTDOWN", 8, 0,
                  (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
        }
    }
    
    for (int i = 0; i < num_monitors; i++) {
        if (monitors[i].active) {
            sendto(server_sock, "SHUTDOWN", 8, 0,
                  (struct sockaddr*)&monitors[i].addr, monitors[i].addr_len);
        }
    }
    
    sleep(1);
    close(server_sock);
    printf("Server shutdown complete\n");
    exit(0);
}

void run_tournament() {
    printf("Starting tournament with %d players...\n", active_clients);
    const char *choices[] = {"Rock", "Scissors", "Paper"};

    for (int i = 0; i < num_clients; i++) {
        if (!clients[i].active) continue;
        
        for (int j = i + 1; j < num_clients; j++) {
            if (!clients[j].active) continue;
            
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
        if (clients[i].active) {
            char entry[20];
            snprintf(entry, 20, " %d:%d", clients[i].id, clients[i].score);
            strncat(ranking, entry, BUFFER_SIZE - strlen(ranking) - 1);
        }
    }

    for (int i = 0; i < num_clients; i++) {
        if (clients[i].active) {
            sendto(server_sock, ranking, strlen(ranking), 0,
                  (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
        }
    }
    send_to_monitors(ranking);

    for (int i = 0; i < num_clients; i++) {
        if (clients[i].active) {
            char end_msg[BUFFER_SIZE];
            snprintf(end_msg, BUFFER_SIZE, "END %d", clients[i].score);
            sendto(server_sock, end_msg, strlen(end_msg), 0,
                  (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
        }
    }
    
    send_to_monitors("END");

    for (int i = 0; i < num_clients; i++) {
        clients[i].score = 0;
    }
    
    printf("Tournament completed\n");
}

void process_incoming_connections() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    int bytes = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, 
                       (struct sockaddr*)&client_addr, &addr_len);
    
    if (bytes <= 0) return;
    
    buffer[bytes] = '\0';

    if (strcmp(buffer, "REGISTER") == 0) {
        int exists = 0;
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].addr.sin_port == client_addr.sin_port &&
                clients[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr) {
                exists = 1;
                if (!clients[i].active) {
                    clients[i].active = 1;
                    active_clients++;
                    printf("Client %d reconnected\n", clients[i].id);
                }
                break;
            }
        }
        if (!exists) {
            if (num_clients < max_students_allowed) {
                clients[num_clients].addr = client_addr;
                clients[num_clients].addr_len = addr_len;
                clients[num_clients].id = next_client_id++;
                clients[num_clients].score = 0;
                clients[num_clients].active = 1;
                clients[num_clients].missed_pings = 0;
                num_clients++;
                active_clients++;
                printf("New client %d registered\n", clients[num_clients-1].id);
            } else {
                printf("Max students reached. Rejecting new client\n");
                char reject_msg[] = "REJECT";
                sendto(server_sock, reject_msg, strlen(reject_msg), 0,
                      (struct sockaddr*)&client_addr, addr_len);
            }
        }
    } 
    else if (strcmp(buffer, "MONITOR") == 0) {
        int exists = 0;
        for (int i = 0; i < num_monitors; i++) {
            if (monitors[i].addr.sin_port == client_addr.sin_port &&
                monitors[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr) {
                exists = 1;
                if (!monitors[i].active) {
                    monitors[i].active = 1;
                    active_monitors++;
                    printf("Monitor %d reconnected\n", i+1);
                }
                break;
            }
        }
        if (!exists && num_monitors < MAX_MONITORS) {
            monitors[num_monitors].addr = client_addr;
            monitors[num_monitors].addr_len = addr_len;
            monitors[num_monitors].active = 1;
            num_monitors++;
            active_monitors++;
            printf("New monitor %d connected\n", num_monitors);
        }
    }
    else if (strcmp(buffer, "PONG") == 0) {
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].addr.sin_port == client_addr.sin_port &&
                clients[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr) {
                clients[i].missed_pings = 0;
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <port> <max_students>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    max_students_allowed = atoi(argv[3]);
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
    printf("Maximum students: %d\n", max_students_allowed);
    printf("Server commands: start, status, exit\n");

    time_t last_activity_check = time(NULL);
    
    while (1) {
        if (time(NULL) - last_activity_check >= PING_INTERVAL) {
            check_activity();
            last_activity_check = time(NULL);
        }
        
        process_incoming_connections();
        
        struct timeval tv = {0, 1000};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
            char command[20];
            if (fgets(command, sizeof(command), stdin)) {
                command[strcspn(command, "\n")] = 0;
                
                if (strcmp(command, "start") == 0) {
                    if (active_clients >= 2) {
                        run_tournament();
                    } else {
                        printf("Not enough active clients (%d). Need at least 2.\n", active_clients);
                    }
                }
                else if (strcmp(command, "status") == 0) {
                    printf("Active clients: %d/%d (max: %d), Active monitors: %d/%d\n", 
                           active_clients, num_clients, max_students_allowed,
                           active_monitors, num_monitors);
                }
                else if (strcmp(command, "exit") == 0) {
                    shutdown_server();
                }
                else {
                    printf("Unknown command: %s\n", command);
                }
            }
        }
    }

    close(server_sock);
    return 0;
}