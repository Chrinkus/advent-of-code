#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aoc_banner.h"
#include <sxc_vector.h>

enum {
	ZERO = 6, ONE = 2, TWO = 5, THREE = 5, FOUR = 4,
	FIVE = 5, SIX = 6, SEVEN = 3, EIGHT = 7, NINE = 6,
	NUM_SEGMENTS = 7,
	SEGBUFF = NUM_SEGMENTS + 1,
	NUM_PATTERNS = 10,
	NUM_OUTPUTS = 4,
};

struct Entry {
	char pats[NUM_PATTERNS][SEGBUFF];
	char outs[NUM_OUTPUTS][SEGBUFF];
};

struct Entry_vector {
	size_t siz;
	size_t cap;
	struct Entry* vec;
};

int count_unique_output(struct Entry_vector* v)
{
	int count = 0;
	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		struct Entry* p = sxc_vector_getp(v, i);
		for (int j = 0; j < NUM_OUTPUTS; ++j) {
			switch (strlen(p->outs[j])) {
			case ONE:
			case FOUR:
			case SEVEN:
			case EIGHT:
				++count;
				break;
			}
		}
	}
	return count;
}

int main()
{
	aoc_banner_2021("08", "Seven Segment Search");

	struct Entry_vector input;
	sxc_vector_init(&input);
	while (!feof(stdin)) {
		struct Entry* p;
		sxc_vector_emplace(&input, p);
		for (int i = 0; i < NUM_PATTERNS &&
				scanf("%s ", p->pats[i]) == 1; ++i)
			;
		getchar();	// burn '|'

		for (int i = 0; i < NUM_OUTPUTS &&
				scanf("%s ", p->outs[i]) == 1; ++i)
			;
	}

	int part1 = count_unique_output(&input);
	int part2 = 0;

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	sxc_vector_free(&input);

	return EXIT_SUCCESS;
}

