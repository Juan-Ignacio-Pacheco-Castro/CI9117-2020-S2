#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    int message_sent = 0;
    int message_received = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    if (my_id == 0) {
        cout << "Hello. I am process: " << my_id << ". I am going to send a message" << endl;
        message_sent = 1234;
        MPI_Send(&message_sent, 1, MPI_INT, 1, 12345, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&message_received, 1, MPI_INT, 0, 12345, MPI_COMM_WORLD, &status);
    }

    cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << ". Message received: " << message_received << endl;

    MPI_Finalize();

    return 0;
}