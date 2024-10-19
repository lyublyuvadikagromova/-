#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define SECONDS 5  

void run_process(const char *process_name, int duration) {
    time_t start_time = time(NULL);
    time_t end_time = start_time + duration;
    unsigned long counter = 0;

    while (time(NULL) < end_time) {
        counter++;  
    }

    printf("%s process finished. Counter = %lu\n", process_name, counter);
}

int main(int argc, char *argv[]) {
    int duration = SECONDS;

    if (argc > 1) {
        duration = atoi(argv[1]);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        run_process("Child", duration);
        exit(0);
    } else {
        run_process("Parent", duration);
        wait(NULL);
    }

    return 0;
}
