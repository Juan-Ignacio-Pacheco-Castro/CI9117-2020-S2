#include <iostream>
#include "mpi.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iomanip>      /* setprecision */

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    int num_values, my_number, min, max, sum;
    double average;
    int* values;
    int message;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    srand(time(NULL) * my_id * 1000);

    if (my_id == 0) {
        cout << "Please enter the number of values: " << endl;
        cin >> num_values;
        
        if (num_values <= 0) return 1;

        values = new int[num_values];

        cout << "Please enter " << num_values << " integer values: " << endl;

        for (int i = 0; i < num_values; ++i) {
            cin >> values[i];
        }
    }

    MPI_Bcast(&num_values, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_id != 0) {
        values = new int[num_values];
    }

    MPI_Bcast(values, num_values, MPI_INT, 0, MPI_COMM_WORLD);

    int index = rand() % num_values;
    my_number = values[index];

    MPI_Reduce(&my_number, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_number, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_number, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_id != 0) {
        MPI_Recv(&message, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    cout << "process " << my_id << ": I choose number " << my_number << endl;

    if (my_id == 0) {
        average = sum / num_processes;
        cout << "process " << my_id << ": Minimun is " << min << endl;
        cout << "process " << my_id << ": Maximun is " << max << endl;
        cout << "process " << my_id << ": Average is " << fixed << setprecision(2) << average << endl;
    }

	if ( my_id < num_processes - 1 ) {
        MPI_Send(&message, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();

     delete [] values;

    return 0;
}
