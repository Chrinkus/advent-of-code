#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>
#include <fruity.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum { MAX_SIDE = 1000 };

struct claim {
        int id, x, y, w, h;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* read_input(struct cgs_array* input)
{
        for (struct claim c; scanf("#%d @ %d,%d: %dx%d ",
                                &c.id, &c.x, &c.y, &c.w, &c.h) == 5; )
                if (!cgs_array_push(input, &c))
                        return NULL;
        return input;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Mark claims on fabric and count overlapping squares
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void increment_claim(const void* element, size_t i, void* data)
{
        const struct claim* c = element;
        int** fabric = data;
        (void)i;

        for (int i = c->y, h = c->y + c->h; i < h; ++i)
                for (int j = c->x, w = c->x + c->w; j < w; ++j)
                        ++fabric[i][j];
}

void* init_grid_and_mark_claims(Fruity2D* g, const struct cgs_array* input)
{
        if (!fruity_new(g, MAX_SIDE, MAX_SIDE, sizeof(int)))
                return NULL;

        int n = 0;
        fruity_init(g, &n);

        cgs_array_foreach(input, increment_claim, fruity_data_mutable(g));

        return g;
}

int is_multi_claim(Fruity2DCell cell, void* data)
{
        const int* p = cell.ptr;
        (void)data;

        return *p > 1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - Find the one claim that doesn't overlap with others
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int is_solo_claim(const void* element, const void* data)
{
        const struct claim* c = element;
        const int*const* fabric = data;

        for (int i = c->y, h = i + c->h; i < h; ++i)
                for (int j = c->x, w = j + c->w; j < w; ++j)
                        if (fabric[i][j] > 1)
                                return CGS_FALSE;
        return CGS_TRUE;
}

int get_solo_claim_id(struct cgs_array* input, const int*const* fabric)
{
        struct claim* solo = cgs_array_find(input, is_solo_claim, fabric);
        if (solo)
                return solo->id;

        return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 3: No Matter How You Slice It\n");

        struct cgs_array input = { 0 };
        if (!cgs_array_new(&input, sizeof(struct claim)))
                return EXIT_FAILURE;

        if (!read_input(&input))
                return EXIT_FAILURE;

        Fruity2D grid = { 0 };
        if (!init_grid_and_mark_claims(&grid, &input))
                return EXIT_FAILURE;

        int part1 = fruity_count_if(&grid, is_multi_claim, NULL);
        int part2 = get_solo_claim_id(&input, fruity_data(&grid));

        printf("Part 1: %d\n", part1);
        printf("Part 2: %d\n", part2);

        cgs_array_free(&input);
        fruity_free(&grid);

        return EXIT_SUCCESS;
}

