#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aoc_banner.h"
#include <sxc_string.h>
#include <sxc_vector.h>

enum Opener { OPAREN = '(', OBRACK = '[', OBRACE = '{', OANGLE = '<' };
enum Closer { CPAREN = ')', CBRACK = ']', CBRACE = '}', CANGLE = '>' };

enum Syntax_points {
	PAREN_SYN_PTS = 3,
	BRACK_SYN_PTS = 57,
	BRACE_SYN_PTS = 1197,
	ANGLE_SYN_PTS = 25137,
};

enum Auto_points {
	PAREN_AUTO_PTS = 1,
	BRACK_AUTO_PTS = 2,
	BRACE_AUTO_PTS = 3,
	ANGLE_AUTO_PTS = 4,
	AUTO_PTS_MULT = 5,
};

int get_syn_points(char ch)
{
	switch (ch) {
	case CPAREN:	return PAREN_SYN_PTS;
	case CBRACK:	return BRACK_SYN_PTS;
	case CBRACE:	return BRACE_SYN_PTS;
	case CANGLE:	return ANGLE_SYN_PTS;
	default:	return 0;
	}
}

int64_t get_auto_points(char* const s)
{
	int64_t n = 0;
	for (char* p = s; *p; ++p)
		switch (*p) {
		case OPAREN:	n = n * AUTO_PTS_MULT + PAREN_AUTO_PTS;	break;
		case OBRACK:	n = n * AUTO_PTS_MULT + BRACK_AUTO_PTS;	break;
		case OBRACE:	n = n * AUTO_PTS_MULT + BRACE_AUTO_PTS;	break;
		case OANGLE:	n = n * AUTO_PTS_MULT + ANGLE_AUTO_PTS;	break;
		default:	return 0;
		}

	return n;
}

int get_opener(char c)
{
	switch (c) {
	case CPAREN:	return OPAREN;
	case CBRACK:	return OBRACK;
	case CBRACE:	return OBRACE;
	case CANGLE:	return OANGLE;
	default:	return 0;
	}
}

char is_corrupt(const char* s, char* const stack)
{
	for (char* top = stack; *s; ++s) {
		switch (*s) {
		case CPAREN:	// fallthrough
		case CBRACK:	// fallthrough
		case CBRACE:	// fallthrough
		case CANGLE:	// fallthrough
			if (*(--top) != get_opener(*s))
				return *s;
			break;
		default:
			*top++ = *s;
			break;
		}
		*top = '\0';
	}
	return *s;
}

void reverse_string(char* s)
{
	char* p = s + strlen(s) - 1;
	for (char tmp; s < p; ++s, --p) {
		tmp = *s;
		*s = *p;
		*p = tmp;
	}
}

// Those autotools points are so high!
struct I64_vector {
	size_t siz;
	size_t cap;
	int64_t* vec;
};

int int64_cmp(const void* a, const void* b)
{
	int64_t tmp = *(const int64_t*) a - *(const int64_t*)b;
	if (tmp < 0)
		return -1;
	if (tmp > 0)
		return 1;
	else
		return 0;
}

int main()
{
	aoc_banner_2021("10", "Syntax Scoring");

	int part1 = 0;
	struct I64_vector auto_pts;
	sxc_vector_init(&auto_pts);

	String s;
	sxc_string_init(&s);
	for (int n; (n = sxc_getline(stdin, &s)) > 0; sxc_string_clear(&s)) {
		char* const stack = malloc(n+1);

		char ch = '\0';
		if ((ch = is_corrupt(s.str, stack)))
			part1 += get_syn_points(ch);
		else {
			reverse_string(stack);
			sxc_vector_push(&auto_pts, get_auto_points(stack));
		}
		
		free(stack);
	}
	sxc_string_free(&s);

	sxc_vector_sort(&auto_pts, int64_cmp);

	size_t median = sxc_vector_size(&auto_pts) / 2;
	int64_t part2 = sxc_vector_get(&auto_pts, median);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %ld\n", part2);

	sxc_vector_free(&auto_pts);

	return EXIT_SUCCESS;
}

