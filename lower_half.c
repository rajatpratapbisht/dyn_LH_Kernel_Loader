#include <stdio.h>

void foo()
{
	printf("hello, this is function foo\n");
}

int main()
{
	void (*fn_ptr)();	//declaring a pointer for function
	
	fn_ptr = &foo;		//storing addr of foo in fn_ptr
	
	printf("Hello, this is main calling function foo by address...\n");
	(*fn_ptr)();

	return 0;

}
