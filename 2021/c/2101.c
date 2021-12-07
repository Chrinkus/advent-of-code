#include <stdlib.h>
#include <stdio.h>

#include "aoc_banner.h"

//Terse like a hearse.
/*
int main()
{
	int p1=0, p2=0;
	for (int p=0, i=0, a[3], c; scanf("%d", &c) == 1; a[i++%3] = p = c) {
		p1 += p && c > p;
		p2 += i >= 3 && c > a[i%3];
	}
	printf("%d, %d\n", p1, p2);
}
*/

enum { WINSIZE = 3 };

int main()
{
	aoc_banner_2021("01", "Sonar Sweep");

	int part1 = 0;
	int part2 = 0;

	for (int prev = 0, curr, i = 0, win[WINSIZE], prev_sum = 0;
			scanf("%d", &curr) == 1; ++i) {
		// Part 1
		part1 += prev && curr > prev;
		prev = curr;

		// Part 2
		win[i % WINSIZE] = curr;

		int curr_sum = 0;
		for (int j = 0; j < WINSIZE; ++j)
			curr_sum += win[j];

		part2 += i >= WINSIZE && curr_sum > prev_sum;
		prev_sum = curr_sum;
	}

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	return EXIT_SUCCESS;
}

