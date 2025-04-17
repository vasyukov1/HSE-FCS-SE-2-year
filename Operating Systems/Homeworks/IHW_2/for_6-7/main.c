#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 10
#define SHM_NAME "/rps_shm_v2"

int N = 4;

typedef enum { ROCK, SCISSORS, PAPER } Move;

typedef struct {
    int moves[MAX_PLAYERS];
    int scores[MAX_PLAYERS];
    int ready[MAX_PLAYERS][MAX_PLAYERS];
    sem_t match_sems[MAX_PLAYERS][MAX_PLAYERS];
    sem_t parent_sync;
} SharedData;

SharedData *shared;
pid_t children[MAX_PLAYERS];

void cleanup();

void sigint_handler(int signo) {
    cleanup();
    exit(0);
}

const char *move_name(int move) {
    switch (move) {
        case ROCK: return "Rock";
        case SCISSORS: return "Scissors";
        case PAPER: return "Paper";
        default: return "Unknown";
    }
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

void create_and_init_shared_memory() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));
    shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    memset(shared, 0, sizeof(SharedData));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                sem_init(&shared->match_sems[i][j], 1, 0);
            }
        }
    }

    sem_init(&shared->parent_sync, 1, 0);
}

void child_process(int id) {
    srand(getpid());

    for (int j = 0; j < N; j++) {
        if (id == j) {
            continue;
        }

        sem_wait(&shared->match_sems[id][j]);

        int move = rand() % 3;
        shared->moves[id] = move;
        shared->ready[id][j] = 1;

        while (!shared->ready[j][id]) {
            usleep(1000);
        }

        __sync_synchronize();
        usleep(1000);

        sem_post(&shared->parent_sync);
    }

    exit(0);
}

void cleanup() {
    printf("Cleaning up...\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                sem_destroy(&shared->match_sems[i][j]);
            }
        }
    }

    sem_destroy(&shared->parent_sync);
    munmap(shared, sizeof(SharedData));
    shm_unlink(SHM_NAME);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        N = atoi(argv[1]);
    }
    if (N > MAX_PLAYERS || N < 2) {
        printf("N must be between 2 and %d\n", MAX_PLAYERS);
        return 1;
    }

    signal(SIGINT, sigint_handler);

    create_and_init_shared_memory();

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            child_process(i);
        } else {
            children[i] = pid;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            sem_post(&shared->match_sems[i][j]);
            sem_post(&shared->match_sems[j][i]);
    
            sem_wait(&shared->parent_sync);
            sem_wait(&shared->parent_sync);
    
            printf("Match %d vs %d: %s vs %s\n", i, j,
                   move_name(shared->moves[i]),
                   move_name(shared->moves[j]));
    
            int result = judge(shared->moves[i], shared->moves[j]);
            if (result == 1) {
                shared->scores[i] += 2;
            } else if (result == -1) {
                shared->scores[j] += 2;
            } else {
                shared->scores[i] += 1;
                shared->scores[j] += 1;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        waitpid(children[i], NULL, 0);
    }

    printf("\nFinal Scores:\n");
    for (int i = 0; i < N; i++) {
        printf("Player %d: %d points\n", i, shared->scores[i]);
    }

    cleanup();
    return 0;
}
