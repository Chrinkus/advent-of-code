#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

enum constants { MUL_1 = 10, MUL_2 = 11, ELF_MAX = 50, };

static int
read_input(FILE* file)
{
        int n = 0;
        if (fscanf(file, "%d ", &n) != 1)
                return -1;
        return n;
}

static int
isqrt(int x)
{
        return (int)sqrt((double)x);
}

static int
get_first_house(int target, int mul, int max)
{
        const int tar = target / mul;

        for (int i = 1, n = isqrt(i); ; ++i, n = isqrt(i)) {
                int sum = (n * n == i && n <= max) ? n : 0;

                for (int j = n - 1; j > 0; --j)
                        if (i % j == 0) {
                                int k = i / j;
                                if (j <= max)
                                        sum += k;
                                if (k <= max)
                                        sum += j;
                        }
                if (sum >= tar)
                        return i;
        }
}

int main(void)
{
        int input = read_input(stdin);

        printf("%d\n", get_first_house(input, MUL_1, INT_MAX));
        printf("%d\n", get_first_house(input, MUL_2, ELF_MAX));

        return EXIT_SUCCESS;
}
