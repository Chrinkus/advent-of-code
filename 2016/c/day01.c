#include <stdlib.h>
#include <stdio.h>

#include "sxc_vector.h"

// Part 1
enum Facing { NORTH, EAST, SOUTH, WEST };

// Part 2
struct Point {
	int x;
	int y;
};

int point_cmp_eq(const struct Point* a, const struct Point* b)
{
	return (a->x == b->x) && (a->y == b->y);
}

struct Point_vector {
	size_t size;
	size_t cap;
	size_t elem_size;
	struct Point* vec;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 1: No Time for a Taxicab\n");

	struct Point_vector pv;
	sxc_vector_init(pv, struct Point);

	struct Point* hq = NULL;

	int x = 0;
	int y = 0;
	for (int c, n, f = NORTH; scanf("%c%d, ", (char*)&c, &n) == 2; ) {
		// Part 1
		switch (f) {
		case NORTH:	f = c == 'L' ? WEST : EAST;	break;
		case EAST:	f = c == 'L' ? NORTH : SOUTH;	break;
		case SOUTH:	f = c == 'L' ? EAST : WEST;	break;
		case WEST:	f = c == 'L' ? SOUTH : NORTH;	break;
		}

		if (!hq) {	// Part 2: only do till hq found
			while (n--) {
				switch (f) {
				case NORTH:	--y;	break;
				case EAST:	++x;	break;
				case SOUTH:	++y;	break;
				case WEST:	--x;	break;
				}

				struct Point pt = { x, y };
				sxc_vector_find(pv, &pt, point_cmp_eq, hq);
				sxc_vector_push(pv, pt);
			}
		} else {	// Part 1 continued..
			switch (f) {
			case NORTH:	y -= n;		break;
			case EAST:	x += n;		break;
			case SOUTH:	y += n;		break;
			case WEST:	x -= n;		break;
			}
		}
	}

	printf("Part 1: %d\n", abs(x) + abs(y));
	printf("Part 2: %d\n", abs(hq->x) + abs(hq->y));

	return EXIT_SUCCESS;
}

