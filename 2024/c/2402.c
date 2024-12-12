#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum constants { GAP = 3, };
enum direction { UP = 1, DOWN = -1, UNKNOWN = 0, };

struct counts {
        int all_safe;
        int dampened;
};

int
is_safe(int curr, int next, enum direction dir)
{
        if (curr == next)
                return CGS_FALSE;

        switch (dir) {
        case UP:
                return (next > curr) && (next - curr <= GAP);
        case DOWN:
                return (next < curr) && (curr - next <= GAP);
        default:
                return cgs_error_retbool("is_safe(): Unknown case %d\n", dir);
        }
}

struct cgs_vector*
parse_levels(struct cgs_vector* vi, const struct cgs_string* s)
{
        const char* p = cgs_string_data(s);
        for (const char* pp = p; *p; p = pp) {
                int val = cgs_strtoi(p, &pp);
                if (!cgs_vector_push(vi, &val))
                        return NULL;
        }
        return vi;
}

int
is_safe_report(const struct cgs_vector* vi)
{
        enum direction dir = UNKNOWN;

        for (size_t i = 0, j = 1; j < cgs_vector_length(vi); ++i, ++j) {
                const int* a = cgs_vector_get(vi, i);
                const int* b = cgs_vector_get(vi, j);

                if (dir == UNKNOWN)
                        dir = *a < *b ? UP : DOWN;

                if (!is_safe(*a, *b, dir))
                        return CGS_FALSE;
        }
        return CGS_TRUE;
}

struct cgs_vector*
copy_with_skip(const struct cgs_vector* src, struct cgs_vector* dst,
                size_t skip)
{
        for (size_t i = 0; i < cgs_vector_length(src); ++i) {
                if (i == skip)
                        continue;
                const int* pi = cgs_vector_get(src, i);
                if (!cgs_vector_push(dst, pi))
                        return cgs_error_retnull("copy_with_skip(): push\n");
        }
        return dst;
}

int
is_dampened_safe(const struct cgs_vector* vi)
{
        struct cgs_vector vskip = cgs_vector_new(sizeof(int));

        for (size_t i = 0; i < cgs_vector_length(vi); ++i) {
                if (!copy_with_skip(vi, &vskip, i))
                        return cgs_error_retbool("is_dampened_safe(): copy\n");

                if (is_safe_report(&vskip))
                        goto safe_cleanup;

                cgs_vector_clear(&vskip);
        }

        cgs_vector_free(&vskip);
        return CGS_FALSE;
safe_cleanup:
        cgs_vector_free(&vskip);
        return CGS_TRUE;
}

struct counts*
count_safe_reports(struct counts* counts)
{
        struct cgs_string buff = cgs_string_new();
        struct cgs_vector vi = cgs_vector_new(sizeof(int));

        while (cgs_io_getline(stdin, &buff) > 0) {
                if (!parse_levels(&vi, &buff))
                        goto error_cleanup;

                if (is_safe_report(&vi))
                        counts->all_safe += 1;
                else if (is_dampened_safe(&vi))
                        counts->dampened += 1;

                cgs_string_clear(&buff);
                cgs_vector_clear(&vi);
        }
        cgs_string_free(&buff);
        cgs_vector_free(&vi);
        return counts;

error_cleanup:
        cgs_string_free(&buff);
        cgs_vector_free(&vi);
        return NULL;
}

int
main(void)
{
        struct counts counts = { 0 };
        if (!count_safe_reports(&counts))
                return cgs_error_retfail("count_safe_reports(): error\n");

        printf("Part 1: %d\n", counts.all_safe);
        printf("Part 2: %d\n", counts.all_safe + counts.dampened);

        return EXIT_SUCCESS;
}
