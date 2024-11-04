#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_ARRAY_SIZE 10
#define RUN_TIME 10

int *array;
int array_size;
int reader_count = 3;
int writer_count = 2;
pthread_rwlock_t rwlock;

void *writer(void *arg) {
    while (1) {
        int index = rand() % array_size;
        int value = rand() % 100;
        pthread_rwlock_wrlock(&rwlock);
        array[index] = value;
        printf("Writer %ld wrote %d at index %d\n", (long)arg, value, index);
        pthread_rwlock_unlock(&rwlock);
        usleep(rand() % 500000);
    }
    return NULL;
}

void *reader(void *arg) {
    while (1) {
        int index = rand() % array_size;
        pthread_rwlock_rdlock(&rwlock);
        int value = array[index];
        printf("Reader %ld read %d from index %d\n", (long)arg, value, index);
        pthread_rwlock_unlock(&rwlock);
        usleep(rand() % 500000);
    }
    return NULL;
}

void *status_printer(void *arg) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("Array status: ");
        for (int i = 0; i < array_size; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    array_size = (argc > 1) ? atoi(argv[1]) : DEFAULT_ARRAY_SIZE;
    array = malloc(array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    pthread_rwlock_init(&rwlock, NULL);
    pthread_t writers[writer_count], readers[reader_count], printer;
    for (long i = 0; i < writer_count; i++) {
        pthread_create(&writers[i], NULL, writer, (void *)i);
    }
    for (long i = 0; i < reader_count; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)i);
    }
    pthread_create(&printer, NULL, status_printer, NULL);
    sleep(RUN_TIME);
    for (int i = 0; i < writer_count; i++) {
        pthread_cancel(writers[i]);
    }
    for (int i = 0; i < reader_count; i++) {
        pthread_cancel(readers[i]);
    }
    pthread_cancel(printer);
    pthread_rwlock_destroy(&rwlock);
    free(array);
    return 0;
}
