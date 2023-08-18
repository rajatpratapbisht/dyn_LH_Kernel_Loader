Project name: HW2
Project description: write a transparent 'checkpointing' program that stores the current process' memory map 
            on disk for later restart from the same point. when target 'myckpt.dat' file is created sucessfully,
            it can be relaunched/restarted using our 'restart' program 

compilation: 
1: make all= compiles all the executeable files 

2: executable files: ./checkpoint ---our wrapper program that helps checkpointing
                    ./counting-test [positive-int-value] --- our target program which we are checkpointing.
                    ./read_ckpt_image  ---reads our default "myckpt.dat" file and prints first 50 hexa-dec values.
                    ./restart          --- reloads the checkpointed snapshot image back on the memory

3: libray files: ckpt.c --> ckpt.o --> libckpt.so ---contains our PRELOADED constructor function and signal handler

EXECUTION: [1-automated execution    2-detailed one-by-one steps execution]

1:(A)- make clean
  (B)- make check

2:(A)- make all
  (B)-./checkpoint ./counting-test
        --asks to enter a poitive number to start counting from
  (C)- from a new terminal read PID
        $ ps -uxw
  (D) send our interrupt 
        $ kill -12 [PID]
  (E) reading the checkpoint file "myckpt.dat"
        $ ./read_ckpt_image
  (F) restart the checkpointed process from saved "myckpt.dat" file
        $ ./restart        
