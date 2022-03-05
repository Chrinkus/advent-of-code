#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum { MAXBUFF = 32 };

int char_cmp(const void* a, const void* b)
{
	return *(char*)a - *(char*)b;
}

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
		if (!*s)
			break;
	}
	*twice += dubbs;
	*thrice += trips;
}

int main(void)
{
	printf("Advent of Code 2018 Day 2: Inventory Management System\n");

	int twice = 0;
	int thrice = 0;
	for (char line[MAXBUFF]; fgets(line, MAXBUFF, stdin); ) {
		qsort(line, strlen(line), sizeof(char), char_cmp);
		count_repeats(line, &twice, &thrice);
	}

	int part1 = twice * thrice;
	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

