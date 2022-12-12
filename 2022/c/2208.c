#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cgs.h"
#include "fruity.h"

struct Tree {
        int height;
        int visible;
        int score;
};

static int
check_visibility(const int height, size_t r, size_t c, const Fruity2D* map)
{
        if (r == 0 || r == fruity_rows(map) - 1 ||
            c == 0 || c == fruity_cols(map) - 1)
                return 1;
        int sides = 4;
        const struct Tree*const* trees = fruity_data(map);
        // UP
        for (size_t i = r - 1; i < fruity_rows(map); --i)
                if (trees[i][c].height >= height) {
                        --sides;
                        break;
                }
        // DOWN
        for (size_t i = r + 1; i < fruity_rows(map); ++i)
                if (trees[i][c].height >= height) {
                        --sides;
                        break;
                }
        // LEFT
        for (size_t j = c - 1; j < fruity_cols(map); --j)
                if (trees[r][j].height >= height) {
                        --sides;
                        break;
                }
        // RIGHT
        for (size_t j = c + 1; j < fruity_cols(map); ++j)
                if (trees[r][j].height >= height) {
                        --sides;
                        break;
                }
        return sides != 0;
}

static int
check_score(const int h, const size_t r, const size_t c, const Fruity2D* map)
{
        if (r == 0 || r == fruity_rows(map) - 1 ||
            c == 0 || c == fruity_cols(map) - 1)
                return 0;

        const struct Tree*const* trees = fruity_data(map);
        int up = 0;
        for (size_t i = r - 1; i < fruity_rows(map); --i) {
                ++up;
                if (trees[i][c].height >= h)
                        break;
        }
        int down = 0;
        for (size_t i = r + 1; i < fruity_rows(map); ++i) {
                ++down;
                if (trees[i][c].height >= h)
                        break;
        }
        int left = 0;
        for (size_t j = c - 1; j < fruity_cols(map); --j) {
                ++left;
                if (trees[r][j].height >= h)
                        break;
        }
        int right = 0;
        for (size_t j = c + 1; j < fruity_cols(map); ++j) {
                ++right;
                if (trees[r][j].height >= h)
                        break;
        }
        return up * down * left * right;
}

static void
set_visible_trees(Fruity2DCell cell, void* data)
{
        struct Tree* t = cell.ptr;
        Fruity2D* map = data;

        t->visible = check_visibility(t->height, cell.row, cell.col, map);
}

static void
set_scenic_score(Fruity2DCell cell, void* data)
{
        struct Tree* t = cell.ptr;
        Fruity2D* map = data;

        t->score = check_score(t->height, cell.row, cell.col, map);
}

static void*
read_and_map_trees(Fruity2D* map)
{
        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        if (!cgs_io_readlines(stdin, &lines))
                return cgs_error_retnull("readlines");

        const size_t rows = cgs_vector_length(&lines);
        const size_t cols = cgs_string_length(cgs_vector_get(&lines, 0));
        if (!fruity_new(map, rows, cols, sizeof(struct Tree)))
                return cgs_error_retnull("fruity_new");

        struct Tree** data = fruity_data_mut(map);
        for (size_t i = 0; i < rows; ++i) {
                const struct cgs_string* s = cgs_vector_get(&lines, i);
                for (size_t j = 0; j < cols; ++j) {
                        data[i][j] = (struct Tree){
                                .height = cgs_string_char(s, j) - '0',
                                .visible = -1,
                                .score = 0,
                        };
                }
        }
        fruity_transform(map, NULL, NULL, set_visible_trees, map);
        fruity_transform(map, NULL, NULL, set_scenic_score, map);

        cgs_vector_free_all_with(&lines, cgs_string_free);
        return map;
}

static int
is_visible(Fruity2DCell cell, void* data)
{
        const struct Tree* t = cell.ptr;
        (void)data;
        return t->visible;
}

static void
get_max_score(Fruity2DCell cell, void* data)
{
        const struct Tree* t = cell.ptr;
        int* max = data;
        *max = CGS_MAX(*max, t->score);
}

int main(void)
{
        Fruity2D map = { 0 };
        if (!read_and_map_trees(&map))
                return EXIT_FAILURE;

        int part1 = fruity_count_if(&map, is_visible, NULL);
        printf("%d\n", part1);

        int part2 = 0;
        fruity_foreach(&map, NULL, NULL, get_max_score, &part2);
        printf("%d\n", part2);

        fruity_free(&map);

        return EXIT_SUCCESS;
}
