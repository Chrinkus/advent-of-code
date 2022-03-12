#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <cgs/cgs.h>

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

int main(void)
{
	printf("Advent of Code 2018 Day 5: Alchemical Reduction\n");

	char* polymer = cgs_io_readline(stdin);

	reduce_polymer(polymer);

	int part1 = strlen(polymer);
	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	free(polymer);

	return EXIT_SUCCESS;
}

