#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define MEM_SIZE 1024  // Размер сегмента разделяемой памяти

// Структура для хранения случайных чисел
struct shared_memory {
    int numbers[10];
};

void cleanup(int shmid) {
    // Отсоединяем сегмент разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    } else {
        printf("Сегмент разделяемой памяти удалён.\n");
    }
}

int main() {
    // Генерация уникального ключа для разделяемой памяти
    key_t key = ftok("server.c", 1);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Получаем идентификатор сегмента разделяемой памяти
    int shmid = shmget(key, sizeof(struct shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Прикрепляем сегмент разделяемой памяти
    struct shared_memory* shm_ptr = (struct shared_memory*)shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    // Чтение и вывод случайных чисел из разделяемой памяти
    printf("Чтение случайных чисел с сервера:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", shm_ptr->numbers[i]);
    }
    printf("\n");

    // Отсоединяем разделяемую память
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Завершаем работу сервера
    printf("Клиент завершил работу.\n");

    return 0;
}
