#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

CGS_ARRAY_DEFINE_STRUCT(int_array, int);

int int_cmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int* check_repeat_freq(struct cgs_rbt* freqs, int freq, int* found)
{
	struct cgs_variant var = { 0 };
	cgs_variant_set_int(&var, freq);

	if (cgs_rbt_search(freqs, &var)) {
		*found = freq;
		return found;
	}
	cgs_rbt_insert(freqs, &var);
	return NULL;
}

int main(void)
{
	printf("Advent of Code 2018 Day 1: Chronal Calibration\n");

	int part1 = 0;
	int part2 = 0;

	struct cgs_rbt* freqs = cgs_rbt_new(int_cmp);

	struct int_array shifts;
	cgs_array_init(&shifts);

	for (int n; scanf(" %d", &n) == 1; ) {
		cgs_array_push(&shifts, n);
		part1 += n;
		if (!part2)
			check_repeat_freq(freqs, part1, &part2);
	}

	for (int freq = part1; 1; ) {
		for (size_t i = 0; i < shifts.len; ++i) {
			freq += cgs_array_get(&shifts, i);
			if (check_repeat_freq(freqs, freq, &part2))
				goto getout;
		}
	}
getout:
	cgs_array_free(&shifts);
	cgs_rbt_free(freqs);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

