#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <sxc_string.h>
#include <sxc_vector.h>
#include <sxc_vector_types.h>

/*
struct U16_vector {
	size_t siz;
	size_t cap;
	uint16_t* vec;
};

struct Report {
	struct U16_vector data;
	uint16_t bits;
	uint16_t mask;
};

struct Report* read_report(FILE* stream, struct Report* r)
{
	String s;
	sxc_string_init(&s);
	for ( ; sxc_getline(stream, &s); sxc_string_clear(&s))
		sxc_vector_push(r->data, strtol(sxc_stirng_str(&s), NULL, 2));

	r->bits = strlen(sxc_string_str(&s));
	r->mask = (1U << r->bits) - 1;

	sxc_string_free(&s);
	return r;
}
*/

typedef char (*Common_at_pos)(const struct Charp_vector*, int);

char most_common_at_pos(const struct Charp_vector* v, int pos)
{
	int count[2] = { 0, 0 };
	for (size_t i = 0; i < sxc_vector_size(*v); ++i) {
		++count[sxc_vector_get(*v, i)[pos] - '0'];
	}
	return count[0] > count[1] ? '0' : '1';
}

char least_common_at_pos(const struct Charp_vector* v, int pos)
{
	int count[2] = { 0, 0 };
	for (size_t i = 0; i < sxc_vector_size(*v); ++i) {
		++count[sxc_vector_get(*v, i)[pos] - '0'];
	}
	return count[0] <= count[1] ? '0' : '1';
}

// Part 1
char* calculate_gamma(const struct Charp_vector* v)
{
	int len = strlen(sxc_vector_get(*v, 0));
	char* buff = malloc(len+1);
	for (int i = 0; i < len; ++i)
		buff[i] = most_common_at_pos(v, i);
	buff[len] = '\0';
	return buff;
}

char* get_compliment(const char* s)
{
	int len = strlen(s);
	char* buff = malloc(len+1);
	for (int i = 0; i < len; ++i)
		buff[i] = s[i] == '0' ? '1' : '0';
	buff[len] = '\0';
	return buff;
}

// Part 2
void filter_by_cmp(const struct Charp_vector* v, int len, int pos,
		Common_at_pos cmp, char** out)
{
	if (sxc_vector_size(*v) == 1) {
		*out = strdup(sxc_vector_get(*v, 0));
		return;
	} 

	struct Charp_vector filtered;
	sxc_vector_init(filtered);
	char ch = cmp(v, pos);
	for (size_t j = 0; j < sxc_vector_size(*v); ++j) {
		char* p = sxc_vector_get(*v, j);
		if (p[pos] == ch)
			sxc_vector_push(filtered, p);
	}

	filter_by_cmp(&filtered, len, pos+1, cmp, out);

	sxc_vector_free(filtered);
}

char* recursive_get_diagnostic(const struct Charp_vector* v, Common_at_pos cmp)
{
	int len = strlen(sxc_vector_get(*v, 0));
	char* diag = NULL;

	filter_by_cmp(v, len, 0, cmp, &diag);

	return diag;
}

enum Diagnostics { GAM, EPS, OXY, CO2, NUM_DIAGS };

int main()
{
	printf("Advent of Code 2021\n");
	printf("Day 03: Binary Diagnostic\n");

	// Read input
	struct Charp_vector input;
	sxc_vector_init(input);

	String s;
	sxc_string_init(&s);
	for ( ; sxc_getline(stdin, &s); sxc_string_clear(&s))
		sxc_vector_push(input, strdup(sxc_string_str(&s)));
	sxc_string_free(&s);

	char* buffs[NUM_DIAGS];
	buffs[GAM] = calculate_gamma(&input);
	buffs[EPS] = get_compliment(buffs[GAM]);
	buffs[OXY] = recursive_get_diagnostic(&input, most_common_at_pos);
	buffs[CO2] = recursive_get_diagnostic(&input, least_common_at_pos);

	unsigned diags[NUM_DIAGS];
	for (int i = 0; i < NUM_DIAGS; ++i) {
		diags[i] = strtol(buffs[i], NULL, 2);
		free(buffs[i]);
	}

	for (size_t i = 0; i < sxc_vector_size(input); ++i)
		free(sxc_vector_get(input, i));
	sxc_vector_free(input);

	printf("Part 1: %u\n", diags[GAM] * diags[EPS]);
	printf("Part 2: %u\n", diags[OXY] * diags[CO2]);

	return EXIT_SUCCESS;
}

