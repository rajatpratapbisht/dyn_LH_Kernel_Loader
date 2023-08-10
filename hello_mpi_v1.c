#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

void round_robin(int rank, int size);

int main(int argc, char * argv[]){

	int rank;
	int num_procs;

	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("%d:Hello (p = %d)\n", rank, num_procs);
	printf("%d:Goodbye\n", rank);

	MPI_Finalize();
	return 0;

}
