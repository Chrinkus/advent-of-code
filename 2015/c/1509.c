#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "cgs.h"
#include "fruity.h"
#include "fruity_io.h"

enum ice { BUFFSIZE = 32, };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Read
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static void*
insert_location(struct cgs_vector* locs, const char* loc)
{
        if (!cgs_vector_find(locs, cgs_string_eq_str, loc)) {
                struct cgs_string buff = cgs_string_new();
                cgs_string_from(loc, &buff);
                cgs_vector_push(locs, &buff);
        }
        return locs;
}

static void*
parse_locations(const struct cgs_vector* lines, struct cgs_vector* locs)
{
        char from[BUFFSIZE];
        char to[BUFFSIZE];

        for (size_t i = 0; i < cgs_vector_length(lines); ++i) {
                const char* s = cgs_string_data(cgs_vector_get(lines, i));
                if (sscanf(s, "%s to %s ", from, to) != 2)
                        return cgs_error_retnull("parse location fail");

                insert_location(locs, from);
                insert_location(locs, to);
        }
        return locs;
}

static int
get_dimension(const struct cgs_vector* vs, const char* s)
{
        for (size_t i = 0; i < cgs_vector_length(vs); ++i) {
                if (cgs_string_eq_str(cgs_vector_get(vs, i), s))
                        return (int)i;
        }
        return -1;
}

static void*
build_and_parse_distances(const struct cgs_vector* lines,
                const struct cgs_vector* locs, struct fruity_2d* dists)
{
        int dim = (int)cgs_vector_length(locs);
        int init = 0;
        fruity_build(dists, dim, dim, &init, sizeof(init));
        int** aai = fruity_data_mut(dists);

        int n;
        char from[BUFFSIZE];
        char to[BUFFSIZE];
        for (size_t i = 0; i < cgs_vector_length(lines); ++i) {
                const char* s = cgs_string_data(cgs_vector_get(lines, i));
                if (sscanf(s, "%s to %s = %d", from, to, &n) != 3)
                        return cgs_error_retnull("parse_distance_fail");

                int d1 = get_dimension(locs, from);
                int d2 = get_dimension(locs, to);
                if (d1 < 0 || d2 < 0)
                        return cgs_error_retnull("get_dimension_fail");
                aai[d1][d2] = n;
                aai[d2][d1] = n;
        }
        return dists;
}

static void*
read_and_parse_data(struct cgs_vector* locs, struct fruity_2d* dists)
{
        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        cgs_io_readlines(stdin, &lines);

        parse_locations(&lines, locs);
        build_and_parse_distances(&lines, locs, dists);

        cgs_vector_free_all_with(&lines, cgs_string_free);
        return locs;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Path finding
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct path {
        int prev;               // The index of the previous location
        int distance;           // The sum of the distances on the path
        unsigned visited;       // A bit mask of the visited indexes
};

typedef int (*PathCmp)(const void*, const void*);

static int
path_min_cmp(const void* a, const void* b)
{
        const struct path* p1 = a;
        const struct path* p2 = b;

        return p1->distance - p2->distance;
}

static int
path_max_cmp(const void* a, const void* b)
{
        const struct path* p1 = a;
        const struct path* p2 = b;

        return p2->distance - p1->distance;
}

static void*
load_path_queue(const struct fruity_2d* data, struct cgs_heap* queue,
                const struct path* from)
{
        const int*const* dists = fruity_data(data);

        for (int i = 0; i < fruity_rows(data); ++i) {
                if (1u << i & from->visited)
                        continue;
                struct path next = {
                        .prev = i,
                        .visited = from->visited | 1u << i,
                        .distance = from->distance + dists[i][from->prev],
                };
                cgs_heap_push(queue, &next);
        }
        return queue;
}

typedef int (*IntCmp)(const int, const int);

static int
int_min(const int a, const int b)
{
        return a < b ? a : b;
}

static int
int_max(const int a, const int b)
{
        return a > b ? a : b;
}

static void
get_desired_path_from_index(const struct fruity_2d* data, unsigned index,
                PathCmp pcmp, int* init, IntCmp icmp)
{
        const unsigned ALL_VISITED = (1u << fruity_rows(data)) - 1;

        struct cgs_heap queue = cgs_heap_new(sizeof(struct path), pcmp);

        struct path path = {
                .prev = index,
                .visited = 1u << index,
                .distance = 0,
        };
        load_path_queue(data, &queue, &path);

        for (struct path p; cgs_heap_pop(&queue, &p); ) {
                if ((p.visited & ALL_VISITED) == ALL_VISITED) {
                        *init = icmp(*init, p.distance);
                        continue;
                }
                load_path_queue(data, &queue, &p);
        }

        cgs_heap_free(&queue);
}

static int
shortest_path_all_points(const struct fruity_2d* data)
{
        struct cgs_vector dists = cgs_vector_new(sizeof(int));

        for (unsigned i = 0; i < (unsigned)fruity_rows(data); ++i) {
                int m = INT_MAX;
                get_desired_path_from_index(data, i, path_min_cmp, &m, int_min);
                cgs_vector_push(&dists, &m);
        }

        int min = *(const int*)cgs_vector_min(&dists, cgs_int_cmp);

        cgs_vector_free(&dists);
        return min;
}

static int
longest_path_all_points(const struct fruity_2d* data)
{
        struct cgs_vector dists = cgs_vector_new(sizeof(int));

        for (unsigned i = 0; i < (unsigned)fruity_rows(data); ++i) {
                int m = INT_MIN;
                get_desired_path_from_index(data, i, path_max_cmp, &m, int_max);
                cgs_vector_push(&dists, &m);
        }

        int max = *(const int*)cgs_vector_max(&dists, cgs_int_cmp);

        cgs_vector_free(&dists);
        return max;
}

int main(void)
{
        struct cgs_vector locs = cgs_vector_new(sizeof(struct cgs_string));
        struct fruity_2d dists = { 0 };
        read_and_parse_data(&locs, &dists);

        int part1 = shortest_path_all_points(&dists);
        printf("%d\n", part1);

        int part2 = longest_path_all_points(&dists);
        printf("%d\n", part2);

        cgs_vector_free_all_with(&locs, cgs_string_free);
        fruity_free(&dists);
        return EXIT_SUCCESS;
}
