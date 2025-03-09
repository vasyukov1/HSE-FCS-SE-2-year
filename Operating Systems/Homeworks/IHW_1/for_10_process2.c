#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <ctype.h>

#define BUFFER_SIZE 128
#define QUEUE_1_ID 12345
#define QUEUE_2_ID 54321

struct msg_buffer {
    long msg_type;
    char msg_text[BUFFER_SIZE];
};

int main(int argc, char *argv[]) {

    int queue1 = msgget(QUEUE_1_ID, 0666);
    int queue2 = msgget(QUEUE_2_ID, 0666);
    if (queue1 == -1 || queue2 == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer message;
    ssize_t bytes_received;

    while (1) {
        bytes_received = msgrcv(queue1, &message, BUFFER_SIZE, 1, 0);
        if (bytes_received == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        if (bytes_received == 1 && message.msg_text[0] == '\0') {
            break;
        }

        for (int i = 0; i < bytes_received; i++) {
            if (message.msg_text[i] == 'a') message.msg_text[i] = 'A';
            else if (message.msg_text[i] == 'e') message.msg_text[i] = 'E';
            else if (message.msg_text[i] == 'i') message.msg_text[i] = 'I';
            else if (message.msg_text[i] == 'o') message.msg_text[i] = 'O';
            else if (message.msg_text[i] == 'u') message.msg_text[i] = 'U';
            else if (message.msg_text[i] == 'y') message.msg_text[i] = 'Y';
        }

        if (msgsnd(queue2, &message, bytes_received, 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    printf("Process 2 is done.\n");
    return 0;
}