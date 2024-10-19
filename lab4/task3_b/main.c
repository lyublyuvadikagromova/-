#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void create_child_processes(int n) {
    pid_t pid;
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            printf("Child process %d started. PID: %d\n", i + 1, getpid());
            while (1) {
                sleep(1);
            }
            exit(0);  
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        fprintf(stderr, "Please specify a valid number of processes.\n");
        exit(EXIT_FAILURE);
    }

    create_child_processes(num_processes);

    printf("Parent process listing child processes:\n");
    system("ps -x");
    sleep(5);
    printf("Parent process killing child processes...\n");
    system("killall -9 task3_b");

    while (wait(NULL) > 0);

    printf("All child processes terminated.\n");

    return 0;
}
