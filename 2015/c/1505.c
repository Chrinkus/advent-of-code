#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

static int
is_nice_1(const char* s)
{
        int vowels = 0;
        int repeat = 0;
        for ( ; *s; ++s) {
                // condition 1
                switch (*s) {
                case 'a':       /* fallthrough */
                case 'e':       /* fallthrough */
                case 'i':       /* fallthrough */
                case 'o':       /* fallthrough */
                case 'u':       ++vowels;               break;
                }

                // condition 2
                const char* t = s + 1;
                if (!repeat && *s == *t)
                        repeat = 1;

                // condition 3
                switch (*s) {
                case 'a':       if (*t == 'b')  return 0;       break;
                case 'c':       if (*t == 'd')  return 0;       break;
                case 'p':       if (*t == 'q')  return 0;       break;
                case 'x':       if (*t == 'y')  return 0;       break;
                }
        }
        return vowels >= 3 && repeat;
}

static int
is_nice_2(const char* s)
{
        int pairs = 0;
        int straddle = 0;
        for (const char* t = s + 2; *t; ++s, ++t) {
                if (!straddle && *s == *t)
                        straddle = 1;
                for (const char* u = t; !pairs && *u; ++u)
                        if (*s == *u && *(s+1) == *(u+1))
                                pairs = 1;
        }

        return pairs && straddle;
}

int main(void)
{
        int nice_1 = 0;
        int nice_2 = 0;

        struct cgs_string line = cgs_string_new();
        for ( ; cgs_io_getline(stdin, &line); cgs_string_clear(&line)) {
                nice_1 += is_nice_1(cgs_string_data(&line));
                nice_2 += is_nice_2(cgs_string_data(&line));
        }

        printf("%d\n", nice_1);
        printf("%d\n", nice_2);

        cgs_string_free(&line);

        return EXIT_SUCCESS;
}
