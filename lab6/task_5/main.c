#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int thread_id;
    char *message;
} ThreadData;

void *child_thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int num_lines = rand() % 10 + 1;
    for (int i = 0; i < num_lines; i++) {
        int random_number = rand() % 100;
        printf("Thread %d: %s - Random Number: %d\n", data->thread_id, data->message, random_number);
        sleep(1);
    }
    pthread_exit(NULL);
}

void create_threads(int num_threads) {
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i + 1;
        thread_data[i].message = "Processing data";
        if (pthread_create(&threads[i], NULL, child_thread_function, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    srand(time(NULL));
    create_threads(num_threads);

    return EXIT_SUCCESS;
}
