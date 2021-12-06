#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include "aoc_banner.h"

#include <sxc_vector.h>
#include <sxc_vector_types.h>

enum {
	RESET = 6, NEWFISH = 8,
	PART_1_DAYS = 256,
	PART_2_DAYS = 256 - PART_1_DAYS,
};

struct U8_vector {
	size_t siz;
	size_t cap;
	uint8_t* vec;
};

void age_fish(struct U8_vector* school, int days)
{
	struct U8_vector spawned;
	sxc_vector_init(&spawned);

	int64_t prev = 0;
	for (int j = 0; j < days; ++j) {
		int64_t start = sxc_vector_size(school);
		for (size_t i = 0; i < sxc_vector_size(school); ++i) {
			uint8_t* fish = sxc_vector_getp(school, i);
			--(*fish);
			if (*fish > NEWFISH) {
				*fish = RESET;
				sxc_vector_push(&spawned, NEWFISH);
			}
		}
		for (size_t i = 0; i < sxc_vector_size(&spawned); ++i)
			sxc_vector_push(school, sxc_vector_get(&spawned, i));

		int64_t end = sxc_vector_size(school);
		int64_t growth = end - start;
		printf("Day: %d, Growth: %lld, Increase: %lld\n",
				j, growth, growth - prev);
		prev = growth;
		spawned.siz = 0;
	}

	sxc_vector_free(&spawned);
}

int main()
{
	aoc_banner_2021("06", "Lanternfish");

	struct U8_vector school;
	sxc_vector_init(&school);
	for (int n, c = ','; c != '\n' && scanf("%d", &n) == 1; c = getchar())
		sxc_vector_push(&school, n);

	age_fish(&school, PART_1_DAYS);
	size_t part1 = sxc_vector_size(&school);

	/* Too long!
	age_fish(&school, PART_2_DAYS);
	size_t part2 = sxc_vector_size(&school);
	*/
	size_t part2 = PART_2_DAYS;

	aoc_report_sizes(part1, part2);

	sxc_vector_free(&school);

	return EXIT_SUCCESS;
}

