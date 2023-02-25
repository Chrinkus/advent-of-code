#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
get_first_house(int target)
{
        for (int i = 1, n = 0; ; ++i, n = isqrt(i)) {
                int sum = n * n == i ? i : 0;

                for (int j = n; j > 0; --j)
                        if (i % j == 0)
                                sum += j + i / j;

                if (sum >= target)
                        return i;
        }
}

int main(void)
{
        int input = read_input(stdin);

        printf("%d\n", get_first_house(input / 10));

        return EXIT_SUCCESS;
}
