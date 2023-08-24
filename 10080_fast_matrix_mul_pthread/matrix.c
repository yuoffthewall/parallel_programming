#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"
#define N_THREADS 8
#define MAXN 2048

#define UINT unsigned long

typedef struct Arg{
	int start, end, n;
	UINT *A, *B, *C;
}Arg;

static void transpose(int N, UINT A[][2048]) {
    UINT t;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++)
                t = A[i][j], A[i][j] = A[j][i], A[j][i] = t;
    }
}
void *thread_mul(void *arg){
	Arg data = *((Arg *)arg);
	int start = data.start, end = data.end, n = data.n;
	UINT *A = data.A, *B = data.B, *C = data.C;

	for (int i = start; i < end; i++) {
        for (int j = 0; j < n; j++) {
            UINT sum = 0;    // overflow, let it go.
            UINT *a = A + (i * MAXN), *b = B + (j *  MAXN);
            for (int k = 0; k < n; k++, a++, b++)
                //sum += (*(A + i * MAXN + k)) * (*(B + k * MAXN + j));
				sum += *a * *b;
			*(C + (i * MAXN + j)) = sum;
        }
    }	
}


void multiply(int n, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {
	
    transpose(n, B);
	pthread_t *threads;
	threads = malloc(N_THREADS * sizeof(pthread_t));

    for (int i = 0; i < N_THREADS; i++) {
    	 Arg *data = malloc(sizeof(Arg));
		 data->start = i * n / N_THREADS;
		 data->end = (i + 1) * n / N_THREADS;
		 data->n = n;
		 data->A = &A[0][0], data->B = &B[0][0], data->C = &C[0][0];
		 pthread_create(&threads[i], NULL, thread_mul, (void *)data);
    }
	for (int i = 0; i < N_THREADS; i++) 
	    pthread_join(threads[i], NULL);
}
