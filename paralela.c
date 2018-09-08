
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>

#define iteraciones 200000000
#define NTHREADS 4

struct timeval tv;
time_t Itime;
time_t Ftime;
time_t Ptime;

float resultado = 0;

void *calculo(void *args)
{
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_usec;
	float suma = 0;
	float pi = 0;
	for(int i = 0;i<iteraciones;i++){//falt ala logica para este pedo
		suma = (pow(-1,i))/((2*i)+1);
		pi = suma + pi;
	}
	resultado = suma;
}

int main(){
	
	int i;
	pthread_t tid[NTHREADS];
	
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_usec;

	for(i=0;i<NTHREADS;i++)
	{
		pthread_create(&tid[i],NULL,calculo,NULL);
	}
	
	for(i=0;i<NTHREADS;i++)
		pthread_join(tid[i],NULL);

	gettimeofday(&tv, NULL); 
	Ftime=tv.tv_usec;
	Ptime = Ftime - Itime;
	
	printf("%30.28f, tiempo: %ld\n", resultado, Ptime);

	return 0;
}
