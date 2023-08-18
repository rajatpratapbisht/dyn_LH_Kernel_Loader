#define _GNU_SOURCE /* Required for 'constructor' attribute (GNU extension) */
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

//#define WSL

void my_constructor();
int main(int, char *[]);
ucontext_t context;

static int is_in_signal_handler = 0;
static int is_restart = 0;

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

//-----------------------------------------------------------------------------------------------------------
//  match_one_line: read one line at a time. return 0 on sucess
//-----------------------------------------------------------------------------------------------------------
int match_one_line(int proc_maps_fd, proc_maps_line_t *proc_maps_line, char *filename)
{
  unsigned long int start, end;
  char rwxp[4];
  char tmp[10];
  int tmp_stdin = dup(0); // Make a copy of stdin
  dup2(proc_maps_fd, 0);  // Duplicate proc_maps_fd to stain
  // scanf() reads stdin (fd:0). It's a dup of proc_maps_fd ('same struct file').
  int rc = scanf("%lx-%lx %4c %*s %*s %*[0-9 ]%[^\n]\n",
                 &start, &end, rwxp, filename);
  // fseek() removes the EOF indicator on stdin for any future calls to scanf().
  assert(fseek(stdin, 0, SEEK_CUR) == 0);
  dup2(tmp_stdin, 0); // Restore original stdin; proc_maps_fd offset was advanced.
  close(tmp_stdin);
  if (rc == EOF || rc == 0)
  {
    proc_maps_line->start = NULL;
    proc_maps_line->end = NULL;
    return EOF;
  }
  if (rc == 3)
  { // if no filename on /proc/self/maps line:
    strncpy(proc_maps_line->name,
            "ANONYMOUS_SEGMENT", strlen("ANONYMOUS_SEGMENT") + 1);
  }
  else
  {
    assert(rc == 4);
    strncpy(proc_maps_line->name, filename, NAME_LEN - 1);
    proc_maps_line->name[NAME_LEN - 1] = '\0';
  }
  proc_maps_line->start = (void *)start;
  proc_maps_line->end = (void *)end;
  memcpy(proc_maps_line->rwxp, rwxp, 4);
  return 0;
}

//-----------------------------------------------------------------------------------------------------------
//  :read /proc/self/maps
//-----------------------------------------------------------------------------------------------------------
int proc_self_maps(proc_maps_line_t proc_maps[])
{
  int proc_maps_fd = open("/proc/self/maps", O_RDONLY); // NOTE:  fopen calls malloc() and potentially extends the heap segment.
  char filename[100];                                   // for debugging
  int i = 0;
  int rc = -2; // any value that will not terminate the 'for' loop.
  for (i = 0; rc != EOF; i++)
  {
    rc = match_one_line(proc_maps_fd, &proc_maps[i], filename);
#ifdef DEBUG
    if (rc == 0)
    {
      printf("proc_self_maps: filename: %s\n", filename); // for debugging
    }
#endif
  }
  close(proc_maps_fd);
  return 0;
}

//-----------------------------------------------------------------------------------------------------------
//  print_proc_self_maps: print /proc/self/maps on screen
//-------------------------------------------------------------------------------------------------------
int print_proc_self_maps(proc_maps_line_t proc_maps[])
{
  // printout
  int i = 0;
  for (i = 0; proc_maps[i].start != NULL; i++)
  {
    printf("%s (%c%c%c)\t"
           "  Address-range: %p - %p\n",
           proc_maps[i].name,
           proc_maps[i].rwxp[0], proc_maps[i].rwxp[1], proc_maps[i].rwxp[2],
           proc_maps[i].start, proc_maps[i].end);
  }

  return 0;
}

//-----------------------------------------------------------------------------------------------------------
//  write_proc_self_maps: write /proc/self/maps into a file proc_self_maps.dat
//-----------------------------------------------------------------------------------------------------------
int write_proc_self_maps(proc_maps_line_t proc_maps[])
{
  // write into proc_self_maps.dat
  int i = 0;
  unsigned int *first = &context;

  printf("First 50 units of data written in image file:\n");
  for (i = 0; i < 50; i++)
  {
    printf("%x\t", *(first + i * sizeof(int)));
  }
  printf("\n");

  int fd = open("/home/rajatbisht/mana/dyn_lh/1_fn_call_by_addr/ckpt/myckpt.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    perror("open:");
  }

  int rc;
  int size = sizeof(proc_maps_line_t);
  long int memory_size;

  // writing context at the top of the file
  rc = write(fd, (void *)&context, sizeof(context));
  if (rc == -1)
  {
    perror("write:");
  }

  // writing mmap to myckpt

  for (i = 0; proc_maps[i].start != NULL; i++)
  {
    if (proc_maps[i].rwxp[0] == 'r'  && strcmp(proc_maps[i].name, "[vsyscall]") != 0)
    {
      memory_size = proc_maps[i].end - proc_maps[i].start;
      // printf("memory_size: %ld\n", memory_size);

      // writing header
      rc = write(fd, (void *)&proc_maps[i], size);

      // writing data
      rc = write(fd, proc_maps[i].start, memory_size);
      while (rc < memory_size)
      {
        rc += write(fd, proc_maps[i].start + rc, memory_size - rc);
      }
      assert(rc == memory_size);
    }
  }
  close(fd);
}

//-----------------------------------------------------------------------------------------------------------
// signal_handler(int signal): is our way of intercepting the interrupt signal. this case we used SIGUSR2
//-----------------------------------------------------------------------------------------------------------
void signal_handler(int signal)
{
  is_in_signal_handler = 1;

  proc_maps_line_t proc_maps[1000];

  is_restart = 0;

  getcontext(&context);
  printf("\nValue of is_restart is : %d\n", is_restart);

  if (is_restart == 1)
  {
    // this is restart
    printf("\nThis is Restart...\n\n");
    printf("\nTHIS IS THE ORIGINAL PROGRAM...\n");

#ifdef WSL
    while (1)
      {}
#endif //WSL
  }

  else
  {
    // this is checkpointing
    is_restart = 1;

    printf("\nThis is CHECKPOINTING...");
    printf("\nValue of is_restart is : %d", is_restart);
    // read /proc/self/maps
    assert(proc_self_maps(proc_maps) == 0);
    // We purposely call proc_self_maps twice.  'scanf()' might call mmap()
    //   during the first execution, above.  But it doesn't need to map
    //   in a second memory segment after that.  It just re-uses the original one.

    //  assert(proc_self_maps(proc_maps) == 0);

    printf("\n#-------------------------------------------------------------------#\n");
    printf("\nReading ALL the info from /proc/self/maps file :\n");
    printf("\n#-------------------------------------------------------------------#\n");
    print_proc_self_maps(proc_maps);
    printf("\n#-------------------------------------------------------------------#\n\n");

    printf("\nWriting to myckpt.dat file: \n");
    write_proc_self_maps(proc_maps);
    printf("\n#-------------------------------------------------------------------#\n");

    // setcontext(&context);
     exit(0);
  }

  is_in_signal_handler = 0;
}

//-----------------------------------------------------------------------------------------------------------
// my_constructor: is our way of initialising some parameters even before 'main' routine is called. simialr to MANA's lower half being loaded
//-----------------------------------------------------------------------------------------------------------
void __attribute__((constructor))
my_constructor()
{
  // signal(SIGINT, &signal_handler);
  signal(SIGUSR2, &signal_handler);

  getcontext(&context);

  fprintf(stderr, "*************************************\n"
                  "***  We are running, using checkpoint. ***\n"
                  "*************************************\n\n");

  fflush(stdout);
}
