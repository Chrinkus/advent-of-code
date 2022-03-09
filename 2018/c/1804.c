#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

enum { NUM_MINUTES = 60 };

struct guard {
	int id;
	int minutes[NUM_MINUTES];
};

int guard_cmp(const void* a, const void* b)
{
	return ((const struct guard*)a)->id - ((const struct guard*)b)->id;
}

struct guard* get_guard(struct cgs_array* guards, const char* data)
{
	struct guard* g = NULL;

	struct guard tmp = { 0 };
	if (sscanf(data, "Guard #%d ", &tmp.id) == 1) {
		g = cgs_array_find(guards, &tmp, guard_cmp);
		if (!g)
			g = cgs_array_push(guards, &tmp);
	}

	return g;
}

struct record {
	int minute;
	int sleep_start;
	char* info;
};

struct cgs_array* plot_guard_sleep_patterns(const struct cgs_array* records)
{
	struct cgs_array* guards = cgs_array_new(struct guard);

	const char* fmt = "[1518-%*d-%*d %*d:%d] %m[ G#a-z0-9]";

	CgsStrIter b = cgs_array_begin(records);
	CgsStrIter e = cgs_array_end(records);

	struct guard* g = NULL;
	for (struct record r; b != e
			&& sscanf(*b, fmt, &r.minute, &r.info) == 2; ++b) {

		switch (r.info[0]) {
		case 'G':
			g = get_guard(guards, r.info);
			break;
		case 'f':
			r.sleep_start = r.minute;
			break;
		case 'w':
			for (int i = r.sleep_start; i < r.minute; ++i)
				++g->minutes[i];
			break;
		}
		free(r.info);
	}

	return guards;
}

int find_egregious_sleeper(const struct cgs_array* guards)
{
	const struct guard* g = cgs_array_begin(guards);
	const struct guard* e = cgs_array_end(guards);

	int max_sleep = 0;
	for (const struct guard* b = g; b != e; ++b) {
		int sum = 0;
		for (int i = 0; i < NUM_MINUTES; ++i)
			sum += b->minutes[i];

		if (sum > max_sleep) {
			g = b;
			max_sleep = sum;
		}
	}

	int max_minute = 0;
	for (int i = 0; i < NUM_MINUTES; ++i)
		if (g->minutes[i] > g->minutes[max_minute])
			max_minute = i;

	return g->id * max_minute;
}

int find_consistent_sleeper(const struct cgs_array* guards)
{
	const struct guard* g = cgs_array_begin(guards);
	const struct guard* e = cgs_array_end(guards);

	int common_minute = 0;
	for (const struct guard* b = g; b != e; ++b) {
		for (int i = 0; i < NUM_MINUTES; ++i)
			if (b->minutes[i] > g->minutes[common_minute]) {
				g = b;
				common_minute = i;
			}
	}

	return g->id * common_minute;
}

int main(void)
{
	printf("Advent of Code 2018 Day 4: Repose Record\n");

	struct cgs_array* records = cgs_io_readlines(stdin);
	cgs_array_sort(records, cgs_str_cmp);

	struct cgs_array* guards = plot_guard_sleep_patterns(records);

	int part1 = find_egregious_sleeper(guards);
	int part2 = find_consistent_sleeper(guards);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	cgs_array_free(guards);
	cgs_array_free_all(records);

	return EXIT_SUCCESS;
}

