#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <sxc_vector.h>
#include <sxc_string.h>
#include <sxc_utils.h>

enum { ROOM_KEYMAX = 6, NUM_COMMON = 5 };

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
	sxc_string_init(&s);
	do {	// already read 'c' above testing for EOF
		sxc_string_push(&s, c);
	} while ((c = getc(stream)) != EOF && !isdigit(c));
	r->name = strdup(s.str);
	sxc_string_free(&s);
	
	// Read room id
	ungetc(c, stream);
	if (scanf("%hd", &r->id) != 1)
		return NULL;

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

struct Room* is_valid_room(struct Room* r)
{
	struct Charmap cm;
	charmap_init(&cm, SXC_ALPHA_LOWER);

	charmap_count_chars(&cm, r->name);

	charmap_sort(&cm);

	// Use strchr to check for key chars in top 5 counts
	for (int i = 0; i < NUM_COMMON; ++i)
		if (!strchr(r->key, charmap_get_index(&cm, i)->ch))
			return NULL;

	charmap_free(&cm);
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

int room_find_substr(const struct Room* r, const char* tar)
{
	if (strstr(r->name, tar))
		return 0;
	return -1;
}

struct Room_vector {
	size_t siz;
	size_t cap;
	struct Room* vec;
};

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 4: Security Through Obscurity\n");

	struct Room_vector vr;
	sxc_vector_init(vr);

	int sum = 0;
	struct Room* r;
	while ((r = room_read(stdin))) {
		if (is_valid_room(r)) {
			sum += r->id;
			sxc_vector_push(vr, *r);
		}
		free(r);
	}

	sxc_vector_foreach(vr, decrypt_room_name);
	sxc_vector_find(vr, "north", room_find_substr, r);

	printf("Part 1: %d\n", sum);
	printf("Part 2: %hd\n", r->id);

	sxc_vector_foreach(vr, room_free);
	sxc_vector_free(vr);

	return EXIT_SUCCESS;
}

