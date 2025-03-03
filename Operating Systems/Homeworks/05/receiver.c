#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

pid_t sender_pid;
volatile sig_atomic_t received_bits[32];
volatile sig_atomic_t counter = 0;

void handler(int sig) {
    received_bits[counter] = (sig == SIGUSR1) ? 0 : 1;
    ++counter;
    kill(sender_pid, SIGUSR1);
}

void end_handler(int sig) {
    int number = 0;
    for (int i = 0; i < counter; i++) {
        number |= (received_bits[i] << i);
    }

    printf("\nReceived number: %d\n", number);
    exit(0);
}

int main() {
    printf("Receiver PID: %d\n", getpid());

    printf("Enter sender PID: ");
    scanf("%d", &sender_pid);

    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    struct sigaction sa_end;
    sa_end.sa_flags = SA_RESTART;
    sa_end.sa_handler = end_handler;
    sigaction(SIGINT, &sa_end, NULL);

    printf("Waiting...\n");

    while (1) {
        pause();
    }

    return 0;
}
