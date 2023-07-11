# AIM: 
we are trying to call functions using their addresses rather than their names. We want to do this so that we can store these function addreses at a well known location (say 0x5000000 in this case) in form of an array. then we want to access these functions from the array without using thier names, instead only address using pointer to that function.

# 1.c - 3.c
we are trying to implement a simple call by reference in this case where the try to call the function by first, directly allocaing function's address and calling using pointer to that address. Then, we introduce an array to store these addresses and use indices to call various functions.

# 4.c - 5.c
We now introduce the concept of custom segment where we have an array stored at a specific well known location in the memory space. We allot the specific well known address ( 0x5000000 in our case) by compiling these programs using a flag " -Wl, -section-start,.custom_section=0x5000000". 
Then in the main function rather than calling the array by its name, we have a constant address which we can use to call all the other functions in that array (using pointer arithmetics).


# Makefile
for compling the the code, use the following command

(.) make a3	: this command will make 3.c into an executable named 'a3'.
(.) make clean	: this command will remove all the output executables.

