#include <stdlib.h>
#include <stdio.h>

enum { BUFFMAX = 8 };

int main()
{
	printf("Advent of Code 2021\n");
	printf("Day 02: Dive!\n");

	int x = 0, y1 = 0, y2 = 0;

	char buff[BUFFMAX];
	for (int n, aim = 0; scanf("%s %d", buff, &n) == 2; ) {
		switch (buff[0]) {
		case 'f':
			x += n;
			y2 += aim * n;
			break;
		case 'u':
			y1 -= n;
			aim -= n;
			break;
		case 'd':
			y1 += n;
			aim += n;
			break;
		}
	}

	printf("Part 1: %d\n", x * y1);
	printf("Part 2: %d\n", x * y2);

	return EXIT_SUCCESS;
}

