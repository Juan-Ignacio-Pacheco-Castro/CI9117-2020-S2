#ifndef _H2O_
#define _H2O_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    // User defined data may be declared here.
    pthread_mutex_t* mutex;
    pthread_cond_t*  h_ready;
    pthread_cond_t*  o_ready;
    int h_waiting;
    int o_waiting;
} H2O;

H2O* h2oCreate();

void hydrogen(H2O*);

void oxygen(H2O*);

void h2oFree(H2O*);
#endif
