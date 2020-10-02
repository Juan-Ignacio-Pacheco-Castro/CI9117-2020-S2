#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

typedef struct{
	size_t fork_count;
	pthread_mutex_t * forks;
	sem_t footman;
} table_t;

typedef struct{
	size_t id;
	table_t * table;
} philosopher_t;

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds){
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}


void think(size_t i){
	printf("Philosopher %zu: Thinking\n", i); 
	random_sleep(4000,8000);	
}

void eat(size_t i){
	printf("Philosopher %zu: Eating\n", i); 
	random_sleep(2000,5000);
}

void get_forks(size_t i, table_t* table){
	printf("Philosopher %zu: Gettin' forks\n", i); 
	sem_wait(&table->footman);
	printf("Philosopher %zu: Sat down\n", i); 
	pthread_mutex_lock(&table->forks[i]);
	printf("Philosopher %zu: Got left fork\n", i); 
	pthread_mutex_lock(&table->forks[(i+1)%table->fork_count]);
	printf("Philosopher %zu: Got right fork\n", i); 
}

void put_forks(size_t i, table_t* table){
	printf("Philosopher %zu: Freed left fork\n", i); 
	pthread_mutex_unlock(&table->forks[i]);
	printf("Philosopher %zu: Freed right fork\n", i); 
	pthread_mutex_unlock(&table->forks[(i+1)%table->fork_count]);
	printf("Philosopher %zu: Standing up\n", i); 	
	sem_post(&table->footman);
}

void* dine(void * args){
	size_t hunger = 2;
	while(hunger--){
		philosopher_t* philosopher = (philosopher_t*) args;
		size_t id = philosopher->id;
		think(id);
		get_forks(id, philosopher->table);
		eat(id);
		put_forks(id, philosopher->table);
	}
}

int main(int argc, char* arg[]){
	size_t philosopher_count = 5;
	
	pthread_t * philosopher = malloc (((size_t)(philosopher_count * sizeof(pthread_t))));
		
	table_t * table = (table_t*)calloc(1,sizeof(table_t));
	table->fork_count = philosopher_count;
	table->forks = malloc((size_t)(philosopher_count* sizeof(pthread_mutex_t)));
	sem_init(&(table->footman), 0, philosopher_count%2 ? philosopher_count-1 : philosopher_count);
	
	philosopher_t * philosopherInfoList = malloc((size_t)(philosopher_count * sizeof(philosopher_t)));

	for(size_t i = 0; i < philosopher_count; ++i){
		pthread_mutex_init(&table->forks[i],NULL);
		philosopherInfoList[i].id = i;
		philosopherInfoList[i].table = table;	
		pthread_create(&philosopher[i], NULL, dine, (void*)& philosopherInfoList[i]);	
	}

	for(size_t i = 0; i < philosopher_count; ++i){
		pthread_join(philosopher[i], NULL);
	}
	
	free(philosopher);
	
	for(size_t i = 0; i < philosopher_count; ++i){
		pthread_mutex_destroy(&table->forks[i]);
	}
	sem_destroy(&table->footman);
	free(table->forks);
	free(table);
	free(philosopherInfoList);
	return 0;
}
