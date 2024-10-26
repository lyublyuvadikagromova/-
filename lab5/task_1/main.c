#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* child_thread_function(void* arg) {
    for (int i = 1; i <= 10; i++) {
        printf("Child Thread. Iteration: %d\n", i);
        sleep(rand() % 3);  
    }
    return NULL;
}

int main() {
    pthread_t child_thread;

    if (pthread_create(&child_thread, NULL, child_thread_function, NULL) != 0) {
        perror("Error creating thread");
        return 1;
    }

    for (int i = 1; i <= 10; i++) {
        printf("Main Thread. Iteration: %d\n", i);
        sleep(rand() % 3);  
    }

    if (pthread_join(child_thread, NULL) != 0) {
        perror("Error joining thread");
        return 1;
    }

    return 0;
}
