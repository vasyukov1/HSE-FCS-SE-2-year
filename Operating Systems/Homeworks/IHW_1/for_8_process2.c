#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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

int main() {
    int fifo1_fd = open(FIFO1, O_RDONLY);
    if (fifo1_fd == -1) {
        perror("open FIFO1 for reading");
        exit(EXIT_FAILURE);
    }

    int fifo2_fd = open(FIFO2, O_WRONLY);
    if (fifo2_fd == -1) {
        perror("open FIFO2 for writing");
        close(fifo1_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fifo1_fd, buffer, BUFFER_SIZE);
    close(fifo1_fd);

    process(buffer, bytes_read);
    write(fifo2_fd, buffer, bytes_read);
    close(fifo2_fd);

    return 0;
}
