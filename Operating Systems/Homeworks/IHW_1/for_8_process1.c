#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 5000
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

void cleanup() {
    unlink(FIFO1);
    unlink(FIFO2);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    atexit(cleanup);
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    int input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        perror("open input file");
        exit(EXIT_FAILURE);
    }

    int fifo1_fd = open(FIFO1, O_WRONLY);
    if (fifo1_fd == -1) {
        perror("open FIFO1 for writing");
        close(input_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(input_fd, buffer, BUFFER_SIZE);
    close(input_fd);

    write(fifo1_fd, buffer, bytes_read);
    close(fifo1_fd);

    int fifo2_fd = open(FIFO2, O_RDONLY);
    if (fifo2_fd == -1) {
        perror("open FIFO2 for reading");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = read(fifo2_fd, buffer, BUFFER_SIZE);
    close(fifo2_fd);

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("open output file");
        exit(EXIT_FAILURE);
    }

    write(output_fd, buffer, bytes_written);
    close(output_fd);

    return 0;
}
