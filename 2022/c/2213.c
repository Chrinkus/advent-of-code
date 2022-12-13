#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "cgs.h"

enum Type { LIST, VALUE };

struct Packet {
        enum Type type;
        struct Packet* parent;
        union {
                struct cgs_vector list;
                int value;
        } data;
};

static struct Packet
packet_new_list(struct Packet* parent)
{
        return (struct Packet){
                .type = LIST,
                .parent = parent,
                .data.list = cgs_vector_new(sizeof(struct Packet)),
        };
}

static struct Packet
packet_new_value(struct Packet* parent, int val)
{
        return (struct Packet){
                .type = VALUE,
                .parent = parent,
                .data.value = val,
        };
}

static void*
packet_list_add(struct Packet* list, struct Packet* p)
{
        if (!cgs_vector_push(&list->data.list, p))
                return cgs_error_retnull("vector_push");
        return list;
}

static const char*
packet_parse(const char* s, struct Packet* parent)
{
        while (*s) {
                if (*s == ',') {                // advance to next char
                        ++s;
                } else if (*s == ']') {         // end of list, return s
                        return ++s;
                } else if (*s == '[') {         // create new list and recurse
                        struct Packet p = packet_new_list(parent);
                        if (!packet_list_add(parent, &p))
                                return cgs_error_retnull("packet_list_add");
                        s = packet_parse(++s, &p);
                } else if (isdigit(*s)) {       // parse int, add to parent list
                        int val = strtol(s, &s, 10);
                        struct Packet p = packet_new_value(parent, val);
                        if (!packet_list_add(parent, &p))
                                return cgs_error_retnull("packet_list_add");
                }
        }
        return s;
}

static void
packet_free(void* packet)
{
        struct Packet* p = packet;
        if (p->type == LIST)
                cgs_vector_free_all_with(&p->data.list, packet_free);
}

static void*
read_input(struct cgs_vector* input)
{
        struct cgs_string buff = cgs_string_new();
        while (!feof(stdin)) {
                if (cgs_io_getline(stdin, &buff) > 0) {
                        struct cgs_string line = cgs_string_new();
                        cgs_string_move(&buff, &line);
                        if (!cgs_vector_push(input, &line)) {
                                cgs_error_msg("vector_push");
                                goto error_cleanup;
                        }
                }
        }

        cgs_string_free(&buff);
        return input;
error_cleanup:
        cgs_string_free(&buff);
        return NULL;
}

static int
packet_cmp(const void* a, const void* b)
{
        const struct Packet* left = a;
        const struct Packet* right = b;

        return 0;
}

static int
sum_ordered_pair_indexes(const struct cgs_vector* input)
{
        int sum = 0;
        for (size_t i = 0, j = 1; i < cgs_vector_length(input) - 1; i+=2, ++j) {
                const struct cgs_string* left = cgs_vector_get(input, i);
                struct Packet pl = packet_new_list(NULL);
                if (!packet_parse(++cgs_string_data(left), &pl))
                        return cgs_error_retfail("packet_parse");

                const struct cgs_string* right = cgs_vector_get(input, i+1);
                struct Packet pr = packet_new_list(NULL);
                if (!packet_parse(++cgs_string_data(right), &pr))
                        return cgs_error_retfail("packet_parse");

                if (packet_cmp(&pl, &pr) < 0)
                        sum += j;
        }
        return sum;
}

int main(void)
{
        struct cgs_vector input = cgs_vector_new(sizeof(struct cgs_string));
        if (!read_input(&input))
                return cgs_error_retfail("read_input");

        int part1 = sum_ordered_pair_indexes(&input);
        printf("%d\n", part1);

        cgs_vector_free_all_with(&input, cgs_string_free);
        return EXIT_SUCCESS;
}
