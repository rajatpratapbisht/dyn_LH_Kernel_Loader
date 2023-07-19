#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int mpi_init(int* argc, char** argv[])
{
    printf("This is lower half wrapper calling MPI_Init()\n");
    MPI_Init(&argc, &argv);

}

int mpi_comm_size(void* comm, int* size)
{
    printf("This is lower half wrapper calling MPI_Comm_size()\n");
    MPI_Comm_size(&comm, &size);

}

int mpi_comm_rank(void* comm, int* rank)
{
    printf("This is lower half wrapper calling MPI_Comm_rank()\n");
    MPI_Comm_rank(&comm, &rank);

}

int mpi_finalize()
{
    printf("This is lower half wrapper calling MPI_Finalize()\n");
    MPI_Finalize();


}
