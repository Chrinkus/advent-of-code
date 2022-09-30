#include <stdlib.h>
#include <stdio.h>

struct vector {
	int* elem;
	size_t sz;
	size_t cp;
};

enum vector_sizes { vec_init = 8 };

void vector_init(struct vector* v)
{
	v->elem = malloc(sizeof(int) * vec_init);
	v->sz = 0;
	v->cp = vec_init;
}

void vector_free(struct vector* v)
{
	free(v->elem);
}

struct vector* vector_resize(struct vector* v)
{
	size_t new_cp = 2 * v->cp;
	v->elem = realloc(v->elem, sizeof(int) * new_cp);
	if (!v->elem)
		return NULL;
	v->cp = new_cp;
	return v;

}

struct vector* vector_push(struct vector* v, int n)
{
	if (v->sz == v->cp && !vector_resize(v))
		return NULL;
	v->elem[v->sz++] = n;
	return v;
}

int main()
{
	puts("Advent of Code 2017 Day 1");
	puts("Inverse Captcha");

	struct vector v;
	vector_init(&v);

	// Part 1
	int part1 = 0;
	int first = getchar();
	int prev = first;
	vector_push(&v, first);
	for (int c; (c = getchar()) != EOF && (c != '\n'); prev = c) {
		vector_push(&v, c - '0');
		if (c == prev)
			part1 += c - '0';
	}
	if (prev == first)
		part1 += prev - '0';

	// Part 2
	int part2 = 0;
	for (size_t i = 0, j = v.sz / 2; j != v.sz; ++i, ++j)
		if (v.elem[i] == v.elem[j])
			part2 += v.elem[i] * 2;

	vector_free(&v);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

