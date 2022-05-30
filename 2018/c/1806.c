#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <cgs/cgs.h>
#include <fruity.h>
#include <fruity_io.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum territory_id { ID_NEUTRAL = -1, ID_START = 0, };

const char* id_symbols = " !@#$%^&*()<>{}[];,~-=+'|?"
                         "abcdefghijklmnopqrstuvwxyz"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data structures
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct location {
	int id;
	int distance;
        int soa_dist;           // sum of all distances (Part 2)
};

struct point {
        int x, y;
};

struct claim {
        struct point pt;        // location of claim
        int id;                 // id of claim-maker
        int distance;           // distance from claim-maker
};

int claim_cmp(const void* a, const void* b)
{
        const struct claim* c1 = a;
        const struct claim* c2 = b;

        return c1->distance - c2->distance;
}

struct coord {
        struct point pt;
        int id;
        int area;
        int is_edge;
};

struct input {
	struct cgs_array* coords;
        int rows;
        int cols;
};

void free_input(struct input* input)
{
        cgs_array_free(input->coords);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Reading input
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void apply_offsets(void* p, size_t i, void* data)
{
        struct coord* c = p;
        struct point* offsets = data;
        (void)i;

        c->pt.x -= offsets->x;
        c->pt.y -= offsets->y;
}

void* read_input(struct input* pi)
{
	struct cgs_array* coords = cgs_array_new(struct coord);
        if (!coords)
                return NULL;

        int id = ID_START;
	struct point mins = { .x = INT_MAX, .y = INT_MAX };
	struct point maxs = { 0 };
	for (struct point pt; scanf("%d, %d", &pt.x, &pt.y) == 2; ++id) {
                struct coord c = { .pt = pt, .id = id };
		if (!cgs_array_push(coords, &c))
                        return NULL;
		mins.x = CGS_MIN(mins.x, pt.x);
		mins.y = CGS_MIN(mins.y, pt.y);
		maxs.x = CGS_MAX(maxs.x, pt.x);
		maxs.y = CGS_MAX(maxs.y, pt.y);
	}

        cgs_array_transform(coords, apply_offsets, &mins);
        pi->coords = coords;
        pi->rows = maxs.y - mins.y + 1;
        pi->cols = maxs.x - mins.x + 1;

        return coords;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Grid operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_sum_of_distances(int r, int c, const struct cgs_array* a)
{
        int sum = 0;
        for (int i = 0; i < cgs_array_length(a); ++i) {
                const struct coord* p = cgs_array_get(a, i);
                sum += abs(r - p->pt.y) + abs(c - p->pt.x);
        }

        return sum;
}

void init_location(Fruity2DCell cell, void* data)
{
        struct location* pl = cell.ptr;
        const struct cgs_array* coords = data;

        pl->id = ID_NEUTRAL;
        pl->distance = INT_MAX;
        pl->soa_dist = get_sum_of_distances(cell.row, cell.col, coords);
}

void* init_grid(Fruity2D* grid, const struct input* input)
{
        void* res = fruity_new(grid, input->rows, input->cols,
                        sizeof(struct location));
        if (!res)
                return NULL;

        fruity_transform(grid, NULL, NULL, init_location, input->coords);

        return res;
}

const void* load_queue(struct cgs_heap* q, const struct cgs_array* a)
{
        const struct coord* b = cgs_array_begin(a);
        const struct coord* e = cgs_array_end(a);

        for (struct claim c = { .distance = 0 }; b != e; ++b) {
                c.id = b->id;
                c.pt = b->pt;
                if (!cgs_heap_push(q, &c))
                        return NULL;
        }
        return e;
}

const void* queue_adjacents(struct cgs_heap* q, struct claim* c,
                struct cgs_array* a, Fruity2D* g)
        // Need queue to push onto
        // Need claim to identify adjacents and queue
        // Need coords array to set edges
        // Need grid to get adjacents from
{
        Fruity2DCell adj[4] = { 0 };
        int n = fruity_adjacent_4(g, c->pt.y, c->pt.x, adj);

        if (n < 4)
                ((struct coord*)cgs_array_get_mutable(a, c->id))->is_edge = 1;

        ++c->distance;
        for (int i = 0; i < n; ++i) {
                c->pt.y = adj[i].row;
                c->pt.x = adj[i].col;
                if (!cgs_heap_push(q, c))
                        return NULL;
        }
        return q;
}

void* mark_territories(Fruity2D* g, struct cgs_array* coords)
        // Create a priority queue
        // Load queue with input claims at distance 0
        // Add adjacent cells to the queue while distances are less or we
        // are claiming neutral territory.
{
        struct cgs_heap pq = { 0 };
        if (!cgs_heap_new(&pq, sizeof(struct claim), claim_cmp))
                return NULL;

        if (!load_queue(&pq, coords))
                goto error_cleanup;

        for (struct claim c; cgs_heap_pop(&pq, &c); ) {
                struct location* p = fruity_get_mutable(g, c.pt.y, c.pt.x);
                if (p->distance < c.distance) {         // lower dist
                        continue;
                } else if (p->distance == c.distance) { // equal dist
                        if (p->id == c.id || p->id == ID_NEUTRAL)
                                continue;
                        p->id = ID_NEUTRAL;
                } else {                                // higher dist
                        p->distance = c.distance;
                        p->id = c.id;
                }

                if (!queue_adjacents(&pq, &c, coords, g))
                        goto error_cleanup;
        }
        cgs_heap_free(&pq);
        return g;

error_cleanup:
        cgs_heap_free(&pq);
        return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Get output for Part 1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void increment_area(Fruity2DCell cell, void* data)
{
        const struct location* pl = cell.ptr;
        struct coord* pc = data;

        if (pl->id != ID_NEUTRAL)
                ++pc[pl->id].area;
}

void find_max_inner_area(const void* element, size_t i, void* data)
{
        (void)i;
        const struct coord* pc = element;
        int* pmax = data;

        if (!pc->is_edge)
                *pmax = CGS_MAX(*pmax, pc->area);
}

int get_largest_area(struct cgs_array* coords, const Fruity2D* g)
{
        struct coord* p = cgs_array_get_mutable(coords, 0);
        fruity_foreach(g, NULL, NULL, increment_area, p);

        int max = INT_MIN;
        cgs_array_foreach(coords, find_max_inner_area, &max);

        return max;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Get output for Part 2
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_safe_region_size(const Fruity2D* g, int safe)
{
        int sum = 0;
        for (int i = 0; i < g->rows; ++i)
                for (int j = 0; j < g->cols; ++j) {
                        const struct location* pl = fruity_get(g, i, j);
                        sum += pl->soa_dist < safe;
                }
        return sum;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Print grid to visualize data
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void print_id(Fruity2DCell cell, void* data)
{
        const char* id_key = id_symbols + 1;
        const struct location* p = cell.ptr;
        (void)data;

        printf("%c", id_key[p->id]);
}

void print_grid(const Fruity2D* g)
{
        fruity_foreach(g, fruity_io_newline, NULL, print_id, NULL);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
	printf("Advent of Code 2018 Day 6: Chronal Coordinates\n");

        struct input input = { 0 };
        if (!read_input(&input))
                return EXIT_FAILURE;

        Fruity2D grid = { 0 };
        if (!init_grid(&grid, &input))
                return EXIT_FAILURE;

        if (!mark_territories(&grid, input.coords))
                return EXIT_FAILURE;

        print_grid(&grid);

	int part1 = get_largest_area(input.coords, &grid);
	int part2 = get_safe_region_size(&grid, 10000);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	fruity_free(&grid);
	free_input(&input);

	return EXIT_SUCCESS;
}

