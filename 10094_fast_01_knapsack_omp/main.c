#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXM 1000005
#define MAXN 10005
int dp[2][MAXM];
int W[MAXN], V[MAXN];

#define max(a, b) (a > b ? a : b)
#define swap(a, b) int tmp = a; a = b; b = tmp;

int main()
{
    int N, M;
    while (scanf("%d %d", &N, &M) == 2) {
        for (int i = 0; i < N; i++)
            scanf("%d %d", &W[i], &V[i]);
        memset(dp, 0, sizeof(dp));

        int old = 0, new = 1;
        for (int i = 0; i < N; i++) {
            swap(old, new);
            int v = V[i], w = W[i];
            #pragma omp parallel
            {
            #pragma omp for
            for(int k = 1; k < w; k++)
               dp[new][k] = dp[old][k];
            #pragma omp for
            for (int k = w; k <= M; k++){
               dp[new][k] = max(dp[old][k - w] + v, dp[old][k]);
            }
            }
        }

        printf("%d\n", dp[new][M]);
    }
    return 0;
}
