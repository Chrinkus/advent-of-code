#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <sxc_vector.h>
#include <sxc_vector_types.h>

#include "aoc_banner.h"

enum {
	ADULTS_LEN = 7,
	BABIES_LEN = 9,
	PART1_DAYS = 80,
	PART2_DAYS = 256
};

// Globals just to see how it feels
int64_t adults[ADULTS_LEN];
int64_t babies[BABIES_LEN];

void advance_time(int days, int* day)
{
	for ( ; *day < days; ++*day) {
		int64_t tmp = babies[*day % BABIES_LEN];
		babies[*day % BABIES_LEN] += adults[*day % ADULTS_LEN];
		adults[*day % ADULTS_LEN] += tmp;
	}
}

int64_t sum_array(int64_t a[], int len)
{
	int64_t sum = 0;
	for (int i = 0; i < len; ++i)
		sum += a[i];
	return sum;
}

int64_t tally_fish()
{
	return sum_array(adults, ADULTS_LEN) + sum_array(babies, BABIES_LEN);
}

int main()
{
	aoc_banner_2021("06", "Lanternfish");

	// Read input
	struct Int_vector input;
	sxc_vector_init(&input);
	for (int n; scanf("%d", &n) == 1; getchar())
		sxc_vector_push(&input, n);

	// Set initial state
	for (size_t i = 0; i < sxc_vector_size(&input); ++i)
		++adults[sxc_vector_get(&input, i)];
	sxc_vector_free(&input);

	int day = 0;
	advance_time(PART1_DAYS, &day);
	int64_t part1 = tally_fish();

	advance_time(PART2_DAYS, &day);
	int64_t part2 = tally_fish();

	aoc_report_ints(part1, part2);

	return EXIT_SUCCESS;
}
// Globals feel dirty..

