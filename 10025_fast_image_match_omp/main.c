#include <stdio.h>
#include <limits.h>
#define MAXN 512
int A[MAXN][MAXN], B[MAXN][MAXN];
int diff_mtx[MAXN][MAXN]; //diff matrix
int main() {
    int ah, aw, bh, bw, num;
    int cases = 0;
    while(scanf("%d %d %d %d", &ah, &aw, &bh, &bw) == 4) {
        for (int i = 0; i < ah; i++)
            for (int j = 0; j < aw; j++)
                scanf("%d", &num), A[i][j] = num;
        for (int i = 0; i < bh; i++)
            for (int j = 0; j < bw; j++)
                scanf("%d", &num), B[i][j] = num;

        int dh = ah - bh + 1, dw = aw - bw + 1;
        int x = -1, y = -1;
        int min_diff = INT_MAX;
		// compute diff matrix
        #pragma omp parallel for
        for (int i = 0; i < dh; i++) {
            for (int j = 0; j < dw; j++) {
                int diff = 0;
                for (int p = 0; p < bh; p++) {
                    for (int q = 0; q < bw; q++) {
                        diff += (B[p][q] - A[i + p][j + q])*(B[p][q] - A[i + p][j + q]);
                    }                    
                }
                diff_mtx[i][j] = diff;
            }
        }

		// find the smallest diff
        for (int i = 0; i < dh; i++) {
            for (int j = 0; j < dw; j++)
                if (diff_mtx[i][j] < min_diff)
                    min_diff = diff_mtx[i][j], x = i, y = j;
        }
 
        printf("%d %d\n", x + 1, y + 1);
    }
    return 0;
}
