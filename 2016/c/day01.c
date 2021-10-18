#include <stdlib.h>
#include <stdio.h>

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

struct Point_vec {
	size_t size;
	size_t cap;
	size_t elem_size;
	struct Point* vec;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// generic vector stuff
enum Vector_scalars {
	VECTOR_INITIAL_CAP = 8,
	VECTOR_GROWTH_RATE = 2
};

#define vector_init(v, type)						\
	do {								\
		(v).size = 0;						\
		(v).cap = VECTOR_INITIAL_CAP;				\
		(v).elem_size = sizeof(type);				\
		(v).vec = malloc((v).elem_size * (v).cap);		\
	} while (0)

#define vector_grow(v)							\
	do {								\
		size_t new_cap = (v).cap * VECTOR_GROWTH_RATE;		\
		(v).vec = realloc((v).vec, (v).elem_size * new_cap);	\
		(v).cap = new_cap;					\
	} while (0)

#define vector_place(v, val)						\
	do {								\
		(v).vec[(v).size++] = (val);				\
	} while (0)			

#define vector_push(v, val)						\
	do {								\
		if ((v).size == (v).cap)				\
			vector_grow((v));				\
		vector_place((v), (val));				\
	} while (0)

#define vector_find(v, tar, cmp, found)					\
	do {								\
		for (int i = 0; i < (v).size; ++i) {			\
			if (cmp( &((v).vec[i]), tar)) {			\
				found = &((v).vec[i]);			\
				break;					\
			}						\
		}							\
	} while (0)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 1: No Time for a Taxicab\n");

	struct Point_vec pv;
	vector_init(pv, struct Point);

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
				vector_find(pv, &pt, point_cmp_eq, hq);
				vector_push(pv, pt);	// last element is hq
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

