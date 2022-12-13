#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "cgs.h"
#include "fruity.h"

enum magic { START = 'S', END = 'E', DEFAULT = INT_MAX, };

struct Square {
        char ch;
        int count;
};

struct Point {
        size_t x;
        size_t y;
};

struct Step {
        struct Point pt;
        int count;
        char ch;
};

static int
step_cmp(const void* a, const void* b)
{
        const struct Step* s1 = a;
        const struct Step* s2 = b;

        return s1->count - s2->count;
}

static void*
read_and_map_elevations(Fruity2D* map, struct Point* start, struct Point* end)
{
        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        if (!cgs_io_readlines(stdin, &lines))
                return cgs_error_retnull("readlines");

        const size_t rows = cgs_vector_length(&lines);
        const size_t cols = cgs_string_length(cgs_vector_get(&lines, 0));

        if (!fruity_new(map, rows, cols, sizeof(char)))
                return cgs_error_retnull("fruity_new");

        char** data = fruity_data_mut(map);
        for (size_t i = 0; i < rows; ++i) {
                const struct cgs_string* s = cgs_vector_get(&lines, i);
                for (size_t j = 0; j < cols; ++j) {
                        char ch = cgs_string_char(s, j);
                        if (ch == START) {
                                *start = (struct Point){ j, i };
                                ch = 'a';
                        } else if (ch == END) {
                                *end = (struct Point){ j, i };
                                ch = 'z';
                        }
                        data[i][j] = ch;
                }
        }
        cgs_vector_free_all_with(&lines, cgs_string_free);
        return map;
}

static void*
init_new_path(const Fruity2D* map, Fruity2D* path)
{
        if (!fruity_new(path, fruity_rows(map), fruity_cols(map), sizeof(int)))
                return cgs_error_retnull("fruity_new");

        int n = DEFAULT;
        fruity_init(path, &n);

        return path;
}

static void*
trace_path(/*const*/ Fruity2D* map, struct cgs_heap* q, Fruity2D* path)
{
        Fruity2DCell adj[4] = { { 0 } };

        for (struct Step s; cgs_heap_pop(q, &s); ) {
                const int n = fruity_adjacent_4(map, s.pt.y, s.pt.x, adj);
                for (int i = 0; i < n; ++i) {
                        char* pc = adj[i].ptr;
                        int* sq = fruity_get_mut(path, adj[i].row, adj[i].col);

                        if (*pc - s.ch <= 1 && *sq > s.count) {
                                *sq = s.count;
                                struct Step next = {
                                        .pt = {
                                                .x = adj[i].col,
                                                .y = adj[i].row,
                                        },
                                        .count = s.count + 1,
                                        .ch = *pc,
                                };
                                if (!cgs_heap_push(q, &next))
                                        return cgs_error_retnull("heap_push");
                        }
                }
        }
        return q;
}

static void*
setup_queue_and_run(/*const*/ Fruity2D* map, const struct Point* start,
                Fruity2D* path)
{
        struct cgs_heap pq = { 0 };
        if (!cgs_heap_new(&pq, sizeof(struct Step), step_cmp))
                return cgs_error_retnull("cgs_heap_new");

        struct Step s1 = { .pt = *start, .count = 1, .ch = 'a', };
        if (!cgs_heap_push(&pq, &s1)) {
                cgs_error_msg("heap_push");
                goto error_cleanup;
        }

        if (!trace_path(map, &pq, path)) {
                cgs_error_msg("trace_path");
                goto error_cleanup;
        }

        cgs_heap_free(&pq);
        return path;

error_cleanup:
        cgs_heap_free(&pq);
        return NULL;
}

static int
get_shortest_path_count(const Fruity2D* path, const struct Point* end)
{
        const int* peak = fruity_get(path, end->y, end->x);
        return *peak;
}

int main(void)
{
        Fruity2D map = { 0 };
        struct Point start = { 0 };
        struct Point end = { 0 };
        if (!read_and_map_elevations(&map, &start, &end))
                return cgs_error_retfail("read_and_map_elevations");

        Fruity2D path = { 0 };
        if (!init_new_path(&map, &path))
                return cgs_error_retfail("init_new_path");

        if (!setup_queue_and_run(&map, &start, &path))
                return cgs_error_retfail("setup_queue_and run");

        int part1 = get_shortest_path_count(&path, &end);
        printf("%d\n", part1);
        int part2 = 0;
        printf("%d\n", part2);

        fruity_free(&map);
        return EXIT_SUCCESS;
}
