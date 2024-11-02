#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *child_thread_function(void *arg) {
    int iteration = 1;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (1) {
        printf("Iteration %d\n", iteration++);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <delay_seconds>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int delay_seconds = atoi(argv[1]);
    pthread_t child_thread;

    if (pthread_create(&child_thread, NULL, child_thread_function, NULL) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    sleep(delay_seconds);

    if (pthread_cancel(child_thread) != 0) {
        perror("Failed to cancel thread");
        return EXIT_FAILURE;
    }

    void *res;
    if (pthread_join(child_thread, &res) != 0) {
        perror("Failed to join thread");
        return EXIT_FAILURE;
    }

    if (res == PTHREAD_CANCELED) {
        printf("The thread was canceled.\n");
    } else {
        printf("The thread terminated normally.\n");
    }

    return EXIT_SUCCESS;
}
