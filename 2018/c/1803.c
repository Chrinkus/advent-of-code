#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

enum { MAX_SIDE = 1000 };

struct claim {
	int id, x, y, w, h;
};

int count_multi_claims(const int arr[][MAX_SIDE])
{
	int count = 0;
	for (int i = 0; i < MAX_SIDE; ++i)
		for (int j = 0; j < MAX_SIDE; ++j)
			count += arr[i][j] > 1;
	return count;
}

int find_solo_claim(const struct cgs_array* claims, const int fab[][MAX_SIDE])
{
	for (size_t i = 0; i < cgs_array_length(claims); ++i) {
		const struct claim* p = cgs_array_get(claims, i);
		for (int j = p->y; j < p->y + p->h; ++j)
			for (int k = p->x; k < p->x + p->w; ++k)
				if (fab[j][k] > 1)
					goto next_claim;
		return p->id;
next_claim:
	}
	return -1;
}

int main(void)
{
	printf("Advent of Code 2018 Day 3: No Matter How You Slice It\n");

	int fabric[MAX_SIDE][MAX_SIDE] = { 0 };
	struct cgs_array* claims = cgs_array_new(struct claim);

	for (struct claim c; scanf("#%d @ %d,%d: %dx%d ",
				&c.id, &c.x, &c.y, &c.w, &c.h) == 5; ) {
		cgs_array_push(claims, &c);
		for (int i = c.y; i < c.y + c.h; ++i)
			for (int j = c.x; j < c.x + c.w; ++j)
				++fabric[i][j];
	}

	int part1 = count_multi_claims(fabric);
	int part2 = find_solo_claim(claims, fabric);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	cgs_array_free(claims);

	return EXIT_SUCCESS;
}

