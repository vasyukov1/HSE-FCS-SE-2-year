#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int number;
    int ready;
} shared_data_t;

int shm_id;
shared_data_t *shm_ptr;

void cleanup(int signum) {
    printf("\nКлиент завершает работу...\n");
    shmdt(shm_ptr);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
    shm_ptr = (shared_data_t *)shmat(shm_id, NULL, 0);

    srand(time(NULL));
    
    while (1) {
        if (!shm_ptr->ready) {
            int num = rand() % 1000;
            shm_ptr->number = num;
            shm_ptr->ready = 1;
            printf("Отправлено число: %d\n", num);
        }
        sleep(1);
    }

    return 0;
}
