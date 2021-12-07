#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sxc_vector.h>
#include <sxc_vector_types.h>

#include "aoc_banner.h"

int fuel_hike(int steps)
{
	int fuel = 0;
	for (int n = 1; steps--; ++n)
		fuel += n;
	return fuel;
}

int get_min(int* arr, int len)
{
	int min = INT_MAX;
	for (int i = 0; i < len; ++i)
		if (arr[i] < min)
			min = arr[i];
	return min;
}

void find_lowest_cost_positions(const struct Int_vector* v, int max,
		int* p1, int* p2)
{
	int* costs1 = calloc(max, sizeof(int));
	int* costs2 = calloc(max, sizeof(int));

	for (int i = 0; i < max; ++i)
		for (size_t j = 0; j < sxc_vector_size(v); ++j) {
			const int steps = abs(i - sxc_vector_get(v, j));
			costs1[i] += steps;
			costs2[i] += fuel_hike(steps);
		}

	*p1 = get_min(costs1, max);
	*p2 = get_min(costs2, max);

	free(costs1);
	free(costs2);
}

int main()
{
	aoc_banner_2021("07", "The Treacheray of Whales");

	struct Int_vector input;
	sxc_vector_init(&input);

	int max = 0;
	for (int n; scanf("%d,", &n) == 1; ) {
		sxc_vector_push(&input, n);
		if (n > max)
			max = n;
	}

	int part1 = 0;
	int part2 = 0;
	find_lowest_cost_positions(&input, max+1, &part1, &part2);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	sxc_vector_free(&input);

	return EXIT_SUCCESS;
}

