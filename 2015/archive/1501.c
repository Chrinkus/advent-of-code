#include <stdlib.h>
#include <stdio.h>

#define UP	'('
#define DOWN	')'

int main()
{
	int floor = 0, count = 0, basement = 0;

	for (int c; (c = getchar()) != EOF; ) {
		switch (c) {
		case UP:	++floor; break;
		case DOWN:	--floor; break;
		}

		if (basement == 0 && ++count && floor < 0)
			basement = count;
	}

	printf("Part 1: %d\n", floor);
	printf("Part 2: %d\n", basement);

	return EXIT_SUCCESS;
}
