
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVINGS 7
#define TOTAL_MEALS 3

typedef struct {
    size_t servings;                // Amount of available serves in the pot
    size_t total_savages;           // Thread count
    int total_meals;                // Amount of refills for the pot
    int finish;                     // Tells the rest of the savages that there is no more food
    sem_t servings_sem;
    sem_t empty_pot_sem;
    sem_t full_pot_sem;
    pthread_mutex_t stdout_mutex;
} shared_data_t;                    // Shared memory among ALL threads

typedef struct {
    size_t thread_num;
    shared_data_t* shared_data;
} thread_data_t;                    // Private memory for each thread

/**
 * @brief Realistic waiting time function so that each thread has a preparation time
 */
void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){
    useconds_t duration = min_milliseconds;
    useconds_t range = max_milliseconds - min_milliseconds;
    if ( range > 0 )
        duration += rand() % range;
    usleep( 1000 * duration );
}

/**
 * @brief The thread in charge of filling the pot (The Chef)
 */
void* cook(void* argument){
    shared_data_t* shared_data = (shared_data_t*)argument;

    // While there is food
    while(shared_data->total_meals >= 0){
        // Cooker waits for pot to be empty
        sem_wait(&shared_data->empty_pot_sem);

        // Less ingredients for food left
        --shared_data->total_meals;

        // Tells the last savage left that there is no more food
        if(shared_data->total_meals < 0){
            shared_data->finish = 1;
            sem_post(&shared_data->full_pot_sem);
            return NULL;
        }

        pthread_mutex_lock(&shared_data->stdout_mutex);
        printf("\n\033[0;32m Cooker: Dinner is ready! Pot is full \\o/ \033[0m\n\n");
        pthread_mutex_unlock(&shared_data->stdout_mutex);

        // Sends signal that there is more food!!!
        sem_post(&shared_data->full_pot_sem);
    }

    return NULL;
}

/**
 * @brief The threads in charge of eating
 */
void* savage_eat(void* argument){
    thread_data_t* thread_data = (thread_data_t*)argument;
    shared_data_t* shared_data = thread_data->shared_data;

    // Aqui es >= 0 porque cuando el cooker termina de cocinar su ultima olla llena
    // del dia, hay que contar la que lleno, porque puede que haya algun o algunos
    // threads rezagados y se desperdicia la comida porque no vuelven a entrar
    // al ciclo
    while(shared_data->total_meals >= 0){
        // Only one thread can enter to critical section
        sem_wait(&shared_data->servings_sem);
        // If pot is empty
        if(shared_data->servings == 0){
            // Send signal to cooker, pot is empty!
            sem_post(&shared_data->empty_pot_sem);
            // Waits until pot is full, from cooker!
            sem_wait(&shared_data->full_pot_sem);

            // For the remaining thread in case that the cooker is gone and there is no more servings
            if(shared_data->finish){
                sem_post(&shared_data->servings_sem);
                continue;           // return NULL
            }
            // Pot is full
            shared_data->servings = SERVINGS;     // M = 15
        }

        shared_data->servings -= 1;

        pthread_mutex_lock(&shared_data->stdout_mutex);
        printf("\033[0;33m Savage %zu eating: YUMMY! What a snack \033[0m\n", thread_data->thread_num);
        pthread_mutex_unlock(&shared_data->stdout_mutex);

        // Signal next savage to eat
        sem_post(&shared_data->servings_sem);

        random_sleep(2000, 4000);                        // savage eats (sleep)
    }

    return NULL;
}

int main(int argc, char* argv[]){

    // Thread count
    size_t savages_count = 0;

    if (argc >= 2)
        savages_count = (size_t)strtoul(argv[1], NULL, 10);
    else
        return fprintf(stderr, "Error, invalid number of parameters\n"), 1;

    srand(time(NULL));

    // Threads
    pthread_t* savages = (pthread_t*)malloc((size_t)((savages_count + 1) * sizeof(pthread_t)));
    if (savages == NULL)
        return fprintf(stderr, "Error: could not allocate memory for %zu savages\n", savages_count), 2;

    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    if (shared_data == NULL )
        return fprintf(stderr, "Error: could not allocate shared memory\n"), 3;

    // Private data for each thread
    thread_data_t* thread_data = (thread_data_t*)malloc((size_t)(savages_count * sizeof(thread_data_t)));
    if (thread_data == NULL )
        return fprintf(stderr, "Error: could not allocate thread data for %zu savages\n", savages_count), 4;

    // At the beginning, the pot is full
    shared_data->servings = SERVINGS;                       // Repeticiones = servings
    shared_data->finish = 0;                                // 0 = false
    shared_data->total_savages = savages_count;             // total threads
    shared_data->total_meals = TOTAL_MEALS;
    sem_init(&shared_data->servings_sem, 0, 1);             // Semaforo en 1 == mutex | Only 1 savage can be served at the time
    sem_init(&shared_data->empty_pot_sem, 0, 0);            // Waits until pot is empty
    sem_init(&shared_data->full_pot_sem, 0, 0);             // Waits for cooker to fill pot
    pthread_mutex_init(&shared_data->stdout_mutex, NULL);

    // Create savages(threads)
    for(size_t savage = 0; savage < savages_count; ++savage){
        thread_data[savage].thread_num = savage;
        thread_data[savage].shared_data = shared_data;
        pthread_create(&savages[savage], NULL, savage_eat, (void*)&thread_data[savage]);
    }

    // Create cooker(thread)
    pthread_create(&savages[savages_count], NULL, cook, (void*)shared_data);

    // Wait for all threads to finish
    for (size_t index = 0; index <= savages_count; ++index)
        pthread_join(savages[index], NULL);

    pthread_mutex_destroy(&shared_data->stdout_mutex);
    sem_destroy(&shared_data->servings_sem);
    sem_destroy(&shared_data->empty_pot_sem);
    sem_destroy(&shared_data->full_pot_sem);

    free(savages);
    free(shared_data);
    free(thread_data);

    return 0;
}

