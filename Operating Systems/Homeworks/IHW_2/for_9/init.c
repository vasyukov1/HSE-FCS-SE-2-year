#include "common.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

int main() {
    int msg_id = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("msgget");
        return 1;
    }

    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        return 1;
    }

    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl");
        return 1;
    }

    printf("Инициализация завершена. Запустите процессы студентов.\n");
    return 0;
}
