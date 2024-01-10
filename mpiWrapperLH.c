#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int mpi_init(int *argc, char ***argv)
{
# ifdef VERBOSE
    printf("This is lower half wrapper calling MPI_Init()\n");
# endif

    MPI_Init(argc, argv);

# ifdef VERBOSE
    printf("LH:This is MPI_Init..\n");
    MPI_Comm *comm = MPI_COMM_WORLD;
    printf("&MPI_COMM_WORLD: %p\n",comm);
# endif

    return 0;    
}

int mpi_comm_size(MPI_Comm comm, int **size)
{

# ifdef VERBOSE
    printf("This is lower half wrapper calling MPI_Comm_size()\n");
# endif

    MPI_Comm_size(MPI_COMM_WORLD, *size);
    return 0;
}

int mpi_comm_rank(MPI_Comm comm, int **rank)
{
# ifdef VERBOSE
    printf("This is lower half wrapper calling MPI_Comm_rank()\n");
# endif

    MPI_Comm_rank(MPI_COMM_WORLD, *rank);

    return 0;
}

int mpi_finalize()
{
# ifdef VERBOSE
    printf("This is lower half wrapper calling MPI_Finalize()\n");
# endif

    MPI_Finalize();

    return 0;
}
