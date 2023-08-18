#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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

int main(int argc, char *argv[])
{
    int *first;
    int i;

    proc_maps_line_t proc_maps;
    // read proc_self_maps.dat and print it on screen

    // printf("%s\n", argv[1]);
    int fd = open("myckpt.dat", O_RDONLY);
    // int fd = open(argv[1], O_RDONLY);

    // read context
    ucontext_t context;
    long int rc = read(fd, &context, sizeof(context));
    if (rc < 0)
    {
        perror("read:");
    }

    first = &context;

    long int data_size;

    rc = -2;

    printf("#-------------------------------------------------------------------#\n");
    printf("Reading HEADER data prom myckpt.dat file\n");
    printf("#-------------------------------------------------------------------#\n\n");

    for (i = 0; rc != EOF; i++)
    {
        // reading header
        rc = read(fd, &proc_maps, sizeof(proc_maps_line_t));
        if (rc != sizeof(proc_maps_line_t))
        {
            perror("rc");
            break;
        }

        // print head
        printf("%s (%c%c%c)\t"
               "  Address-range: %p - %p\n",
               proc_maps.name,
               proc_maps.rwxp[0], proc_maps.rwxp[1], proc_maps.rwxp[2],
               proc_maps.start, proc_maps.end);

        lseek(fd, proc_maps.end - proc_maps.start, SEEK_CUR);

    }

    // printing first 50 units of data
    printf("#-------------------------------------------------------------------#\n");
    printf("First 50 units of data written in image file:\n");
    for ( i = 0; i < 50; i++)
    {
        printf("%x\t", *(first + i * sizeof(int)));
    }
    printf("\n#-------------------------------------------------------------------#\n");

    return 0;
}
