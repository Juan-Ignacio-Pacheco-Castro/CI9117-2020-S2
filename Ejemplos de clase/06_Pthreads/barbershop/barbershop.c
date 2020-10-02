#define _XOPEN_SOURCE 600

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

// Va a actuar de memoria compartida
typedef struct{    
    int num_customers;
    size_t num_waiting_room;

    size_t customers_sitting;

    pthread_mutex_t mutex;

    sem_t barber_semaphore;
    sem_t customer_semaphore;
    sem_t barber_chair;

}barbershop_t; 

typedef struct{
    size_t customer_id;
    barbershop_t* barber_shop;
}customer_t;



// Metodos propios para el cliente
int customer_arrives(size_t id);
int customer_sits(size_t id);
int customer_leaves(size_t id);
int customer_leaves_full(size_t id);
int get_haircut(size_t id);

// Metodos propios para el barbero
int cut_hair();
int barber_leaves();
int barber_wakes();
int barber_sleeps();

// otros metodos
void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds);


void* barber_method(void * args){
    
    customer_t* private_data = (customer_t*)args;
    barbershop_t* shared_data = (barbershop_t*)private_data->barber_shop;

    printf("Barber has arrived \n");

    int stop = 1;
    size_t customers_remaining = shared_data->num_customers;
    size_t customers_sitting = 0;


    while(stop){
        // este sleep, se tiene que preguntar
        random_sleep(1500, 3000);

        pthread_mutex_lock(&shared_data->mutex);
            customers_remaining = shared_data->num_customers;
            customers_sitting = shared_data->customers_sitting;
        pthread_mutex_unlock(&shared_data->mutex);

        if(customers_remaining == 0){
            barber_leaves();
            return NULL;
        }

        if(customers_sitting != 0){
            sem_post(&shared_data->customer_semaphore);
            random_sleep(1500, 3000);
            cut_hair();
            random_sleep(1500, 3000);
            sem_post(&shared_data->barber_chair);
        }else{
            barber_sleeps();
            sem_wait(&shared_data->barber_semaphore);
            barber_wakes();
        }

    }

    return NULL;   
}

void* customer_method(void * args){

    customer_t* private_data = (customer_t*)args;
    barbershop_t* shared_data = (barbershop_t*)private_data->barber_shop;
        
    customer_arrives(private_data->customer_id);

    if(shared_data->customers_sitting < shared_data->num_waiting_room){
        sem_post(&shared_data->barber_semaphore);
        
        pthread_mutex_lock(&shared_data->mutex);
            shared_data->customers_sitting++;    
            customer_sits(private_data->customer_id);
        pthread_mutex_unlock(&shared_data->mutex);
        
        sem_wait(&shared_data->customer_semaphore);

        pthread_mutex_lock(&shared_data->mutex);
            shared_data->customers_sitting--;   
        pthread_mutex_unlock(&shared_data->mutex);        
        
        get_haircut(private_data->customer_id); 
        sem_wait(&shared_data->barber_chair);
        customer_leaves(private_data->customer_id);
    }else{
        customer_leaves_full(private_data->customer_id);
    }

    pthread_mutex_lock(&shared_data->mutex);
        shared_data->num_customers--;
    pthread_mutex_unlock(&shared_data->mutex);


    if(shared_data->num_customers == 0){
      sem_post(&shared_data->barber_semaphore);  
    }

    return NULL;
}


int main(int argc, char* arg[]) {

	srand( time(NULL) );
    size_t num_customers = 0;
    size_t num_waiting_room = 0;

    if (argc >= 3) {
        num_customers = (size_t)strtoul(arg[1], NULL, 10);
        num_waiting_room = (size_t)strtoul(arg[2], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        printf("1st parameter : Number of clients\n");
        printf("2nd parameter : Waiting room size\n");
        return 1;
    }

    // +1 por el barbero
    // Revisar porque hay que dar las validacion
    // Por si no c pueden almacenar memoria
    pthread_t* threads = (pthread_t*)malloc((num_customers + 1) * sizeof(pthread_t));
    barbershop_t* shared_barbershop = (barbershop_t*)calloc(1, sizeof(barbershop_t));

    customer_t* customer_data = (customer_t*)malloc((num_customers + 1) * (sizeof(customer_t)));

    shared_barbershop->num_customers = num_customers;
    shared_barbershop->num_waiting_room = num_waiting_room;
    shared_barbershop->customers_sitting = 0;

    // Metodos de sincronizacion
    pthread_mutex_init(&shared_barbershop->mutex, NULL);

    sem_init(&shared_barbershop->customer_semaphore, 0, 0);
    sem_init(&shared_barbershop->barber_semaphore, 0, 0);
    sem_init(&shared_barbershop->barber_chair, 0, 0);

    for(size_t i = 0; i <= num_customers; i++){
        customer_data[i].barber_shop = shared_barbershop;
        customer_data[i].customer_id = i;
        if(i == 0){
            pthread_create(&threads[i],  NULL, barber_method, (void*)&customer_data[i]);
            //random_sleep(500, 1500);
        }else{
            //random_sleep(500, 1500);
            pthread_create(&threads[i],  NULL, customer_method, (void*)&customer_data[i]);        
        }
    }    

    for(size_t j = 0; j <= num_customers; j++){
        pthread_join(threads[j], NULL);
    }
    
    
    pthread_mutex_destroy(&shared_barbershop->mutex);

    sem_destroy(&shared_barbershop->customer_semaphore);
    sem_destroy(&shared_barbershop->barber_semaphore);
    sem_destroy(&shared_barbershop->barber_chair);

    free(threads);
    free(customer_data);
    free(shared_barbershop);
    

    return 0;
}

int customer_arrives(size_t id){
    random_sleep(10,3000);
    printf("Customer with id %zd has arrived\n", id);
    return 1;
}


int customer_sits(size_t id){
    printf("Customer with %zd is sitting in the waiting room\n", id);
    return 1;
}

int customer_leaves_full(size_t id){
    printf("Waiting room is full, customer with id %zd is leaving\n", id);
    return 1;
}


int customer_leaves(size_t id){
    printf("Customer with id %zd is leaving\n", id);
    return 1;
}

int get_haircut(size_t id){
    printf("Customer with id %zd will get a haircut\n", id);
    return 1;
}

// metodos barbero
int cut_hair(){
    //printf("Barber is giving a haircut\n");
    //random_sleep(1500, 3000);
    printf("The barber has finished with the haircut\n");
    return 1;
}

int barber_leaves(){
    printf("The barber has finished for today\n");
    return 1;    
}

int barber_sleeps(){
    printf("There are no customers, the barber will go to sleep \n");
    return 1;
}


int barber_wakes(){
    printf("The barber is awake\n");
    return 1;
}

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}