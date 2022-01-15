/*
 * GLib Usage Info
 *
 * GArray
 * ======
 * GArray works well. It's easy to use, has intuitive functions and a few
 * algorithms. The upfront ease of creation is a little offset by the
 * accessor function being a macro that takes a type. There is also the
 * warning that `g_array_append_val` cannot be passed a literal.
 *
 * Also there is no `size` function, we just directly access the `len` struct
 * member.
 *
 * Valgrind reports un-freed memory even after the `unref` call. I've been
 * assured this is okay and that glib has it's own valgrind suppression file.
 */
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

enum { TARGET = 2020 };

int int_cmp(const void* a, const void* b)
{
	const int v1 = *(const int*)a;
	const int v2 = *(const int*)b;

	return v1 - v2;
}

int two_sum(const GArray* arr, int target)
{
	for (guint i = 0; i < arr->len; ++i) {
		int a = g_array_index(arr, int, i);
		for (guint j = i+1; j < arr->len; ++j) {
			int b = g_array_index(arr, int, j);
			if (a + b == target)
				return a * b;
		}
	}
	return -1;
}

int three_sum(const GArray* arr, int target)
{
	for (guint i = 0; i < arr->len; ++i) {
		int a = g_array_index(arr, int, i);
		for (guint j = i+1; j < arr->len; ++j) {
			int b = g_array_index(arr, int, j);
			for (guint k = j+1; k < arr->len; ++k) {
				int c = g_array_index(arr, int, k);
				if (a + b + c == target)
					return a * b * c;
			}
		}
	}
	return -1;
}

int main()
{
	printf("Advent of Code 2020 Day 1: Report Repair\n");

	GArray* input = g_array_new(FALSE, FALSE, sizeof(int));
	for (int n; scanf("%d", &n) == 1; )
		g_array_append_val(input, n);

	g_array_sort(input, int_cmp);

	int part1 = two_sum(input, TARGET);
	int part2 = three_sum(input, TARGET);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	g_array_unref(input);

	return EXIT_SUCCESS;
}

