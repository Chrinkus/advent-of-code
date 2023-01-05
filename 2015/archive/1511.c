#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isvalid(char s[])
{
	// Policy 2: no confusing letters
	if (strpbrk(s, "ilo"))
		return false;

	bool straight = false;
	int pairs = 0;
	char pair = '\0';
	char prev = '\0';
	while (*s) {
		// Policy 3: must contain two different, non-overlapping pairs
		if (pairs < 2 && *s == pair) {
			++pairs;
			pair = '\0';	// reset ch
		} else {
			pair = *s;	// for next iter
		}

		// Policy 2: must include increasing straight of three letters
		if (!straight && (prev == *s - 1) && (*(s+1) == *s + 1))
			straight = true;
		prev = *s;
		++s;
	}
	return pairs == 2 && straight;
}

void get_next_password(char s[])
{
	do {
		for (size_t i = strlen(s); i--; )
			if (s[i] == 'z')
				s[i] = 'a';	// rollover and continue
			else {
				++s[i];		// change char and check
				break;
			}

	} while (!isvalid(s));
}

enum { inmax = 9 };		// input password is limited to 8 chars

int main()
{
	puts("Advent of Code 2015");
	puts("Day 11: Corporate Policy");

	char input[inmax];
	if (!fgets(input, inmax, stdin))
		return EXIT_FAILURE;

	get_next_password(input);
	printf("Part 1: %s\n", input);

	get_next_password(input);
	printf("Part 2: %s\n", input);

	return EXIT_SUCCESS;
}
