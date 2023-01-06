#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"
#include "md5.h"

typedef int (*Predicate)(const struct md5_context*);

static int
starts_with_5_zeros(const struct md5_context* ctx)
{
        return ctx->digest[0] == 0 &&
                ctx->digest[1] == 0 &&
                ctx->digest[2] < 16;    // upper bits are zero < 16
}

static int
starts_with_6_zeros(const struct md5_context* ctx)
{
        for (int i = 0; i < 3; ++i)
                if (ctx->digest[i] != 0)
                        return 0;
        return 1;
}

static int
hash_until(const struct cgs_string* input, Predicate pred, int n)
{
        struct cgs_string main_buff = cgs_string_new();
        cgs_string_copy(input, &main_buff);
        struct cgs_string int_buff = cgs_string_new();

        for (struct md5_context ctx; 1; ++n) {
                cgs_string_from_int(n, &int_buff);
                cgs_string_cat(&main_buff, &int_buff);

                md5_from_string(cgs_string_data(&main_buff), &ctx);
                if (pred(&ctx))
                        break;

                cgs_string_trunc(&main_buff, cgs_string_length(input));
                cgs_string_clear(&int_buff);
        }

        cgs_string_free(&main_buff);
        cgs_string_free(&int_buff);
        return n;
}

int main(void)
{
        struct cgs_string input = cgs_string_new();
        cgs_io_getline(stdin, &input);

        int part1 = hash_until(&input, starts_with_5_zeros, 0);
        printf("%d\n", part1);

        int part2 = hash_until(&input, starts_with_6_zeros, part1 + 1);
        printf("%d\n", part2);

        cgs_string_free(&input);
        return EXIT_SUCCESS;
}
