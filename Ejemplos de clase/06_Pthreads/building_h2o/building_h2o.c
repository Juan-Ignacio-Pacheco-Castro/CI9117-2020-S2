#include "building_h2o.h"

static pthread_mutex_t release_mutex = PTHREAD_MUTEX_INITIALIZER;

H2O* h2oCreate() {
    H2O* obj = (H2O*) malloc(sizeof(H2O));
    obj->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    obj->h_ready = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
    obj->o_ready = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
    obj->h_waiting =  0;
    obj->o_waiting =  0;
    
    pthread_mutex_init(obj->mutex,NULL);
    pthread_cond_init(obj->o_ready,NULL);
    pthread_cond_init(obj->h_ready,NULL);
    
    return obj;
}

void release_oxygen(){
    pthread_mutex_lock(&release_mutex);
    printf("O");
    pthread_mutex_unlock(&release_mutex);
}

void release_hydrogen(){
    pthread_mutex_lock(&release_mutex);
    printf("H");
    pthread_mutex_unlock(&release_mutex);
}
void hydrogen(H2O* obj) {
    pthread_mutex_lock(obj->mutex);
    
    if(((obj->o_waiting) != 0) && (obj->h_waiting >= 1)){   // oxygen waiting and hydrogen ready so signal oxygen to unlock us   
        --(obj->h_waiting);
        release_hydrogen();
        release_hydrogen();
        --(obj->o_waiting);
        release_oxygen();
        pthread_cond_signal(obj->h_ready);
        pthread_cond_signal(obj->o_ready); 
    }
    else{
        ++(obj->h_waiting);
        pthread_cond_wait(obj->o_ready, obj->mutex);        
    }
    
    pthread_mutex_unlock(obj->mutex);
}

void oxygen(H2O* obj) {
    pthread_mutex_lock(obj->mutex);
    
    if((obj->h_waiting) < 2){
        ++(obj->o_waiting);
        pthread_cond_wait(obj->h_ready,obj->mutex);
    }
    else{
        (obj->h_waiting)-=2;
        release_hydrogen();
        release_hydrogen();
        release_oxygen();
        pthread_cond_signal(obj->o_ready);
        pthread_cond_signal(obj->o_ready);
    }
    pthread_mutex_unlock(obj->mutex);
}

void h2oFree(H2O* obj) {
    // User defined data may be cleaned up here.
    free(obj->mutex);
    free(obj->o_ready);
    free(obj->h_ready);
    free(obj);
}
