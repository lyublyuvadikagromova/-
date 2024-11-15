#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("Отримано сигнал SIGINT (переривання)\n");
    } else if (sig == SIGTERM) {
        printf("Отримано сигнал SIGTERM (завершення)\n");
        exit(0);
    } else if (sig == SIGUSR1) {
        printf("Отримано сигнал SIGUSR1 (користувацький сигнал)\n");
        exit(0);
    }
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGUSR1, handle_signal);
    signal(SIGPROF, SIG_DFL);
    signal(SIGHUP, SIG_IGN);

    while (1) {
        pause();
    }

    return 0;
}
