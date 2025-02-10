#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int size = 2;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Need 2 arguments.\n");
        exit(-1);
    }

    const char *read_file_name = argv[1];
    const char *write_file_name = argv[2];

    char buffer[size];
    ssize_t read_bytes;

    int file;
    if ((file = open(read_file_name, O_RDONLY)) < 0) {
        printf("Can't open file for reading.\n");
        exit(-1);
    }

    struct stat st;
    if (fstat(file, &st) == -1) {
        printf("Can't get file info.\n");
        close(file);
        exit(-1);
    }

    int new_file;
    if ((new_file = open(write_file_name, O_WRONLY | O_CREAT, st.st_mode)) < 0) {
        printf("Can't open file for writing.\n");
        close(file);
        exit(-1);
    }

    do {
        read_bytes = read(file, buffer, size);
        if (read_bytes == -1) {
            printf("Can't write this file.\n");
            close(file);
            close(new_file);
            exit(-1);
        }
        write(new_file, buffer, read_bytes);

    } while (read_bytes == size);

    if (close(new_file) < 0) {
        printf("Can't close file for writing.\n");
    }

    if (close(file) < 0) {
        printf("Can't close file for reading.\n");
    }

    return 0;
}