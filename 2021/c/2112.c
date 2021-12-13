#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

#include "aoc_banner.h"

#include <sxc_vector.h>
#include <sxc_string.h>

enum Constants { NAMEBUFF = 8 , SEP = '-' };

struct Con_read {
	char a[NAMEBUFF];
	char b[NAMEBUFF];
};

SXC_NEW_VECTOR(Con_read_vector, struct Con_read);

struct Connection;

struct Cave {
	uint64_t id;
	char name[NAMEBUFF];
	struct Connection* next;
};

SXC_NEW_VECTOR(Cave_vector, struct Cave);

struct Connection {
	struct Cave* cave;
	struct Connection* next;
};

int cave_find_cmp(const struct Cave* cave, const char* name)
{
	return strcmp(cave->name, name);
}

void read_connections(Con_read_vector* cons)
{
	String s;
	sxc_string_init(&s);
	for (int n, i; (n = sxc_getline(stdin, &s)) > 0; sxc_string_clear(&s)) {
		struct Con_read* con;
		sxc_vector_emplace(cons, con);

		const char* t = sxc_string_str(&s);

		for (i = 0; *t && *t != SEP; ++i, ++t)
			con->a[i] = *t;
		con->a[i] = '\0';

		for (i = 0; *++t; ++i)
			con->b[i] = *t;
		con->b[i] = '\0';
	}
	sxc_string_free(&s);
}

void print_connection(struct Con_read* con, void* data)
{
	SXC_UNUSED(data);

	printf("%s connects to %s\n", con->a, con->b);
}

void print_cave(struct Cave* cave, void* data)
{
	SXC_UNUSED(data);

	printf("Cave %s has id %lu\n", cave->name, cave->id);
	printf("\t and connections to ");
	for (const struct Connection* p = cave->next; p; p = p->next)
		printf("%s, ", p->cave->name);
	printf("\n");
}

void insert_cave(Cave_vector* caves, const char* name, size_t* id_gen)
{
	struct Cave* p = NULL;
	sxc_vector_find(caves, name, cave_find_cmp, p);

	if (!p) {
		sxc_vector_emplace(caves, p);
		strncpy(p->name, name, NAMEBUFF-1);
		p->id = strcmp(p->name, "start") == 0 ? 0 : (*id_gen)++;
		p->next = NULL;
	}
}

void connect_caves(const Con_read_vector* cons, const Cave_vector* caves)
{
	for (size_t i = 0; i < sxc_vector_size(cons); ++i) {
		const struct Con_read* p = sxc_vector_getp(cons, i);

		struct Cave* a;
		sxc_vector_find(caves, p->a, cave_find_cmp, a);
		struct Cave* b;
		sxc_vector_find(caves, p->b, cave_find_cmp, b);

		struct Connection* q = malloc(sizeof(struct Connection));
		q->cave = b;
		q->next = a->next;
		a->next = q;

		q = malloc(sizeof(struct Connection));
		q->cave = a;
		q->next = b->next;
		b->next = q;
	}
}

void identify_caves(const Con_read_vector* cons, Cave_vector* caves)
{
	for (size_t i = 0, id_gen = 1; i < sxc_vector_size(cons); ++i) {
		const struct Con_read* p = sxc_vector_getp(cons, i);

		insert_cave(caves, p->a, &id_gen);
		insert_cave(caves, p->b, &id_gen);
	}
}

void find_end(const struct Connection* con, int* count, uint32_t paint)
{
	if (!con)	return;			// end of connections
	find_end(con->next, count, paint);

	const struct Cave* cave = con->cave;
	if (cave->id == 0)			// back at start, return
		return;
	if (strcmp(cave->name, "end") == 0) {	// found end! inc and return
		++(*count);
		return;
	}

	uint32_t visited = 1U << cave->id;
	if (islower(cave->name[0]) && ((paint & visited) == visited))
		return;			// small cave, already visited
	paint |= visited;
	find_end(cave->next, count, paint);
}

int count_paths_1(const Cave_vector* caves)
{
	int ret = 0;

	const struct Cave* src;
	sxc_vector_find(caves, "start", cave_find_cmp, src);

	find_end(src->next, &ret, 0U);
	return ret;
}

void find_end_2(const struct Connection* con, int* count, uint32_t paint,
		bool twice)
{
	if (!con)	return;			// end of connections
	find_end_2(con->next, count, paint, twice);

	const struct Cave* cave = con->cave;
	if (cave->id == 0)			// back at start, return
		return;
	if (strcmp(cave->name, "end") == 0) {	// found end! inc and return
		++(*count);
		return;
	}

	uint32_t visited = 1U << cave->id;
	if (islower(cave->name[0]) && ((paint & visited) == visited)) {
		if (twice)
			return;
		else
			twice = 1;
	}
	paint |= visited;
	find_end_2(cave->next, count, paint, twice);
}

int count_paths_2(const Cave_vector* caves)
{
	int ret = 0;

	const struct Cave* src;
	sxc_vector_find(caves, "start", cave_find_cmp, src);

	find_end_2(src->next, &ret, 0U, 0);
	return ret;
}

int main()
{
	aoc_banner_2021("12", "Passage Pathing");

	Con_read_vector cons;
	sxc_vector_init(&cons);
	read_connections(&cons);
	//sxc_vector_foreach(&cons, print_connection, NULL);
	
	Cave_vector caves;	// Adj list
	sxc_vector_init(&caves);
	identify_caves(&cons, &caves);
	connect_caves(&cons, &caves);
	//sxc_vector_foreach(&caves, print_cave, NULL);

	int part1 = count_paths_1(&caves);
	int part2 = count_paths_2(&caves);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	sxc_vector_free(&cons);

	return EXIT_SUCCESS;
}

