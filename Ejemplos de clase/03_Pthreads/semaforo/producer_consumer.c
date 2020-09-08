#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

// 2 hilos: Productor y Consumidor
// Buffer de numeros, tamanio como parametro
// Cantidad total de productos = Tamano del buffer * 3

typedef struct 
{
    size_t* buffer;
    size_t buffer_size;
    size_t num_products;

    sem_t producer_semaphore;
    sem_t consumer_semaphore;

} shared_data_t;


// Productor: Cuanta memoria hay libre en el buffer?
void* produce(void* args) {

    shared_data_t* shared_data = (shared_data_t*) args;
    size_t* buffer = shared_data->buffer;

    // wait - decrementa
    // post - incrementa

    for (size_t i = 1; i <= shared_data->num_products; ++i) {

        sem_wait(&shared_data->producer_semaphore);

        size_t index = (i-1)%shared_data->buffer_size;
        buffer[index] = i;
        printf("Produced %zu \n", i);

        sem_post(&shared_data->consumer_semaphore);

    }

    return NULL;
}

// Consumidor: Cuantos espacios tienen producto / estan llenos en el buffer?
void* consume(void* args) {
    shared_data_t* shared_data = (shared_data_t*) args;
    size_t* buffer = shared_data->buffer;

    for (size_t i = 1; i <= shared_data->num_products; ++i) {

        sem_wait(&shared_data->consumer_semaphore);

        size_t index = (i-1)%shared_data->buffer_size;
        size_t product = buffer[index];
        printf("    Consumed %zu \n", product);
        buffer[index] = 0;

        sem_post(&shared_data->producer_semaphore);
    }

    return NULL;

}

int main(int argc, char* arg[]) {

    size_t buffer_size;

    if (argc >= 2) {
        buffer_size = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        printf("Invalid number of params");
        return 1;
    }

    shared_data_t* shared_data = (shared_data_t*)malloc(sizeof(shared_data_t));

    pthread_t producer;
    pthread_t consumer;

    shared_data->buffer_size = buffer_size;
    shared_data->num_products = buffer_size * 3;
    shared_data->buffer = (size_t*)calloc(buffer_size, sizeof(size_t));

    sem_init(&shared_data->producer_semaphore, 0, buffer_size);
    sem_init(&shared_data->consumer_semaphore, 0, 0);

// Thread creation

    pthread_create(&producer, NULL, produce, (void*)shared_data);
    pthread_create(&consumer, NULL, consume, (void*)shared_data);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    sem_destroy(&shared_data->producer_semaphore);
    sem_destroy(&shared_data->consumer_semaphore);

    free(shared_data->buffer);
    free(shared_data);

}
