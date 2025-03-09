#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 5000

void process(char *buffer, ssize_t length) {
    for (ssize_t i = 0; i < length; ++i) {
        if (buffer[i] == 'a') buffer[i] = 'A';
        else if (buffer[i] == 'e') buffer[i] = 'E';
        else if (buffer[i] == 'i') buffer[i] = 'I';
        else if (buffer[i] == 'o') buffer[i] = 'O';
        else if (buffer[i] == 'u') buffer[i] = 'U';
        else if (buffer[i] == 'y') buffer[i] = 'Y';
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t reader, processor, writer;
    reader = fork();
    if (reader == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (reader == 0) {
        close(pipe1[0]);
        int input_fd = open(input_file, O_RDONLY);
        if (input_fd == -1) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(input_fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(input_fd);
        write(pipe1[1], buffer, bytes_read);
        close(pipe1[1]);
        exit(EXIT_SUCCESS);
    }

    processor = fork();
    if (processor == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (processor == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(pipe1[0], buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(pipe1[0]);
        process(buffer, bytes_read);
        write(pipe2[1], buffer, bytes_read);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    }

    writer = fork();
    if (writer == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (writer == 0) {
        close(pipe2[1]);
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(pipe2[0], buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(pipe2[0]);
        write(output_fd, buffer, bytes_read);
        close(output_fd);
        exit(EXIT_SUCCESS);
    }

    close(pipe1[1]);
    close(pipe1[0]);
    close(pipe2[1]);
    close(pipe2[0]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}
