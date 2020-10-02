#ifndef ROLLER_COASTER
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
	size_t passenger_counter;
    size_t max_car_capacity;
    size_t max_car_rides;
	sem_t* semaphores_list; //Queque, CheckIn, Boardng, Riding, Unloading
} shared_data_t;

typedef struct {
	size_t thread_num;
	shared_data_t* shared_data;
} thread_data_t;

void delay(useconds_t min_milliseconds, useconds_t max_milliseconds);
void* carThread(void*);
void* passengerThread(void*);

#endif
