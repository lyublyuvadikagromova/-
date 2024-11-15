#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5   // Розмір буфера
#define PRODUCER_COUNT 2 // Кількість потоків виробників
#define CONSUMER_COUNT 2 // Кількість потоків споживачів

int buffer[BUFFER_SIZE];
int count = 0;  // Лічильник кількості елементів у буфері
int in = 0;     // Індекс для вставки в буфер
int out = 0;    // Індекс для вилучення з буфера

pthread_mutex_t mutex;
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

void *producer(void *param) {
    while (1) {
        int item = rand() % 100;
        sleep(2);  // Затримка для імітації роботи виробника

        pthread_mutex_lock(&mutex);
        
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Виробник %ld згенерував: %d\n", (long)param, item);

        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *param) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Споживач %ld забрав: %d\n", (long)param, item);

        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        sleep(1);  // Затримка для імітації роботи споживача
    }
}

int main() {
    pthread_t producers[PRODUCER_COUNT], consumers[CONSUMER_COUNT];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    for (long i = 0; i < PRODUCER_COUNT; i++) {
        pthread_create(&producers[i], NULL, producer, (void *)i);
    }
    for (long i = 0; i < CONSUMER_COUNT; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void *)i);
    }

    sleep(20);  // Час виконання програми

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_cancel(producers[i]);
    }
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_cancel(consumers[i]);
    }

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
