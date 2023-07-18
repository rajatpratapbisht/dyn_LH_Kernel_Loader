#include <stdio.h>

int MPI_Init(int* argc, char*** argv)
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;

#ifdef VERBOSE
    printf("MPI_INIT:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))(&argc, &argv);

}

int MPI_Comm_size(void* comm, int* size)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 1;

#ifdef VERBOSE
    printf("MPI_Comm_size:\t %x\n", *fn_ptr);
#endif

//    (*(fn_ptr))(&comm, &size);

}

int MPI_Comm_rank(void* comm, int* rank)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 2;

#ifdef VERBOSE
    printf("MPI_Comm_rank:\t %x\n", *fn_ptr);
#endif

//    (*(fn_ptr))(&comm, &rank);

}
int MPI_Finalize()
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 3;

#ifdef VERBOSE
    printf("MPI_Finalize:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))();

}
int hello_from_LH()
{

    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    fn_ptr = fn_ptr + 4;

#ifdef VERBOSE
    printf("hello_from_LD:\t %x\n", *fn_ptr);
#endif

    (*(fn_ptr))();

}
