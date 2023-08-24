#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define UINT unsigned long

static void transpose(int N, UINT A[][2048]) {
    UINT t;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++)
        	t = A[i][j], A[i][j] = A[j][i], A[j][i] = t;
    }
}

void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {
	transpose(N, B);

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            register UINT sum = 0;
            UINT *a = &A[i][0], *b = &B[j][0];
			for (int k = 0; k < N; k++, a++, b++)
				sum += *a * *b;
				
            C[i][j] = sum;
        }
    }	

}
