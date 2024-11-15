#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_rt_signal(int sig, siginfo_t *info, void *context) {
    int signal_number = info->si_value.sival_int;
    printf("Дочірній процес: отримано сигнал реального часу, номер виклику: %d\n", signal_number);
}

void handle_sigterm(int sig) {
    printf("Дочірній процес: отримано сигнал завершення (SIGTERM)\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Використання: %s <кількість сигналів>\n", argv[0]);
        return 1;
    }

    int num_signals = atoi(argv[1]);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        struct sigaction sa;
        sa.sa_sigaction = handle_rt_signal;
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);

        sigaction(SIGRTMIN, &sa, NULL);

        signal(SIGTERM, handle_sigterm);

        while (1) {
            pause();
        }
    } else {
        for (int i = 1; i <= num_signals; i++) {
            sleep(1);
            union sigval value;
            value.sival_int = i;
            sigqueue(pid, SIGRTMIN, value);
        }

        sleep(1);
        kill(pid, SIGTERM);
        wait(NULL);

        printf("Батьківський процес: дочірній процес завершено\n");
    }

    return 0;
}
