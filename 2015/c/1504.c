#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"
#include "md5.h"

static int
starts_with_5_zeros(const char* s)
{
        for (int i = 0; *s && i < 5; ++i, ++s)
                if (*s != '0')
                        return 0;
        return 1;
}

static unsigned
hash_until_5_zeros(const struct cgs_string* input, unsigned n)
{
        struct cgs_string s = cgs_string_new();
        cgs_string_copy(input, &s);

        for (struct md5_context ctx; 1; ++n) {
                char buff[16];
                sprintf(buff, "%u", n);
                cgs_string_cat_str(&s, buff, strlen(buff));

                md5_from_string(cgs_string_data(&s), &ctx);

                char hash[MD5_STRING_LEN];
                md5_sprint(&ctx, hash);
                if (starts_with_5_zeros(hash))
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

        unsigned part1 = hash_until_5_zeros(&input, 0);

        printf("%u\n", part1);

        cgs_string_free(&input);
        return EXIT_SUCCESS;
}
