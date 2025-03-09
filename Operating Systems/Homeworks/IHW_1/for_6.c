#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
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

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(pipe1[0], buffer, BUFFER_SIZE);
        
        process(buffer, bytes_read);
        write(pipe2[1], buffer, bytes_read);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        int input_fd = open(input_file, O_RDONLY);
        if (input_fd == -1) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(input_fd, buffer, BUFFER_SIZE);
        close(input_fd);
        
        if (bytes_read > 0) {
            write(pipe1[1], buffer, bytes_read);
        }
        close(pipe1[1]);

        wait(NULL);

        ssize_t bytes_written = read(pipe2[0], buffer, BUFFER_SIZE);
        close(pipe2[0]);
        
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        
        write(output_fd, buffer, bytes_written);
        close(output_fd);
    }
    return 0;
}
