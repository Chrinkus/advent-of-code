#include <stdlib.h>
#include <stdio.h>

#define NUM_SIDES 3

int cmp_ints(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int calc_paper(const int box[NUM_SIDES])
{
	int sum = box[0] * box[1];	// init to 'extra' paper
	for (int i = 0, j = 1; i < NUM_SIDES; ++i, j = (j+1) % NUM_SIDES)
		sum += box[i] * box[j] * 2;
	return sum;
}

int calc_ribbon(const int box[NUM_SIDES])
{
	return box[0] * box[1] * box[2] + 2 * box[0] + 2 * box[1];
}

int main()
{
	int paper = 0, ribbon = 0, box[NUM_SIDES];

	while (scanf("%dx%dx%d\n", &box[0], &box[1], &box[2]) != EOF) {
		qsort((void*)box, NUM_SIDES, sizeof(box[0]), cmp_ints);
		paper += calc_paper(box);
		ribbon += calc_ribbon(box);
	}

	printf("Part 1: %d\n", paper);
	printf("Part 2: %d\n", ribbon);

	return EXIT_SUCCESS;
}
