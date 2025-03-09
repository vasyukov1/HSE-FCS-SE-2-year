#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1235
#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int number;
    int ready;
    int terminate;
} shared_data_t;

int shm_id;
shared_data_t *shm_ptr;

void cleanup(int signum) {
    printf("\nСервер завершает работу...\n");
    shm_ptr->terminate = 1;

    sleep(2);

    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    printf("Разделяемая память удалена. Сервер завершил работу.\n");
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Ошибка создания разделяемой памяти");
        exit(1);
    }

    shm_ptr = (shared_data_t *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("Ошибка присоединения к разделяемой памяти");
        exit(1);
    }

    shm_ptr->ready = 0;
    shm_ptr->terminate = 0;

    printf("Сервер запущен. Ожидание данных...\n");

    while (1) {
        if (shm_ptr->ready) {
            printf("Получено число: %d\n", shm_ptr->number);
            shm_ptr->ready = 0;
        }

        sleep(1);
    }

    cleanup(0);
    return 0;
}
