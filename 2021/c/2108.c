#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aoc_banner.h"

enum {
	NUM_SEGMENTS = 7,
	SEGBUFF = NUM_SEGMENTS + 1,
	NUM_PATS = 10,
	NUM_OUTS = 4,
};

uint8_t atou8(const char* s)
{
	uint8_t ret = 0;
	for ( ; *s; ++s)
		ret |= 1U << (*s - 'a');
	return ret;
}

int len_cmp(const void* a, const void* b)
{
	const char* s1 = (const char*)a;
	const char* s2 = (const char*)b;

	return strlen(s1) - strlen(s2);
}

void solve_cipher(char pats[][SEGBUFF], int len, uint8_t* cipher)
{
	uint8_t mask = (1U << 7) - 1;
	for (int i = 0; i < len; ++i) {
		uint8_t tmp = atou8(pats[i]);
		switch (strlen(pats[i])) {
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

int decode_output(char outs[][SEGBUFF], int outs_len, uint8_t* cipher,
		int cipher_len)
{
	int output = 0;
	for (int i = 0; i < outs_len; ++i)
		output = output * 10 + decode_digit(cipher, cipher_len,
				atou8(outs[i]));
	return output;
}

int main()
{
	aoc_banner_2021("08", "Seven Segment Search");

	int part1 = 0;
	int part2 = 0;
	while (!feof(stdin)) {
		// Reads and part 1
		char pats[NUM_PATS][SEGBUFF];
		char outs[NUM_OUTS][SEGBUFF];
		for (int i = 0; i < NUM_PATS && scanf("%s ", pats[i]) == 1; ++i)
			;
		getchar();	// burn '|'

		for (int i = 0; i < NUM_OUTS && scanf("%s ", outs[i]) == 1; ++i)
			switch (strlen(outs[i])) {
			case 2:	case 3: case 4: case 7:
				++part1;
			}

		// Part 2
		qsort(pats, NUM_PATS, sizeof(pats[0]), len_cmp);
		uint8_t cipher[NUM_PATS] = { 0 };
		solve_cipher(pats, NUM_PATS, cipher);

		part2 += decode_output(outs, NUM_OUTS, cipher, NUM_PATS);
	}

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	return EXIT_SUCCESS;
}

