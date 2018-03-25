#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>



double get_time() {
	struct timespec tiempo;
	double sec;
;
	if (clock_gettime(CLOCK_REALTIME, &tiempo)==0) {
		sec=tiempo.tv_sec+tiempo.tv_nsec/(double) 1000000000.0;
    		return sec;
	} else {
		printf("Obtener tiempo: NO HA HABIDO RESPUESTA DE GETTIME\n");
		return 0;
	}
}

int main(int argc, char* argv[]){

float *a,*b,*c;

int count;
int nt;
for (nt = 2; nt <= 8; nt*=2) {
	printf("------------------\n");
	printf("Num_threads = %d\n", nt);
	printf("------------------\n");
	for (count = 0; count <=20; count++) {
		int orden = count*100;
		printf ("Orden %d: ", orden);
		int n,m,p;

		n = orden;
		m = orden;
		p = orden;

		a = (float*)malloc(n*p*sizeof(float));
		b = (float*)malloc(p*m*sizeof(float));
		c = (float*)malloc(n*m*sizeof(float));

		if(a == NULL || b == NULL || c == NULL){
			printf("Alguna matriz no tiene memoria asignada");
			return -1;
		}
		int try;
		float  media = 0;
		for (try = 0; try < 3; try++) {
			double start = get_time();
			int i,j,k;
			float rtemp;
			//pragma omp parallel for private (i, j, k, rtemp), num_threads(nt) //bucle i
			//#pragma omp parallel private (i), num_threads(nt) // bucle j
			#pragma omp parallel for collapse(2), private (i, j, k, rtemp), num_threads(nt) //collapse
			for(i=0;i<n;i++){
				//#pragma omp for private (j, k, rtemp)	//bucle j
				for(j=0;j<m;j++){
					rtemp = 0;
					for(k=0;k<p;k++){
						rtemp += a[i*p+k] * b[k*m+j];
					}	
					c[(i*m)+j] = rtemp;
				}
			}

			double end = get_time();
			//printf ("%lf \n", (end-start));
			media += end -start;		
		}
		printf ("%lf \n", (media/3));
	}
}
return 0;
}
