#include <stdlib.h>
#include <stdio.h>

struct params {
	int lo;
	int hi;
	char ch;
};

int main()
{
	printf("Advent of Code 2020 Day 2: Password Philosophy\n");

	int part1 = 0;
	int part2 = 0;

	for (struct params p; scanf("%d-%d %c: ", &p.lo, &p.hi, &p.ch) == 3; ) {
		int count = 0;
		int occur = 0;
		for (int n = 1, c; (c = getchar()) != EOF && c != '\n'; ++n)
			if (c == p.ch) {
				++count;
				occur += n == p.lo || n == p.hi;
			}

		part1 += p.lo <= count && count <= p.hi;
		part2 += occur % 2;	// only valid on single occurance
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

