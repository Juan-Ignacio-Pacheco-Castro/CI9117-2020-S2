#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* arg[]) {

    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    int position = 1;
    omp_lock_t lock;
    omp_init_lock (&lock);
    #pragma omp parallel num_threads(thread_count)
    {
        omp_set_lock (&lock);
            printf("Thread %i: position %i\n", omp_get_thread_num(), position);
            ++position;
        omp_unset_lock (&lock);
        
    }

    omp_destroy_lock(&lock);

    return 0;
}