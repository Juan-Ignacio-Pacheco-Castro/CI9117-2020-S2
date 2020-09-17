#define _XOPEN_SOURCE 600 // Optional for VS Studio to recognize certain values

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    size_t total;
    size_t position;
    pthread_mutex_t mutex_position;
    pthread_cond_t cond_var_ready;
    pthread_mutex_t mutex_ready;
    size_t num_ready;
} shared_data_t;

typedef struct {
    size_t thread_num;
    shared_data_t* shared_data;

} thread_data_t;


void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds)
{
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}

void* run(void* args) {

    random_sleep(3000, 10000);

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;

    pthread_mutex_lock(&shared_data->mutex_ready);
    ++shared_data->num_ready;
    printf("Thread %zu/%zu: I am ready. But I need to wait!\n", thread_num, shared_data->total);
    
    if (shared_data->num_ready == shared_data->total) {
        printf("Thread %zu/%zu: We are all ready. Go!\n", thread_num, shared_data->total);
        pthread_cond_broadcast(&shared_data->cond_var_ready);
    }
    
    /*
    if (shared_data->num_ready < shared_data->total) {
        pthread_cond_wait(&shared_data->cond_var_ready, &shared_data->mutex_ready);
    }*/

    while(shared_data->num_ready < shared_data->total) {
        pthread_cond_wait(&shared_data->cond_var_ready, &shared_data->mutex_ready);
    }
    
    pthread_mutex_unlock(&shared_data->mutex_ready);

    pthread_mutex_lock(&shared_data->mutex_position);

    printf("Thread %zu/%zu: I arrived at position %zu\n", thread_num, shared_data->total, shared_data->position);

    ++shared_data->position;
    
    pthread_mutex_unlock(&shared_data->mutex_position);
    
    return NULL;
}


int main(int argc, char* arg[]) {

	srand( time(NULL) );
    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));

    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));

    shared_data->total = thread_count;
    shared_data->position = 1;
    shared_data->num_ready = 0;
    pthread_mutex_init(&shared_data->mutex_position, /*attr*/ NULL);

    pthread_cond_init(&shared_data->cond_var_ready, NULL);
    pthread_mutex_init(&shared_data->mutex_ready, NULL);    

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, run, (void*)&thread_data_list[i]);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&shared_data->mutex_ready);
    pthread_cond_destroy(&shared_data->cond_var_ready);
    pthread_mutex_destroy(&shared_data->mutex_position);
    free(threads);
    free(shared_data);
    free(thread_data_list);

    return 0;
}