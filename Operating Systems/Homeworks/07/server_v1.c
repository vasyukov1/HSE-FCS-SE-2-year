#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE sizeof(shared_data_t)

typedef struct {
    int number;
    int ready;
    int terminate;
} shared_data_t;

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Ошибка создания разделяемой памяти");
        exit(1);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("Ошибка при ftruncate");
        exit(1);
    }

    shared_data_t *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Ошибка при mmap");
        exit(1);
    }

    shm_ptr->ready = 0;
    shm_ptr->terminate = 0;

    printf("Сервер запущен. Ожидание данных...\n");

    while (1) {
        if (shm_ptr->terminate) {
            printf("Сервер завершает работу по сигналу клиента...\n");
            munmap(shm_ptr, SHM_SIZE);
            close(shm_fd);
            shm_unlink(SHM_NAME);
            exit(0);
        }

        if (shm_ptr->ready) {
            printf("Получено число: %d\n", shm_ptr->number);
            shm_ptr->ready = 0;
        }

        usleep(100000);
    }

    return 0;
}
