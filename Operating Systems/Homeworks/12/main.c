#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>

#define SEM_KEY 0x1234
#define BUF_SIZE 128

#define SEM_PARENT 0
#define SEM_CHILD 1

int pipe_fd[2];
int semid;
pid_t child_pid;

volatile sig_atomic_t terminate = 0;

void handle_sigint(int sig) {
    terminate = 1;
}

void sem_op(int semnum, int op) {
    struct sembuf sb = {semnum, op, 0};
    if (semop(semid, &sb, 1) == -1) {
        if (errno == EINTR && terminate) return;
        perror("semop");
        exit(1);
    }
}

void cleanup(int is_parent) {
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    if (is_parent) {
        semctl(semid, 0, IPC_RMID);
        printf("Parent: resources cleaned up.\n");
    } else {
        printf("Child: resources cleaned up.\n");
    }
}

void parent_process() {
    char buffer[BUF_SIZE];
    ssize_t bytes;

    while (!terminate) {
        sem_op(SEM_PARENT, -1);
        if (terminate) break;

        sleep(1);
        snprintf(buffer, BUF_SIZE, "Message from parent: %ld", time(NULL));
        bytes = write(pipe_fd[1], buffer, strlen(buffer) + 1);
        if (bytes <= 0) {
            if (errno == EINTR) break;
            perror("parent write");
            break;
        }
        printf("Parent sent: %s\n", buffer);

        sem_op(SEM_CHILD, 1);
        if (terminate) break;

        sem_op(SEM_PARENT, -1);
        if (terminate) break;

        bytes = read(pipe_fd[0], buffer, BUF_SIZE);
        if (bytes <= 0) {
            if (errno == EINTR) break;
            perror("parent read");
            break;
        }
        buffer[bytes] = '\0';
        printf("Parent received: %s\n", buffer);

        sem_op(SEM_PARENT, 1);
    }

    if (child_pid > 0) {
        kill(child_pid, SIGINT);
        wait(NULL);
    }
    cleanup(1);
}

void child_process() {
    char buffer[BUF_SIZE];
    ssize_t bytes;

    while (!terminate) {
        sem_op(SEM_CHILD, -1);
        if (terminate) break;

        bytes = read(pipe_fd[0], buffer, BUF_SIZE);
        if (bytes <= 0) {
            if (errno == EINTR) break;
            perror("child read");
            break;
        }
        buffer[bytes] = '\0';
        printf("Child received: %s\n", buffer);

        sleep(1);
        snprintf(buffer, BUF_SIZE, "Message from child: %ld", time(NULL));
        bytes = write(pipe_fd[1], buffer, strlen(buffer) + 1);
        if (bytes <= 0) {
            if (errno == EINTR) break;
            perror("child write");
            break;
        }
        printf("Child sent: %s\n", buffer);

        sem_op(SEM_PARENT, 1);
    }

    cleanup(0);
    exit(0);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(1);
    }

    semid = semget(SEM_KEY, 2, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1) {
        if (errno == EEXIST) {
            semid = semget(SEM_KEY, 2, 0666);
        } else {
            perror("semget");
            exit(1);
        }
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    
    arg.val = 1;
    if (semctl(semid, SEM_PARENT, SETVAL, arg) == -1) {
        perror("semctl SEM_PARENT");
        exit(1);
    }
    
    arg.val = 0;
    if (semctl(semid, SEM_CHILD, SETVAL, arg) == -1) {
        perror("semctl SEM_CHILD");
        exit(1);
    }

    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0) {
        child_process();
    } else {
        parent_process();
    }

    return 0;
}