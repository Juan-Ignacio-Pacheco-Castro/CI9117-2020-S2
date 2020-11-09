#include <iostream>
#include <omp.h>
#include <stdio.h>

int main()
{
    omp_set_num_threads(4);

#pragma omp parallel
    {
        int sum = 0;
        for (int i = 0; i < 10; i++)
        {
#pragma omp atomic
        sum +=i;
        printf("Sum = %d Thread =%d\n", sum, omp_get_thread_num());

        }
       
    }
    return 0;
}