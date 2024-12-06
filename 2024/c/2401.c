#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

void*
read_lists(struct cgs_vector* va, struct cgs_vector* vb)
{
        struct cgs_string buff = cgs_string_new();
        while (cgs_io_getline(stdin, &buff) > 0) {
                int a, b;
                if (sscanf(cgs_string_data(&buff), "%d %d", &a, &b) != 2)
                        goto error_cleanup;
                if (!cgs_vector_push(va, &a) || !cgs_vector_push(vb, &b))
                        goto error_cleanup;

                cgs_string_clear(&buff);
        }

        cgs_string_free(&buff);
        return va;
error_cleanup:
        cgs_string_free(&buff);
        return NULL;
}

int
get_distance_sum(const struct cgs_vector* va, const struct cgs_vector* vb)
{
        int sum = 0;

        for (size_t i = 0; i < cgs_vector_length(va); ++i) {
                const int* pa = cgs_vector_get(va, i);
                const int* pb = cgs_vector_get(vb, i);

                sum += abs(*pa - *pb);
        }

        return sum;
}

int
get_similarity_sum(const struct cgs_vector* va, const struct cgs_vector* vb)
{
        int sum = 0;

        for (size_t i = 0; i < cgs_vector_length(va); ++i) {
                int count = 0;
                const int* pa = cgs_vector_get(va, i);
                for (size_t j = 0; j < cgs_vector_length(vb); ++j) {
                        const int* pb = cgs_vector_get(vb, j);
                        count += *pa == *pb;
                        if (*pb > *pa)
                                break;
                }
                sum += *pa * count;
        }

        return sum;
}

int
main(void)
{
        struct cgs_vector list_a = cgs_vector_new(sizeof(int));
        struct cgs_vector list_b = cgs_vector_new(sizeof(int));

        if (!read_lists(&list_a, &list_b))
                return cgs_error_retfail("Input read failure\n");

        if (cgs_vector_length(&list_a) != cgs_vector_length(&list_b))
                return cgs_error_retfail("Lists are different lengths\n");

        cgs_vector_sort(&list_a, cgs_int_cmp);
        cgs_vector_sort(&list_b, cgs_int_cmp);

        int part1 = get_distance_sum(&list_a, &list_b);
        printf("Part 1: %d\n", part1);

        int part2 = get_similarity_sum(&list_a, &list_b);
        printf("Part 2: %d\n", part2);

        cgs_vector_free(&list_a);
        cgs_vector_free(&list_b);

        return EXIT_SUCCESS;
}
