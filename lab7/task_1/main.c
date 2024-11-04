#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define DEFAULT_NUM_PAIRS 5
#define DEFAULT_NUM_OPERATIONS 10000

long shared_variable = 0;
int num_pairs;
int num_operations;

void *increment(void *arg) {
    for (int i = 0; i < num_operations; i++) {
        shared_variable++;
    }
    return NULL;
}

void *decrement(void *arg) {
    for (int i = 0; i < num_operations; i++) {
        shared_variable--;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    num_pairs = (argc > 1) ? atoi(argv[1]) : DEFAULT_NUM_PAIRS;
    num_operations = (argc > 2) ? atoi(argv[2]) : DEFAULT_NUM_OPERATIONS;

    printf("Starting program with %d pairs of threads and %d operations per thread.\n", num_pairs, num_operations);
    printf("Initial value of shared variable: %ld\n", shared_variable);

    pthread_t *inc_threads = malloc(num_pairs * sizeof(pthread_t));
    pthread_t *dec_threads = malloc(num_pairs * sizeof(pthread_t));

    for (int i = 0; i < num_pairs; i++) {
        pthread_create(&inc_threads[i], NULL, increment, NULL);
        pthread_create(&dec_threads[i], NULL, decrement, NULL);
    }

    for (int i = 0; i < num_pairs; i++) {
        pthread_join(inc_threads[i], NULL);
        pthread_join(dec_threads[i], NULL);
    }

    printf("Final value of shared variable: %ld\n", shared_variable);

    free(inc_threads);
    free(dec_threads);

    return 0;
}
