#include <iostream>
#include <omp.h>
#include <string>

using namespace std;

/*
* First argument (argv[1]) corresponds to the number of threads to use. The rest of the arguments
* are the list of numbers to be used by the program in the average calculation
*/
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
    
    #pragma omp parallel for num_threads(num_threads) reduction(+: sum)
    for (int i = 2; i < argc; i++) {        
        sum += stoi(argv[i]);
    }

    cout << "Average: " << (sum / (argc - 2)) << endl;
    
    return 0;
}