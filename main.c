// #include "include/erosion2.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

// Структура для передачи данных в поток
typedef struct {
    int thread_id;
    char message[50];
} thread_data_t;

// Функция, которую выполняет каждый поток
void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    printf("Поток %d: %s\n", data->thread_id, data->message);
    printf("Поток %d: Завершил работу\n", data->thread_id);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int rc;
    
    printf("Основной поток: Создаю %d потоков\n", NUM_THREADS);
    
    // Создаем потоки
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        snprintf(thread_data[i].message, sizeof(thread_data[i].message), 
                "Привет из потока %d!", i);
        
        rc = pthread_create(&threads[i], NULL, thread_function, (void*)&thread_data[i]);
        
        if (rc) {
            printf("Ошибка: не удалось создать поток %d, код ошибки: %d\n", i, rc);
            exit(-1);
        }
    }
    
    // Ожидаем завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            printf("Ошибка: не удалось присоединить поток %d, код ошибки: %d\n", i, rc);
        }
    }
    
    printf("Основной поток: Все потоки завершили работу\n");
    pthread_exit(NULL);
}