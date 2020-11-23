#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[])
{

    int my_id, num_processes;
    int message_received, message_sent;
    MPI_Status status;
    int *recv_buffer;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    recv_buffer = new int[num_processes];

    MPI_Gather(&my_id, 1, MPI_INT, recv_buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (my_id != 0)
    {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }

    cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << endl;

    if (my_id == 0)
    {
        cout << "Buffer received: ";
        for (int i = 0; i < num_processes; ++i)
        {
            cout << recv_buffer[i] << " ";
        }
        cout << endl;
    }

    if (my_id < num_processes - 1)
    {
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    delete[] recv_buffer;

    return 0;
}