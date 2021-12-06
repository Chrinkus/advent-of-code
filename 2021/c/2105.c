#include <stdlib.h>
#include <stdio.h>

#include "aoc_banner.h"

#include <sxc_vector.h>
#include <sxc_point.h>

struct Line {
	struct SXC_Point p1;
	struct SXC_Point p2;
};

struct Line_vector {
	size_t siz;
	size_t cap;
	struct Line* vec;
};

int max_of_two(const int a, const int b)
{
	return a > b ? a : b;
}

int max_of_three(const int a, const int b, const int c)
{
	return max_of_two(max_of_two(a, b), c);
}

int* alloc_and_set_graph(int xmax, int ymax, int init)
{
	int* p = malloc(sizeof(int) * xmax * ymax);
	for (int i = 0; i < xmax * ymax; ++i)
		p[i] = init;
	return p;
}

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void swap_points(struct SXC_Point* a, struct SXC_Point* b)
{
	struct SXC_Point tmp = *a;
	*a = *b;
	*b = tmp;
}

void draw_vertical_lines(int* graph, int xmax, const struct Line* line)
{
	int head = line->p1.y;
	int tail = line->p2.y;
	if (head > tail)
		swap(&head, &tail);

	while (head <= tail)
		++graph[head++ * xmax + line->p1.x];
}

void draw_horizontal_lines(int* graph, int xmax, const struct Line* line)
{
	int head = line->p1.x;
	int tail = line->p2.x;
	if (head > tail)
		swap(&head, &tail);

	while (head <= tail)
		++graph[line->p1.y * xmax + head++];
}

void draw_lines_1(int* graph, int xmax, const struct Line* line)
{
	if (line->p1.x == line->p2.x) {		// vertical
		draw_vertical_lines(graph, xmax, line);
	} else if (line->p1.y == line->p2.y) {	// horizontal
		draw_horizontal_lines(graph, xmax, line);
	}
}

void draw_diagonal_lines(int* graph, int xmax, const struct Line* line)
{
	struct SXC_Point p1 = line->p1;
	struct SXC_Point p2 = line->p2;
	if (sxc_point_cmp(&p1, &p2) >= 0)
		swap_points(&p1, &p2);

	if (p1.y < p2.y) {		// line goes up and right
		while (p1.y <= p2.y)
			++graph[p1.y++ * xmax + p1.x++];
	} else {			// line goes down and right
		while (p1.y >= p2.y)
			++graph[p1.y-- * xmax + p1.x++];
	}
}

void draw_lines_2(int* graph, int xmax, const struct Line* line)
{
	if ((line->p1.x != line->p2.x) && (line->p1.y != line->p2.y))
		draw_diagonal_lines(graph, xmax, line);
}

int count_danger(int* graph, int len)
{
	int count = 0;
	for (int i = 0; i < len; ++i)
		count += graph[i] > 1;
	return count;
}

int main()
{
	aoc_banner_2021("05", "Hydrothermal Venture");

	struct Line_vector lines;
	sxc_vector_init(&lines);

	int xmax = 0;
	int ymax = 0;
	for (int x1, y1, x2, y2;
			scanf("%d,%d -> %d,%d", &x1, &y1, &x2, &y2) == 4; ) {
		struct Line line = { .p1 = {x1, y1}, .p2 = {x2, y2} };
		sxc_vector_push(&lines, line);

		xmax = max_of_three(xmax, x1, x2);
		ymax = max_of_three(ymax, y1, y2);
	}
	++xmax;
	++ymax;

	int* sea_floor = alloc_and_set_graph(xmax, ymax, 0);

	// Part 1
	for (size_t i = 0; i < sxc_vector_size(&lines); ++i)
		draw_lines_1(sea_floor, xmax, sxc_vector_getp(&lines, i));

	int part1 = count_danger(sea_floor, xmax * ymax);

	// Part 2
	for (size_t i = 0; i < sxc_vector_size(&lines); ++i)
		draw_lines_2(sea_floor, xmax, sxc_vector_getp(&lines, i));

	int part2 = count_danger(sea_floor, xmax * ymax);

	//printf("Part 1: %d\n", part1);
	//printf("Part 2: %d\n", part2);
	aoc_report_ints(part1, part2);

	free(sea_floor);
	sxc_vector_free(&lines);

	return EXIT_SUCCESS;
}

