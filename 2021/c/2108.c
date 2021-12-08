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

void get_cipher(const struct Entry* entry, uint8_t* cipher, int len)
{
	uint8_t mask = (1U << 7) - 1;
	for (int i = 0; i < len; ++i) {
		const char* p = entry->pats[i];
		uint8_t tmp = atou8(p);
		switch (strlen(p)) {
		case 2:	cipher[1] = tmp;	break;
		case 3:	cipher[7] = tmp;	break;
		case 7:	cipher[8] = tmp;	break;
		case 4:	cipher[4] = tmp;	break;
		case 5:
			if ((~cipher[4] & tmp) == (~cipher[4] & mask))
				cipher[2] = tmp;
			else if ((cipher[1] & tmp) == cipher[1])
				cipher[3] = tmp;
			else
				cipher[5] = tmp;
			break;
		case 6:
			if ((cipher[4] & tmp) == cipher[4])
				cipher[9] = tmp;
			else if ((cipher[1] & tmp) == cipher[1])
				cipher[0] = tmp;
			else
				cipher[6] = tmp;
			break;
		}
	}
}

int decode_digit(const uint8_t* cipher, int len, const uint8_t bits)
{
	for (int i = 0; i < len; ++i)
		if (cipher[i] == bits)
			return i;
	return -1;
}

int get_decoded_output(const struct Entry* ent)
{
	int output = 0;

	uint8_t cipher[NUM_PATS] = { 0 };
	get_cipher(ent, cipher, NUM_PATS);

	for (int i = 0; i < NUM_OUTS; ++i) {
		uint8_t digit = decode_digit(cipher, NUM_PATS,
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

