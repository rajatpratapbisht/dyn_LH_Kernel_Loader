#include <stdio.h>

void (*fn_Arr[2])();

void foo(int i)
{
	printf("hello, this is function foo\n");
	printf("value of int i is: %d\n", i);
}

void bar(int j, int k)
{
	printf("hello, this is function bar\n");
	printf("value of int j is: %d\n", j);
	printf("value of int k is: %d\n", k);
}

void (*fn_Arr[2])() __attribute__((section(".custom_section"))) = {&foo, &bar};
int main()
{
	void (**fn_ptr)();	//declaring a pointer for function
	
	fn_ptr = (void (**)()) 0x5000000;		//storing addr of foo in fn_ptr
	printf("\nHello, this is main calling function foo by address...\n");

//	(*fn_Arr[0])(23);
//	(*fn_Arr[1])(2, 3);
	
	printf("----------------------------------------------------------\n");
	(*fn_ptr)(23);
	printf ("\n");

        printf("Hello, this is main calling function foo by address...\n");
        printf("----------------------------------------------------------\n");	
	(*(fn_ptr + 1))(2, 3);
	printf("\n");

	return 0;

}
