#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aoc_banner.h"
#include <sxc_string.h>
#include <sxc_vector.h>
#include <sxc_utils.h>

struct Reaction {
	char pair[4];
	char ins;
};

SXC_NEW_VECTOR(React_vec, struct Reaction);

int reaction_cmp(const void* a, const void* b)
{
	const struct Reaction* r1 = (const struct Reaction*)a;
	const struct Reaction* r2 = (const struct Reaction*)b;

	return strcmp(r1->pair, r2->pair);
}

int rule_find_cmp(const struct Reaction* r, const char* a)
{
	return strcmp(a, r->pair);
}

void print_reaction(const struct Reaction* r, void* data)
{
	SXC_UNUSED(data);

	printf("Pair %s inserts %c\n", r->pair, r->ins);
}

void extend_polymer(String* src, const React_vec* rules)
{
	String dst;
	sxc_string_init(&dst);
	for (const char* p = sxc_string_str(src); *p; ++p) {
		sxc_string_push(&dst, *p);

		char tmp[4];
		strncpy(tmp, p, 2);
		
		struct Reaction* r = NULL;
		sxc_vector_find(rules, tmp, rule_find_cmp, r);
		if (r) {
			sxc_string_push(&dst, r->ins);
		}
	}
	// need sxc_string_move or _copy
	free(src->str);
	src->str = dst.str;
	src->siz = dst.siz;
	src->cap = dst.cap;
}

int most_minus_least(const String* s)
{
	struct Charmap cm;
	charmap_init(&cm, SXC_ALPHA_UPPER);
	charmap_count_chars(&cm, sxc_string_str(s));
	charmap_sort(&cm);

	const struct Charcount* most = charmap_sorted_most(&cm);
	const struct Charcount* least = charmap_sorted_least(&cm);

	int res = most->count - least->count;

	charmap_free(&cm);

	return res;
}

int main()
{
	aoc_banner_2021("14", "Extended Polymerization");

	String polymer;
	sxc_string_init(&polymer);
	sxc_getline(stdin, &polymer);

	React_vec rules;
	sxc_vector_init(&rules);
	for (struct Reaction r; scanf(" %s -> %c ", r.pair, &r.ins) == 2; )
		sxc_vector_push(&rules, r);

	//sxc_vector_sort(&rules, reaction_cmp);
	for (int i = 0; i < 10; ++i) {
		extend_polymer(&polymer, &rules);
	}

	int part1 = most_minus_least(&polymer);
	int part2 = 0;

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	sxc_string_free(&polymer);
	sxc_vector_free(&rules);

	return EXIT_SUCCESS;
}

