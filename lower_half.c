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
	void (**fn_ptr)(int);	//declaring a pointer for function
	
	fn_ptr = (void (**)(int)) 0x5000000;		//storing addr of foo in fn_ptr
//	fn_Arr[0] = &foo;
//	fn_Arr[1] = &bar;		
	printf("Hello, this is main calling function foo by address...\n");
//	fn_ptr(42);
	(*fn_Arr[0])(23);
	(*fn_Arr[1])(2, 3);
	
	printf("----------------------------------------------------------\n");
	(*fn_ptr)(42);

	return 0;

}
