#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pid_t receiver_pid;
volatile sig_atomic_t ready = 1;

void handler(int sig) {
    ready = 1;
}

void send_bit(int bit) {
    int signal = (bit == 0) ? SIGUSR1 : SIGUSR2;
    kill(receiver_pid, signal);
    ready = 0;
    while (!ready);
}

void send_number(int num) {
    for (int i = 0; i < 32; i++) {
        send_bit((num >> i) & 1);
    }
    kill(receiver_pid, SIGINT);
}

int main() {
    printf("Sender PID: %d\n", getpid());

    printf("Enter receiver PID: ");
    scanf("%d", &receiver_pid);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    int num;
    printf("Enter an integer: ");
    scanf("%d", &num);

    send_number(num);
    printf("Transmission complete.\n");

    return 0;
}