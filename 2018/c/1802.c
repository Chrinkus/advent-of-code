#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cgs/cgs.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structure
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct repeat_count {
        int pairs;
        int trios;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Count ids with double and triple character occurrances
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void count_repeats(char* s, struct repeat_count* rc)
{
        qsort(s, strlen(s), sizeof(char), cgs_char_cmp);

        int dubbs = 0;
        int trips = 0;
        for (int c = '\0', count = 1; !dubbs || !trips; c = *s++) {
                if (*s == c) {
                        ++count;
                } else {
                        dubbs += !dubbs && count == 2;
                        trips += !trips && count == 3;
                        count = 1;
                }
                if (!*s)        // test here to catch last count
                        break;
        }
        rc->pairs += dubbs;
        rc->trios += trips;
}

void setup_counter(void* element, size_t i, void* data)
{
        char* s = *(char**)element;
        struct repeat_count* rc = data;
        (void)i;
        count_repeats(s, rc);
}

int get_checksum(struct cgs_array* ids)
{
        struct repeat_count rc = { 0 };

        cgs_array_transform(ids, setup_counter, &rc);

        return rc.pairs * rc.trios;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - Get common characters of 2 ids that differ by 1 character
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
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
        if (!s)
                return NULL;

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
	printf("Advent of Code 2018 Day 2: Inventory Management System\n");

        struct cgs_array input = { 0 };
        if (!cgs_array_new(&input, sizeof(char*)))
                return EXIT_FAILURE;

        if (!cgs_io_readlines(stdin, &input))
                return EXIT_FAILURE;

        char* part2 = find_common_ids(&input);
        if (!part2)
                return EXIT_FAILURE;

        int part1 = get_checksum(&input);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %s\n", part2);

	free(part2);
	cgs_array_free_all(&input);

	return EXIT_SUCCESS;
}

