#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

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

typedef int (*cmpf)(const void*, const void*);

int intcmp(const int* a, const int* b)
{
	return *a - *b;
}

int even_division(int* p, size_t len)
{
	for (int* end = p+len-1; --len; --end)
		for (int* start = p; *start <= *end / 2; ++start)
			if (*end % *start == 0)
				return *end / *start;
	return 0;
}

int main()
{
	puts("Advent of Code 2017 Day 2");
	puts("Corruption Checksum");

	size_t len = 64;
	char* line = malloc(len);

	int checksum1 = 0;
	int checksum2 = 0;
	while (getline(&line, &len, stdin) != -1) {
		int max = 0, min = INT_MAX;
		char* p = line;
		char* q = p;

		struct vector v;
		vector_init(&v);

		for (int val; (val = strtol(p, &q, 10)) > 0; p = q) {
			vector_push(&v, val);
			if (val > max)
				max = val;
			if (val < min)
				min = val;
		}
		checksum1 += max - min;

		qsort(v.elem, v.sz, sizeof(int), (cmpf)intcmp);
		checksum2 += even_division(v.elem, v.sz);
		vector_free(&v);
	}
	free(line);

	printf("Part 1: %d\n", checksum1);
	printf("Part 2: %d\n", checksum2);

	return EXIT_SUCCESS;
}
