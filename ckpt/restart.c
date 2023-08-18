#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ucontext.h>
#include <assert.h>


//-----------------------------------------------------------------------------------------------------------
// struct proc_map_line: creating a custom data type for reading /proc/self/maps file
//-----------------------------------------------------------------------------------------------------------
#define NAME_LEN 80
struct proc_maps_line
{
    void *start;
    void *end;
    char rwxp[4];
    int read, write, execute; // Not used in this version of the code
    char name[NAME_LEN];      // for debugging only
} typedef proc_maps_line_t;



void do_work()
{
    int *first; // write it on /proc/self/maps
    proc_maps_line_t proc_maps;
    int i;

    int fd = open("/home/rajatbisht/mana/dyn_lh/1_fn_call_by_addr/ckpt/myckpt.dat", O_RDONLY);

    // int mmap_fd = open("/proc/self/maps", )
    // read context
    ucontext_t context;
    long int rc = read(fd, &context, sizeof(context));
    if (rc < 0)
    {
        perror("read:");
    }

    first = &context;

#ifdef VERBOSE
	// printing first 50 units of data
	printf("#-------------------------------------------------------------------#\n");
    	printf("First 50 units of data written in image file:\n");
    	for ( i = 0; i < 50; i++)
    	{
    	    printf("%x\t", *(first + i * sizeof(int)));
    	}
    	printf("\n#-------------------------------------------------------------------#\n");
    	printf("#-------------------------------------------------------------------#\n");
    	printf("Reading HEADER data from myckpt.dat file\n");
    	printf("#-------------------------------------------------------------------#\n\n");
#endif // VERBOSE

    size_t data_size;

    unsigned int *data;

    rc = -2;

    for ( i = 0; rc != EOF; i++)
    {

        // reading header from myckpt.dat file
        rc = read(fd, &proc_maps, sizeof(proc_maps_line_t));
        if (rc != sizeof(proc_maps_line_t))
        {
            perror("rc");
            break;
        }

#ifdef VERBOSE
        // print head
        printf("%s (%c%c%c)\t"
               "  Address-range: %p - %p\t",
               proc_maps.name,
               proc_maps.rwxp[0], proc_maps.rwxp[1], proc_maps.rwxp[2],
               proc_maps.start, proc_maps.end);
#endif //VERBOSE

        // size of data is precalculated here.
        data_size = proc_maps.end - proc_maps.start;

#ifdef VERBOSE
        printf("datasize = %ld\n", (long)data_size);
#endif //VERBOSE

        // allocating size to data at runtime
        data = malloc(data_size);
        if (data == NULL)
        {
            perror("malloc");
        }

        // read data from myckpt.dat file
        rc = read(fd, (void *)data, data_size);
        if (rc == -1)
        {
            perror("read1");
        }

        if (rc < data_size)
        {
            rc += read(fd, data + rc, data_size - rc);
        }
        //assert(rc == data_size);

        // creating memory segment at /proc/self/maps
        void *addr = mmap((void *)proc_maps.start, data_size,PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1, 0);
        if (addr == MAP_FAILED)
        {
            perror("mmap");
        }

        // writing to the memory segments our data
        unsigned int *wc_m = memcpy(addr, data, data_size);
        if (wc_m != addr)
        {
            perror("memcpy");
        }
        free(data);
        // lseek(fd, proc_maps.end - proc_maps.start, SEEK_CUR);
    }

    printf("\nwriting to mmap: SUCESSFULL\n");

    printf("\nJUMPING TO CHECKPOINTED PROCESS....\n");

    setcontext(&context);

    printf("this line will not work");

}

void recursive(int levels)
{
    if (levels > 0)
    {
        recursive(levels-1);
    }
    else{
        //do work;
        do_work();
    }
}


int main()
{
    recursive(1000);
    
    return 0;
}
