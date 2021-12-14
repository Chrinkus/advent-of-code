#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aoc_banner.h"
#include <sxc_string.h>
#include <sxc_vector.h>
#include <sxc_utils.h>

enum Constants {
	PAIRLEN = 2,
	PAIRBUFF = PAIRLEN + 1,
	PART_1_STEPS = 10,
	PART_2_STEPS = 40 - PART_1_STEPS,
};

struct Reaction {
	char pair[PAIRBUFF];
	char ins;
	int ileft;		// index to left reacted pair
	int iright;		// index to right reacted pair
	int64_t count;
};

SXC_NEW_VECTOR(Reaction_vec, struct Reaction);

int reaction_cmp(const void* a, const void* b)
{
	const struct Reaction* r1 = (const struct Reaction*)a;
	const struct Reaction* r2 = (const struct Reaction*)b;
	return strcmp(r1->pair, r2->pair);
}

int reaction_find(const struct Reaction* r, const char* pair)
{
	return strcmp(r->pair, pair);
}

void print_reaction(const struct Reaction* r, void* data)
{
	SXC_UNUSED(data);

	printf("%s->%c, left: %d,\tright: %d,\tcount: %ld\n",
			r->pair,
			r->ins,
			r->ileft,
			r->iright,
			r->count);
}

void connect_rules(struct Reaction* r, void* data)
{
	Reaction_vec* rv = (Reaction_vec*)data;

	char  left[PAIRBUFF] = { r->pair[0], r->ins, '\0' };
	char right[PAIRBUFF] = { r->ins, r->pair[1], '\0' };

	for (int i = 0; i < (int)sxc_vector_size(rv); ++i) {
		const struct Reaction* p = sxc_vector_getp(rv, i);
		if (strcmp(left,  p->pair) == 0) r->ileft  = i;
		if (strcmp(right, p->pair) == 0) r->iright = i;
	}
	r->count = 0;
}

void set_initial_state(const String* poly, Reaction_vec* rv)
{
	for (const char* p = sxc_string_str(poly); *(p+1); ++p) {
		char pair[PAIRBUFF];
		strncpy(pair, p, PAIRLEN);
		struct Reaction* r = NULL;
		sxc_vector_find(rv, pair, reaction_find, r);
		if (r)
			++r->count;
	}
}

void react_polymer(Reaction_vec* rv)
{
	size_t size = sxc_vector_size(rv);
	int64_t* counts = calloc(size, sizeof(int64_t));

	for (size_t i = 0; i < size; ++i) {
		const struct Reaction* p = sxc_vector_getp(rv, i);
		counts[p->ileft] += p->count;
		counts[p->iright] += p->count;
	}

	for (size_t i = 0; i < size; ++i) {
		struct Reaction* p = sxc_vector_getp(rv, i);
		p->count = counts[i];	// counts replace!!
	}
	free(counts);
}

void count_chars(const struct Reaction* r, void* data)
{
	struct Charmap* cm = (struct Charmap*)data;

	charmap_add(cm, r->pair[0], r->count);
	charmap_add(cm, r->pair[1], r->count);
}

int64_t most_minus_least(const Reaction_vec* rv, char first, char last)
{
	struct Charmap cm;
	charmap_init(&cm, SXC_ALPHA_UPPER);
	sxc_vector_foreach(rv, count_chars, &cm);

	// Account for duplicates
	for (size_t i = 0; i < cm.num_chars; ++i) {
		struct Charcount* p = charmap_get_index(&cm, i);
		p->count /= 2;
	}
	charmap_inc(&cm, first);	// first not duplicated
	charmap_inc(&cm, last);		// last not duplicated
	charmap_sort(&cm);

	int64_t most = charmap_sorted_most(&cm)->count;
	int64_t least = charmap_sorted_least(&cm)->count;

	charmap_free(&cm);
	return most - least;
}

void run_reactions(Reaction_vec* rv, int n)
{
	while (n--)
		react_polymer(rv);
}

int main()
{
	aoc_banner_2021("14", "Extended Polymerization");

	String polymer;
	sxc_string_init(&polymer);
	sxc_getline(stdin, &polymer);
	const char first = sxc_string_head(&polymer);
	const char last  = sxc_string_tail(&polymer);

	Reaction_vec rules;
	sxc_vector_init(&rules);
	for (struct Reaction r; scanf("%s -> %c ", r.pair, &r.ins) == 2;)
		sxc_vector_push(&rules, r);

	sxc_vector_foreach(&rules, connect_rules, &rules);
	set_initial_state(&polymer, &rules);

	run_reactions(&rules, PART_1_STEPS);
	int64_t part1 = most_minus_least(&rules, first, last);

	run_reactions(&rules, PART_2_STEPS);
	int64_t part2 = most_minus_least(&rules, first, last);

	printf(TCINV "Part 1:" TCRINV " %ld\n", part1);
	printf(TCINV "Part 2:" TCRINV " %ld\n", part2);

	sxc_string_free(&polymer);
	sxc_vector_free(&rules);

	return EXIT_SUCCESS;
}

