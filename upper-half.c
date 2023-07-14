#include <stdio.h>

int main()
{
	void (**fn_ptr)();      //declaring a pointer for function

        printf("Hello, World!\nThis is Upper Half program :)\n");

        fn_ptr = (void (**)()) 0x5000000;               //storing addr of foo in fn_ptr


        printf("Hello, this is upper-half calling function 'hello_from_LH' by address...\n");
        printf("----------------------------------------------------------\n");
        (*(fn_ptr + 4))();
        printf("\n");

	return 0;
}
