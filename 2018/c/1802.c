#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cgs/cgs.h>

void count_repeats(const char* s, int* twice, int* thrice)
{
	int dubbs = 0;
	int trips = 0;
	int count = 1;
	for (char c = '\0'; !dubbs || !trips; c = *s++) {
		if (*s == c) {
			++count;
		} else {
			dubbs += !dubbs && count == 2;
			trips += !trips && count == 3;
			count = 1;
		}
		if (!*s)	// test here to catch last count
			break;
	}
	*twice += dubbs;
	*thrice += trips;
}

int count_mismatches(const char* a, const char* b, int limit)
{
	int count = 0;
	for (size_t i = 0, l = strlen(a); i < l && count < limit; ++i)
		count += a[i] != b[i];
	return count;
}

char* common_chars(const char* a, const char* b)
{
	char* s = malloc(strlen(a) + 1);
	char* p = s;
	for ( ; *a; ++a, ++b)
		if (*a == *b)
			*p++ = *a;
	*p = '\0';
	return s;
}

char* find_common_ids(struct cgs_array* ids)
{
	char* ret = NULL;

	cgs_array_sort(ids, cgs_str_cmp);

	CgsStrIter b = cgs_array_begin(ids);
	CgsStrIter e = cgs_array_end(ids);
	for (CgsStrIter next = b + 1; next != e; ++b, ++next)
		if (count_mismatches(*b, *next, 2) == 1) {
			ret = common_chars(*b, *next);
			break;
		}
	return ret;
}

int main(void)
{
	printf("Advent of Code 2018 Day 2: Inventory Management System\n");

	int twice = 0;
	int thrice = 0;

	struct cgs_array* ids = cgs_array_new(char*);
	struct cgs_string* buff = cgs_string_new();
	for ( ; cgs_io_getline(stdin, buff) > 0; cgs_string_clear(buff)) {
		// Part 2
		char* p = cgs_strdup(cgs_string_read(buff));
		cgs_array_push(ids, &p);

		// Part 1
		cgs_string_sort(buff);
		count_repeats(cgs_string_read(buff), &twice, &thrice);
	}
	cgs_string_free(buff);

	int part1 = twice * thrice;
	char* part2 = find_common_ids(ids);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %s\n", part2);

	free(part2);
	cgs_array_free_all(ids);

	return EXIT_SUCCESS;
}

