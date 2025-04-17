#include "common.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

const char *move_name(int m) {
    switch (m) {
        case ROCK: return "Rock";
        case SCISSORS: return "Scissors";
        case PAPER: return "Paper";
        default: return "?";
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    int shm_id = shmget(SHM_KEY, sizeof(SharedData), 0666);
    SharedData *shared = (SharedData *)shmat(shm_id, NULL, 0);
    int sem_id = semget(SEM_KEY, 0, 0666);

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int idx_ij = SEM_READY_BASE + i * MAX_PLAYERS + j;
            int idx_ji = SEM_READY_BASE + j * MAX_PLAYERS + i;
            semctl(sem_id, idx_ij, SETVAL, 1);
            semctl(sem_id, idx_ji, SETVAL, 1);
            usleep(100000);
        }
    }

    printf("Waiting for results...\n");
    sleep(2);

    printf("Final scores:\n");
    for (int i = 0; i < N; i++) {
        printf("Player %d: %d points\n", i, shared->scores[i]);
    }

    shmdt(shared);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}
