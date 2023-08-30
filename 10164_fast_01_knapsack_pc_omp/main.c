#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <assert.h>

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

typedef struct{
	int w, v;
}ele;

void knapsack(int n, int m, ele E[], int dp[])
{
    memset(dp, 0, sizeof(dp[0]) * (m + 1));
	int wsum = 0;
    for (int i = 0; i < n; i++) {
        int v = E[i].v, w = E[i].w;
		/*
		for(int k = m; k >= w; k--)
			dp[k] = max(dp[k], dp[k - w] + v);
		*/
#define UNLOOP { dp[k] = max(dp[k], dp[k - w] + v), k--; }
#define UNLOOP4 { UNLOOP UNLOOP UNLOOP UNLOOP }
#define UNLOOP8 { UNLOOP4 UNLOOP4 }
		int k = m;
		while(k - 8 >= w)
			UNLOOP8;
		while(k >= w)
			UNLOOP;
    }
}

#define MAXM 5000005
#define MAXN 10005
int dp[2][MAXM];
ele E[MAXN];

int main()
{
    omp_set_num_threads(2);
    #pragma omp parallel
    {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	int tid = omp_get_thread_num();
	for (int i = tid * 6; i < (tid + 1) * 6; i++)
		CPU_SET(i, &cpuset);
	assert(sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0);
    }

    int N, M;
    while (scanf("%d %d", &N, &M) == 2) {
        for (int i = 0; i < N; i++)
            scanf("%d %d", &E[i].w, &E[i].v);

        #pragma omp parallel sections
        {
        #pragma omp section
		knapsack(N / 2, M, E, dp[0]);
        #pragma omp section
		knapsack(N - N / 2, M, &E[N / 2], dp[1]);
        }

        int ans = 0;
        for(int w1 = M; w1 >= 0; w1--){
            int w2 = M - w1;
            ans = max(ans, dp[0][w1] + dp[1][w2]);
        }
        printf("%d\n", ans);
    }
    return 0;
}
