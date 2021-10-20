#include <stdlib.h>
#include <stdio.h>

#include "aoc_vector.h"

// Part 1
struct Int_vector {
	size_t size;
	size_t cap;
	size_t elem_size;
	int* vec;
};

struct Int_vector intcode_run(const struct Int_vector* src, int noun, int verb)
{
	struct Int_vector state;
	aoc_vector_copy(state, *src);

	state.vec[1] = noun;
	state.vec[2] = verb;

	int* p = state.vec;
	while (*p != 99) {
		switch (*p) {
		case 1:
			{
				int x = state.vec[*(p+1)];
				int y = state.vec[*(p+2)];
				state.vec[*(p+3)] = x + y;
				p += 4;
			}
			break;
		case 2:
			{
				int x = state.vec[*(p+1)];
				int y = state.vec[*(p+2)];
				state.vec[*(p+3)] = x * y;
				p += 4;
			}
			break;
		}
	}
	return state;
}

// Part 2
struct Int_pair {
	int noun;
	int verb;
};

struct Int_pair brute_force_solve(const struct Int_vector* prog,
		const int target)
{
	for (int i = 0; i < 100; ++i)
		for (int j = 0; j < 100; ++j) {
			struct Int_vector out = intcode_run(prog, i, j);
			int zero = out.vec[0];
			aoc_vector_free(out);

			if (zero == target)
				return (struct Int_pair){ i, j };
		}

	return (struct Int_pair){ -1, -1 };	// should never get here
}

#define calc_output(pair) 100 * (pair).noun + (pair).verb	// why not?

int main()
{
	printf("Advent of Code 2019\n");
	printf("Day 2: 1202 Program Alarm\n");

	struct Int_vector vi;
	aoc_vector_init(vi, int);

	for (int n, c = 0; c != EOF && scanf("%d", &n) == 1; c = getchar())
		aoc_vector_push(vi, n);

	struct Int_vector end_state = intcode_run(&vi, 12, 2);
	struct Int_pair inputs = brute_force_solve(&vi, 19690720);

	printf("Part 1: %d\n", end_state.vec[0]);
	printf("Part 2: %d\n", calc_output(inputs));

	aoc_vector_free(vi);

	return EXIT_SUCCESS;
}

