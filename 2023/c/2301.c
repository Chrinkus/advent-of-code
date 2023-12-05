#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum {
        BUFFMAX = 128,
        TENS_1 = 1,
        TENS_2 = 1 << 1,
        ONES_1 = 1 << 2,
        ONES_2 = 1 << 3,
};

enum digits {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        NUM_DIGITS,
};

const char* const digits[NUM_DIGITS] = {
        [ZERO]  = "zero",
        [ONE]   = "one",
        [TWO]   = "two",
        [THREE] = "three",
        [FOUR]  = "four",
        [FIVE]  = "five",
        [SIX]   = "six",
        [SEVEN] = "seven",
        [EIGHT] = "eight",
        [NINE]  = "nine",
};

static int
get_spelled_num(const char* s)
{
        for (int i = ZERO; i < NUM_DIGITS; ++i)
                if (strncmp(digits[i], s, strlen(digits[i])) == 0)
                        return i;
        return -1;
}

static void
check_and_flag(const char* s, int* n1, int* n2, int* flags, int f1, int f2,
                const int mul)
{
        // Part 1
        int tmp = isdigit(s[0]) ? s[0] - '0' : -1;

        if (tmp >= 0 && (*flags & f1) == 0) {
                *n1 += mul * tmp;
                *flags |= f1;
        }

        // Part 2
        if (tmp < 0)
                tmp = get_spelled_num(s);

        if (tmp >= 0 && (*flags & f2) == 0) {
                *n2 += mul * tmp;
                *flags |= f2;
        }
}

static void
get_calib_sum(int* p1, int* p2)
{
        char buff[BUFFMAX];

        while (fgets(buff, BUFFMAX, stdin)) {
                const int l = strlen(buff);
                int flags = 0;

                for (int i = 0; i < l; ++i) {
                        check_and_flag(&buff[i], p1, p2, &flags,
                                        TENS_1, TENS_2, 10);

                        if (flags & TENS_1 && flags & TENS_2)
                                break;
                }

                for (int i = l; i >= 0; --i) {
                        check_and_flag(&buff[i], p1, p2, &flags,
                                        ONES_1, ONES_2, 1);

                        if (flags & ONES_1 && flags & ONES_2)
                                break;
                }
        }
}

int main(void)
{
        int part1 = 0;
        int part2 = 0;

        get_calib_sum(&part1, &part2);

        printf("%d\n", part1);
        printf("%d\n", part2);

        return EXIT_SUCCESS;
}
