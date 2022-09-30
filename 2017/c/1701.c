#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

enum magic { ZERO_CHAR = '0' };

const void*
read_input(struct cgs_array* input)
{
        for (int c; (c = getchar()) != EOF && c != '\n'; ) {
                c -= ZERO_CHAR ;
                if (!cgs_array_push(input, &c))
                        return cgs_error_retnull("cgs_array_push");
        }
        return input;
}

int
next_match_sum(const int* arr, const size_t len)
{
        int sum = 0, prev = 0;
        for (size_t i = 0; i < len; prev = arr[i], ++i)
                if (arr[i] == prev)
                        sum += prev;

        if (arr[0] == arr[len-1])
                sum += arr[0];
        return sum;
}

int
half_match_sum(const int* arr, const size_t len)
{
        int sum = 0;
        for (size_t i = 0, j = len / 2; j < len; ++i, ++j)
                if (arr[i] == arr[j])
                        sum += arr[i] * 2;

        return sum;
}

int main(void)
{
        struct cgs_array input = { 0 };
        if (!cgs_array_new(&input, sizeof(int)))
                return cgs_error_retfail("cgs_array_new");

        if (!read_input(&input))
                return cgs_error_retfail("read_input");

        int part1 = next_match_sum(cgs_array_data(&input),
                        cgs_array_length(&input));

        int part2 = half_match_sum(cgs_array_data(&input),
                        cgs_array_length(&input));

        printf("%d\n", part1);
        printf("%d\n", part2);

        return EXIT_SUCCESS;
}
