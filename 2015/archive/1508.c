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

struct lengths {
	size_t code;
	size_t memory;
	size_t encoded;
};

void get_lengths(char* s, struct lengths* l)
{
	l->code = strtrim(s);
	l->memory = l->code - 2;
	l->encoded = l->code + 4;

	for ( ; *s; ++s) {
		if (*s == '\\') {
			++s;
			switch (*s) {
			case '"':
			case '\\':
				l->memory -= 1;
				l->encoded += 2;
				break;
			case 'x':
				l->memory -= 3;
				l->encoded += 1;
				s += 2;
				break;
			}
		}
	}
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
		struct lengths ls = { 0 };
		get_lengths(line, &ls);

		code += ls.code;
		memory += ls.memory;
		encoded += ls.encoded;
	}
	free(line);

	int part1 = code - memory;
	int part2 = encoded - code;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}
