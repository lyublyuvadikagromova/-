#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_CATALAN 5
#define NUM_PRIMES 5

unsigned long long catalan_result[NUM_CATALAN];

unsigned long long catalan(int n) {
    if (n == 0) return 1;
    return (2 * (2 * n - 1) * catalan(n - 1)) / (n + 1);
}

void* calculate_catalan(void* arg) {
    for (int i = 0; i < NUM_CATALAN; i++) {
        catalan_result[i] = catalan(i);
    }
    pthread_exit((void*)catalan_result); }

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* calculate_primes(void* arg) {
    static int primes[NUM_PRIMES];
    int count = 0;
    int num = 2;

    while (count < NUM_PRIMES) {
        if (is_prime(num)) {
            primes[count] = num;
            count++;
        }
        num++;
    }
    pthread_exit((void*)primes); 
}

int main() {
    pthread_t thread1, thread2;
    void* catalan_result_ptr;
    void* primes_result_ptr;

    if (pthread_create(&thread1, NULL, calculate_catalan, NULL) != 0) {
        perror("Error creating Catalan thread");
        return 1;
    }

    if (pthread_create(&thread2, NULL, calculate_primes, NULL) != 0) {
        perror("Error creating Primes thread");
        return 1;
    }

    if (pthread_join(thread1, &catalan_result_ptr) != 0) {
        perror("Error joining Catalan thread");
        return 1;
    }

    if (pthread_join(thread2, &primes_result_ptr) != 0) {
        perror("Error joining Primes thread");
        return 1;
    }

    printf("First %d Catalan numbers:\n", NUM_CATALAN);
    unsigned long long* catalan_numbers = (unsigned long long*)catalan_result_ptr;
    for (int i = 0; i < NUM_CATALAN; i++) {
        printf("%llu ", catalan_numbers[i]);
    }
    printf("\n");

    printf("First %d prime numbers:\n", NUM_PRIMES);
    int* prime_numbers = (int*)primes_result_ptr;
    for (int i = 0; i < NUM_PRIMES; i++) {
        printf("%d ", prime_numbers[i]);
    }
    printf("\n");

    return 0;
}
