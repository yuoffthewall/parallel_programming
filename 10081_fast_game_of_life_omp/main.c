#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 2048
char g[2][MAXN][MAXN];

#define UNLOOP { \
		int adj = g[flag][i - 1][j - 1] + g[flag][i - 1][j] + \
				  g[flag][i - 1][j + 1] + g[flag][i][j - 1] + \
				  g[flag][i][j + 1] + g[flag][i + 1][j - 1] + \
				  g[flag][i + 1][j] + g[flag][i + 1][j + 1]; \
		g[!flag][i][j] = \
			(g[flag][i][j] == 0 && adj == 3) || \
			(g[flag][i][j] == 1 && (adj == 2 || adj == 3)); j++; \
	}
#define UNLOOP4 {UNLOOP UNLOOP UNLOOP UNLOOP}

int main()
{
	memset(g[0][0], 0, MAXN * sizeof(char));
	int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        while (getchar() != '\n');

        // read input
		memset(g[0][n + 1], 0, (n + 2) * sizeof(char));
        for (int i = 1; i <= n; i++) {
            fgets(&g[0][i][1], MAXN, stdin);
            for (int j = 1; j <= n; j++)
                g[0][i][j] -= '0';
            g[0][i][n + 1] = 0;
            g[0][i][0] = 0;
        }

        // game of life
        int flag = 0;
        int chunk = n / omp_get_max_threads();
        for (int it = 0; it < m; it++) {
			#pragma omp parallel for schedule(static, chunk)
            for (int i = 1; i <= n; i++) {
				int adj, j = 1;
				while(j + 4 <= n)
					UNLOOP4;
				while(j <= n)
					UNLOOP;
				/*
                for (int j = 1; j <= n; j++) {
                    int adj = g[flag][i - 1][j - 1] + g[flag][i - 1][j] +
                              g[flag][i - 1][j + 1] + g[flag][i][j - 1] +
                              g[flag][i][j + 1] + g[flag][i + 1][j - 1] +
                              g[flag][i + 1][j] + g[flag][i + 1][j + 1];
                    g[!flag][i][j] =
                        (g[flag][i][j] == 0 && adj == 3) ||
                        (g[flag][i][j] == 1 && (adj == 2 || adj == 3));
                }
				*/
            }
            flag = !flag;
        }

        // print result
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                g[flag][i][j] += '0';
            g[flag][i][n + 1] = '\0';
            puts(g[flag][i] + 1);
        }
    }
    return 0;
}
