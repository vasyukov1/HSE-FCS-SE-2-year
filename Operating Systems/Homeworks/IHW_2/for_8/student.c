#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <signal.h>

int id, N;
int shm_id, sem_id;
SharedData *shared;

void sem_post(int i) {
    struct sembuf op = {i, 1, 0};
    semop(sem_id, &op, 1);
}

void sem_wait(int i) {
    struct sembuf op = {i, -1, 0};
    semop(sem_id, &op, 1);
}

void cleanup() {
    shmdt(shared);
}

void sigint_handler(int signo) {
    cleanup();
    exit(0);
}

int judge(int m1, int m2) {
    if (m1 == m2) {
        return 0;
    }
    if ((m1 == ROCK && m2 == SCISSORS) ||
        (m1 == SCISSORS && m2 == PAPER) ||
        (m1 == PAPER && m2 == ROCK)) return 1;
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <id> <N>\n", argv[0]);
        return 1;
    }
    id = atoi(argv[1]);
    N = atoi(argv[2]);

    signal(SIGINT, sigint_handler);
    srand(getpid());

    shm_id = shmget(SHM_KEY, sizeof(SharedData), 0666);
    sem_id = semget(SEM_KEY, 0, 0666);
    shared = (SharedData *)shmat(shm_id, NULL, 0);

    for (int j = 0; j < N; j++) {
        if (id == j) {
            continue;
        }

        int idx = SEM_READY_BASE + id * MAX_PLAYERS + j;

        sem_wait(idx);

        int move = rand() % 3;
        shared->moves[id] = move;
        shared->ready[id][j] = 1;

        while (!shared->ready[j][id]) {
            usleep(1000);
        }

        int result = judge(move, shared->moves[j]);
        if (result == 1) {
            shared->scores[id] += 2;
        } else if (result == 0) {
            shared->scores[id] += 1;
        }

        const char *names[] = {"Rock", "Scissors", "Paper"};
        printf("Student %d played %s vs %s (student %d)\n",
               id, names[move], names[shared->moves[j]], j);
    }

    cleanup();
    return 0;
}
