#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int number;
    int ready;
} shared_data_t;

int shm_id, sem_id;
shared_data_t *shm_ptr;

void cleanup(int signum) {
    printf("\nСервер завершает работу...\n");
    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    shm_ptr = (shared_data_t *)shmat(shm_id, NULL, 0);
    shm_ptr->ready = 0;

    sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, 1);

    printf("Сервер запущен. Ожидание данных...\n");

    while (1) {
        if (shm_ptr->ready) {
            printf("Получено число: %d\n", shm_ptr->number);
            shm_ptr->ready = 0;
        }
        usleep(100000);
    }

    return 0;
}
