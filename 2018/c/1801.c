#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	printf("Advent of Code 2018 Day 1: Chronal Calibration\n");

	int part1 = 0;
	int part2 = 0;
	for (int n; scanf(" %d", &n) == 1; )
		part1 += n;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

