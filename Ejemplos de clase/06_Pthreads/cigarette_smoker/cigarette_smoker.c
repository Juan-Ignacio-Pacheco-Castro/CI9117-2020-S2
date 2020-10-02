#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


sem_t semaforos[3]; // vector de 3 semaforos uno por fumador
sem_t agente;
typedef void *(*smokers_t)(void *);


 // El fumador cero tiene papel infinito.
void* fumador_0(void* arg)
{
	while (true)
	{
		nanosleep((struct timespec[]){{0, rand() % 3000000000}}, NULL); // Sleep para visualizar de mejor manera los threads
		printf("El fumador cero tiene papel y esta esperando para fumar.\n");

		sem_wait(&semaforos[0]); // Semáforo para esperar que el agente ponga los ingredientes que necesita el fumador
		printf("El fumador cero recibio tabaco y cerillos. Ahora esta fumando.\n");
		sem_post(&agente);// Activa el semáforo del agente para que proceda a poner los siguientes ingredientes.
	}

	return NULL;
}


 
//El fumador uno tiene cerillos infinitos.
void* fumador_1(void* arg)
{
	while (true)
	{
		nanosleep((struct timespec[]){{0, rand() % 30000000000}}, NULL);
		printf("El fumador uno tiene cerillos y esta esperando para fumar.\n");

		sem_wait(&semaforos[1]);
		printf("El fumador uno recibio tabaco y papel. Ahora esta fumando.\n");
		sem_post(&agente);
	}

	return NULL;
}

//El fumador dos tiene tacabo infinito.
void* fumador_2(void* arg)
{
	while (true)
	{
		nanosleep((struct timespec[]){{0, rand() % 3000000000}}, NULL);
		printf("El fumador dos tiene tabaco y esta esperando para fumar.\n");

		sem_wait(&semaforos[2]);
		printf("El fumador dos recibio cerillos y papel. Ahora esta fumando.\n");
		sem_post(&agente);
	}

	return NULL;
}


int main(int argc, char* arvg[])
{
	srand(time(NULL));
	sem_init(&agente, 0, 1); // semaforo del agente.
	pthread_t fumadores[3];// vector de threads
	smokers_t metodos_fumadores[3] = { fumador_0, fumador_1, fumador_2 };// Metodos para los threads.

	for (int i = 0; i < 3; ++i) // Aqui se inicializa el semaforo de cada fumador *thread* y se crea cada thread con su respectiva funcion.
	{
		sem_init(&semaforos[i], 0, 0);
		if(pthread_create(&fumadores[i], NULL, metodos_fumadores[i], NULL) == EAGAIN)
		{
			perror("Recursos insuficientes, no se han creado los hilos\n");
			return 0;
		}
	}

	while (true) // aca el agente va a empeza a distribuir 2 ingredientes en cada iteracion, solo un thread podra tomarlos para evitar deadlocks.
	{
		sem_wait(&agente);// espera para que que el fumador que este fumando termine de fumar y el pueda poner  los materiales de nuevo.
		sem_post(&semaforos[rand() % 3]); // distribuye dos materiales para un solo fumador.
	}

	for (int i = 0; i < 3; ++i)
	{
		pthread_join(fumadores[i], NULL);
	}

}
