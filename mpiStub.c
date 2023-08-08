#include <stdio.h>
#include "mpi.h"

//enumns

#ifdef MPI_COMM_WORLD
# undef MPI_COMM_WORLD
#endif
//extern MPI_Comm MPI_COMM_WORLD;

// Allocate storage in this file only; Others see the extern type decl.
MPI_Comm MPI_COMM_WORLD;


// #define MPI_COMM_WORLD (void (**)()) 0x5000000 


//####################################################################################
int MPI_Init(int* argc, char*** argv)
{

    int (**fn_ptr)();
    fn_ptr = (int (**)()) 0x5000000;

#ifdef VERBOSE
    printf("&MPI_INIT:\t %x\n", *fn_ptr);
#endif

  (*(fn_ptr))(&argc, &argv);

  
// populate the address of MPI_COMM_WORLD
  fn_ptr = fn_ptr + 5;
  MPI_COMM_WORLD = (MPI_Comm)(*(fn_ptr))();


}

//####################################################################################
int MPI_Comm_size(MPI_Comm comm, int *size)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 1;

#ifdef VERBOSE
    printf("&MPI_Comm_size:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))(comm, &size);

}

//####################################################################################
int MPI_Comm_rank(MPI_Comm comm, int* rank)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 2;

#ifdef VERBOSE
    printf("&MPI_Comm_rank:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))(comm, &rank);

}


//####################################################################################
int MPI_Finalize()
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 3;

#ifdef VERBOSE
    printf("&MPI_Finalize:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))();

}

//####################################################################################
void  hello_from_LH()
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 4;

#ifdef VERBOSE
    printf("&hello_from_LH:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))();

}
