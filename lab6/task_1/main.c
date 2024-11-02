#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 100

int results[MAX_THREADS];
int num_threads = 0;
int completed_threads = 0;
pthread_mutex_t lock;

void *calculation_thread(void *arg) {
    int thread_num = *(int *)arg;
    sleep(thread_num);
    int random_number = rand() % 10 + 1;
    pthread_mutex_lock(&lock);
    results[thread_num] = random_number;
    completed_threads++;
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void *monitor_thread(void *arg) {
    while (1) {
        sleep(1);
        pthread_mutex_lock(&lock);
        printf("Current state of results array:\n");
        for (int i = 0; i < num_threads; i++) {
            if (results[i] != 0) {
                printf("Thread %d: %d\n", i, results[i]);
            } else {
                printf("Thread %d: Not completed yet\n", i);
            }
        }
        if (completed_threads == num_threads) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }
    printf("All threads have completed their work. Monitor thread exiting.\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    num_threads = atoi(argv[1]);
    if (num_threads > MAX_THREADS) {
        fprintf(stderr, "Maximum number of threads is %d.\n", MAX_THREADS);
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);

    pthread_t threads[num_threads];
    pthread_t monitor;
    int thread_nums[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_nums[i] = i;
        if (pthread_create(&threads[i], NULL, calculation_thread, &thread_nums[i]) != 0) {
            perror("Failed to create thread");
            return EXIT_FAILURE;
        }
    }

    if (pthread_create(&monitor, NULL, monitor_thread, NULL) != 0) {
        perror("Failed to create monitor thread");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_join(monitor, NULL);
    pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}
