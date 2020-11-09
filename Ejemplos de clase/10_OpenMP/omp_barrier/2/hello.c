#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

void helloWorld() {
    printf("Hello world from thread number # %i\n", omp_get_thread_num());
}

int main(int argc, char* arg[]) 
{  

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
        printf("I'm thread %d, I'm ready!\n",omp_get_thread_num());        
        #pragma omp barrier// Aqui los hilos esperan a que todos esten listos para continuar
        printf("We're all ready, go!\n");
        usleep(1000);
        #pragma omp critical 
        {
            printf("Thread %i: position %i\n", omp_get_thread_num(), position);
            ++position;
        }
    }    

    return 0;
}
