#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 1024
#define WHITE 0
#define RED   1
#define BLUE  2
 
char g1[MAXN][MAXN], g2[MAXN][MAXN];
 
void moveRed(int n) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++)
        memset(g2[i], WHITE, sizeof(char) * n);
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g1[i][j] == BLUE) {
                g2[i][j] = BLUE;
            } else if (g1[i][j] == RED) {
                int next = j+1 == n ? 0 : j+1;
                if (g1[i][next] == WHITE)
                    g2[i][next] = RED;
                else
                    g2[i][j] = RED;
            }
        }
    }
}
void moveBlue(int n) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++)
        memset(g1[i], WHITE, sizeof(char) * n);
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g2[i][j] == RED) {
                g1[i][j] = RED;
            } else if (g2[i][j] == BLUE) {
                int next = i+1 == n ? 0 : i+1;
                if (g2[next][j] == WHITE)
                    g1[next][j] = BLUE;
                else
                    g1[i][j] = BLUE;
            }
        }
    }
}
int toIndex(char c) {
    if (c == 'W')    return WHITE;
    if (c == 'R')    return RED;
    if (c == 'B')    return BLUE;
}
void printBoard(int n) {
    char color[4] = "WRB";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            putchar(color[g1[i][j]]);
        putchar('\n');
    }
}
int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        for (int i = 0; i < n; i++) {
            scanf("%s", g1[i]);
            for (int j = 0; j < n; j++)
                g1[i][j] = toIndex(g1[i][j]);
        }
 
        for (int it = 0; it < m; it++) {
            moveRed(n);
            moveBlue(n);
        }
        printBoard(n);
    }
    return 0;
}
