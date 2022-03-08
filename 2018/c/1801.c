#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

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

	struct cgs_rbt* freqs = cgs_rbt_new(cgs_int_cmp);
	struct cgs_array* shifts = cgs_array_new(int);

	for (int n; scanf(" %d", &n) == 1; ) {
		cgs_array_push(shifts, &n);
		part1 += n;
		if (!part2)
			check_repeat_freq(freqs, part1, &part2);
	}

	size_t len;
	int* ai = cgs_array_xfer(shifts, &len);
	for (int i = 0, freq = part1; 1; i = (i+1) % len) {
		freq += ai[i];
		if (check_repeat_freq(freqs, freq, &part2))
			goto getout;
	}
getout:
	free(ai);
	cgs_rbt_free(freqs);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

