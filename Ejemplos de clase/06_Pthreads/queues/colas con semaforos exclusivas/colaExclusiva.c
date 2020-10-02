#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct{
    sem_t followerQueue;
    sem_t leaderQueue;
    sem_t mutex;
    sem_t rendezvouz;
    size_t follower;
    size_t leader;
}shared_data_t;

void dance(){
    printf("Estoy bailando\n");
}

void* follower(void* args) {
    
    shared_data_t* shared_data = (shared_data_t*) args;
    sem_wait(&shared_data->mutex);
    
    if(shared_data->leader > 0){
        shared_data->leader--;
        sem_post(&shared_data->leaderQueue);
    }
    else{
        printf("Soy un seguidor y estoy esperando\n");
        shared_data->follower++;
        sem_post(&shared_data->mutex);
        sem_wait(&shared_data->followerQueue);
        
    }
    
    dance();
    sem_post(&shared_data->rendezvouz);
    

}

void* leader(void* args) {

    shared_data_t* shared_data = (shared_data_t*) args;
    sem_wait(&shared_data->mutex);
    if(shared_data->follower > 0){
         printf("Soy un líder\n");
        shared_data->follower--;
        sem_post(&shared_data->followerQueue);
    }
    else{
        shared_data->leader++;
        sem_post(&shared_data->mutex);
        sem_wait(&shared_data->leaderQueue);
    }
    dance();
    sem_wait(&shared_data->rendezvouz);
    sem_post(&shared_data->mutex);
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
    
    shared_data->leader = 0;
    shared_data->follower = 0;

    sem_init(&shared_data->leaderQueue,0,0);
    sem_init(&shared_data->followerQueue,0,0);
    
    sem_init(&shared_data->mutex,0,1);

    sem_init(&shared_data->rendezvouz,0,0);
    
    
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
    
    
    sem_destroy(&shared_data->mutex);
    sem_destroy(&shared_data->rendezvouz);
    

    free(shared_data);
}