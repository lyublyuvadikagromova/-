#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *child_thread_function(void *arg) {
    int iterations = *(int *)arg * 2;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    for (int i = 1; i <= iterations; i++) {
        printf("Iteration %d\n", i);
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

    if (pthread_create(&child_thread, NULL, child_thread_function, &delay_seconds) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    sleep(delay_seconds);

    if (pthread_cancel(child_thread) != 0) {
        perror("Failed to cancel thread");
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
