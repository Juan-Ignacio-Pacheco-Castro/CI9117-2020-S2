#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    int message_received, message_sent;
    MPI_Status status;

    int * array_sent;
    int * array_received;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int array_sent_size = num_processes * 3; 

   array_sent = new int[array_sent_size];
   array_received = new int[3];

    if (my_id == 0) {
        for (int i =0 ; i < array_sent_size; ++i)
            array_sent[i] = 2020 + i;
    }

    MPI_Scatter(&array_sent, 3, MPI_INT, &array_received, 3, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_id != 0) {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }

    cout << "Hello. I am process: " << my_id << ". Array received: " << array_received[0] << ", " << array_received[1] << ", " << array_received[2] << endl;
    // cout << "Hello. I am process: " << my_id << ". Array received: " << value_received << endl;

	if ( my_id < num_processes - 1 ) {
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }

    
    MPI_Finalize();

    delete [] array_sent;
    delete [] array_received;

    return 0;
}
