#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct{
    sem_t followerQueue;
    sem_t leaderQueue;

}shared_data_t;

typedef struct{
    shared_data_t* shared_data;

}thread_data_t;


void dance(){
    printf("Estoy bailando\n");
}

void* follower(void* args) {

    shared_data_t* shared_data = (shared_data_t*) args;
    printf("Soy un seguidor y estoy esperando\n");
    sem_post(& shared_data->leaderQueue);
    sem_wait(& shared_data->followerQueue);
    dance();
}

void* leader(void* args) {

    shared_data_t* shared_data = (shared_data_t*) args;
    printf("Soy un líder\n");
   sem_post(& shared_data->followerQueue);
   sem_wait(& shared_data->leaderQueue);
   dance();
}



int main(int argc, char* arg[]){
    size_t thread_count;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
        if(thread_count % 2 != 0){
            printf("Invalid number of params, it has to be an even number\n");
            return 1;
        }
    } else {
        printf("Invalid number of params\n");
        return 1;
    }
    
    
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    

    sem_init(&shared_data->leaderQueue,0,0);
    sem_init(&shared_data->followerQueue,0,0);
    
    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    
    
    for(size_t i = 0; i < thread_count/2; ++i){
        pthread_create(&threads[i], NULL, follower, (void*)shared_data);
    }
    
    for(size_t i = thread_count/2 ; i < thread_count ; ++i){
        pthread_create(&threads[i], NULL, leader , (void*)shared_data);
    }


    
    for(size_t i = 0; i < thread_count ; ++i){
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&shared_data->followerQueue);
    sem_destroy(&shared_data->leaderQueue);

    free(shared_data);
}
