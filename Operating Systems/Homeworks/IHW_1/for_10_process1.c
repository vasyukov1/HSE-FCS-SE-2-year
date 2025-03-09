#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define BUFFER_SIZE 128
#define QUEUE_1_ID 12345
#define QUEUE_2_ID 54321

struct msg_buffer {
    long msg_type;
    char msg_text[BUFFER_SIZE];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    int fd_in = open(input_file, O_RDONLY);
    if (fd_in == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int queue1 = msgget(QUEUE_1_ID, 0666 | IPC_CREAT);
    int queue2 = msgget(QUEUE_2_ID, 0666 | IPC_CREAT);
    if (queue1 == -1 || queue2 == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer message;
    message.msg_type = 1;

    ssize_t bytes_read;
    while ((bytes_read = read(fd_in, message.msg_text, BUFFER_SIZE)) > 0) {
        if (msgsnd(queue1, &message, bytes_read, 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_received = msgrcv(queue2, &message, BUFFER_SIZE, 1, 0);
        if (bytes_received == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        if (write(fd_out, message.msg_text, bytes_received) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    message.msg_type = 1;
    message.msg_text[0] = '\0';
    if (msgsnd(queue1, &message, strlen(message.msg_text) + 1, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    close(fd_in);
    close(fd_out);

    if (msgctl(queue1, IPC_RMID, NULL) == -1 || msgctl(queue2, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Process 1 is done.\n");
    return 0;
}