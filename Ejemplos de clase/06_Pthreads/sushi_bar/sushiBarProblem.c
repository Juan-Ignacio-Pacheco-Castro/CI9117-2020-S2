#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

//Based and adapted from: http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds);

typedef struct 
{
    int eating;
    int waiting;
    bool must_wait;
    sem_t block;                 
    pthread_mutex_t mutex;
}shared_data_t;

typedef struct
{
    int thread_num;
    shared_data_t* shared_data;
}thread_data_t;


void eatingThread(shared_data_t* shared_data, int threadNum){
    
    printf("El hilo %d se sentó a comer sushi. Quedan %d asientos disponibles.\n",threadNum, 5-shared_data->eating-1);
        shared_data->eating++;
        shared_data->must_wait = (shared_data->eating == 5);

    if(shared_data->waiting && (!shared_data->must_wait)) //Si hay mas de 0 esperando y no se debe esperar.
        sem_post(& shared_data->block); //Los hilos esperando entran a comer
    else
        pthread_mutex_unlock(&shared_data->mutex); 
}

void stopEating(shared_data_t* shared_data, int threadNum){
    random_sleep(5000,10000);

    pthread_mutex_lock(&shared_data->mutex);
    shared_data->eating--; 
    printf("El hilo %d dejó de comer. Quedan %d asientos disponibles.\n", threadNum, 5-shared_data->eating);

    if (shared_data->eating == 0) //Si hay 0 hilos comiendo
         shared_data->must_wait = false; //Ya no se debe esperar

    if (shared_data->waiting && !shared_data->must_wait) //Si hay mas de 0 hilos esperando y no deben esperar
        sem_post(&shared_data->block); //Los hilos esperando entrar a comer
    else
        pthread_mutex_unlock(&shared_data->mutex); 
}


void * sushiBar(void * args)
{
    random_sleep(5000,10000);

    thread_data_t* thread_data = (thread_data_t*)args;
    shared_data_t* shared_data =  thread_data->shared_data;
    pthread_mutex_lock(&shared_data->mutex);
    int threadNum =  thread_data->thread_num;

    if(shared_data->must_wait){
        printf("El hilo %d está esperando. Hay esperando %d hilos.\n", threadNum,  shared_data->waiting+1);
        shared_data->waiting++;
        pthread_mutex_unlock(&shared_data->mutex);
        sem_wait(& shared_data->block);//Los hilos esperando quedan aqui
        shared_data->waiting--;
    }

    //EL HILO ENTRA A COMER
    eatingThread(shared_data,threadNum);


    //EL HILO DEJA DE COMER
    stopEating(shared_data,threadNum);

    return 0;
}

int main(int argc, char* arg[]){

    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    srand(time(NULL));

    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    shared_data->eating = 0; 
    shared_data->waiting = 0;
    shared_data->must_wait = false;
    pthread_mutex_init(&shared_data->mutex, NULL);
    sem_init(&shared_data->block, 0, 0);



    pthread_t *threads = (pthread_t *) malloc (thread_count*sizeof(pthread_t));
    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i=0; i<thread_count; i++){
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, sushiBar, (void*)&thread_data_list[i]);
    }

    for(size_t i=0; i<thread_count; i++){
        pthread_join(threads[i],NULL);
    }

    //Liberacion de memoria
    pthread_mutex_destroy(&shared_data->mutex);
    sem_destroy(&shared_data->block);
    free(shared_data);
    free(threads);
    free(thread_data_list);
   
    return 0;
}

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds)
{
    useconds_t duration = min_milliseconds;
    useconds_t range = max_milliseconds - min_milliseconds;
    if ( range > 0 )
        duration += rand() % range;
    usleep( 1000 * duration );
}