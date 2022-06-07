#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

enum flags {
        FIRST = 1U,
        TWICE = 1U << 1,
        BOTH = FIRST | TWICE,
};

void* read_input(struct cgs_array* shifts)
{
        for (int n; scanf(" %d", &n) == 1; )
                if (!cgs_array_push(shifts, &n))
                        return NULL;
        return shifts;
}

void* check_freq(struct cgs_rbt* seen, const int freq, size_t* flags, int* out)
{
        struct cgs_variant v = { 0 };
        cgs_variant_set_int(&v, freq);
        if (cgs_rbt_search(seen, &v)) {
                *out = freq;
                *flags |= TWICE;
        } else {
                if (!cgs_rbt_insert(seen, &v))
                        return NULL;
        }
        return out;
}

void* run_shifts(const struct cgs_array* input, int* p1, int* p2)
{
        const int* shifts = cgs_array_data(input);
        const size_t len = cgs_array_length(input);

        struct cgs_rbt seen = { 0 };
        cgs_rbt_new(&seen, cgs_int_cmp);

        int freq = 0;
        for (size_t i = 0, flags = 0; flags ^ BOTH; i = (i + 1) % len) {
                freq += shifts[i];
                if (flags ^ TWICE && !check_freq(&seen, freq, &flags, p2))
                        return NULL;

                if (i == len - 1 && flags ^ FIRST) {
                        *p1 = freq;
                        flags |= FIRST;
                }
        }

        cgs_rbt_free(&seen);
        return p1;
}

int main(void)
{
	printf("Advent of Code 2018 Day 1: Chronal Calibration\n");

        struct cgs_array input = { 0 };
        if (!cgs_array_new(&input, sizeof(int)))
                return EXIT_FAILURE;

        if (!read_input(&input))
                return EXIT_FAILURE;

	int part1 = 0;
	int part2 = 0;
        if (!run_shifts(&input, &part1, &part2))
                return EXIT_FAILURE;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

        cgs_array_free(&input);

	return EXIT_SUCCESS;
}

