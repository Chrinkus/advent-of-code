#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum ice { LITRES = 150, };

static void*
read_containers(struct cgs_vector* v)
{
        for (int n; scanf("%d ", &n) == 1; )
                cgs_vector_push(v, &n);
        return v;
}

static void
recursive_count(const int* ai, size_t len, size_t i, int tar,
                int tot, int* p1, int n, int* ways)
{
        if (tot == tar) {
                *p1 += 1;
                ways[n] += 1;
                return;
        }

        if (i == len || tot > tar)
                return;

        recursive_count(ai, len, i+1, tar, tot + ai[i], p1, n+1, ways); // with
        recursive_count(ai, len, i+1, tar, tot, p1, n, ways);           // w/o
}

static void
count_combinations(const struct cgs_vector* v, int tar, int* p1, int* p2)
{
        const size_t len = cgs_vector_length(v);
        int* ways = calloc(len + 1, sizeof(int));       // Needed for part 2

        recursive_count(cgs_vector_data(v),
                        len,
                        0, tar, 0, p1,
                        0, ways);

        // Find the number of ways using minimum number of containers
        for (size_t i = 0; i < len; ++i)
                if (ways[i] != 0) {
                        *p2 = ways[i];
                        break;
                }
        free(ways);
}

int main(void)
{
        struct cgs_vector containers = cgs_vector_new(sizeof(int));
        read_containers(&containers);
        cgs_vector_sort(&containers, cgs_int_cmp);

        int part1 = 0;
        int part2 = 0;
        count_combinations(&containers, LITRES, &part1, &part2);

        printf("%d\n", part1);
        printf("%d\n", part2);

        cgs_vector_free(&containers);
        return EXIT_SUCCESS;
}
