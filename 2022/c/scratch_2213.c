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

static void
packet_print(const struct Packet* pack)
{
        if (pack->type == LIST) {
                printf("[\n");
                const struct cgs_vector* v = &pack->data.list;
                for (size_t i = 0; i < cgs_vector_length(v); ++i) {
                        const struct Packet* p = cgs_vector_get(v, i);
                        packet_print(p);
                }
                printf("]\n");
        } else if (pack->type == VALUE) {
                printf("'%d'", pack->data.value);
        }
}

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

static char*
packet_parse(char* s, struct Packet* parent)
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
                        char* pc = NULL;
                        int val = strtol(s, &pc, 10);
                        if (pc == s)
                                return cgs_error_retnull("strtol");
                        s = pc;
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
read_packets(struct cgs_vector* packets)
{
        struct cgs_string buff = cgs_string_new();

        while (!feof(stdin)) {
                if (cgs_io_getline(stdin, &buff) > 0) {
                        char* s = cgs_string_data_mut(&buff);
                        struct Packet pack = packet_new_list(NULL);
                        if (!packet_parse(++s, &pack)) {
                                cgs_error_msg("packet_parse");
                                goto error_cleanup;
                        }
                        if (!cgs_vector_push(packets, &pack)) {
                                cgs_error_msg("vector_push");
                                packet_free(&pack);
                                goto error_cleanup;
                        }
                }
                cgs_string_clear(&buff);
        }

        cgs_string_free(&buff);
        return packets;
error_cleanup:
        cgs_string_free(&buff);
        return NULL;
}

static int
packet_cmp(const void* a, const void* b)
{
        const struct Packet* left = a;
        const struct Packet* right = b;

        (void)left;
        (void)right;

        return 0;
}

static int
sum_ordered_pair_indexes(struct cgs_vector* vp)
{
        int sum = 0;
        for (size_t i = 0, j = 1; i < cgs_vector_length(vp) - 1; i+=2, ++j) {
                const struct Packet* pl = cgs_vector_get(vp, i);
                const struct Packet* pr = cgs_vector_get(vp, i+1);

                if (packet_cmp(&pl, &pr) < 0)
                        sum += j;
        }
        return sum;
}

int main(void)
{
        struct cgs_vector packets = cgs_vector_new(sizeof(struct Packet));
        if (!read_packets(&packets))
                return cgs_error_retfail("read_input");

        const struct Packet* p = cgs_vector_get(&packets, 0);
        packet_print(p);

        int part1 = sum_ordered_pair_indexes(&packets);
        printf("%d\n", part1);

        cgs_vector_free_all_with(&packets, packet_free);
        return EXIT_SUCCESS;
}
