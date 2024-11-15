#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t flag = 0;

void handle_sigusr1(int sig) {
    flag = 1;
}

int main() {
    signal(SIGUSR1, handle_sigusr1);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Дочірній процес
        for (int i = 0; i < 5; i++) {
            while (!flag);  // Чекає на сигнал від батьківського процесу
            flag = 0;
            printf("Дочірній процес: Повідомлення %d\n", i + 1);
            kill(getppid(), SIGUSR1);  // Надсилає сигнал батьківському процесу
        }
    } else {
        // Батьківський процес
        for (int i = 0; i < 5; i++) {
            printf("Батьківський процес: Повідомлення %d\n", i + 1);
            kill(pid, SIGUSR1);  // Надсилає сигнал дочірньому процесу
            while (!flag);  // Чекає на сигнал від дочірнього процесу
            flag = 0;
        }
        wait(NULL);  // Чекає завершення дочірнього процесу
    }

    return 0;
}
