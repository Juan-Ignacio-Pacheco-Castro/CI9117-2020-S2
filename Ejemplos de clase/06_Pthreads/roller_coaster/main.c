#include <pthread.h>
#include <stdio.h>

#include "roller_coaster.h"

int main(int argc, char* arg[]) {
	size_t total_passangers = 1;
	size_t max_car_capacity = 1;
	size_t max_car_rides = 1;

	if (argc >= 4) {
		total_passangers = (size_t)strtoul(arg[1], NULL, 10);
		max_car_capacity = (size_t)strtoul(arg[2], NULL, 10);
		max_car_rides = (size_t)strtoul(arg[3], NULL, 10);
	} else {
		fprintf(stderr, "Error, invalid number of parameters\n");
		return 1;
	}

	pthread_t* passengers_list = (typeof(passengers_list))
		malloc((size_t)(total_passangers * sizeof(pthread_t)));

	shared_data_t* shared_data = (shared_data_t*) calloc(1, sizeof(shared_data_t));

	shared_data->semaphores_list = (typeof(shared_data->semaphores_list))
		calloc(5,sizeof(*shared_data->semaphores_list));
	sem_init(&shared_data->semaphores_list[0], 0, 0);
	sem_init(&shared_data->semaphores_list[1], 0, 1);
	sem_init(&shared_data->semaphores_list[2], 0, 0);
	sem_init(&shared_data->semaphores_list[3], 0, 0);
	sem_init(&shared_data->semaphores_list[4], 0, 0);

	shared_data->max_car_rides = max_car_rides;
	shared_data->max_car_capacity = max_car_capacity;
	shared_data->passenger_counter = 0;

	thread_data_t* passengers_data_list = (typeof(passengers_data_list))
		malloc((size_t)(total_passangers * sizeof(thread_data_t)));

	pthread_t* car = (typeof(car)) malloc((size_t)(1*sizeof(pthread_t)));

	thread_data_t* car_data = (typeof(car_data))
		malloc((size_t)(1 * sizeof(thread_data_t)));

	car_data->shared_data = shared_data;
	car_data->thread_num = 0;
	pthread_create(car, NULL, carThread, (void*)car_data);

	for (size_t i = 0; i < total_passangers; ++i) {
		passengers_data_list[i].thread_num = (i+1);
		passengers_data_list[i].shared_data = shared_data;
		pthread_create(&passengers_list[i], NULL, passengerThread,
			(void*)&passengers_data_list[i]);
	}

	pthread_join(*car, NULL);

	for (size_t i = 0; i < total_passangers; ++i) {
		sem_destroy(&shared_data->semaphores_list[i]);
	}

	free(shared_data->semaphores_list);
	free(passengers_list);
	free(shared_data);
	free(passengers_data_list);

	return 0;
}
