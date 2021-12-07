#include <stdlib.h>
#include <stdio.h>

#include "aoc_banner.h"

enum { BUFFMAX = 8 };

int main()
{
	aoc_banner_2021("02", "Dive!");

	int x = 0, y1 = 0, y2 = 0;	// aim == y1

	char buff[BUFFMAX];
	for (int n; scanf("%s %d", buff, &n) == 2; )
		switch (buff[0]) {
		case 'f':
			x += n;
			y2 += y1 * n;
			break;
		case 'u':
			y1 -= n;
			break;
		case 'd':
			y1 += n;
			break;
		}

	printf(TCINV "Part 1:" TCRINV " %d\n", x * y1);
	printf(TCINV "Part 2:" TCRINV " %d\n", x * y2);

	return EXIT_SUCCESS;
}

