#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define SHM_KEY 1235
#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int number;
    int ready;
    int terminate;
} shared_data_t;

int main() {
    int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shm_id == -1) {
        perror("Ошибка подключения к разделяемой памяти");
        exit(1);
    }

    shared_data_t *shm_ptr = (shared_data_t *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("Ошибка присоединения к разделяемой памяти");
        exit(1);
    }

    srand(time(NULL));

    while (1) {
        if (shm_ptr->terminate) {
            printf("Сервер завершил работу. Клиент тоже завершает работу...\n");
            break;
        }

        if (!shm_ptr->ready) {
            int num = rand() % 1000;
            shm_ptr->number = num;
            shm_ptr->ready = 1;
            printf("Отправлено число: %d\n", num);
        }

        sleep(1);
    }

    shmdt(shm_ptr);
    return 0;
}
