#include <stdio.h>
#include <omp.h>

#define MAXN 4096
#define INF (1LL<<60)

long long dp[MAXN * MAXN], SZ[MAXN + 5];
int main() {
	int N;
    while (scanf("%d", &N) == 1) {
        for (int i = 0; i <= N; i++)
            scanf("%lld", &SZ[i]);
        for (int i = 0; i < N; i++)
            dp[i * N + i] = 0;
		
		// spawn threads before get in to the loop to avoid respawning
		#pragma omp parallel
		{
		for (int len = 1; len <= N; len++) {
			#pragma omp for
			for (int l = 0; l < N - len; l++) {
				int r = l + len;
				long long cost = INF;
				dp[l * N + r] = INF;
				for (int k = l; k < r; k++) {
					long long t = dp[l * N + k] + dp[(k + 1) * N + r] + SZ[l] * SZ[k+1] * SZ[r+1];
					if (t < cost)
						cost = t;
				}
				if (cost < dp[l * N + r])
					dp[l * N + r] = cost;
			}
		}
		}
        printf("%lld\n", dp[0 * N + (N - 1)]);
    }
    return 0;
}
