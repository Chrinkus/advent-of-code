#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aoc_banner.h"

struct velocity {
	int dx;
	int dy;
};

struct rect {
	int x1;
	int y1;
	int x2;
	int y2;
};

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

struct rect* read_target(struct rect* r)
{
	if (scanf("target area: x=%d..%d, y=%d..%d ",
				&r->x1, &r->x2, &r->y1, &r->y2) != 4)
		return NULL;
	
	// Ensure top-left, bottom-right
	if (r->y2 > r->y1)	swap(&r->y2, &r->y1);
	if (r->x1 > r->x2)	swap(&r->x1, &r->x2);

	return r;
}

int hits_target(const struct rect* r, struct velocity v, int* apex)
{
	*apex = INT_MIN;
	for (int x = v.dx, y = v.dy; x <= r->x2 && y >= r->y2;
			x += v.dx, y += v.dy) {
		if (y > *apex) *apex = y;
		if (r->x1 <= x && x <= r->x2 && r->y1 >= y && y >= r->y2)
			return 1;
		--v.dy;
		if (v.dx > 0)
			--v.dx;
	}
	return 0;
}

int get_minx(const struct rect* r)
{
	int x = 1;
	for (int n; x < r->x1; ++x) {
		n = x * (x + 1) / 2;
		if (r->x1 <= n && n <= r->x2)
			break;
	}
	return x;
}

int get_maxy(const struct rect* r, int x, int* apex)
{
	struct velocity tmp = { .dx = x, .dy = r->y2 * -1 };
	while (!hits_target(r, tmp, apex))
		--tmp.dy;
	return tmp.dy;
}

int count_hits(const struct rect* r, const struct velocity* restrict mins,
		const struct velocity* restrict maxs)
{
	int count = 0;
	for (int i = mins->dy, n; i <= maxs->dy; ++i)
		for (int j = mins->dx; j <= maxs->dx; ++j) {
			struct velocity tmp = { .dx = j, .dy = i };
			if (hits_target(r, tmp, &n))
				++count;
		}
	return count;
}

int main()
{
	aoc_banner_2021("17", "Trick Shot");

	struct rect target = { 0 };
	if (!read_target(&target)) {
		fprintf(stderr, "Failed to read target\n");
		return EXIT_FAILURE;
	}

	struct velocity mins = {
		.dx = get_minx(&target),
		.dy = target.y2
	};

	int part1 = 0;
	struct velocity maxs = {
		.dx = target.x2,
		.dy = get_maxy(&target, mins.dx, &part1)
	};

	int part2 = count_hits(&target, &mins, &maxs);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	return EXIT_SUCCESS;
}

