#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define MEM_SIZE 1024  // Размер сегмента разделяемой памяти

// Структура для хранения случайных чисел
struct shared_memory {
    int numbers[10];
};

void cleanup(int shmid) {
    // Удаляем сегмент разделяемой памяти
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

    // Создаём сегмент разделяемой памяти
    int shmid = shmget(key, sizeof(struct shared_memory), 0666 | IPC_CREAT);
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

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация случайных чисел и запись в разделяемую память
    for (int i = 0; i < 10; i++) {
        shm_ptr->numbers[i] = rand() % 100;  // Генерация чисел от 0 до 99
    }

    printf("Сервер запущен. Ожидаем клиента...\n");

    // Ожидание сигнала от клиента (например, завершения работы)
    while (1) {
        sleep(1);  // Пауза
    }

    // Очистка ресурсов перед завершением
    cleanup(shmid);

    return 0;
}
