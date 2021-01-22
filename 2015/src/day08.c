#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

size_t strtrim(char* s)
{
	// remove leading whitespace
	char* p = s;
	while (isspace(*p))
		++p;
	if (p != s)
		strcpy(s, p);

	// remove trailing whitespace
	size_t len = strlen(s);
	while (len && isspace(s[len-1]))
		s[--len] = '\0';
	return len;
}

int main()
{
	puts("Advent of Code 2015");
	puts("Day 8: Matchsticks\n");

	size_t len = 64;
	char* line = malloc(len);

	int code = 0;
	int memory = 0;
	int encoded = 0;
	while ((getline(&line, &len, stdin)) > 1) {
		size_t cl = strtrim(line);
		size_t ml = cl - 2;		// quotes
		size_t el = cl + 4;

		for (char* p = line; *p; ++p)
			if (*p == '\\') {
				++p;
				--ml;
				++el;		// encode the slash
				if (*p == 'x') {
					ml -= 2;
					p += 2;
				} else {
					++el;	// encode the quote
				}
			}

		code += cl;
		memory += ml;
		encoded += el;
	}
	free(line);

	int part1 = code - memory;
	int part2 = encoded - code;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}
