#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"
#include <assert.h>
typedef struct Argu {
    int l, r;
    uint32_t *psum;
    uint32_t key;
} Argu;
void* do_local_psum(void *argu) {
    Argu data = *((Argu *) argu);
    int l = data.l, r = data.r;
    uint32_t sum = 0;
	for(int i = l; i <= r; i++){
		sum += encrypt(i, data.key);
		data.psum[i] = sum; 
	}
    free(argu);
}
void* pass_psum(void *argu) {
    Argu data = *((Argu *) argu);
    int l = data.l, r = data.r;
	for(int i = l; i <= r; i++){
		data.psum[i] += data.key; 
	}
    free(argu);
}
#define MAXN 10000005
#define MAX_THREAD 4
uint32_t prefix_sum[MAXN];
int main() {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	for (int i = 0; i < 6; i++)
			CPU_SET(i, &cpuset);
	assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);

    int n;
    uint32_t key;
    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        pthread_t threads[MAX_THREAD];
        for (int i = 0; i < MAX_THREAD; i++) {
            Argu *argu = malloc(sizeof(Argu));
            argu->l = i * n / MAX_THREAD + 1;
            argu->r = (i + 1) * n / MAX_THREAD;
            argu->psum = prefix_sum;
			argu->key = key;
            pthread_create(&threads[i], NULL, do_local_psum, argu);
        }
        for (int i = 0; i < MAX_THREAD; i++)
            pthread_join(threads[i], NULL);
 
        uint32_t local_psum = 0;
        for (int i = 0; i < MAX_THREAD; i++) {
            Argu *argu = malloc(sizeof(Argu));
            argu->l = i * n / MAX_THREAD + 1;
            argu->r = (i + 1) * n / MAX_THREAD;
            argu->psum = prefix_sum;
			argu->key = local_psum;
            local_psum += prefix_sum[argu->r];
            pthread_create(&threads[i], NULL, pass_psum, argu);
        }
        for (int i = 0; i < MAX_THREAD; i++)
            pthread_join(threads[i], NULL);

        output(prefix_sum, n);
    }
    return 0;
}
