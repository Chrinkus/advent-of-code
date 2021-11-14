#include <stdlib.h>
#include <stdio.h>

int is_triangle(int x, int y, int z)
{
	return (x + y > z) && (x + z > y) && (y + z > x);
}

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 03: Squares With Three Sides\n");

	int count1 = 0;
	int count2 = 0;
	for (int x, y, z, a[3][3], r = 0;
			scanf("%d %d %d\n", &x, &y, &z) == 3; ++r, r %= 3) {
		if (is_triangle(x, y, z))
			++count1;

		a[r][0] = x;
		a[r][1] = y;
		a[r][2] = z;

		if (r == 2)
			for (int j = 0; j < 3; ++j)
				if (is_triangle(a[0][j], a[1][j], a[2][j]))
					++count2;
	}

	printf("Part 1: %d\n", count1);
	printf("Part 2: %d\n", count2);

	return EXIT_SUCCESS;
}

