#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* trim_string(char* s)
{
	size_t len = strlen(s);
	while (len && isspace(s[len-1]))
		s[--len] = '\0';
	return s;
}

size_t look_say_inner(char* dest, char* src)
{
	size_t len = 0;
	while (*src) {
		int count = 0;
		char* c = src;
		for ( ; *c == *src; ++c) {
			++count;
		}
		*dest++ = count + '0';		// count is only ever 1-3
		*dest++ = *src;
		len += 2;
		src = c;
	}
	*dest = '\0';
	return len;
}

char* look_say(char* input, size_t n)
{
	static const double scale = 1.5;

	size_t len = strlen(input) * scale;
	while (n--) {
		char* next = malloc(len);
		next[0] = '\0';
		len = look_say_inner(next, input) * scale;

		free(input);
		input = next;
	}

	return input;
}

int main()
{
	puts("Advent of Code 2015");
	puts("Day 10: Elves Look, Elves Say");

	size_t len = 16;		// enough for input
	char* input = malloc(len);

	if (!fgets(input, len, stdin))
		return EXIT_FAILURE;

	trim_string(input);

	input = look_say(input, 40);
	size_t part1 = strlen(input);
	printf("Part 1: %zu\n", part1);

	input = look_say(input, 10);
	size_t part2 = strlen(input);
	printf("Part 2: %zu\n", part2);

	free(input);

	return EXIT_SUCCESS;
}
