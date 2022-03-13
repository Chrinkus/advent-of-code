#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <cgs/cgs.h>

enum { NUM_TRIALS = 26 };

int reaction(char a, char b)
{
	return (islower(a) && b == toupper(a))
		|| (isupper(a) && b == tolower(a));
}

void reduce_polymer(char* polymer)
{
	const char* const start = polymer;
	for (const char* p = start, *s = p+1; *p; ++s) {
		if (!*s) {
			*polymer++ = *p++;
		} else if (reaction(*p, *s)) {
			p = p == start || polymer == start ? ++s : --polymer;
		} else {
			*polymer++ = *p;
			p = s;
		}
	}
	*polymer = '\0';
}

void prepare_polymer(const char* base, char lo, char* polymer)
{
	char up = toupper(lo);
	for (const char* r = base; *r; ++r)
		if (*r != lo && *r != up)
			*polymer++ = *r;
	*polymer = '\0';
}

int find_max_collapse(const char* polymer, int max)
{
	char* buff = malloc(strlen(polymer) + 1);

	int trials[NUM_TRIALS] = { 0 };

	for (int i = 0; i < NUM_TRIALS; ++i) {
		prepare_polymer(polymer, 'a' + i, buff);
		reduce_polymer(buff);
		trials[i] = strlen(buff);
	}
	free(buff);

	int min = max;
	for (int i = 0; i < NUM_TRIALS; ++i)
		if (trials[i] < min)
			min = trials[i];
	return min;
}

int main(void)
{
	printf("Advent of Code 2018 Day 5: Alchemical Reduction\n");

	char* polymer = cgs_io_readline(stdin);

	reduce_polymer(polymer);

	int part1 = strlen(polymer);
	int part2 = find_max_collapse(polymer, part1);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	free(polymer);

	return EXIT_SUCCESS;
}

