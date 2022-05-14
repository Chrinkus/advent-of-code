#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <cgs/cgs.h>
#include <fruity.h>

struct point {
        int x, y;
};

struct input_data {
	struct cgs_array* coords;
	struct point mins;
	struct point maxs;
};

void free_input(struct input_data* input)
{
        cgs_array_free(input->coords);
}

void* read_input(struct input_data* pi)
{
	struct cgs_array* coords = cgs_array_new(struct point);
        if (!coords)
                return NULL;

	struct point mins = { .x = INT_MAX, .y = INT_MAX };
	struct point maxs = { 0 };
	for (struct point pt; scanf("%d, %d", &pt.x, &pt.y) == 2; ) {
		cgs_array_push(coords, &pt);
		mins.x = CGS_MIN(mins.x, pt.x);
		mins.y = CGS_MIN(mins.y, pt.y);
		maxs.x = CGS_MAX(maxs.x, pt.x);
		maxs.y = CGS_MAX(maxs.y, pt.y);
	}

        pi->coords = coords;
        pi->mins = mins;
        pi->maxs = maxs;

        return (void*)coords;
}

struct location {
	char area_id;
	int distance;
};

struct grid {
        Fruity2D grid;
        int roff;
        int coff;
};

void free_grid(struct grid* grid)
{
        fruity_free(&grid->grid);
}

void* init_grid(struct grid* g, const struct input_data* input)
{
        int r = input->maxs.y - input->mins.y + 1;
        int c = input->maxs.x - input->mins.x + 1;

        void* res = fruity_new(&g->grid, r, c, sizeof(struct location));
        if (!res)
                return NULL;

        struct location init = { .area_id = '.', .distance = INT_MAX };
        fruity_init(&g->grid, &init, sizeof(init));

        g->roff = input->mins.y;
        g->coff = input->mins.x;

        return res;
}

int main(void)
{
	printf("Advent of Code 2018 Day 6: Chronal Coordinates\n");

        struct input_data input = { 0 };
        if (!read_input(&input))
                return EXIT_FAILURE;

        struct grid grid = { 0 };
        if (!init_grid(&grid, &input))
                return EXIT_FAILURE;

	int part1 = 0;
	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	free_input(&input);
	free_grid(&grid);

	return EXIT_SUCCESS;
}

