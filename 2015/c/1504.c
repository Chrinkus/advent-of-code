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
                ctx->digest[2] < 16;
}

static int
starts_with_6_zeros(const struct md5_context* ctx)
{
        for (int i = 0; i < 3; ++i)
                if (ctx->digest[i] != 0)
                        return 0;
        return 1;
}

static unsigned
hash_until(const struct cgs_string* input, Predicate pred, unsigned n)
{
        struct cgs_string s = cgs_string_new();
        cgs_string_copy(input, &s);

        for (struct md5_context ctx; 1; ++n) {
                char buff[16];
                sprintf(buff, "%u", n);
                cgs_string_cat_str(&s, buff, strlen(buff));

                md5_from_string(cgs_string_data(&s), &ctx);

                if (pred(&ctx))
                        break;

                cgs_string_trunc(&s, cgs_string_length(input));
        }

        cgs_string_free(&s);
        return n;
}

int main(void)
{
        struct cgs_string input = cgs_string_new();
        cgs_io_getline(stdin, &input);

        unsigned part1 = hash_until(&input, starts_with_5_zeros, 0);
        printf("%u\n", part1);

        unsigned part2 = hash_until(&input, starts_with_6_zeros, part1 + 1);
        printf("%u\n", part2);

        cgs_string_free(&input);
        return EXIT_SUCCESS;
}
