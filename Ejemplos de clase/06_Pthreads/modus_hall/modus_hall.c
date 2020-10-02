#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

   int _heathensCounter = 0; //numero de heathens
   int _prudesCounter = 0; //numero de impostores

   pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER; //Mutex de región crítica

   sem_t _heathensTurn; //Semaforo para los heathens
   sem_t _prudesTurn; //Semaforo para los prudes

void *prudes() {
    
    //Llegada de la cola, es decir los hilos que estaban esperando entrar a la zona critica, van llegando
    pthread_mutex_lock(&_mutex);
    sleep(1);

    _prudesCounter++;
    printf("\nHa llegado un Prude\nPrudes: %d | Heathens: %d\n", _prudesCounter, _heathensCounter);
    
    pthread_mutex_unlock(&_mutex);

    sleep(1);
    
    //Controla el tiempo entre prudes y heathens. Evita que un individuo solicite el mutex sin el turno de la facción
    sem_wait(&_prudesTurn);

    pthread_mutex_lock(&_mutex);
    _prudesCounter--; //Cruce
    
    if(!_prudesCounter){ //Si no hay más prudes, prudes = 0
        if(_heathensCounter){//Y hay heathens
            sem_post(&_heathensTurn); //Libera la ejecución de los heathens, es el turno de los heathens, banderazo verde a heathens para ocupar                          la zona critica
            }
    }
    
    if(_heathensCounter > _prudesCounter) { //Si hay más heathens que prudes
        sem_post(&_heathensTurn); //Libera la ejecución de los hetahens
    } else {  //De lo contrario, sigue corriendo los prudes, turno de los Prudes
        sem_post(&_prudesTurn);
        } 

    printf("\nPRUDE sale de la zona critica\nRestantes: %d Prudes | %d Heathens\n", _prudesCounter, _heathensCounter); 
    
    pthread_mutex_unlock(&_mutex);
    pthread_exit(0);
}

void *heathens() {

    //Llegada en fila de heathens
    pthread_mutex_lock(&_mutex);
    sleep(1);
    
    _heathensCounter++;  //llegada de un heathens a la fila
    printf("\nHa llegado un Heathen\nPrudes: %d | Heathens: %d\n", _prudesCounter, _heathensCounter);
    
    pthread_mutex_unlock(&_mutex);
    
    sleep(1);
    
    //Controla el tiempo entre prudes y heathens. Evita que un individuo solicite el mutex sin el turno de la facción
    sem_wait(&_heathensTurn);
    
    pthread_mutex_lock(&_mutex);
    _heathensCounter--;
    
    if(!_heathensCounter) //Si no hay mas heathebs
        if(_prudesCounter) //Y hay prudes
            sem_post(&_prudesTurn); //Libera la ejecución de los prudes

    if(_prudesCounter > _heathensCounter) { //Si hay más prudes que heathens
        sem_post(&_prudesTurn); //Libera la ejecución de los prudes
    } else {sem_post(&_heathensTurn);} //sino deja la ejecuciòn del turno de los heathens

    printf("\nHEATHEN sale de la zona crìtica\nRestantes: %d Prudes | %d Heathens\n", _prudesCounter, _heathensCounter);

    pthread_mutex_unlock(&_mutex);
    pthread_exit(0);
}

int main() {
    int nHeathens, nPrudes, start;
    pthread_t *_tidPrudes;
    pthread_t *_tidHeathens;
   
    printf("¿Cuántos Heathens? (al menos 1)\n");
    scanf("%d", &nHeathens);
    if(nHeathens < 1) nHeathens = 20;

    printf("¿Cuánto Prudes? (al menos 1)\n");
    scanf("%d", &nPrudes);
    if(nPrudes < 1) nPrudes = 20;
    
    //Define el valor inicial de cada semáforo. El semáforo de la facción más grande comenzará con 1 y el más pequeño con 0. Garantiza la         alternancia para la mayoría desde el primer caso.
    if(nHeathens > nPrudes)
        start = 1; //SemHeathens comienza como 1
    else
        start = 0; //SemPrudes comienza como 1 
    
    _tidHeathens = (pthread_t *) malloc(nHeathens * sizeof(pthread_t)); //Vector de hilo que se inicializará para paganos
    _tidPrudes = (pthread_t *) malloc(nPrudes * sizeof(pthread_t)); //Vector de hilos a inicializar para los Heathens

    //Semáforo de heathens
    if (sem_init(&_heathensTurn, 0, start) < 0) {
        exit(1);
    }

    //Semáforo de prudes
    if (sem_init(&_prudesTurn, 0, !start) < 0) {
        exit(1);
    }
    
    //Creacion de hilos
    if(nHeathens >= nPrudes) {
        //Inicializando los threads
        for(int i = 0; i < nHeathens; i++) {
            pthread_create(&_tidHeathens[i], NULL, &heathens, NULL);
            if(i < nPrudes) {
                pthread_create(&_tidPrudes[i], NULL, &prudes, NULL);
            }
        }
        
        //Espera a que todos sean ejecuatados
        for(int i = 0; i < nHeathens; i++) {
            pthread_join(_tidHeathens[i], NULL);
            if(i < nPrudes) {
                pthread_join(_tidPrudes[i], NULL);
            }
         }
         
    } else {
        //Inicializando los threads
         for(int i = 0; i < nPrudes; i++) {
            pthread_create(&_tidPrudes[i], NULL, &prudes, NULL);
            if(i < nHeathens) {
                pthread_create(&_tidHeathens[i], NULL, &heathens, NULL);
            }
         }
         
         //Espera a que todos sean ejecuatdos
         for(int i = 0; i < nPrudes; i++) {
            pthread_join(_tidPrudes[i], NULL);
            if(i < nHeathens){ 
                pthread_join(_tidHeathens[i], NULL);
            }
         }
        
    }
    return 0;