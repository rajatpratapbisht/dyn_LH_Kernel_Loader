#include <stdio.h>
#include </usr/include/openmpi-x86_64/mpi.h>
int main(int argc, char *argv[])
{
#ifdef VERBOSE

    printf("-----------------------------------------\n");
    printf("argc: %d, argv[]: %s\n", argc, *argv);
    printf("-----------------------------------------\n");

#endif
    printf("Hello, World!\nThis is Upper Half program :)\n");

    int size;

    MPI_Init(&argc, &argv);
/*
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("Size: %d\n", size);
*/
    hello_from_LH();
    MPI_Finalize();
	return 0;
}
