
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
	int nthread=*((int *) args);
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_usec;
	float suma = 0;
	float pi = 0;
	long Iteraciones;
	long i;
	switch(nthread){
		case 0: 
			Iteraciones = 50000000;
			i = 0;
			break;
		case 1: 
			Iteraciones = 100000000;
			i = 50000000;
			break;
		case 2: 
			Iteraciones = 150000000;
			i = 100000000;
			break;
		case 3: 
			Iteraciones = 200000000;
			i = 150000000;
			break;
		}

	for(i;i<Iteraciones;i++){
		suma = (pow(-1,i))/((2*i)+1);
		pi = suma + pi;
	}
	resultado = pi;
}

int main(){
	
	int i;
	int parArr[NTHREADS];
	pthread_t tid[NTHREADS];
	
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_usec;

	for(i=0;i<NTHREADS;i++)
	{
		pthread_create(&tid[i],NULL,calculo,(void *) &parArr[i]);
	}
	
	for(i=0;i<NTHREADS;i++)
		pthread_join(tid[i],NULL);

	gettimeofday(&tv, NULL); 
	Ftime=tv.tv_usec;
	Ptime = Ftime - Itime;
	
	printf("%30.28f, tiempo: %ld\n", resultado, Ptime);

	return 0;
}
