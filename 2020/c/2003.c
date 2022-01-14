/*
 * GLib Usage Info
 *
 * This is my first use of the GString. The new, free and append functions
 * work as expected. Character access within the string is accomplished
 * directly on the stored memory by dereferencing the `str` member.
 *
 * My output for part 2 was getting close to INT_MAX so I increased the size
 * of the integer type. Here the use of gint64 over int64_t seemed not to
 * provide much benefit.
 */
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

enum { TREE = '#' };

struct Hill {
	GString* data;
	int w;
	int h;
};

struct Slope {
	int r;
	int d;
};

const struct Slope slopes[] = {
	{ .r = 3, .d = 1 },
	{ .r = 1, .d = 1 },
	{ .r = 5, .d = 1 },
	{ .r = 7, .d = 1 },
	{ .r = 1, .d = 2 },
};

const int num_slopes = sizeof slopes / sizeof slopes[0];

void read_hill_data(struct Hill* hill)
{
	for (int n = 0, c; (c = getchar()) != EOF; ++n)
		if (c == '\n') {
			++hill->h;
			if (!hill->w)
				hill->w = n;
		} else {
			g_string_append_c(hill->data, c);
		}
}

gint64 count_trees_on_slope(const struct Hill* h, const struct Slope* s)
{
	gint64 count = 0;

	const char* data = h->data->str;
	for (int x = 0, y = 0; y < h->h; x += s->r, y += s->d)
		count += data[y * h->w + x % h->w] == TREE;

	return count;
}

int main()
{
	printf("Advent of Code 2020 Day 3: Toboggan Trajectory\n");

	struct Hill hill = { .data = g_string_new(NULL), .w = 0, .h = 0 };

	read_hill_data(&hill);

	gint64 part1 = count_trees_on_slope(&hill, &slopes[0]);
	gint64 part2 = part1;	// gets close to INT_MAX

	for (guint i = 1; i < num_slopes; ++i)
		part2 *= count_trees_on_slope(&hill, &slopes[i]);
	
	g_string_free(hill.data, TRUE);

	printf("Part 1: %" G_GINT64_FORMAT "\n", part1);
	printf("Part 2: %" G_GINT64_FORMAT "\n", part2);

	return EXIT_SUCCESS;
}

