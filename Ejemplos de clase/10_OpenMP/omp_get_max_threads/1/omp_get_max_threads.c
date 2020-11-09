#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>


typedef struct timespec walltime_t;

void walltime_start(walltime_t* start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);

	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;

	return elapsed;
}

double calcularRectangulo(double diferencialX, double altura)
{
    return diferencialX*altura;
}

double sumasDeRiemann(double a, double b, size_t n, int thread_count)
{
    double diferencialX = ( (b-a)/n ); //conocido como delta de x o el largo de cada cuadrado.
    double extremoIzq;
    double sum = 0;


	#pragma omp parallel for num_threads(thread_count) default(none) shared(a, n, diferencialX) reduction(+: sum)
    	for(size_t iteration = 0; iteration < n ; ++iteration)
    	{
            double extremoIzq = a + (iteration*diferencialX);
            double altura = (extremoIzq*extremoIzq) + 1;
            sum+= calcularRectangulo(diferencialX, altura);
    	}


    return sum;
}

int main(int argc, char *argv[])
{
	double a,b;
    size_t numRectangulos;
	int threads;
    if(argc != 5)
    {
        printf("Error: Se debe ingresar un limite inferior, superior, numero de rectangulos e hilos seguidos. Ejemplo: a,b,n,t. Ejemplo: 2 5 1000 64\n");
        return -1;
    }
    a = strtod(argv[1], NULL);
    b = strtod(argv[2], NULL);
    numRectangulos = strtoul(argv[3], NULL, 10);
    threads = strtoul(argv[4], NULL, 10);

    walltime_t* time = (walltime_t*) malloc(sizeof(walltime_t));
    walltime_start(time);

    double suma = sumasDeRiemann(a,b,numRectangulos,threads);

    double timepoEjecucion = walltime_elapsed(time);
    printf("\nSe aproxima: %.10lf con %zu cuadrados.\n", suma, numRectangulos);
    printf("Tiempo de ejecucion: %lfs\n", timepoEjecucion);
	printf("\nMax threads sin modificar: %d\n", omp_get_max_threads());

	omp_set_num_threads(16);
	printf("Max threads modificado con omp_set_num_threads(16): %d\n", omp_get_max_threads());
    free(time);
    return 0;
}
