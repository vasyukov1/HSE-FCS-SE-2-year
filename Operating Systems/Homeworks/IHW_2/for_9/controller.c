#include "common.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

int main() {
    int msg_id = msgget(MSG_KEY, 0666);
    if (msg_id != -1) {
        msgctl(msg_id, IPC_RMID, NULL);
    }

    int sem_id = semget(SEM_KEY, 1, 0666);
    if (sem_id != -1) {
        semctl(sem_id, 0, IPC_RMID);
    }

    printf("Ресурсы очищены.\n");
    return 0;
}
