#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char name[50];
    char str[50];
    int num;
} ThreadParams;

void* thread_function(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    for (int i = 1; i <= params->num; i++) {
        printf("Thread %s. %s %d\n", params->name, params->str, i);
    }
    return NULL;
}

int main() {
    pthread_t threads[4];
    ThreadParams params[4] = {
        {"One", "Str", 3},
        {"Two", "Str", 5},
        {"Three", "Str", 4},
        {"Four", "Str", 2}
    };

    for (int i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &params[i]) != 0) {
            perror("Error creating thread");
            return 1;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error joining thread");
            return 1;
        }
    }

    return 0;
}
