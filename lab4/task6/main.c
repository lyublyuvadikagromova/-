#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child process (zombie) created with PID: %d\n", getpid());
        exit(0);
    } else {
        printf("Parent process. Child process is now a zombie. Run 'ps' to verify.\n");
        system("ps -l | grep Z");
        sleep(5);

        printf("Cleaning up zombie process...\n");
        wait(NULL);
        printf("Zombie process cleaned. Run 'ps' to verify again.\n");
        system("ps -l | grep Z");
    }

    return 0;
}
