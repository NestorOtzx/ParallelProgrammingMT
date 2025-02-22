#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int num;
    MPI_Status status;
    double start_time, end_time;
    
    if (rank == 0)
    {
        num = std::rand() % 100;
        std::cout<<"jugador "<<rank+1<<" Iniciamos con "<<num<<"\n";
    }

    for (int i = 0; i<5; i++)
    {

        if (rank == 0) {
            std::srand(std::time(nullptr));
            
            std::cout<<"jugador 1 iter: "<<i<<" envia "<<num<<"\n";
            start_time = MPI_Wtime();
            MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            end_time = MPI_Wtime();
            
            std::cout<<"jugador 1 iter: "<<i<<" recibió: "<<num<<" con RTT: "<<(end_time - start_time)*1000<<" ms\n";
        } 
        else if (rank == 1) {
            start_time = MPI_Wtime();
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            std::cout<<"jugador 2 iter: "<<i<<" recibe dato "<<num<<"\n";
            num += 1;
            MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            end_time = MPI_Wtime();
            
            std::cout<<"jugador 2 iter: "<<i<<" recibió: "<<num<<" con RTT: "<<(end_time - start_time)*1000<<" ms\n";
        }
    }

    MPI_Finalize();
    return 0;
}
