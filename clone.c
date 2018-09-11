#define _GNU_SOURCE
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>

#define iteraciones 200000000
#define NTHREADS 4
#define FIBER_STACK 1024*64

struct timeval tv;
time_t Itime;
time_t Ftime;
time_t Ptime;

float resultado = 0;

int clone_thread(void *args)
{
  float suma = 0;
  float pi = 0;
  int i, start, end, Itnum;
  Itnum = *((int *) args);
  start = (iteraciones/NTHREADS)*Itnum;
  end = (iteraciones/NTHREADS)*(Itnum+1);
for(i=start; i<end; i++)
{
  suma = (pow(-1,i))/((2*i)+1);
  pi = suma + pi;
}
  resultado = pi;
  return 0;

}

int main(){
  void* stack;
  pid_t pid;
  int status;
  int i;
  int args[NTHREADS];

  gettimeofday(&tv, NULL);
 	Itime=tv.tv_sec;

  // Allocate the stack
  stack = malloc( FIBER_STACK * NTHREADS);
  if ( stack == 0 )
  {
          perror( "malloc: could not allocate stack" );
          exit( 1 );
  }

  for(i=0;i<NTHREADS;i++)
  {
    // Call the clone system call to create the child thread
    args[i] = i;
    pid = clone( &clone_thread, (char*) stack + (FIBER_STACK*(i+1)),
                SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*)&args[i]);
    if ( pid == -1 )
    {
            perror( "clone" );
            exit( 2 );
    }
  }

  for(i=0;i<NTHREADS;i++)
  {
    // Wait for the child thread to exit
    pid=wait(&status);
    if ( pid == -1 )
    {
            perror( "waitpid" );
            exit( 3 );
    }
  }
  // Free the stack
free( stack );
gettimeofday(&tv, NULL);
Ftime=tv.tv_sec;
Ptime = Ftime - Itime;

printf("%30.28f, tiempo: %ld\n", resultado, Ptime);

//printf( "Child thread returned and stack freed.\n" );
return 0;
}
