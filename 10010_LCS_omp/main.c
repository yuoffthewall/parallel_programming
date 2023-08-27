#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define swap(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

#define MAXN 65536

int lcs(const char *A, int na, const char *B, int nb)
{
    if (na > nb){
        swap(A, B);
		swap(na, nb);
	}
    int dp[3][MAXN * 2];
    for (int i = 0; i < 3; i++)
        memset(dp[i], 0, sizeof(int) * (nb + na + 2));
    // memset(dpf, 0, sizeof(int) * (nb + 1));
    omp_set_num_threads(4);

    // Anti-Diagonal Parallel Approach (go bottom left to up right))
    // Rotate the dp table by 45 degrees
    int last_l = 0, last_r = 0;
    for (int i = 0; i < na + nb - 1; i++) {
        int l = max(0, i - na + 1);
		int r = min(i, nb - 1);
		#pragma omp parallel for schedule(static)
        for (int j = l; j <= r; j++) {
            int x = i - j;
			int y = j;
            if (A[x] == B[y])
                dp[2][j + 1] = dp[0][j] + 1;
            else
                dp[2][j + 1] = max(dp[1][j], dp[1][j + 1]);
        }
        memcpy(&dp[0][last_l + 1], &dp[1][last_l + 1], sizeof(int) * (last_r - last_l + 1));
        memcpy(&dp[1][l + 1], &dp[2][l + 1], sizeof(int) * (r - l + 1));
        last_l = l, last_r = r;
    }
    return dp[2][last_r + 1]; 
}
int main()
{
	char *A = malloc(MAXN * sizeof(char));
	char *B = malloc(MAXN * sizeof(char));
    while (scanf("%s %s", A, B) == 2) {
        int len = lcs(A, strlen(A), B, strlen(B));
        printf("%d\n", len);
    }
    return 0;
}
