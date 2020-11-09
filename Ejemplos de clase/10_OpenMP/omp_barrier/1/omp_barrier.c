#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds)
{
    useconds_t duration = min_milliseconds;
    useconds_t range = max_milliseconds - min_milliseconds;
    if ( range > 0 )
        duration += rand() % range;
    usleep( 1000 * duration );
}

void race(size_t* thread_pos) {

    printf("Thread %d/%d: I am ready!\n", omp_get_thread_num() + 1, omp_get_num_threads());
    #pragma omp barrier 

    #pragma omp critical 
    {
        printf( "Thread %d/%d: I arrived at position %zu\n", omp_get_thread_num() + 1, omp_get_num_threads(), *thread_pos);
        ++(*thread_pos);
    }
}


int main(int argc, char* arg[]) {

    srand( time(NULL) );

    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

	random_sleep(1000,3000);

    size_t thread_pos = 1;

    #pragma omp parallel num_threads(thread_count) default(none) shared(thread_pos)
    {
        race(&thread_pos);
    }

    return 0;
}