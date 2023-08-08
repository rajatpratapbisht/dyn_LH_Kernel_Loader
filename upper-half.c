#include <stdio.h>
#include <mpi.h>

#ifdef MPI_COMM_WORLD
# undef MPI_COMM_WORLD
#endif
extern MPI_Comm MPI_COMM_WORLD;


int main(int argc, char *argv[])
{

    int num_procs;
    int rank;

# ifdef BLOCKING
    int i = 1;
    while (i);
# endif

# ifdef VERBOSE

    printf("-----------------------------------------\n");
    printf("argc: %d, argv[]: %s\n", argc, *argv);
    printf("-----------------------------------------\n");

    printf("Hello, World!\nThis is Upper Half program :)\n");
# endif


	MPI_Init(&argc, &argv);
        
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("-----------------------------------------\n");
        printf("%d:Hello (p = %d)\n", rank, num_procs);
	hello_from_LH();
        printf("%d:Goodbye\n", rank);
	printf("-----------------------------------------\n");

        MPI_Finalize();

return 0;
}
