#include <stdio.h>

int main()
{
	void (**fn_ptr)();      //declaring a pointer for function

        printf("Hello, World!\nThis is Upper Half program :)\n");

        fn_ptr = (void (**)()) 0x5000000;               //storing addr of foo in fn_ptr

        printf("\nHello, this is upper-half calling function foo by address...\n");
        printf("----------------------------------------------------------\n");
        (*fn_ptr)(8);
        printf ("\n");

        printf("Hello, this is upper-half calling function foo by address...\n");
        printf("----------------------------------------------------------\n");
        (*(fn_ptr + 1))(2, 8);
        printf("\n");

	return 0;
}
