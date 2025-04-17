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
#define SHM_NAME "/rps_shm"
#define SEM_PARENT_SYNC "/sem_parent_sync"
#define SEM_MATCH_PREFIX "/sem_match_"

int N = 4;

typedef enum { ROCK, SCISSORS, PAPER } Move;

typedef struct {
    int moves[MAX_PLAYERS];
    int scores[MAX_PLAYERS];
    int ready[MAX_PLAYERS][MAX_PLAYERS];
} SharedData;

SharedData *shared;
sem_t *sem_parent_sync;
sem_t *sem_match[MAX_PLAYERS][MAX_PLAYERS];
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
        (m1 == PAPER && m2 == ROCK)) {
        return 1;
    }
    return -1;
}

void generate_sem_name(char *buffer, int i, int j) {
    sprintf(buffer, "%s%d_%d", SEM_MATCH_PREFIX, i, j);
}

void create_and_init_shared_memory() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));
    shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    memset(shared, 0, sizeof(SharedData));
}

void create_semaphores() {
    sem_parent_sync = sem_open(SEM_PARENT_SYNC, O_CREAT, 0666, 0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                char name[64];
                generate_sem_name(name, i, j);
                sem_match[i][j] = sem_open(name, O_CREAT, 0666, 0);
            }
        }
    }
}

void child_process(int id) {
    srand(getpid());

    for (int j = 0; j < N; j++) {
        if (id == j) {
            continue;
        }

        char sem_name[64];
        generate_sem_name(sem_name, id, j);
        sem_t *match_sem = sem_open(sem_name, 0);

        sem_wait(match_sem);

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

        sem_post(sem_parent_sync);
    }
    exit(0);
}

void cleanup() {
    printf("Cleaning up...\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                char name[64];
                generate_sem_name(name, i, j);
                sem_unlink(name);
            }
        }
    }
        
    sem_close(sem_parent_sync);
    sem_unlink(SEM_PARENT_SYNC);
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
    create_semaphores();

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
            sem_post(sem_match[i][j]);
            sem_post(sem_match[j][i]);
            sem_wait(sem_parent_sync);
            sem_wait(sem_parent_sync);

            printf("Match %d vs %d: %s vs %s\n", i, j,
                move_name(shared->moves[i]),
                move_name(shared->moves[j]));
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
