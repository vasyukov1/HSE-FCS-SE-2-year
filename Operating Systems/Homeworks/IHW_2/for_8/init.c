#include "common.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int shm_id = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    SharedData *shared = (SharedData *)shmat(shm_id, NULL, 0);
    memset(shared, 0, sizeof(SharedData));
    shmdt(shared);

    int sem_id = semget(SEM_KEY, 1 + MAX_PLAYERS * MAX_PLAYERS, IPC_CREAT | 0666);

    for (int i = 0; i < 1 + MAX_PLAYERS * MAX_PLAYERS; i++) {
        semctl(sem_id, i, SETVAL, 0);
    }

    printf("Init done. Run ./student <id> from other terminals.\n");
    return 0;
}
