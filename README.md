# AIM: 
I are trying to call functions using their addresses rather than their names. I want to do this so that I can store these function addreses at a well known location (say 0x5000000 in this case) in form of an array. then we want to access these functions from the array without using thier names, instead only address using pointer to that function.
In this feature I have implemented a 'lower-half' capable of kernel-loading 'upper-half'. 
I have shown that 'upper-half' can call the functions declared and defined in lower half, using addresses and pointer arithmentics.

# lower-half
I have implemented the kernel-loader program (provided to me by Prof. Gene Coopermam) on my program which stores addresses of some custom functions in an array stored at a well known location in memoruy (0x5000000 in this case). This lower-half program now compiles with two necessary flags: 
	(a) -Wl,-section-start,.custom_section=0x5000000: this section contains our array of stored function addresses.
	(b) -Wl,-Ttext-segment=0x1000000 		: this will load our text segment of progarm in the provided address to avoid conflict when we load upper-half.
 
# upper-half
in the upper-half, I have created a pointer type prototype to call functions defined in the lower half using the well-known address (0x5000000). This implementation can now successfully call the functions stored in the lower-half.  

# Makefile
for compling the the code, use the following command

(.) make 		: this command will compile 'lower-half' and 'upper-half' executeables.
(.) make clean		: this command will remove all the output executables.
(.) make check_ls	: this command will test the working of lower-half's capability to  kernel load /bin/ls program
(.) make check		: this command will test if lower-half is capable of callind a custom program named 'upper-half'
(.) make cehck_mpi	: this command will test if the program is compatible with mpirun command with {-np 4} flag

