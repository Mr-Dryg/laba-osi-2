// test_parallel.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 10000000
#define ITERATIONS 100

int num_threads;
double data[SIZE];
pthread_barrier_t barrier;

void* worker(void* arg) {
    long thread_id = (long)arg;
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int iter = 0; iter < ITERATIONS; iter++) {
        // Простая работа, которую можно распараллелить
        for (size_t i = thread_id; i < SIZE; i += num_threads) {
            data[i] = data[i] * 1.000001;
        }
        pthread_barrier_wait(&barrier);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + 
                    (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Поток %ld: %.4f сек\n", thread_id, elapsed);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Использование: %s <число_потоков>\n", argv[0]);
        return 1;
    }
    
    num_threads = atoi(argv[1]);
    
    // Инициализация данных
    for (size_t i = 0; i < SIZE; i++) {
        data[i] = (double)i;
    }
    
    pthread_barrier_init(&barrier, NULL, num_threads);
    pthread_t threads[num_threads];
    
    struct timespec total_start, total_end;
    clock_gettime(CLOCK_MONOTONIC, &total_start);
    
    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &total_end);
    double total_elapsed = (total_end.tv_sec - total_start.tv_sec) + 
                          (total_end.tv_nsec - total_start.tv_nsec) / 1e9;
    
    printf("\nВсего времени: %.4f сек с %d потоками\n", 
           total_elapsed, num_threads);
    
    pthread_barrier_destroy(&barrier);
    return 0;
}