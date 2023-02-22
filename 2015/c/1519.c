#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum { BUFFMAX = 16, };

struct replacement {
        struct cgs_string seq;
        struct cgs_string rep;
};

static struct replacement
replacement_new(void)
{
        return (struct replacement){
                .seq = cgs_string_new(),
                .rep = cgs_string_new(),
        };
}

static void
replacement_free(void* repl)
{
        struct replacement* r = repl;
        cgs_string_free(&r->seq);
        cgs_string_free(&r->rep);
}

static int
replacement_cmp(const void* a, const void* b)
{
        const struct replacement* r1 = a;
        const struct replacement* r2 = b;
        const size_t s1 = cgs_string_length(&r1->rep);
        const size_t s2 = cgs_string_length(&r2->rep);
        if (s1 < s2)
                return 1;
        if (s1 > s2)
                return -1;
        return 0;
}

static void*
read_input(struct cgs_vector* repls, struct cgs_string* input)
{
        char seq[BUFFMAX];
        char rep[BUFFMAX];

        struct cgs_string line = cgs_string_new();
        while (cgs_io_getline(stdin, &line) > 0) {
                sscanf(cgs_string_data(&line), "%s => %s ", seq, rep);

                struct replacement r = replacement_new();
                cgs_string_from(seq, &r.seq);
                cgs_string_from(rep, &r.rep);
                cgs_vector_push(repls, &r);

                cgs_string_clear(&line);
        }

        cgs_io_getline(stdin, input);

        cgs_string_free(&line);
        return repls;
}

static int
sequence_match(const char* s, const struct cgs_string* seq)
{
        return strncmp(s, cgs_string_data(seq), cgs_string_length(seq)) == 0;
}

static void*
add_molecule(const struct cgs_string* input, size_t pos,
                const struct replacement* r, struct cgs_rbt* mtree)
{
        struct cgs_string* p = malloc(sizeof(struct cgs_string));
        if (!p)
                return NULL;
        *p = cgs_string_new();

        if (!cgs_string_copy(input, p))
                goto error_cleanup;
        if (!cgs_string_replace(p, pos, cgs_string_length(&r->seq), &r->rep))
                goto error_cleanup;

        struct cgs_variant v = { 0 };
        cgs_variant_set_data(&v, p);

        if (cgs_rbt_search(mtree, &v)) {
                cgs_string_free(p);
                free(p);
        } else {
                cgs_rbt_insert(mtree, &v);
        }

        return mtree;

error_cleanup:
        cgs_string_free(p);
        free(p);
        return NULL;
}

static void*
calibrate_machine(const struct cgs_vector* vr, const struct cgs_string* input,
                struct cgs_rbt* mtree)
{
        const char* s = cgs_string_data(input);
        for (size_t i = 0; i < cgs_vector_length(vr); ++i) {
                const struct replacement* r = cgs_vector_get(vr, i);

                for (size_t j = 0; j < cgs_string_length(input); ++j)
                        if (sequence_match(&s[j], &r->seq) &&
                                !add_molecule(input, j, r, mtree))
                                return NULL;
        }
        return mtree;
}

static int
count_fabrication_steps(const struct cgs_vector* vr, const char* target,
                struct cgs_string* s)
{
        int count = 0;
        while (!cgs_string_eq_str(s, target)) {
                for (size_t i = 0; i < cgs_vector_length(vr); ++i) {
                        const struct replacement* r = cgs_vector_get(vr, i);
                        size_t pos = cgs_string_find(s, &r->rep);
                        if (pos == cgs_string_length(s))
                                continue;
                        cgs_string_replace(s, pos, cgs_string_length(&r->rep),
                                        &r->seq);
                        count += 1;
                        break;
                }
        }
        return count;
}

int main(void)
{
        struct cgs_vector repls = cgs_vector_new(sizeof(struct replacement));
        struct cgs_string input = cgs_string_new();
        read_input(&repls, &input);

        struct cgs_rbt molecules = cgs_rbt_new(cgs_string_cmp, cgs_string_free);
        calibrate_machine(&repls, &input, &molecules);
        printf("%zu\n", cgs_rbt_length(&molecules));

        cgs_vector_sort(&repls, replacement_cmp);
        printf("%d\n", count_fabrication_steps(&repls, "e", &input));

        cgs_vector_free_all_with(&repls, replacement_free);
        cgs_string_free(&input);
        cgs_rbt_free(&molecules);
        return EXIT_SUCCESS;
}
