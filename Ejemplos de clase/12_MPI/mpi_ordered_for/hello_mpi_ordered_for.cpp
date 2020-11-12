#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    int message_received, message_sent;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    if (my_id == 0) {
        message_sent = 1;
        cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << endl;
        for (int i = 1; i < num_processes; ++i) {
            // Synchronous send
            MPI_Send(&message_sent, 1 /*count*/, MPI_INT, i /*dest*/, 123 /*message id*/, MPI_COMM_WORLD); // Send signal to say hello to process i
            MPI_Recv(&message_received, 1 /* count*/, MPI_INT, i /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status); // Waits for process i to finish

        }
    } else {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, 0 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status); // Receives signal from process 0 to print
        cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << endl;
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, 0 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD); // Tells process 0 to continue

    }

    MPI_Finalize();

    return 0;
}