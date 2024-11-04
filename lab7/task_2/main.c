#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

double a = 0.0;       // нижня межа інтегрування
double b = 2.0;       // верхня межа інтегрування
double epsilon = 0.0001; // точність обчислення
int p;               
double result = 0.0; 
pthread_mutex_t mutex;

double f(double x) {
    return 4 - x * x; // приклад функції f(x) = 4 - x^2
}

void *integrate(void *arg) {
    int id = *(int *)arg;
    double local_result = 0.0;
    double h = (b - a) / p;
    double x_start = a + id * h;
    double x_end = x_start + h;

    int n = 1000; // кількість часткових прямокутників для кожного потоку
    double step = (x_end - x_start) / n;

    for (int i = 0; i < n; i++) {
        double x = x_start + (i + 0.5) * step;
        local_result += f(x) * step;
    }

    pthread_mutex_lock(&mutex);
    result += local_result;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        p = atoi(argv[1]);
    } else {
        p = 4; // кількість потоків за замовчуванням
    }

    pthread_t threads[p];
    int thread_ids[p];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < p; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, integrate, &thread_ids[i]);
    }

    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Approximate value of the integral: %.6f\n", result);

    return 0;
}
