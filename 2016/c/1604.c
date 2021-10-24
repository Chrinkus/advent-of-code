#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <aoc_vector.h>
#include <aoc_string.h>

enum { ROOM_KEYMAX = 6 };

struct Room {
	char* name;
	short id;
	char key[ROOM_KEYMAX];
};

struct Room* room_read(FILE* stream)
{
	int c;
	if ((c = getc(stream)) == EOF)
		return NULL;

	struct Room* r = malloc(sizeof(struct Room));

	// Read name
	String s;
	aoc_string_init(&s);
	do {	// already read 'c' above testing for EOF
		aoc_string_push(&s, c);
	} while ((c = getc(stream)) != EOF && !isdigit(c));
	r->name = strdup(s.str);
	aoc_string_free(&s);
	
	// Read room id
	ungetc(c, stream);
	scanf("%hd", &r->id);

	// Read key
	for (int i = 0; (c = getc(stream)) != '\n'; )
		if (isalpha(c))		// ignore '[' and ']'
			r->key[i++] = c;
	r->key[ROOM_KEYMAX-1] = '\0';

	return r;
}

void room_free(struct Room* r)
{
	free(r->name);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct charmap {
	char ch;
	short count;
};

int charmap_cmp(const void* a, const void* b)
{
	const struct charmap* cma = (const struct charmap*)a;
	const struct charmap* cmb = (const struct charmap*)b;

	return cmb->count - cma->count;		// reverse sort
}

enum { NUM_ALPHABET = 26, NUM_COMMON = 5 };

struct Room* is_valid_room(struct Room* r)
{
	// Initialize charmap
	struct charmap cm[NUM_ALPHABET];
	for (int i = 0, ch = 'a'; i < NUM_ALPHABET; ++i, ++ch)
		cm[i] = (struct charmap){ .ch = ch, .count = 0 };

	// Get char counts
	for (char* p = r->name; *p; ++p)
		if (isalpha(*p))
			++cm[*p - 'a'].count;
	
	// Sort charmap counts large to small
	qsort(&cm, NUM_ALPHABET, sizeof(struct charmap), charmap_cmp);

	// Use strchr to check for key chars in top 5 counts
	for (int i = 0; i < NUM_COMMON; ++i)
		if (!strchr(r->key, cm[i].ch))
			return NULL;
	return r;
}

void decrypt_room_name(struct Room* r)
{
	for (char* p = r->name; *p; ++p)
		if (*p == '-')
			*p = ' ';
		else
			*p = 'a' + (*p - 'a' + r->id) % 26;
}

const char* room_find_substr(const struct Room* r, const char* tar)
{
	return strstr(r->name, tar);
}

struct Room_vector {
	size_t size;
	size_t cap;
	struct Room* vec;
};

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 4: Security Through Obscurity\n");

	struct Room_vector vr;
	aoc_vector_init(vr);

	int sum = 0;
	struct Room* r;
	while ((r = room_read(stdin))) {
		if (is_valid_room(r)) {
			sum += r->id;
			aoc_vector_push(vr, *r);
		}
		free(r);
	}

	aoc_vector_foreach(vr, decrypt_room_name);
	aoc_vector_find(vr, "north", room_find_substr, r);

	printf("Part 1: %d\n", sum);
	printf("Part 2: %hd\n", r->id);

	aoc_vector_foreach(vr, room_free);
	aoc_vector_free(vr);

	return EXIT_SUCCESS;
}

