#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "cgs.h"

typedef uint16_t Sig;

enum ice { ID_BUFF = 8, };

enum gate { PASS = 1, AND, OR, NOT, LSHIFT, RSHIFT, };

struct wire {
        Sig sig;
        enum gate gate;
        char w1[ID_BUFF];
        char w2[ID_BUFF];
};

static struct wire*
wire_create(void)
{
        struct wire* w = malloc(sizeof(struct wire));
        *w = (struct wire){ 0 };
        return w;
}

// forward decl
static Sig
get_signal(struct wire* w, struct cgs_hashtab* ht);

static Sig
read_signal(const char* s, struct cgs_hashtab* ht)
{
        return isdigit(s[0])
                ? strtoul(s, NULL, 10)
                : get_signal(cgs_hashtab_lookup_mut(ht, s), ht);
}

static Sig
get_signal(struct wire* w, struct cgs_hashtab* ht)
{
        if (w->sig)
                return w->sig;

        Sig lhs = read_signal(w->w1, ht);

        if (w->gate == PASS) {          // PASS and NOT only need a single
                w->sig = lhs;           // parameter so handle them first
        } else if (w->gate == NOT) {    // without examining w->w2 since that
                w->sig = ~lhs;          // may be empty and mess with recursion
        } else {
                Sig rhs = read_signal(w->w2, ht);

                switch (w->gate) {
                case PASS:      /* handled above */     break;
                case NOT:       /* handled above */     break;
                case AND:       w->sig = lhs & rhs;     break;
                case OR:        w->sig = lhs | rhs;     break;
                case LSHIFT:    w->sig = lhs << rhs;    break;
                case RSHIFT:    w->sig = lhs >> rhs;    break;
                }
        }

        return w->sig;
}

void*
wire_table_add_wire(struct cgs_hashtab* wt, const char* line)
{
        struct wire* w = wire_create();

        char id[ID_BUFF];
        char ch;
        if (sscanf(line, "%s -> %s ", w->w1, id) == 2) {
                w->gate = PASS;
        } else if (sscanf(line, "NOT %s -> %s ", w->w1, id) == 2) {
                w->gate = NOT;
        } else if (sscanf(line, "%s AND %s -> %s ", w->w1, w->w2, id) == 3) {
                w->gate = AND;
        } else if (sscanf(line, "%s OR %s -> %s ", w->w1, w->w2, id) == 3) {
                w->gate = OR;
        } else if (sscanf(line, "%s %cSHIFT %s -> %s ",
                                w->w1, &ch, w->w2, id) == 4) {
                w->gate = ch == 'L' ? LSHIFT : RSHIFT;
        } else {
                free(w);
                return cgs_error_retnull("Could not add wire: %s", line);
        }
        struct cgs_variant* v = cgs_hashtab_insert(wt, id, NULL);
        cgs_variant_set_data(v, w);

        return wt;
}

void*
read_and_build_wire_table(struct cgs_hashtab* wt)
{
        struct cgs_string buff = cgs_string_new();

        for ( ; cgs_io_getline(stdin, &buff); cgs_string_clear(&buff)) {
                wire_table_add_wire(wt, cgs_string_data(&buff));
        }

        cgs_string_free(&buff);
        return wt;
}

static void
setup_part_2(struct cgs_hashtab* ht, Sig bval)
{
        // Reset all signals to 0
        struct cgs_hashtab_iter_mut it = cgs_hashtab_begin_mut(ht);
        while (cgs_hashtab_iter_mut_next(&it)) {
                struct cgs_variant* pv = cgs_hashtab_iter_mut_get(&it);
                struct wire* w = cgs_variant_get_mut(pv);
                w->sig = 0;
        }

        // Set wire "b" to bval
        struct wire* w = cgs_hashtab_lookup_mut(ht, "b");
        w->sig = bval;
}

int main(void)
{
        struct cgs_hashtab wt = cgs_hashtab_new(free);

        read_and_build_wire_table(&wt);

        Sig part1 = get_signal(cgs_hashtab_lookup_mut(&wt, "a"), &wt);
        printf("%hd\n", part1);

        setup_part_2(&wt, part1);

        Sig part2 = get_signal(cgs_hashtab_lookup_mut(&wt, "a"), &wt);
        printf("%hd\n", part2);

        cgs_hashtab_free(&wt);
        return EXIT_SUCCESS;
}
