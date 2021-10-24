#ifndef AOC_STRING_H
#define AOC_STRING_H

enum String_defaults {
	STRING_DEFAULT_CAPACITY = 16,
	STRING_GROWTH_RATE = 2,
};

typedef struct string String;
struct string {
	char*  str;
	size_t siz;
	size_t cap;
};

char* aoc_string_init(String* s)
{
	if ((s->str = malloc(STRING_DEFAULT_CAPACITY))) {
		s->str[0] = '\0';
		s->siz = 0;
		s->cap = STRING_DEFAULT_CAPACITY;
	}
	return s->str;
}

void aoc_string_free(String* s)
{
	free(s->str);
	s->str = NULL;
	s->siz = 0;
	s->cap = 0;
}

char* aoc_string_resize(String* s)
{
	size_t new_cap = s->cap > 0 ? s->cap * STRING_GROWTH_RATE
		: STRING_DEFAULT_CAPACITY;
	char* p = realloc(s->str, new_cap);
	if (p) {
		s->str = p;
		s->cap = new_cap;
	}
	return p;
}

char* aoc_string_push(String* s, int ch)
{
	if (s->siz + 1 == s->cap && !aoc_string_resize(s))
		return NULL;
	s->str[s->siz++] = ch;
	s->str[s->siz] = '\0';
	return s->str;
}

void aoc_string_reset(String* s)
{
	s->str[0] = '\0';
	s->siz = 0;
}

#endif//AOC_STRING_H

