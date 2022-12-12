#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

void*
read_elf_calories(struct cgs_vector* v)
{
        struct cgs_string buff = cgs_string_new();
        while (!feof(stdin)) {
                int elf = 0;
                for (int n = 0; (n = cgs_io_getline(stdin, &buff)) > 0; ) {
                        if (n < 0)
                                return cgs_error_retnull("cgs_io_getline");

                        elf += strtol(cgs_string_data(&buff), NULL, 10);
                        cgs_string_clear(&buff);
                }
                if (!cgs_vector_push(v, &elf))
                        return cgs_error_retnull("cgs_vector_push");
        }
        cgs_string_free(&buff);
        return v;
}

int
get_max_elf(const struct cgs_vector* v)
{
        return *((const int*)cgs_vector_begin(v));
}

int
top_three_elves(const struct cgs_vector* v)
{
        int sum = 0;
        for (const int* p = cgs_vector_begin(v), *q = p + 3; p != q; ++p)
                sum += *p;

        return sum;
}

int main(void)
{
        struct cgs_vector elves = cgs_vector_new(sizeof(int));
        if (!read_elf_calories(&elves))
                return cgs_error_retfail("read_elf_calories");

        cgs_vector_sort(&elves, cgs_int_cmp_rev);

        const int part1 = get_max_elf(&elves);
        printf("%d\n", part1);

        const int part2 = top_three_elves(&elves);
        printf("%d\n", part2);

        cgs_vector_free(&elves);
        return EXIT_SUCCESS;
}
