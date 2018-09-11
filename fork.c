
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>

#define iteraciones 200000000

/*Estructura para tomar el tiempo*/
struct timeval tv;

/*Variables para guardar los tiempos*/
time_t Itime;
time_t Ftime;
time_t Ptime;

/*Numero de hijo para cada caso*/
int nhijo = 0;
int Iteraciones;

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

/*Apuntdor para obtener la direccion de la variable resultado*/
	float *address;

/*Tiempo inicial*/
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_sec;
	int status;

/*Variable para sumar el cálculo de cada proceso*/
	float resultado = 0;

/*Crear memoria compartida de tamaño 128*/
	void* shmem = create_shared_memory(128);

/*Creación de los procesos*/
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
		case 0: /*Caso papa*/
			Iteraciones = 50000000;
			i = 0;
			for(i;i<Iteraciones;i++){
				suma = (pow(-1,i))/((2*i)+1);
				pi = suma + pi;
			}
			/*Guardamos el valor de la memoria en la variable resultado*/
			resultado = *(float*)shmem;
			/*Le sumamos a resultado el calculo de pi*/
			resultado = resultado + pi;
			/*Obtenemos la direccion de memoria de resultado*/
			address = &resultado;
			/*Copiamos el valor de la memoria de resultado a la memoria compartida*/
			memcpy(shmem, address, sizeof(resultado));
			break;
		case 1: /*Hijo1*/
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
		case 2: /*Hijo2*/
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
		case 3: /*Hijo3*/
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
 	Ftime=tv.tv_sec;
	Ptime = Ftime - Itime;
	printf("%30.28f, tiempo: %ld\n", resultado, Ptime);
}
