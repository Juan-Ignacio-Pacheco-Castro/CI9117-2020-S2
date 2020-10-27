#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void helloWorld() {
    printf("Hello world from thread number # %i\n", omp_get_thread_num());
}

int main(int argc, char* arg[]) {

    // arg[0] = command
    // arg[1] = first param

    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    int position = 1;

    #pragma omp parallel num_threads(thread_count) default(none) shared(position)
    {
        #pragma omp critical 
        {
            printf("Thread %i: position %i\n", omp_get_thread_num(), position);
            ++position;
        }
    }

    /*
    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_create(&threads[i], NULL, helloWorld, (void*)i);
    }

    printf("Hello world from main thread\n");

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    */

    return 0;
}
