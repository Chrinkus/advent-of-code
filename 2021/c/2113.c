#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "aoc_banner.h"
#include <sxc_string.h>
#include <sxc_vector.h>

enum Constants { DOT = '#', CLEAR = ' ', HORIZ = 'y', VERT = 'x', };

struct Point {
	int x;
	int y;
};

SXC_NEW_VECTOR(Point_vector, struct Point);

struct Fold {
	char axis;
	int index;
};

SXC_NEW_VECTOR(Fold_vector, struct Fold);

struct Graph {
	char* graph;
	int cols;
	int rows;
};

void mark_dot(const struct Point* p, struct Graph* g)
{
	g->graph[p->y * g->cols + p->x] = DOT;
}

int fold_cmp(const struct Fold* f, char tar)
{
	return f->axis - tar;
}

void init_graph(struct Graph* g, const Fold_vector* folds)
{
	int rows = 0;
	int cols = 0;
	for (const struct Fold* f = sxc_vector_getp(folds, 0);
			f && (!rows || !cols); ++f)
		switch (f->axis) {
		case HORIZ:	if (!rows) rows = f->index;	break;
		case VERT:	if (!cols) cols = f->index;	break;
		}

	g->rows = rows * 2 + 1;
	g->cols = cols * 2 + 1;
	g->graph = malloc(sizeof(char) * g->rows * g->cols);
}

void plot_graph(struct Graph* g, const Point_vector* pts)
{
	for (int i = 0; i < g->rows; ++i)
		for (int j = 0; j < g->cols; ++j)
			g->graph[i * g->cols + j] = CLEAR;

	sxc_vector_foreach(pts, mark_dot, g);
}

void print_graph(const struct Graph* g)
{
	for (int i = 0; i < g->rows; ++i) {
		for (int j = 0; j < g->cols; ++j)
			printf("%c", g->graph[i * g->cols + j]);
		printf("\n");
	}
}

void print_fold(const struct Fold* f, void* data)
{
	SXC_UNUSED(data);

	printf("Fold %d along %c axis\n", f->index, f->axis);
}

void collapse_horizontal(struct Graph* g, int index)
	// Necessary for flat 2D array
{
	char* p = g->graph;
	for (int i = 1; i < g->rows; ++i) {
		for (int j = 0; j < index; ++j) {
			p[i*index+j] = p[i*g->cols+j];
		}
	}
}

void fold_graph(struct Graph* g, Fold_vector* folds, size_t start, size_t stop)
{
	char* p = g->graph;
	for (size_t i = start; i < stop; ++i) {
		const struct Fold* f = sxc_vector_getp(folds, i);
		if (f->axis == HORIZ) {
			// Fold along horizontal index
			for (int i = g->rows-1, k = 0; i > f->index; --i, ++k)
				for (int j = 0; j < g->cols; ++j)
					if (p[i*g->cols+j] == DOT)
						p[k*g->cols+j] = DOT;
			g->rows = f->index;
		} else {
			// Fold along vertical index
			for (int i = 0; i < g->rows; ++i)
				for (int j = g->cols-1, k = 0; j > f->index;
						--j, ++k)
					if (p[i*g->cols+j] == DOT)
						p[i*g->cols+k] = DOT;
			collapse_horizontal(g, f->index);
			g->cols = f->index;
		}
	} 
}

int count_dots(const struct Graph* g)
{
	int count = 0;
	for (int i = 0; i < g->rows; ++i)
		for (int j = 0; j < g->cols; ++j)
			if (g->graph[i*g->cols+j] == DOT)
				++count;
	return count;
}

int main()
{
	aoc_banner_2021("13", "Transparent Origami");

	// Read points
	Point_vector dots;
	sxc_vector_init(&dots);
	for (struct Point p; scanf("%d,%d ", &p.x, &p.y) == 2; ) {
		sxc_vector_push(&dots, p);
	}

	// Read folds
	Fold_vector folds;
	sxc_vector_init(&folds);
	for (struct Fold f;
			scanf("fold along %c=%d ", &f.axis, &f.index) == 2; ) {
		sxc_vector_push(&folds, f);
	}

	struct Graph graph;
	init_graph(&graph, &folds);
	plot_graph(&graph, &dots);

	fold_graph(&graph, &folds, 0, 1);
	int part1 = count_dots(&graph);

	fold_graph(&graph, &folds, 1, sxc_vector_size(&folds));
	const char* part2 = "See print-out below:\n";

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %s\n", part2);

	print_graph(&graph);

	free(graph.graph);
	sxc_vector_free(&dots);
	sxc_vector_free(&folds);

	return EXIT_SUCCESS;
}

