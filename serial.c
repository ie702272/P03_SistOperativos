
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>

#define iteraciones 200000000

struct timeval tv;
time_t Itime;
time_t Ftime;
time_t Ptime;

int main(){

	float suma = 0;
	float pi = 0;
	gettimeofday(&tv, NULL); 
 	Itime=tv.tv_usec;

	for(int i = 0;i<iteraciones;i++){
		suma = (pow(-1,i))/((2*i)+1);
		pi = suma + pi;
	}
	
	gettimeofday(&tv, NULL); 
 	Ftime=tv.tv_usec;
	Ptime = Ftime - Itime;
	printf("%30.28f, tiempo: %ld\n", pi, Ptime);
}
