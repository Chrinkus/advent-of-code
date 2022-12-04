#include <stdlib.h>
#include <stdio.h>

static void
count_section_overlaps(int* p1, int* p2)
{
        for (int a, b, c, d; scanf("%d-%d,%d-%d", &a, &b, &c, &d) == 4; ) {
                *p1 += ((a >= c && b <= d) || (c >= a && d <= b));
                *p2 += ((a <= d && b >= c) || (c <= b && d >= a));
        }
}

int main(void)
{
        int part1 = 0;
        int part2 = 0;

        count_section_overlaps(&part1, &part2);

        printf("%d\n", part1);
        printf("%d\n", part2);

        return EXIT_SUCCESS;
}
/*
#include <stdio.h>
int main(void) {
        int p1 = 0, p2 = 0;
        for (int a,b,c,d; scanf("%d-%d,%d-%d", &a,&b,&c,&d) == 4; ) {
                p1 += ((a >= c && b <= d) || (c >= a && d <= b));
                p2 += ((a <= d && b >= c) || (c <= b && d >= a));
        }
        printf("%d\n%d\n", p1, p2);
}
*/
