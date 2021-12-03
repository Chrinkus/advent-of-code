#include <stdlib.h>
#include <stdio.h>

#include <sxc_string.h>

enum { NUM_SLOPES = 5, TREE = '#', SNOW = '.', LINE = '\n' };

struct Hill {
	String data;
	int w;
	int h;
};

struct Slope {
	int dx;
	int dy;
};

const struct Slope slopes[NUM_SLOPES] = {
	{ 3, 1 },	// swapped to first position to single out part 1
	{ 1, 1 },
	{ 5, 1 },
	{ 7, 1 },
	{ 1, 2 },
};

int trees_per_slope(const struct Hill* hill, const struct Slope* m)
{
	int trees = 0;

	const char* p = sxc_string_str(&hill->data);
	for (int x = 0, y = 0; y < hill->h; x += m->dx, y += m->dy)
		trees += p[y * hill->w + (x % hill->w)] == TREE;

	return trees;
}

int main()
{
	printf("Advent of Code 2020\n");
	printf("Day 3: Toboggan Trajectory\n");

	struct Hill hill = { .w = 0, .h = 0 };
	sxc_string_init(&hill.data);
	for (int i = 0, c; (c = getchar()) != EOF; ++i)
		switch (c) {
		case TREE:
		case SNOW:
			sxc_string_push(&hill.data, c);
			break;
		case LINE:
			if (!hill.w) hill.w = i;
			++hill.h;
			break;
		}

	int part1 = trees_per_slope(&hill, &slopes[0]);
	int part2 = part1;
	for (int i = 1; i < NUM_SLOPES; ++i)
		part2 *= trees_per_slope(&hill, &slopes[i]);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	sxc_string_free(&hill.data);

	return EXIT_SUCCESS;
}

