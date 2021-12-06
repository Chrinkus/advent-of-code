#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "aoc_banner.h"

enum {
	SCHOOL_LEN = 7,
	SPAWN_LEN = 9,
	PART1_DAYS = 80,
	PART2_DAYS = 256
};

void advance_time(int64_t school[], int64_t spawn[], int days, int* day)
{
	for ( ; *day < days; ++*day) {
		int64_t tmp = spawn[*day % SPAWN_LEN];
		spawn[*day % SPAWN_LEN] += school[*day % SCHOOL_LEN];
		school[*day % SCHOOL_LEN] += tmp;
	}
}

int64_t sum_array(int64_t a[], int len)
{
	int64_t sum = 0;
	for (int i = 0; i < len; ++i)
		sum += a[i];
	return sum;
}

int64_t tally_fish(int64_t school[], int64_t spawn[])
{
	return sum_array(school, SCHOOL_LEN) + sum_array(spawn, SPAWN_LEN);
}

int main()
{
	aoc_banner_2021("06", "Lanternfish");

	int day = 0;
	int64_t school[SCHOOL_LEN] = { 0 };
	int64_t spawn[SPAWN_LEN] = { 0 };

	// Read input
	int size = 0, alloc = 8;
	int* input = malloc(sizeof(int) * alloc);
	for (int n; scanf("%d", &n) == 1; getchar()) {
		if (size == alloc) {
			alloc *= 2;
			input = realloc(input, sizeof(int) * alloc);
		}
		input[size++] = n;
	}

	// Set initial state
	for (int i = 0; i < size; ++i)
		++school[input[i]];
	free(input);

	advance_time(school, spawn, PART1_DAYS, &day);
	int64_t part1 = tally_fish(school, spawn);

	advance_time(school, spawn, PART2_DAYS, &day);
	int64_t part2 = tally_fish(school, spawn);

	aoc_report_ints(part1, part2);

	return EXIT_SUCCESS;
}

