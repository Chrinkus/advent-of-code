#include <stdlib.h>
#include <stdio.h>

static void
count_section_overlaps(int* p1, int* p2)
{
        for (int a, b, c, d; scanf("%d-%d,%d-%d", &a, &b, &c, &d) == 4; ) {
                if ((a >= c && b <= d) || (c >= a && d <= b))
                        ++*p1;
                if ((a <= d && b >= c) || (c <= b && d >= a))
                        ++*p2;
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
/* Golf'd
#include <stdio.h>
int main(void) {
        int p1 = 0, p2 = 0;
        for (int a,b,c,d; scanf("%d-%d,%d-%d", &a,&b,&c,&d) == 4; ) {
                if ((a >= c && b <= d) || (c >= a && d <= b))   ++p1;
                if ((a <= d && b >= c) || (c <= b && d >= a))   ++p2;
        }
        printf("%d\n%d\n", p1, p2);
}
*/
