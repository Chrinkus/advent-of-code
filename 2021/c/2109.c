#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "aoc_banner.h"
#include <sxc_vector.h>
#include <sxc_vector_types.h>

enum { SENT = 10, COUNTED = -1, HIGH = 9 };

struct I8_vector {
	size_t siz;
	size_t cap;
	int8_t* vec;
};

struct Cave {
	int8_t* map;
	int rows;
	int cols;
};

int find_low_point_sums(const struct Cave* cave)
{
	int risk = 0;
	const int8_t* p = cave->map;
	int cols = cave->cols;
	int rows = cave->rows;

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j) {
			int cur = p[i * cols + j];
			int n = i > 0 ? p[(i-1) * cols + j] : SENT;
			int s = i < rows-1 ? p[(i+1) * cols + j] : SENT;
			int e = j < cols-1 ? p[i * cols + j+1] : SENT;
			int w = j > 0 ? p[i * cols + j-1] : SENT;

			if (cur < n && cur < s && cur < e && cur < w) {
				risk += cur + 1;	// risk = height + 1
			}
		}
	return risk;
}

int int_rev_cmp(const void* a, const void* b)
{
	const int* v1 = (const int*)a;
	const int* v2 = (const int*)b;
	return *v2 - *v1;
}

int mark_basin(struct Cave* cave, int x, int y)
{
	if (x < 0 || cave->cols <= x || y < 0 || cave->rows <= y)
		return 0;

	int8_t* cur = &cave->map[y * cave->cols + x];
	if (*cur == COUNTED || *cur == HIGH)
		return 0;

	*cur = COUNTED;
	int sum = 1;	// this one

	sum += mark_basin(cave, x, y-1);	// north
	sum += mark_basin(cave, x, y+1);	// south
	sum += mark_basin(cave, x-1, y);	// west
	sum += mark_basin(cave, x+1, y);	// east
	return sum;
}

int multiply_basins(struct Cave* cave)
{
	int ret = 1;

	Int_vector basins;
	sxc_vector_init(&basins);

	for (int i = 0; i < cave->rows; ++i) {
		for (int j = 0; j < cave->cols; ++j) {
			int cur = cave->map[i * cave->cols + j];
			if (COUNTED < cur && cur < HIGH) {
				int basin = mark_basin(cave, j, i);
				sxc_vector_push(&basins, basin);
			}
		}
	}
	sxc_vector_sort(&basins, int_rev_cmp);
	for (size_t i = 0; i < 3; ++i)
		ret *= sxc_vector_get(&basins, i);

	sxc_vector_free(&basins);
	return ret;
}

int main()
{
	aoc_banner_2021("09", "Smoke Basin");

	struct Cave cave = { .map = NULL, .rows = 0, .cols = 0 };

	struct I8_vector input;
	sxc_vector_init(&input);
	for (int n = 0, c; (c = getchar()) != EOF; ++n) {
		if (isdigit(c))
			sxc_vector_push(&input, (c - '0'));
		else if (!cave.cols && (c == '\n'))
			cave.cols = n;
	}
	cave.rows = sxc_vector_size(&input) / cave.cols;
	sxc_vector_xfer(&input, cave.map);	// cave manages memory now

	int part1 = find_low_point_sums(&cave);
	int part2 = multiply_basins(&cave);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	free(cave.map);

	return EXIT_SUCCESS;
}

