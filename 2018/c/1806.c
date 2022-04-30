#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <cgs/cgs.h>
#include <fruity.h>

// Add to libfruity
struct fruity_pt {
	int x, y;
};

int fruity_get_adjacents(Fruity2DMutable arr, int rows, int cols,
		struct fruity_pt* loc, void* out[4])
{
	int count = 0;
	const int x = loc->x;
	const int y = loc->y;

	if (x - 1 >= 0)
		out[count++] = &arr[y][x-1];
	if (x + 1 < cols)
		out[count++] = &arr[y][x+1];
	if (y - 1 >= 0)
		out[count++] = &arr[y-1][x];
	if (y + 1 < rows)
		out[count++] = &arr[y+1][x];

	return count;
}

// This solution
struct input_data {
	struct cgs_array* coords;
	struct fruity_pt mins;
	struct fruity_pt maxs;
};

struct input_data read_input(void)
{
	struct cgs_array* coords = cgs_array_new(struct fruity_pt);

	struct fruity_pt mins = { .x = INT_MAX, .y = INT_MAX };
	struct fruity_pt maxs = { 0 };
	for (struct fruity_pt pt; scanf("%d, %d", &pt.x, &pt.y) == 2; ) {
		cgs_array_push(coords, &pt);
		mins.x = CGS_MIN(mins.x, pt.x);
		mins.y = CGS_MIN(mins.y, pt.y);
		maxs.x = CGS_MAX(maxs.x, pt.x);
		maxs.y = CGS_MAX(maxs.y, pt.y);
	}

	return (struct input_data){
		.coords = coords,
		.mins = mins,
		.maxs = maxs,
	};
}

struct location {
	char area_id;
	int distance;
};

struct grid {
	struct location** grid;
	int rows;
	int cols;
	int roff;	// row offset
	int coff;	// column offset
};

void location_init(Fruity2DMutable arr, int row, int col, void* data)
{
	struct location** al = (struct location**)arr;
	(void)data;

	al[row][col] = (struct location){
		.area_id = '.',
		.distance = INT_MAX,
	};
}

struct grid create_grid(const struct input_data* data)
{
	int rows = data->maxs.y - data->mins.y + 1;
	int cols = data->maxs.x - data->mins.x + 1;

	struct location** grid = NULL;
	fruity_new(struct location, rows, cols, grid);
	fruity_transform(fruity_cast_mutable(grid), rows, cols,
			NULL, location_init, NULL);

	return (struct grid){
		.grid = grid,
		.rows = rows,
		.cols = cols,
		.roff = data->mins.y,
		.coff = data->mins.x,
	};
}

struct claim {
	char claim_id;
	int distance;
	struct fruity_pt location;
};

void mark_territories(struct grid* grid, const struct input_data* input)
{
	// 
}

int main(void)
{
	printf("Advent of Code 2018 Day 6: Chronal Coordinates\n");

	struct input_data input = read_input();
	struct grid grid = create_grid(&input);

	mark_territories(&grid, &input);

	int part1 = 0;
	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	cgs_array_free(input.coords);
	fruity_free(grid.grid);

	return EXIT_SUCCESS;
}

