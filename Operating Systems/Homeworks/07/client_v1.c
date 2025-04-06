#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int number;
    int ready;
    int terminate;
} shared_data_t;

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Ошибка подключения к разделяемой памяти");
        exit(1);
    }

    shared_data_t *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Ошибка при mmap");
        exit(1);
    }

    srand(time(NULL));

    while (1) {
        int num = rand() % 1000;

        while (shm_ptr->ready) {
            usleep(100000);
        }

        shm_ptr->number = num;
        shm_ptr->ready = 1;
        printf("Отправлено число: %d\n", num);

        printf("Продолжить отправку? (y/n): ");
        char choice;
        scanf(" %c", &choice);

        if (choice == 'n' || choice == 'N') {
            shm_ptr->terminate = 1;
            printf("Клиент завершает сервер...\n");
            break;
        }
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    return 0;
}
