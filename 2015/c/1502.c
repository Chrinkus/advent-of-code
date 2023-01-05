#include <stdlib.h>
#include <stdio.h>

enum magic { SIDES = 3 };

static int
int_cmp(const void* a, const void* b)
{
        const int* p1 = a;
        const int* p2 = b;
        return *p1 - *p2;
}

int main(void)
{
        int paper = 0;
        int ribbon = 0;
        for (int a[SIDES]; scanf("%dx%dx%d ", &a[0], &a[1], &a[2]) == SIDES;) {
                qsort(a, SIDES, sizeof(int), int_cmp);

                paper += 3 * a[0] * a[1] + 2 * a[1] * a[2] + 2 * a[2] * a[0];
                ribbon += 2 * a[0] + 2 * a[1] + a[0] * a[1] * a[2];
        }

        printf("%d\n", paper);
        printf("%d\n", ribbon);

        return EXIT_SUCCESS;
}
