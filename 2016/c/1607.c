#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <sxc_string.h>
#include <sxc_vector.h>
#include <sxc_vector_types.h>

/*
struct Char_vector {
	size_t siz;
	size_t cap;
	char* vec[4];
};
*/

int has_abba(const char* s)
{
	char a = s[0];
	char b = a && s[1] ? s[1] : '\0';
	char c = b && s[2] ? s[2] : '\0';
	char d = c && s[3] ? s[3] : '\0';

	return isalpha(a) && isalpha(b) && isalpha(c) && isalpha(d) &&
			a == d && b == c && a != b;
}

int has_aba(const char* s)
{
	char a = s[0];
	char b = a && s[1] ? s[1] : '\0';
	char c = b && s[2] ? s[2] : '\0';

	return a == c && a != b;
}

const char* ip_supports_tls(const char* s)
{
	int supports = 0;
	for (int hypernet = 0; *s; ++s) {
		switch (*s) {
		case '[':	hypernet = 1;	break;
		case ']':	hypernet = 0;	break;
		default:
			if (supports && !hypernet) {
				break;
			} else if (has_abba(s)) {
				if (hypernet)
					return NULL;
				else
					supports = 1;
			}
		}
	}
	return supports ? s : NULL;
}

int ssl_cmp(char** a, const char* b)
{
	char tmp[4] = {
		[0] = b[1],
		[1] = b[0],
		[2] = b[1],
		[3] = '\0'
	};
	return strcmp(*a, tmp);
}

int ip_supports_ssl(const char* s)
{
	int supports = 0;
	struct Charp_vector vaba;
	sxc_vector_init(vaba);
	struct Charp_vector vbab;
	sxc_vector_init(vbab);

	for (int hypernet = 0; *s; ++s) {
		switch (*s) {
		case '[':	hypernet = 1;	break;
		case ']':	hypernet = 0;	break;
		default:
			if (hypernet && has_aba(s)) {		// bab stuff
				sxc_vector_push(vbab, strndup(s, 3));
			} else if (!hypernet && has_aba(s)) {	// aba stuff
				sxc_vector_push(vaba, strndup(s, 3));
			}
		}
	}
	char** found = NULL;
	for (size_t i = 0; i < sxc_vector_size(vaba); ++i) {
		char* p = sxc_vector_get(vaba, i);
		sxc_vector_find(vbab, p, ssl_cmp, found);
	}

	if (found) supports = 1;

	for (size_t i = 0; i < sxc_vector_size(vaba); ++i)
		free(sxc_vector_get(vaba, i));
	sxc_vector_free(vaba);
	for (size_t i = 0; i < sxc_vector_size(vbab); ++i)
		free(sxc_vector_get(vbab, i));
	sxc_vector_free(vbab);

	return supports;
}

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 7: Internet Protocol Version 7\n");

	int part1 = 0;
	int part2 = 0;

	String s;
	sxc_string_init(&s);
	for ( ; sxc_getline(stdin, &s); sxc_string_clear(&s)) {
		if (ip_supports_tls(s.str))
			++part1;
		if (ip_supports_ssl(s.str))
			++part2;
	}
	sxc_string_free(&s);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	return EXIT_SUCCESS;
}

