#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char *message;

void handle_alarm(int sig) {
    printf("%s\n", message);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Використання: %s <інтервал в секундах> <повідомлення>\n", argv[0]);
        return 1;
    }

    int seconds = atoi(argv[1]);
    message = argv[2];

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        signal(SIGALRM, handle_alarm);
        alarm(seconds);
        pause();
    } else {
        printf("Основний процес завершено\n");
    }

    return 0;
}
