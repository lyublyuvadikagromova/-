#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void* print_text(void* arg) {
    int repeat = *(int*)arg;
    for (int i = 1; i <= repeat; i++) {
        printf("Thread 1: Iteration %d\n", i);
        sleep(1); 
    }
    printf("Thread 1 has finished its work.\n");
    return NULL;
}

void* generate_random_numbers(void* arg) {
    int count = *(int*)arg;
    int random_number;

    srand(time(NULL));
    for (int i = 1; i <= count; i++) {
        random_number = rand() % 100;
        printf("Thread 2: Random number %d\n", random_number);
        if (random_number == 50) {
            printf("Thread 2 generated the number 50 and will now exit.\n");
            pthread_exit(NULL); 
        }
        sleep(1); 
    }
    printf("Thread 2 has finished its work.\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int repeat = 5; 
    int random_count = 5;  

    if (pthread_create(&thread1, NULL, print_text, &repeat) != 0) {
        perror("Error creating thread 1");
        return 1;
    }

    if (pthread_create(&thread2, NULL, generate_random_numbers, &random_count) != 0) {
        perror("Error creating thread 2");
        return 1;
    }

    if (pthread_join(thread1, NULL) != 0) {
        perror("Error joining thread 1");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("Error joining thread 2");
        return 1;
    }

    printf("Main thread: Both threads have completed their work.\n");
    return 0;
}
