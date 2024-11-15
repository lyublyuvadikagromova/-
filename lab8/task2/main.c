#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int data;
int data_available = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

void *producer(void *param) {
    while (1) {
        sleep(3);  // Великий інтервал для імітації затримки
        int item = rand() % 100;
        
        pthread_mutex_lock(&mutex);
        
        while (data_available) {
            pthread_cond_wait(&cond_producer, &mutex);
        }

        data = item;
        data_available = 1;
        printf("Виробник згенерував: %d\n", item);

        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *param) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (!data_available) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        int item = data;
        data_available = 0;
        printf("Споживач забрав: %d\n", item);

        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        sleep(2);  // Інтервал для споживача
    }
}

int main() {
    pthread_t prod, cons;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    sleep(15);  // Час виконання програми

    pthread_cancel(prod);
    pthread_cancel(cons);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
