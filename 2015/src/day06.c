#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// should be lib'd out..
int readline(char* line_buff, size_t buff_max)
{
	if (line_buff == NULL || (buff_max == 0))
		return 0;

	int len = 0;
	for (int c; --buff_max && ((c = getchar()) != EOF) && (c != '\n'); ) {
		*line_buff++ = c;
		++len;
	}
	*line_buff = '\0';
	return len;
}

enum buff_sizes {
	op_max = 4,		// "turn" operation (on/off)
	line_max = 64,		// enough to read input by lines
	arr_dim = 1000,		// single dimension of two dimension array
	arr_max = 1000000	// number of elements in light array
};

enum ctrl { toggle, turn_on, turn_off };
enum state { on = '#', off = ' ' };

void switch_lights(char lights[][arr_dim], enum ctrl ct, int x1, int y1,
		int x2, int y2)
{
	for (int y = y1; y <= y2; ++y)
		for (int x = x1; x <= x2; ++x) {
			char* c = &lights[y][x];
			switch (ct) {
			case turn_on:	*c = on;			break;
			case turn_off:	*c = off;			break;
			case toggle:	*c = *c == off ? on : off;	break;
			}
		}
}

void adjust_lights(int* lights, enum ctrl ct, int x1, int y1, int x2, int y2)
{
	for (int y = y1; y <= y2; ++y)
		for (int x = x1; x <= x2; ++x) {
			int* n = &lights[y * arr_dim + x];
			switch (ct) {
			case turn_on:	*n += 1;			break;
			case turn_off:	*n = *n > 0 ? *n - 1 : 0;	break;
			case toggle:	*n += 2;			break;
			}
		}
}

void parse_and_dispatch(char lights1[][arr_dim], int* lights2)
{
	static const char* togg = "toggle %d,%d through %d,%d";
	static const char* turn	= "turn %s %d,%d through %d,%d";

	char line[line_max];
	for (int nread; (nread = readline(line, line_max)) != 0; ) {
		int x1, y1, x2, y2;
		enum ctrl ct;
		if (sscanf(line, togg, &x1, &y1, &x2, &y2) == 4)
			ct = toggle;
		else {
			char op[op_max];
			sscanf(line, turn, op, &x1, &y1, &x2, &y2);
			ct = strcmp(op, "on") == 0 ? turn_on : turn_off;
		}
		switch_lights(lights1, ct, x1, y1, x2, y2);
		adjust_lights(lights2, ct, x1, y1, x2, y2);
	}
}

int count_lights(char lights[][arr_dim], char ch)
{
	int count = 0;
	for (int i = 0; i < arr_dim; ++i)
		for (int j = 0; j < arr_dim; ++j)
			if (lights[i][j] == ch)
				++count;
	return count;
}

void set_all_lights(char lights[][arr_dim], char ch)
{
	for (int i = 0; i < arr_dim; ++i)
		for (int j = 0; j < arr_dim; ++j)
			lights[i][j] = ch;
}

int accumulate(const int arr[], int len, int n)
{
	for (int i = 0; i < len; ++i)
		n += arr[i];
	return n;
}

int main()
	// Two 1000x1000 grids. Char grid treated as 2-dimensional array,
	// int grid treated as one big array. The int array was easier to
	// initialize and work with.
{
	char lights1[arr_dim][arr_dim];
	set_all_lights(lights1, off);		// is there a better way?

	int lights2[arr_max] = { 0 };

	parse_and_dispatch(lights1, lights2);

	int part1 = count_lights(lights1, on);
	int part2 = accumulate(lights2, arr_max, 0);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

