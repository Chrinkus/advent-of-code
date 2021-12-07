#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sxc_string.h>
#include <sxc_vector.h>

#include "aoc_banner.h"

// Data
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
	for (int n; (n = sxc_getline(stream, &s)) > 0; sxc_string_clear(&s)) {
		sxc_vector_push(&r->data, strtol(sxc_string_str(&s), NULL, 2));
		if (!r->bits)
			r->bits = n;
	}
	r->mask = (1U << r->bits) - 1;

	sxc_string_free(&s);
	return r;
}

// Compare
typedef int (*Common_at_pos)(const struct U16_vector* v, int pos);

int most_common_bit_at_pos(const struct U16_vector* v, int pos)
{
	int count[2] = { 0, 0 };
	for (size_t i = 0; i < sxc_vector_size(v); ++i)
		++count[(sxc_vector_get(v, i) >> pos) & 1U];
	return count[1] >= count[0];
}

int least_common_bit_at_pos(const struct U16_vector* v, int pos)
{
	int count[2] = { 0, 0 };
	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		++count[(sxc_vector_get(v, i) >> pos) & 1U];
	}
	return count[1] < count[0];
}

// Part 1
uint16_t calculate_gamma(const struct Report* r)
{
	uint16_t gamma = 0;
	for (int i = 0; i < r->bits; ++i)
		if (most_common_bit_at_pos(&r->data, i))
			gamma |= 1U << i;
	return gamma;
}

// Part 2
void filter_pos_by_cmp(const struct U16_vector* v, int pos, Common_at_pos cmp,
		uint16_t* out)
{
	if (sxc_vector_size(v) == 1) {
		*out = sxc_vector_get(v, 0);
		return;
	} 

	struct U16_vector f;
	sxc_vector_init(&f);
	uint16_t common = cmp(v, pos) << pos;
	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		uint16_t n = sxc_vector_get(v, i);
		if ((n & (1U << pos)) == common)
			sxc_vector_push(&f, n);
	}

	filter_pos_by_cmp(&f, pos-1, cmp, out);
	sxc_vector_free(&f);
}

uint16_t get_diagnostic(const struct Report* r, Common_at_pos cmp)
{
	uint16_t diag = 0;

	filter_pos_by_cmp(&r->data, r->bits-1, cmp, &diag);

	return diag;
}

int main()
{
	aoc_banner_2021("03", "Binary Diagnostic");

	struct Report r = { 0 };
	sxc_vector_init(&r.data);
	if (!read_report(stdin, &r)) {
		fprintf(stderr, "Failed to read report\n");
		return EXIT_FAILURE;
	}

	uint16_t gam = calculate_gamma(&r);
	uint16_t eps = (~gam) & r.mask;
	uint16_t oxy = get_diagnostic(&r, most_common_bit_at_pos);
	uint16_t co2 = get_diagnostic(&r, least_common_bit_at_pos);

	printf(TCINV "Part 1:" TCRINV " %u\n", gam * eps);
	printf(TCINV "Part 2:" TCRINV " %u\n", oxy * co2);

	sxc_vector_free(&r.data);

	return EXIT_SUCCESS;
}

