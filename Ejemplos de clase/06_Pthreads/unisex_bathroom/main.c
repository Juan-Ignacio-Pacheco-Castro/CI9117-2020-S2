#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

//definicion de constantes
#define THREAD_COUNT 15

//definiciones de tipos
typedef struct {
    int counter;
    sem_t * s;
} Lightswitch;

//constructor de semaphore
sem_t *make_semaphore (int n) {
    sem_t *sem = malloc (sizeof(sem_t));
    int ret = sem_init(sem, 0, n);
    if (ret == -1) {
        //exit si no se puede inicializar el semaphore
        perror ("sem_init failed");
        exit(-1);
    }
    return sem;
}

//declaracion de variables
sem_t * empty;
Lightswitch maleSwitch;
Lightswitch femaleSwitch;
sem_t * maleMultiplex;
sem_t * femaleMultiplex;
sem_t * turnstile; //barrier

//constructor de lightswitch
Lightswitch make_lightswitch() {
    Lightswitch l;
    l.counter=0;
    l.s = make_semaphore(1);
    return l;
}

//funcion lock del lightswitch
void lock (Lightswitch * lightswitch, sem_t *semaphore) {
    sem_wait(lightswitch->s);
    ++(lightswitch->counter);
    if (lightswitch->counter==1) {
        sem_wait(semaphore);
    }
    sem_post(lightswitch->s);
}

//funcion unlock del lightswitch
void unlock (Lightswitch * lightswitch, sem_t *semaphore) {
    sem_wait(lightswitch->s);
    --(lightswitch->counter);
    if (lightswitch->counter==0) {
        sem_post(semaphore);
    }
    sem_post(lightswitch->s);
}

//hombre entra
void* male() {
    sem_wait(turnstile);
    lock(&maleSwitch,empty);
    int c = maleSwitch.counter;
    sem_post(turnstile);
    sem_wait(maleMultiplex);
    printf("Hombre entra...(%i)\n",c);
    sleep(1);
    printf("Hombre sale\n");
    sem_post(maleMultiplex);
    unlock(&maleSwitch,empty);
    return NULL;
}

//mujer entra
void * female() {
    sem_wait(turnstile);
    lock(&femaleSwitch,empty);
    int c = femaleSwitch.counter;
    sem_post(turnstile);
    sem_wait(femaleMultiplex);
    printf("Mujer entra...(%i)\n",c);
    sleep(1);
    printf("Mujer sale\n");
    sem_post(femaleMultiplex);
    unlock(&femaleSwitch,empty);
    return NULL;
}

int main() {
    //inicializar variables
    empty = make_semaphore(1);
    maleSwitch = make_lightswitch();
    femaleSwitch = make_lightswitch();
    maleMultiplex = make_semaphore(3);
    femaleMultiplex = make_semaphore(3);
    turnstile = make_semaphore(1);
    printf("variables inicializadas\n");

    //se crean los hilos
    pthread_t * threads = malloc((size_t)(THREAD_COUNT * sizeof(pthread_t)));

    srand(time(NULL));
    for(int i=0;i<THREAD_COUNT;++i) {
        //si un numero entero pseudo aleatorio es par, entra un hombre
        //si no, entra una mujer
        if(rand()%2==0) {
            pthread_create(&threads[i],NULL,male,NULL);
        }
        else {
            pthread_create(&threads[i],NULL,female,NULL);
        }
    }

    //se espera que terminen todos los threads
    for(int i=0;i<THREAD_COUNT;++i) {
        pthread_join(threads[i],NULL);
    }

    //liberar memoria
    sem_destroy(empty);
    sem_destroy(maleMultiplex);
    sem_destroy(femaleMultiplex);
    sem_destroy(maleSwitch.s);
    sem_destroy(femaleSwitch.s);
    printf("memoria liberada\n");

    return 0;
}