#include <stdio.h>

int MPI_Init(int* argc, char*** argv)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    (*(fn_ptr + 0))(&argc, &argv);
}

int MPI_Comm_size(void* comm, int* size)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    (*(fn_ptr + 1))(&comm, &size);

}

int MPI_Comm_rank(void* comm, int* rank)
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    (*(fn_ptr + 2))(&comm, &rank);

}
int MPI_Finalize()
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    (*(fn_ptr + 3))();

}
int hello_from_LH()
{
    void (**fn_ptr)();
    fn_ptr = (void (**)()) 0x5000000;
    (*(fn_ptr + 4))();

}
