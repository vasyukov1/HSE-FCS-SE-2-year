#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 5000
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

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

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    pid_t reader, processor;

    reader = fork();
    if (reader == 0) {
        int input_fd = open(input_file, O_RDONLY);
        int fifo1_fd = open(FIFO1, O_WRONLY);
        
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(input_fd, buffer, BUFFER_SIZE);
        write(fifo1_fd, buffer, bytes_read);

        close(input_fd);
        close(fifo1_fd);
        exit(EXIT_SUCCESS);
    }

    processor = fork();
    if (processor == 0) {
        int fifo1_fd = open(FIFO1, O_RDONLY);
        int fifo2_fd = open(FIFO2, O_WRONLY);
        
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(fifo1_fd, buffer, BUFFER_SIZE);
        process(buffer, bytes_read);
        write(fifo2_fd, buffer, bytes_read);

        close(fifo1_fd);
        close(fifo2_fd);
        exit(EXIT_SUCCESS);
    }

    int fifo2_fd = open(FIFO2, O_RDONLY);
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fifo2_fd, buffer, BUFFER_SIZE);
    write(output_fd, buffer, bytes_read);

    close(fifo2_fd);
    close(output_fd);

    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}
