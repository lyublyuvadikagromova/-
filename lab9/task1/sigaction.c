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
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = SIG_DFL;
    sigaction(SIGPROF, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGHUP, &sa, NULL);

    while (1) {
        pause();
    }

    return 0;
}
