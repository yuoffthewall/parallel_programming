#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BLOCK 100000
int prime[5500], np = 0; // store primes
void sieve()
{
	bool is_prime[50001];  // mark primes
	is_prime[0] = is_prime[1] = false;
	for(int i = 0; i < 50000; i++)
		is_prime[i] = true;
    int n = 50000;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            for (int k = n / i, j = k * i; k >= i; k--, j -= i)
                is_prime[j] = false;
            prime[np++] = i;
        }
    }
}

int local_sieve(int l, int r)
{
    int sqr = (int) sqrt(r), gap = r - l;
	// mark all non-prime numbers in the current block
    bool not_prime_local[BLOCK] = {false};

    for (int i = 0; i < np && prime[i] <= sqr; i++) {
        unsigned p = prime[i], base = (l / p) * p;
        while (base < l)
            base += p;
        if (base == p)
            base += p;
        for (unsigned j = base; j <= r; j += p)
            not_prime_local[j - l] = true;
    }
	if(l == 1)
		not_prime_local[0] = true;
    int ret = gap + 1;
    for (int i = gap; i >= 0; i--)
        ret -= not_prime_local[i];
    return ret;
}
long long min(long long x, long long y)
{
    return x < y ? x : y;
}
int main()
{
    sieve();
    int l, r;
    while (scanf("%d %d", &l, &r) == 2) {
        int ret = 0;
#pragma omp parallel for schedule(dynamic) reduction(+ : ret)
        for (long long i = l; i <= r; i += BLOCK)
            ret += local_sieve(i, min(i + BLOCK - 1, r));
        printf("%d\n", ret);
    }
    return 0;
}
