#include <omp.h>
#include <iostream>

using namespace std;

int main(){
    #pragma omp parallel
    {
        #pragma omp critical
        {
            cout << "Hello from thread : " << omp_get_thread_num() << endl;
        }
    
        #pragma omp for 
        for(int i = 0; i < omp_get_num_threads(); i++){
            
            #pragma omp critical
            {
            cout << "Thread " << omp_get_thread_num() << " gets iteration number " << i << endl;
            }

        }
    }
    return 0;
}