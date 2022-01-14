#include <stdlib.h>
#include <stdio.h>

int main()
{
	printf("Advent of Code 2020 Day 2: Password Philosophy\n");

	int part1 = 0;
	int part2 = 0;
	for (int lo, hi, ch; scanf("%d-%d %c: ", &lo, &hi, (char*)&ch) == 3; ) {
		int count = 0;
		int occur = 0;
		for (int n = 1, c; (c = getchar()) != EOF && c != '\n'; ++n)
			if (c == ch) {
				++count;
				occur += n == lo || n == hi;
			}

		part1 += lo <= count && count <= hi;
		part2 += occur % 2;	// only valid on single occurance
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

