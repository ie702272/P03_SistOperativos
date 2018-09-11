
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>

#define iteraciones 200000000

struct timeval tv;
time_t Itime;
time_t Ftime;
time_t Ptime;
int nhijo = 0;
int Iteraciones;
/* pointer to shared memory obect */
void *ptr;

void* create_shared_memory(size_t size) {
  // Memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // Buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
	
  return mmap(NULL, size, protection, visibility, 0, 0);
}

int main(){

	float suma = 0;
	float pi = 0;
	int i=0;
	float *address;
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_usec;
	int status;
	float resultado = 0;

	void* shmem = create_shared_memory(128);

	int pid = fork();
	if(pid==0){
		nhijo = 1;
		pid = fork();
			if(0==pid){
				nhijo = 2;
				pid = fork();
				if(0==pid){
					nhijo = 3;
				}
			}
	}

	switch(nhijo){
		case 0: 
			Iteraciones = 50000000;
			i = 0;
			for(i;i<Iteraciones;i++){
				suma = (pow(-1,i))/((2*i)+1);
				pi = suma + pi;
			}
			resultado = *(float*)shmem;
			resultado = resultado + pi;
			address = &resultado;
			memcpy(shmem, address, sizeof(resultado));
			break;
		case 1: 
			Iteraciones = 100000000;
			i = 50000000;
			for(i;i<Iteraciones;i++){
				suma = (pow(-1,i))/((2*i)+1);
				pi = suma + pi;
			}
			resultado = *(float*)shmem;
			resultado = resultado + pi;
			address = &resultado;
			memcpy(shmem, address, sizeof(resultado));
			exit(2);
			break;
		case 2: 
			Iteraciones = 150000000;
			i = 100000000;
			for(i;i<Iteraciones;i++){
				suma = (pow(-1,i))/((2*i)+1);
				pi = suma + pi;
			}
			resultado = *(float*)shmem;
			resultado = resultado + pi;
			address = &resultado;
			memcpy(shmem, address, sizeof(resultado));
			exit(2);
			break;
		case 3: 
			Iteraciones = 200000000;
			i = 150000000;
			for(i;i<Iteraciones;i++){
				suma = (pow(-1,i))/((2*i)+1);
				pi = suma + pi;
				}
			resultado = *(float*)shmem;
			resultado = resultado + pi;
			address = &resultado;
			memcpy(shmem, address, sizeof(resultado));
			exit(2);
			break;
		}
	
	gettimeofday(&tv, NULL); 
 	Ftime=tv.tv_usec;
	Ptime = Ftime - Itime;
	printf("%30.28f, tiempo: %ld\n", resultado, Ptime);
}
