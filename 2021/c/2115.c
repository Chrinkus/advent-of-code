#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#include "aoc_banner.h"
#include <sxc_string.h>
#include <sxc_vector.h>

struct sxc_grid {
	char* data;
	int dimx;
	int dimy;
};

struct sxc_vertex {
	int w;			// weight
	int d;			// distance
	struct sxc_vertex* p;	// parent
};

SXC_NEW_VECTOR(vertex_vec, struct sxc_vertex*);

struct sxc_graph {
	struct sxc_grid* data;
	struct sxc_vertex* graph;
	int size;
};

void sxc_graph_print(const struct sxc_graph* g)
{
	for (int i = 0; i < g->data->dimy; ++i) {
		for (int j = 0; j < g->data->dimx; ++j)
			printf("%8d ", g->graph[i * g->data->dimx + j].d);
		printf("\n");
	}
}

int sxc_grid_size(const struct sxc_grid* g)
{
	return g->dimx * g->dimy;
}

char sxc_grid_get(const struct sxc_grid* g, int x, int y)
{
	return g->data[y * g->dimx + x];
}

char sxc_grid_get_index(const struct sxc_grid* g, int index)
{
	return g->data[index];
}

void sxc_grid_print(const struct sxc_grid* g)
{
	for (int i = 0; i < g->dimy; ++i) {
		for (int j = 0; j < g->dimx; ++j)
			printf("%c", sxc_grid_get(g, j, i));
		printf("\n");
	}
}

struct sxc_grid* read_input(struct sxc_grid* g)
{
	struct sxc_string s;
	sxc_string_init(&s);
	g->dimx = g->dimy = sxc_getline(stdin, &s);
	g->data = malloc(sizeof(char) * g->dimx * g->dimy);

	if (!g->data)
		return NULL;

	int i = 0;
	for (const char* p = sxc_string_str(&s); *p; ++p)
		g->data[i++] = *p;
	sxc_string_free(&s);

	for (int c; i < sxc_grid_size(g) && (c = getchar()) != EOF; )
		if (isdigit(c))
			g->data[i++] = c;

	return g;
}

struct sxc_vertex* sxc_graph_init(struct sxc_graph* g, struct sxc_grid* grid)
{
	g->data = grid;
	g->size = sxc_grid_size(grid);
	g->graph = malloc(sizeof(struct sxc_vertex) * g->size);

	if (g->graph)
		for (int i = 0; i < sxc_grid_size(grid); ++i) {
			struct sxc_vertex* p = &g->graph[i];
			p->w = sxc_grid_get_index(grid, i) - '0';
			p->d = INT_MAX;
			p->p = NULL;
		}
	return g->graph;
}

void sxc_graph_free(struct sxc_graph* g)
{
	free(g->data);
	free(g->graph);
}

int sxc_graph_get_adjacents(struct sxc_vertex* v, struct sxc_vertex* adj[],
		const struct sxc_graph* g)
{
	int num_adj = 0;
	int xmax = g->data->dimx;
	int ymax = g->data->dimy;
	int i = v - g->graph;

	if (i > 1 && i % xmax > 0)	adj[num_adj++] = v - 1;
	if (i + 1 % xmax > 0)		adj[num_adj++] = v + 1;
	//if (i - xmax > 0)		adj[num_adj++] = v - xmax; // don't need
	if (i + xmax < xmax * ymax)	adj[num_adj++] = v + xmax;

	return num_adj;
}

int map_shortest_paths(struct sxc_graph* g)
{
	// init source
	struct sxc_vertex* src = &g->graph[0];
	src->d = 0;
	src->p = NULL;

	// init queue
	vertex_vec queue;
	sxc_vector_init(&queue);

	// get source adjacents
	struct sxc_vertex* adj[4] = { 0 };
	int num_adj = sxc_graph_get_adjacents(src, adj, g);

	// add adjacents to queue
	for (int i = 0; i < num_adj; ++i) {
		struct sxc_vertex* p = adj[i];
		if (!p->p || p->d > src->d + p->w) {
			p->p = src;
			p->d = src->d + p->w;
			sxc_vector_push(&queue, p);
		}
	}

	// process queue
	while (sxc_vector_size(&queue)) {
		struct sxc_vertex* p = NULL;
		sxc_vector_pop_front(&queue, p);

		num_adj = sxc_graph_get_adjacents(p, adj, g);
		for (int i = 0; i < num_adj; ++i) {
			struct sxc_vertex* q = adj[i];
			if (!q->p || q->d > p->d + q->w) {
				q->p = p;
				q->d = p->d + q->w;
				sxc_vector_push(&queue, q);
			}
		}
	}
	sxc_vector_free(&queue);
	return g->graph[g->size-1].d;
}

void trace_path_back(const struct sxc_graph* g)
{
	printf("Trace back:\n");
	for (const struct sxc_vertex* p = &g->graph[g->size-1]; p; p = p->p)
		printf("\tDist: %d, Weight: %d\n", p->d, p->w);
}

char shift_digit(char ch, int n)
{
	int d = ch - '0';
	d += n;
	if (d > 9) d = d % 10 + 1;
	return '0' + d;
}

struct sxc_grid* expand_data(struct sxc_grid* big, int n,
		const struct sxc_grid* data)
{
	big->dimx = data->dimx * n;
	big->dimy = data->dimy * n;
	big->data = malloc(sizeof(char) * big->dimx * big->dimy);

	if (!big->data)
		return NULL;

	for (int i = 0, j = 0, dx = 0, dy = 0, joffset = 0;
			i < sxc_grid_size(big); ++i) {
		big->data[i] = shift_digit(data->data[j], dx + dy);
		++j;
		if (j % data->dimx == 0) {
			++dx;
			if (dx == n) {
				dx = 0;
				joffset += data->dimx;
				if (joffset == sxc_grid_size(data)) {
					++dy;
					joffset = 0;
				}
			}
			j = joffset;
		}
	}
	return big;
}

int main()
{
	aoc_banner_2021("15", "Chiton");

	// Input
	struct sxc_grid* data = malloc(sizeof(struct sxc_grid));
	if (!read_input(data))
		return EXIT_FAILURE;

	// Part 1
	struct sxc_graph graph = { 0 };
	if (!sxc_graph_init(&graph, data))
		return EXIT_FAILURE;

	int part1 = map_shortest_paths(&graph);

	// Part 2
	struct sxc_grid* big_data = malloc(sizeof(struct sxc_grid));
	if (!expand_data(big_data, 5, data))
		return EXIT_FAILURE;

	sxc_graph_free(&graph);

	struct sxc_graph big_graph = { 0 };
	if (!sxc_graph_init(&big_graph, big_data))
		return EXIT_FAILURE;
	int part2 = map_shortest_paths(&big_graph);

	sxc_graph_free(&big_graph);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	return EXIT_SUCCESS;
}

