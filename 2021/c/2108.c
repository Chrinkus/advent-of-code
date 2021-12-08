#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aoc_banner.h"
#include <sxc_vector.h>

enum {
	NUM_SEGMENTS = 7,
	SEGBUFF = NUM_SEGMENTS + 1,
	NUM_PATS = 10,
	NUM_OUTS = 4,
};

struct Entry {
	char pats[NUM_PATS][SEGBUFF];
	char outs[NUM_OUTS][SEGBUFF];
};

struct Entry_vector {
	size_t siz;
	size_t cap;
	struct Entry* vec;
};

int count_unique_output(struct Entry_vector* v)
{
	int count = 0;
	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		struct Entry* p = sxc_vector_getp(v, i);
		for (int j = 0; j < NUM_OUTS; ++j)
			switch (strlen(p->outs[j])) {
			case 2:	// '1'
			case 4:	// '4'
			case 3:	// '7'
			case 7: // '8'
				++count;
				break;
			}
	}
	return count;
}

// Part 2
uint8_t atou8(const char* s)
{
	uint8_t ret = 0;
	for (uint8_t n = 0; *s; ++s) {
		n = *s - 'a';
		ret |= 1U << n;
	}
	return ret;
}

int len_cmp(const void* a, const void* b)
{
	const char* s1 = (const char*)a;
	const char* s2 = (const char*)b;

	return strlen(s1) - strlen(s2);
}

void get_cypher(const struct Entry* entry, uint8_t* cypher, int len)
	// So.. much.. magic..
{
	// Uknowns w/n segments
	uint8_t u5s[3] = { 0 };
	uint8_t u6s[3] = { 0 };

	// Set '1', '4', '7' and '8', sort unknowns
	for (int i = 0, j = 0, k = 0; i < len; ++i)
		switch (i) {
		case 0:	cypher[1] = atou8(entry->pats[i]);	break;
		case 1:	cypher[7] = atou8(entry->pats[i]);	break;
		case 2:	cypher[4] = atou8(entry->pats[i]);	break;
		case 9:	cypher[8] = atou8(entry->pats[i]);	break;
		case 3:	/* fallthrough */
		case 4:	/* fallthrough */
		case 5:	u5s[j++] = atou8(entry->pats[i]);	break;
		case 6:	/* fallthrough */
		case 7:	/* fallthrough */
		case 8:	u6s[k++] = atou8(entry->pats[i]);	break;
		}

	// Find '3' from the 5's using '1'
	for (int i = 0; i < 3; ++i)
		if ((u5s[i] & cypher[1]) == cypher[1]) {
			cypher[3] = u5s[i];
			u5s[i] = 0;
		}
	// Find '9' from the 6's using '3'
	for (int i = 0; i < 3; ++i)
		if ((u6s[i] & cypher[3]) == cypher[3]) {
			cypher[9] = u6s[i];
			u6s[i] = 0;
		}
	// Set '5' and '2'
	for (int i = 0; i < 3; ++i)
		if (u5s[i]) {
			if ((cypher[9] & u5s[i]) == u5s[i])
				cypher[5] = u5s[i];
			else
				cypher[2] = u5s[i];
		}
	// Set '6' and '0'
	for (int i = 0; i < 3; ++i)
		if (u6s[i]) {
			if ((cypher[5] & u6s[i]) == cypher[5])
				cypher[6] = u6s[i];
			else
				cypher[0] = u6s[i];
		}
}

int decode_digit(const uint8_t* cypher, int len, const uint8_t bits)
{
	for (int i = 0; i < len; ++i)
		if (cypher[i] == bits)
			return i;
	return -1;
}

int get_decoded_output(const struct Entry* ent)
{
	int output = 0;

	uint8_t cypher[NUM_PATS] = { 0 };
	get_cypher(ent, cypher, NUM_PATS);

	for (int i = 0; i < NUM_OUTS; ++i) {
		uint8_t digit = decode_digit(cypher, NUM_PATS,
				atou8(ent->outs[i]));
		output = output * 10 + digit;
	}

	return output;
}

int sum_outputs(const struct Entry_vector* v)
{
	int sum = 0;

	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		struct Entry* p = sxc_vector_getp(v, i);

		qsort(p->pats, NUM_PATS, sizeof(p->pats[0]), len_cmp);
		sum += get_decoded_output(p);
	}

	return sum;
}

int main()
{
	aoc_banner_2021("08", "Seven Segment Search");

	struct Entry_vector input;
	sxc_vector_init(&input);
	while (!feof(stdin)) {
		struct Entry* p;
		sxc_vector_emplace(&input, p);
		for (int i = 0; i < NUM_PATS &&
				scanf("%s ", p->pats[i]) == 1; ++i)
			;
		getchar();	// burn '|'

		for (int i = 0; i < NUM_OUTS &&
				scanf("%s ", p->outs[i]) == 1; ++i)
			;
	}

	int part1 = count_unique_output(&input);
	int part2 = sum_outputs(&input);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	sxc_vector_free(&input);

	return EXIT_SUCCESS;
}

