#include <iostream>
#include <omp.h>
#include <string> 
#include <math.h>

using namespace std;

/*
* This code is an example of how we could manipulate the workload distributed among threads
* without using omp pallel for directive. However, we can see that the logic is considerably
* simplified when we use parallel for and reductions.
*/
static void calculate_sum(char **array, int array_size, double* global_sum) {
    double sum = 0.0;

    int local_size =  (int)ceil((double)array_size / (double)omp_get_num_threads());

    int start = (omp_get_thread_num() * local_size) + 2;

    int end = min(start + local_size, array_size + 2);

    for (int i = start; i < end; ++i) {

        #pragma omp critical // This is not correct for optimization problems! Solution is still serial
        cout << "Thread " << omp_get_thread_num() << ": Processing number " << array[i] << endl;
        sum += atoi(array[i]);
    }

    #pragma omp critical
    *global_sum += sum;
}

int main(int argc, char *argv[])
{
    int num_threads;

    if (argc > 1) {
        num_threads = stoi(argv[1]);
    } else {
        cout << "Invalid number of parameters.\n";
        return 1;
    }

    double sum = 0.0;

    #pragma omp parallel num_threads(num_threads)
    {
        calculate_sum(argv, argc - 2, &sum);
    }

    cout << "Average: " << (sum / (argc - 2)) << endl;
    
    return 0;
}