#include "include/filter.h"
#include "include/matrix.h"
#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
// #include <time.h>
#include <sys/time.h>

typedef struct
{
    ull rows;
    ull columns;
    matrix* erosion_origin;
    matrix* dilation_origin;
    matrix* erosion_res;
    matrix* dilation_res;
    int filter_overlays;
    int working_threds;
    ull step;
} public_task;

typedef struct
{
    public_task* pub_task;
    ull index_0;
} task;

int num_threads;
pthread_mutex_t mutex_working_threds;
pthread_barrier_t barrier;

void* thread_function(void* arg)
{
    task* data = arg;
    for (int i = 0; i < data->pub_task->filter_overlays; i++)
    {
        erosion_filter(
            data->pub_task->erosion_origin,
            data->pub_task->erosion_res,
            data->index_0,
            data->pub_task->step
        );
        
        dilation_filter(
            data->pub_task->dilation_origin,
            data->pub_task->dilation_res,
            data->index_0,
            data->pub_task->step
        );

        pthread_barrier_wait(&barrier);

        if (data->index_0 == 0)
        {
            matrix* tmp = data->pub_task->erosion_origin;
            data->pub_task->erosion_origin = data->pub_task->erosion_res;
            data->pub_task->erosion_res = tmp;

            tmp = data->pub_task->dilation_origin;
            data->pub_task->dilation_origin = data->pub_task->dilation_res;
            data->pub_task->dilation_res = tmp;
        }

        pthread_barrier_wait(&barrier);
    }
    free(data);
    pthread_exit(NULL);
}

// double elapsed_time(clock_t abs_end, clock_t abs_start)
// {
//     return ((double) (abs_end - abs_start)) / CLOCKS_PER_SEC;;
// }

double elapsed_time(struct timeval end_tv, struct timeval start_tv)
{
    long long start, end;
    start = start_tv.tv_sec * 1000000 + start_tv.tv_usec;
    end = end_tv.tv_sec * 1000000 + end_tv.tv_usec;
    return (end - start) / 1000000.0;
}

int main(int argc, char* argv[])
{
    if (argc != 6) {
        // ./main 1 resources/matrix.txt 1000 res/erosion_res.txt res/dilation_res.txt
        printf("Correct args: %s <k> <matrix_file> <iterations> <erosion_res_file> <dilation_res_file>\n", argv[0]);
        return 1;
    }
    // clock_t abs_start = clock();
    // time_t abs_start = time(NULL);
    struct timeval abs_start;
    gettimeofday(&abs_start, NULL);

    num_threads = atoi(argv[1]);
    char* filename = argv[2];
    int filter_overlays = atoi(argv[3]);
    char* erosion_res_file = argv[4];
    char* dilation_res_file = argv[5];

    public_task pub_task;
    pub_task.erosion_origin = load_matrix_from_file(filename);
    pub_task.dilation_origin = copy_matrix(pub_task.erosion_origin);
    pub_task.rows = pub_task.erosion_origin->rows;
    pub_task.columns = pub_task.erosion_origin->columns;
    pub_task.erosion_res = copy_matrix(pub_task.erosion_origin);
    pub_task.dilation_res = copy_matrix(pub_task.erosion_origin);
    pub_task.filter_overlays = filter_overlays;
    pub_task.working_threds = num_threads;
    pub_task.step = num_threads;

    pthread_t threads[num_threads ? num_threads : 1];
    pthread_mutex_init(&mutex_working_threds, NULL);
    pthread_barrier_init(&barrier, NULL, num_threads ? num_threads : 1);
    int status;

    // clock_t threads_start = clock();
    // time_t threads_start = time(NULL);
    struct timeval threads_start;
    gettimeofday(&threads_start, NULL);

    for (int i = 0; i < num_threads; i++)
    {
        task* t = malloc(sizeof(task));
        if (t == NULL)
        {
            perror("Memory allocation error");
            destroy_matrix(pub_task.erosion_origin);
            destroy_matrix(pub_task.dilation_origin);
            destroy_matrix(pub_task.erosion_res);
            destroy_matrix(pub_task.dilation_res);
            exit(1);
        }

        t->pub_task = &pub_task;
        t->index_0 = i;
        status = pthread_create(&threads[i], NULL, thread_function, t);
        if (status)
        {
            perror("Thread creation error");
            destroy_matrix(pub_task.erosion_origin);
            destroy_matrix(pub_task.dilation_origin);
            destroy_matrix(pub_task.erosion_res);
            destroy_matrix(pub_task.dilation_res);
            free(t);
            exit(1);
        }
    }

    for (int i = 0; i < num_threads; i++)
    {
        status = pthread_join(threads[i], NULL);
        if (status)
        {
            perror("Thread joining error");
        }
    }
    // clock_t threads_end = clock();
    // time_t threads_end = time(NULL);
    struct timeval threads_end;
    gettimeofday(&threads_end, NULL);

    save_matrix_in_file(pub_task.erosion_res, erosion_res_file);
    save_matrix_in_file(pub_task.dilation_res, dilation_res_file);

    pthread_mutex_destroy(&mutex_working_threds);
    pthread_barrier_destroy(&barrier);

    destroy_matrix(pub_task.erosion_origin);
    destroy_matrix(pub_task.dilation_origin);
    destroy_matrix(pub_task.erosion_res);
    destroy_matrix(pub_task.dilation_res);

    // clock_t abs_end = clock();
    // time_t abs_end = time(NULL);
    struct timeval abs_end;
    gettimeofday(&abs_end, NULL);

    // double abs_time = elapsed_time(abs_end, abs_start);
    // double threads_time = elapsed_time(threads_end, threads_start);

    // double abs_time = abs_end - abs_start;
    // double threads_time = threads_end - threads_start;

    double abs_time = elapsed_time(abs_end, abs_start);
    double threads_time = elapsed_time(threads_end, threads_start);

    // if (num_threads == 1)
    // {
    //     printf("a = %.4lf\n", (abs_time - threads_time) / abs_time);
    // }
    printf("num_threads = %d: ", num_threads);
    printf("abs_time = %.4lf; threads_time = %.4lf\n", abs_time, threads_time);
    return 0;
}
