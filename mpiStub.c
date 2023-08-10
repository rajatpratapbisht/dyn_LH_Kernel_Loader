#include <stdio.h>
#include "mpi.h"

#define FUNC_ARR_ADDR 0x5000000

enum MPI{
	INIT, 		// 0
	COMM_SIZE,	// 1
	COMM_RANK,	// 2
	FINALIZE,	// 3
	HELLO_LH,	// 4
	COMM_WORLD	// 5
};

#ifdef MPI_COMM_WORLD
# undef MPI_COMM_WORLD
#endif
//extern MPI_Comm MPI_COMM_WORLD;

// Allocate storage in this file only; Others see the extern type decl.
MPI_Comm *MPI_COMM_WORLD;


// #define MPI_COMM_WORLD (void (**)()) 0x5000000 


//####################################################################################
int MPI_Init(int* argc, char*** argv)
{

    int (**fn_ptr)();
    fn_ptr = (int (**)()) FUNC_ARR_ADDR;

#ifdef VERBOSE
    printf("UH:&MPI_INIT:\t %p\n", *fn_ptr);
#endif

  (*(fn_ptr))(&argc, &argv);

  
// populate the address of MPI_COMM_WORLD
  fn_ptr = fn_ptr + COMM_WORLD;
  MPI_COMM_WORLD = (MPI_Comm)(*(fn_ptr))();

#ifdef VERBOSE
     printf("UH:&MPI_COMM_WORLD:\t %p\n", MPI_COMM_WORLD);
#endif

}

//####################################################################################
int MPI_Comm_size(MPI_Comm comm, int *size)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) FUNC_ARR_ADDR;
//    fn_ptr = fn_ptr + 1;
    fn_ptr = fn_ptr + COMM_SIZE;

#ifdef VERBOSE
    printf("UH:&MPI_Comm_size:\t %p\n", *fn_ptr);
#endif

    (*(fn_ptr))(comm, &size);

}

//####################################################################################
int MPI_Comm_rank(MPI_Comm comm, int* rank)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) FUNC_ARR_ADDR;
    fn_ptr = fn_ptr + COMM_RANK;

#ifdef VERBOSE
    printf("UH:&MPI_Comm_rank:\t %p\n", *fn_ptr);
#endif

    (*(fn_ptr))(comm, &rank);

}


//####################################################################################
int MPI_Finalize()
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) FUNC_ARR_ADDR;
    fn_ptr = fn_ptr + FINALIZE;

#ifdef VERBOSE
    printf("UH:&MPI_Finalize:\t %p\n", *fn_ptr);
#endif

    (*(fn_ptr))();

}

//####################################################################################
void  hello_from_LH()
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) FUNC_ARR_ADDR;
    fn_ptr = fn_ptr + HELLO_LH;

#ifdef VERBOSE
    printf("UH:&hello_from_LH:\t %p\n", *fn_ptr);
#endif

    (*(fn_ptr))();

}
