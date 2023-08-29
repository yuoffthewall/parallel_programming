#include <stdio.h>

int can_place[32] = {0}; // obstacle mask

int dfs(int row, int mask_col, int mask_45, int mask_135, int mask)
{
	// return if all possible columns have been tried
    if (mask_col == mask)
        return 1;
	// bitmask of all possible columns
    int pos = mask & (~(mask_col | mask_45 | mask_135)) & can_place[row];
    int ret = 0;
    while (pos) {
        int bit = pos & (-pos); // find LSB
        pos ^= bit; // clear LSB
        ret += dfs(row + 1, (mask_col | bit), (mask_45 | bit) << 1, (mask_135 | bit) >> 1, mask);
    }
    return ret;
}

int totalNQueens(int n)
{
    int sum = 0;
    int mask_col = 0, mask_45 = 0, mask_135 = 0;
    int mask = (1 << n) - 1; // we set last n bits to get a mask of all possible columns
	int pos = mask & (~(mask_col | mask_45 | mask_135)) & can_place[0];
	#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; i++) {
        if ((pos >> i) & 1) {
            int bit = 1 << i; // the position to set
            sum += dfs(1, (mask_col | bit), (mask_45 | bit) << 1, (mask_135 | bit) >> 1, mask);
        }
    }
    return sum;
}

int main()
{
    int cases = 0, n;
    char s[32];
    while (scanf("%d", &n) == 1 && n) {
        for (int i = 0; i < n; i++) {
            scanf("%s", s);
            can_place[i] = (1 << n) - 1;  // set last n bits
            for (int j = 0; j < n; j++) {
                if (s[j] == '*')
                    can_place[i] ^= (1 << j);
            }
        }
        int ret = totalNQueens(n);
        printf("Case %d: %d\n", ++cases, ret);
    }
    return 0;
}
