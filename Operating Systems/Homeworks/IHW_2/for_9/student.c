#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

int msg_id, sem_id;
int player_id, total_players;
int score = 0;

const char* move_to_str(Move m) {
    switch (m) {
        case ROCK: return "Камень";
        case SCISSORS: return "Ножницы";
        case PAPER: return "Бумага";
        default: return "???";
    }
}

void sem_wait(int sem_id) {
    struct sembuf op = {0, -1, 0};
    semop(sem_id, &op, 1);
}

void sem_post(int sem_id) {
    struct sembuf op = {0, 1, 0};
    semop(sem_id, &op, 1);
}

void cleanup(int signo) {
    printf("\nИгрок %d набрал %d очков\n", player_id, score);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <player_id> <total_players>\n", argv[0]);
        return 1;
    }

    player_id = atoi(argv[1]);
    total_players = atoi(argv[2]);

    signal(SIGINT, cleanup);
    srand(time(NULL) + player_id);

    msg_id = msgget(MSG_KEY, 0666);
    sem_id = semget(SEM_KEY, 1, 0666);

    for (int opponent_id = 0; opponent_id < total_players; opponent_id++) {
        if (opponent_id == player_id) {
            continue;
        }

        if (player_id < opponent_id) {
            Message opponent_msg;

            while (1) {
                sem_wait(sem_id);
                int res = msgrcv(msg_id, &opponent_msg, sizeof(Message) - sizeof(long), player_id + 1, IPC_NOWAIT);
                sem_post(sem_id);
                if (res != -1) {
                    break;
                }
                usleep(100000);
            }

            Move my_move = rand() % 3;
            Move opponent_move = opponent_msg.move;

            int result;
            if (my_move == opponent_move) {
                result = 1;
                score += 1;
            } else if ((my_move == ROCK && opponent_move == SCISSORS) ||
                       (my_move == SCISSORS && opponent_move == PAPER) ||
                       (my_move == PAPER && opponent_move == ROCK)) {
                result = 2;
                score += 2;
            } else {
                result = 0;
            }

            ResultMessage result_msg;
            result_msg.mtype = opponent_id + 1;
            result_msg.my_move = my_move;
            result_msg.opponent_move = opponent_move;

            sem_wait(sem_id);
            msgsnd(msg_id, &result_msg, sizeof(ResultMessage) - sizeof(long), 0);
            sem_post(sem_id);

            printf("Игрок %d (%s) vs Игрок %d (%s): ", player_id, move_to_str(my_move), opponent_id, move_to_str(opponent_move));
            if (result == 0) {
                printf("Поражение\n");
            } else if (result == 1) {
                printf("Ничья\n");
            } else {
                printf("Победа\n");
            }

        } else {
            Message msg;
            msg.mtype = opponent_id + 1;
            msg.sender_id = player_id;
            msg.receiver_id = opponent_id;
            msg.move = rand() % 3;

            sem_wait(sem_id);
            msgsnd(msg_id, &msg, sizeof(Message) - sizeof(long), 0);
            sem_post(sem_id);

            ResultMessage result_msg;
            while (1) {
                sem_wait(sem_id);
                int res = msgrcv(msg_id, &result_msg, sizeof(ResultMessage) - sizeof(long), player_id + 1, IPC_NOWAIT);
                sem_post(sem_id);
                if (res != -1) {
                    break;
                }
                usleep(100000);
            }

            Move my_move = msg.move;
            Move opponent_move = result_msg.my_move;

            int result;
            if (my_move == opponent_move) {
                result = 1;
                score += 1;
            } else if ((my_move == ROCK && opponent_move == SCISSORS) ||
                       (my_move == SCISSORS && opponent_move == PAPER) ||
                       (my_move == PAPER && opponent_move == ROCK)) {
                result = 2;
                score += 2;
            } else {
                result = 0;
            }

            printf("Игрок %d (%s) vs Игрок %d (%s): ", player_id, move_to_str(my_move), opponent_id, move_to_str(opponent_move));
            if (result == 0) {
                printf("Поражение\n");
            } else if (result == 1) {
                printf("Ничья\n");
            } else {
                printf("Победа\n");
            }
        }
    }

    printf("Игрок %d финальный счёт: %d очков\n", player_id, score);
    return 0;
}
