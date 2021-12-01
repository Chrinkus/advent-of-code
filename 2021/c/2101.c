#include <stdlib.h>
#include <stdio.h>

enum { WINSIZE = 3 };

int main()
{
	printf("Advent of Code 2021\n");
	printf("Day 1: Sonar Sweep\n");

	int part1 = 0;
	int part2 = 0;

	for (int prev = 0, curr, i = 0, win[WINSIZE], prev_sum = 0;
			scanf("%d", &curr) == 1; ++i) {
		// Part 1
		if (prev && curr > prev)
			++part1;
		prev = curr;

		// Part 2
		win[i % WINSIZE] = curr;

		int curr_sum = 0;
		for (int j = 0; j < WINSIZE; ++j)
			curr_sum += win[j];

		if (i >= WINSIZE)
			part2 += curr_sum > prev_sum;

		prev_sum = curr_sum;
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

