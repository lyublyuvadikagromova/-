#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 100000

typedef struct {
    int canceled;
    double pi;
} ThreadData;

void *calculate_pi(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->pi = 0.0;
    int sign = 1;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (int i = 0; i < N; i++) {
        pthread_testcancel();
        data->pi += sign * (1.0 / (2 * i + 1));
        sign = -sign;
    }

    data->canceled = 0;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <delay_seconds>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int delay_seconds = atoi(argv[1]);
    pthread_t thread;
    ThreadData data = { .canceled = 1, .pi = 0.0 };

    if (pthread_create(&thread, NULL, calculate_pi, &data) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    sleep(delay_seconds);

    if (pthread_cancel(thread) != 0) {
        perror("Failed to cancel thread");
    }

    void *res;
    if (pthread_join(thread, &res) != 0) {
        perror("Failed to join thread");
        return EXIT_FAILURE;
    }

    if (data.canceled) {
        printf("Thread was canceled.\n");
    } else {
        printf("Calculated value of pi: %.10f\n", data.pi * 4);
    }

    return EXIT_SUCCESS;
}
