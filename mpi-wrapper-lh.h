#include <stdio.h>
#include <mpi.h>

int mpi_init(int* argc, char*** argv);
int mpi_comm_size(void* comm, int* size);
int mpi_comm_rank(void* comm, int* rank);
int mpi_finalize();
