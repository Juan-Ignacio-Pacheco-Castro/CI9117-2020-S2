#include "roller_coaster.h"

void takeRide(thread_data_t*);

void delay(useconds_t min_milliseconds, useconds_t max_milliseconds) {
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}

void* carThread(void* arg) {
	thread_data_t* data = (typeof(data)) arg;
	shared_data_t* shared_data = data->shared_data;
	for (size_t i = 1; i <= shared_data->max_car_rides; ++i) {
		for (size_t j = 1; j <= shared_data->max_car_capacity; ++j) {
			sem_post(&shared_data->semaphores_list[0]);
		}
		sem_wait(&shared_data->semaphores_list[2]);

		printf("The car it's running for the %zu time\n", i);
		delay(0.5*1000,1*1000); // ride for a while

		shared_data->passenger_counter = 0;
		for (size_t j = 1; j <= shared_data->max_car_capacity; j++) {
			sem_post(&shared_data->semaphores_list[3]);
			sem_wait(&shared_data->semaphores_list[4]);
		}
		printf("The car it's empty\n");
	}
	printf("Thats it! No more rides for today :(\n");
	return NULL;
}

void* passengerThread(void* arg) {
	thread_data_t* data = (typeof(data))arg;
	while (1) {
		delay(0.5*1000, 1*1000); // walk around for a while
		takeRide(data); // take ride
	}
	return NULL;
}

void takeRide(thread_data_t* data) {
	shared_data_t* shared_data = data->shared_data;
	sem_wait(&shared_data->semaphores_list[0]); // wait in Queque
	sem_wait(&shared_data->semaphores_list[1]); // wait in CheckIn
	++shared_data->passenger_counter;
	printf("Passenger %zu is riding the car\n", data->thread_num);
	if (shared_data->passenger_counter == shared_data->max_car_capacity) {
		sem_post(&shared_data->semaphores_list[2]);
	}
	sem_post(&shared_data->semaphores_list[1]); // frees CheckIn
	sem_wait(&shared_data->semaphores_list[3]);
	sem_post(&shared_data->semaphores_list[4]);
	printf("Passenger %zu is getting out the car\n", data->thread_num);
}
