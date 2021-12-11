#include <stdlib.h>
#include <stdio.h>

#include "aoc_banner.h"

enum {
	COLS = 10,
	ROWS = 10,
	FLASH_AT = 9,
	FLASH = -1,
	PART_1_STEPS = 100,
	ALL_FLASH = 100,
};

void print_octos(int octos[][COLS])
{
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j)
			printf("%d", octos[i][j]);
		printf("\n");
	}
}

void increase_octos(int octos[][COLS])
{
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j)
			++octos[i][j];
	}
}

int is_valid(int x, int y)
{
	return (0 <= x && x < COLS) && (0 <= y && y < ROWS);
}

void flash_neighbours(int octos[][COLS], int x, int y)
{
	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
			if ((i || j) && is_valid(x+j, y+i) &&
					(octos[y+i][x+j] != FLASH))
				++octos[y+i][x+j];
}

int flash_octos(int octos[][COLS])
{
	int count = 0;
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
			if (octos[i][j] > FLASH_AT) {
				flash_neighbours(octos, j, i);
				octos[i][j] = FLASH;
				++count;
			}
	return count;
}

void reset_flashes(int octos[][COLS])
{
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
			if (octos[i][j] == -1)
				octos[i][j] = 0;
}

int step_octos(int octos[][COLS])
{
	int sum = 0;
	increase_octos(octos);
	for (int n; (n = flash_octos(octos)); )
		sum += n;
	reset_flashes(octos);
	return sum;
}

int main()
{
	aoc_banner_2021("11", "Dumbo Octapus");

	int octos[ROWS][COLS];
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0, c; (c = getchar()) != '\n' && j < COLS; ++j)
			octos[i][j] = c - '0';

	int part1 = 0;
	int part2 = 0;
	for (/*int i = 0*/; part2 < PART_1_STEPS; ++part2)
		part1 += step_octos(octos);
	
	for (int sum = 0; sum != ALL_FLASH; ++part2)
		sum = step_octos(octos);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	return EXIT_SUCCESS;
}

