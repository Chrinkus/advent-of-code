#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>
#include <fruity.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum { DIM = 300, PART1_SIZE = 3 };

struct cell {
        int power;
        int areasum;
};

struct square {
        int x, y;
        int size;
        int power;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing & Grid Initialization
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_power(int x, int y, int sn)
{
        int rack_id = x + 10;
        int power = (((rack_id * y + sn) * rack_id) % 1000) / 100 - 5;

        return power;
}

int get_areasum(struct cell** g, int r, int c)
{
        int as = g[r][c].power;
        if (r > 0)
                as += g[r-1][c].areasum;
        if (c > 0)
                as += g[r][c-1].areasum;
        if (r > 0 && c > 0)
                as -= g[r-1][c-1].areasum;
        return as;
}

void* grid_init(Fruity2D* grid, int sn)
{
        if (!fruity_new(grid, DIM, DIM, sizeof(struct cell)))
                return cgs_error_retnull("fruity_new");

        struct cell** g = fruity_data(grid);

        for (int i = 0; i < DIM; ++i) {
                for (int j = 0; j < DIM; ++j) {
                        g[i][j].power = get_power(j+1, i+1, sn);
                        g[i][j].areasum = get_areasum(g, i, j);
                }
        }

        return grid;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Find largest 3x3 sum
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_area_power(struct cell** g, int r, int c, int sz)
{
        int tl = r > 0 && c > 0 ? g[r-1][c-1].areasum : 0;      // outside
        int tr = r > 0 ? g[r-1][c+sz-1].areasum : 0;            // outside
        int bl = c > 0 ? g[r+sz-1][c-1].areasum : 0;            // outside
        int br = g[r+sz-1][c+sz-1].areasum;                     // inside

        return br - tr - bl + tl;
}

struct square get_largest_power(Fruity2D* grid, int size)
{
        struct square sq = { .size = size };
        struct cell** g = fruity_data(grid);

        for (int r = 0; r < DIM - size + 1; ++r)
                for (int c = 0; c < DIM - size + 1; ++c) {
                        int curr = get_area_power(g, r, c, size);
                        if (curr > sq.power) {
                                sq.power = curr;
                                sq.y = r+1;
                                sq.x = c+1;
                        }
                }
        return sq;
}

struct square get_largest_possible_power(Fruity2D* grid)
{
        struct square sq = { 0 };

        for (int sz = DIM; sz > 0; --sz) {
                struct square tmp = get_largest_power(grid, sz);
                if (tmp.power > sq.power)
                        sq = tmp;
        }

        return sq;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 11: Chronal Charge\n");

        int input;
        if (scanf(" %d", &input) != 1)
                return cgs_error_retfail("scanf read fail");

        Fruity2D grid = { 0 };
        if (!grid_init(&grid, input))
                return cgs_error_retfail("grid_init w/%d", input);

        struct square part1 = get_largest_power(&grid, PART1_SIZE);
        struct square part2 = get_largest_possible_power(&grid);

        printf("Part 1: %d,%d\n", part1.x, part1.y);
        printf("Part 2: %d,%d,%d\n", part2.x, part2.y, part2.size);

        fruity_free(&grid);

        return EXIT_SUCCESS;
}

