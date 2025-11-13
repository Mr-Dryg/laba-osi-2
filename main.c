#include "include/filter.h"
#include "include/matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>

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
    }

    pthread_exit(NULL);
}

int main()
{
    // get from user
    num_threads = 5;
    int filter_overlays = 5;
    char filename[] = "resources/matrix1.txt";

    public_task pub_task;
    pub_task.erosion_origin = load_matrix_from_file(filename);
    pub_task.dilation_origin = copy_matrix(pub_task.erosion_origin);
    pub_task.rows = pub_task.erosion_origin->rows;
    pub_task.columns = pub_task.erosion_origin->columns;
    pub_task.erosion_res = create_matrix(pub_task.rows, pub_task.columns);
    pub_task.dilation_res = create_matrix(pub_task.rows, pub_task.columns);
    pub_task.filter_overlays = filter_overlays;
    pub_task.working_threds = num_threads;
    pub_task.step = num_threads;

    pthread_t threads[num_threads];
    pthread_mutex_init(&mutex_working_threds, NULL);
    pthread_barrier_init(&barrier, NULL, num_threads);
    int status;

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

    save_matrix_in_file(pub_task.erosion_res, "res/erosion_res.txt");
    save_matrix_in_file(pub_task.dilation_res, "res/dilation_res.txt");

    pthread_mutex_destroy(&mutex_working_threds);
    pthread_barrier_destroy(&barrier);

    destroy_matrix(pub_task.erosion_origin);
    destroy_matrix(pub_task.dilation_origin);
    destroy_matrix(pub_task.erosion_res);
    destroy_matrix(pub_task.dilation_res);

    return 0;
}