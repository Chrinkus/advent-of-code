#include <stdlib.h>
#include <stdio.h>

#include <sxc_utils.h>

enum { READBUFF = 16, NUM_COLS = 8 };

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 6: Signals and Noise\n");

	struct Charmap maps[NUM_COLS];
	for (int i = 0; i < NUM_COLS; ++i)
		charmap_init(&maps[i], SXC_ALPHA_LOWER);

	for (char buff[READBUFF]; fgets(buff, READBUFF, stdin); )
		for (int i = 0; buff[i] != '\n'; ++i)
			charmap_inc(&maps[i], buff[i]);

	char part1[NUM_COLS+1];
	char part2[NUM_COLS+1];
	for (int i = 0; i < NUM_COLS; ++i) {
		// Part 1
		struct Charmap* cm = &maps[i];
		charmap_sort(cm);
		part1[i] = charmap_sorted_most(cm)->ch;

		// Part 2
		part2[i] = charmap_sorted_least(cm)->ch;
	}
	part1[NUM_COLS] = '\0';
	part2[NUM_COLS] = '\0';


	printf("Part 1: %s\n", part1);
	printf("Part 2: %s\n", part2);

	return EXIT_SUCCESS;
}

