#include <stdlib.h>
#include <stdio.h>

enum { 
	BUFFMAX = 16,
	MIN = 0,
	// Part 1
	GRID_SIZE_1 = 3,
	MAX1 = GRID_SIZE_1 - 1, 
	MID1 = GRID_SIZE_1 / 2,
	// Part 2
	GRID_SIZE_2 = 5,
	MAX2 = GRID_SIZE_2 - 1,
	MID2 = GRID_SIZE_2 / 2,
};

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 2: Bathroom Security\n");

	// Part 1
	int keypad_1[GRID_SIZE_1][GRID_SIZE_1] = {
		[0] = { '1', '2', '3' },
		[1] = { '4', '5', '6' },
		[2] = { '7', '8', '9' },
	};
	char code_1[BUFFMAX];
	char* p = code_1;

	// Part 2
	int keypad_2[GRID_SIZE_2][GRID_SIZE_2] = {
		[0] = { ' ', ' ', '1', ' ', ' ' },
		[1] = { ' ', '2', '3', '4', ' ' },
		[2] = { '5', '6', '7', '8', '9' },
		[3] = { ' ', 'A', 'B', 'C', ' ' },
		[4] = { ' ', ' ', 'D', ' ', ' ' },
	};
	char code_2[BUFFMAX];
	char* q = code_2;
	
	for (int x1 = MID1, y1 = MID1, x2 = MIN, y2 = MID2; 1; ) {
		int c;
		int num_read = 0;
		while ((c = getchar()) != '\n' && c != EOF) {
			// Part 1 - stay in a grid
			switch (c) {
			case 'U':	if (y1 > MIN)	--y1;	break;
			case 'D':	if (y1 < MAX1)	++y1;	break;
			case 'L':	if (x1 > MIN)	--x1;	break;
			case 'R':	if (x1 < MAX1)	++x1;	break;
			}
			// Part 2 - stay in a diamond
			switch (c) {
			case 'U':
				if ((y2 > MIN+1 && x2 > MIN && x2 < MAX2)
						|| (y2 == MIN+1 && x2 == MID2))
					--y2;
				break;
			case 'D':
				if ((y2 < MAX2-1 && x2 > MIN && x2 < MAX2)
						|| (y2 == MAX2-1 && x2 == MID2))
					++y2;
				break;
			case 'L':
				if ((x2 > MIN+1 && y2 > MIN && y2 < MAX2)
						|| (x2 == MIN+1 && y2 == MID2))
					--x2;
				break;
			case 'R':
				if ((x2 < MAX2-1 && y2 > MIN && y2 < MAX2)
						|| (x2 == MAX2-1 && y2 == MID2))
					++x2;
				break;
			}
			++num_read;
		}
		if (num_read) {
			*p++ = keypad_1[y1][x1];
			*q++ = keypad_2[y2][x2];
		}

		if (c == EOF)
			break;
	}
	*p = '\0';
	*q = '\0';

	printf("Part 1: %s\n", code_1);
	printf("Part 2: %s\n", code_2);

	return EXIT_SUCCESS;
}
