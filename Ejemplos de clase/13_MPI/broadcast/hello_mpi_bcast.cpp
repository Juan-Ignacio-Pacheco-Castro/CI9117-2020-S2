#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    int message_received, message_sent;
    MPI_Status status;

    int bcast_message = 2020;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);


    if (my_id == 0) {
        cout << "Please enter a value: " << endl;
        cin >> bcast_message;
    }

    MPI_Bcast(&bcast_message, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_id != 0) {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }

    cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << ". Bcast Message: " << bcast_message << endl;

	if ( my_id < num_processes - 1 ) {
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();

    return 0;
}
