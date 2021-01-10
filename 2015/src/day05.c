#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int readline(char* lineptr, size_t n, FILE* stream)
{
	int count = 0;
	for (int c; --n && (c = fgetc(stream)) != EOF && c != '\n'; ++count) {
		*lineptr++ = c;
	}
	*lineptr = '\0';

	return count;
}

bool is_nice_v1(char* s)
{
	int vowels = 0;
	bool has_repeats = false;

	for (char* p = s; *p != '\0'; ++p) {
		char next = *(p+1);

		switch (*p) {
		case 'a':	/* fall-through */
		case 'e':	/* fall-through */
		case 'i':	/* fall-through */
		case 'o':	/* fall-through */
		case 'u':	++vowels;	break;
		}

		if (!has_repeats && (*p == next))
			has_repeats = true;

		switch (*p) {
		case 'a': if (next == 'b') return false;	break;
		case 'c': if (next == 'd') return false;	break;
		case 'p': if (next == 'q') return false;	break;
		case 'x': if (next == 'y') return false;	break;
		}
	}

	return vowels >= 3 && has_repeats;
}

bool detect_repeat_pair(char* s)
{
	if (*s == '\0' || (*(s+1) == '\0'))
		return false;

	for (char* p = s+2; *p != '\0'; ++p)
		if (strncmp(s, p, 2) == 0)
			return true;

	return false;
}

bool is_nice_v2(char* s)
{
	bool has_skip_repeat = false;
	bool has_repeat_pair = false;

	for (char* p = s; *p != '\0'; ++p) {
		char skip = *(p+1) == '\0' ? '\0' : *(p+2);

		if (!has_skip_repeat && (*p == skip))
			has_skip_repeat = true;
		if (!has_repeat_pair && detect_repeat_pair(p))
			has_repeat_pair = true;

		if (has_skip_repeat && has_repeat_pair)
			return true;
	}

	return false;
}

void count_nice_strings(int* count1, int* count2)
{
	const size_t buf_max = 20;
	char line[buf_max];

	for (int nread; (nread = readline(line, buf_max, stdin)) != 0; ) {
		if (is_nice_v1(line))
			++*count1;
		if (is_nice_v2(line))
			++*count2;
	}
}

int main()
{
	int part1 = 0;
	int part2 = 0;
	count_nice_strings(&part1, &part2);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}
