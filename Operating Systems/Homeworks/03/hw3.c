#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

uint64_t fibonacci(unsigned int n) {
    uint64_t a = 0, b = 1;
    for (unsigned int i = 2; i <= n; ++i) {
        b = a + b;
        if (b < a) {
            fprintf(stderr, "Переполнение при вычислении Fibonacci(%u)\n", n);
            exit(EXIT_FAILURE);
        }
        a = b - a;
    }
    return a;
}

uint64_t factorial(unsigned int n) {
    uint64_t fact = 1;
    for (unsigned int i = 1; i <= n; ++i) {
        uint64_t next_fact = fact * i;
        if (next_fact < fact) {
            fprintf(stderr, "Переполнение при вычислении Factorial(%u)\n", n);
            exit(EXIT_FAILURE);
        }
        fact = next_fact;
    }
    return fact;
}

void print_directory() {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("Ошибка при открытии каталога");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    printf("\nСодержимое текущего каталога:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Нужен 1 аргумент!");
        return EXIT_FAILURE;
    }

    unsigned int n = atoi(argv[1]);

    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 < 0) {
        perror("Ошибка при применении fork()");
        return EXIT_FAILURE;
    }

    if (pid1 == 0) {
        printf("Процесс Factorial: PID = %d, PPID = %d\n", getpid(), getppid());
        uint64_t fact = factorial(n);
        printf("Factorial(%u) = %llu\n\n", n, fact);
        exit(EXIT_SUCCESS);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("Ошибка при применении fork()");
        return EXIT_FAILURE;
    }

    if (pid2 == 0) {
        printf("Процесс Directory: PID = %d, PPID = %d\n", getpid(), getppid());
        print_directory();
        exit(EXIT_SUCCESS);
    }

    printf("Процесс Fibonacci: PID = %d, PPID = %d\n", getpid(), getppid());
    uint64_t fib = fibonacci(n);
    printf("Fibonacci(%u) = %llu\n\n", n, fib);

    wait(NULL);
    wait(NULL);
    return 0;
}


