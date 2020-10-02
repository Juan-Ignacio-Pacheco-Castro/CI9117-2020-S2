#include <string.h>

#include "building_h2o.h"

typedef struct{
    pthread_barrier_t* barrier_ptr;
    H2O* obj;
}data;

void* create_oxygen(void* args){
    data* shared_data = (data*)args;
    printf("Created: O\n");

    pthread_barrier_wait(shared_data->barrier_ptr);
    oxygen(shared_data->obj);
}

void* create_hydrogen(void* args){
    data* shared_data = (data*)args;
    printf("Created: H\n");

    pthread_barrier_wait(shared_data->barrier_ptr);
    hydrogen(shared_data->obj);
}


int main(int argc, char* argv[]){
    
    size_t total_h2o = 10;
    size_t i;

    if(argc == 2){
        size_t temp;
        sscanf(argv[1],"%d",&temp);
        if(temp > 0)
            total_h2o = temp;
    }


    // ************** Memory Initialization ****************************************
    data* shared_data = (data*)malloc(sizeof(shared_data));
    shared_data->barrier_ptr = (pthread_barrier_t*)malloc(sizeof(pthread_barrier_t));
    shared_data->obj = (H2O*)malloc(sizeof(H2O));

    pthread_t* oxy_threads = (pthread_t*)malloc(total_h2o*sizeof(pthread_t));
    pthread_t* hyd_threads = (pthread_t*)malloc(total_h2o*2*sizeof(pthread_t));

    pthread_barrier_init(shared_data->barrier_ptr, NULL, total_h2o*3);

    shared_data->obj = h2oCreate();
    // ************** ********************** ****************************************
    


    for(i = 0;i < total_h2o; ++i){
        pthread_create(oxy_threads + i, NULL, create_oxygen,(void*)shared_data);
        pthread_create(hyd_threads + i*2, NULL, create_hydrogen,(void*)shared_data);
        pthread_create(hyd_threads + i*2+1, NULL, create_hydrogen,(void*)shared_data);
    }

    for(i = 0; i < total_h2o; ++i)
        pthread_join(oxy_threads[i], NULL);

    for(i = 0; i < total_h2o*2; ++i)
        pthread_join(hyd_threads[i], NULL);

    printf("\nEND of program\n");

    // **************** Memory Claim *************************************************
    h2oFree(shared_data->obj);

    pthread_barrier_destroy(shared_data->barrier_ptr);
    
    free(shared_data->barrier_ptr);
    free(shared_data);
    
    free(oxy_threads);
    free(hyd_threads);

}
