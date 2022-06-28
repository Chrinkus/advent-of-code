#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <cgs/cgs.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
typedef int64_t Int;                    // Big numbers here..

enum {
        PAT_LEN = 5,                    // growth patterns are 5 pots
        PAT_BUFF = PAT_LEN + 1,
        PLANT = '#',
        EMPTY = '.',
        PAD_LEN = 4,                    // 'prepare' pads front and back..
        PAD_BUFF = PAD_LEN + 1,         // ..with EMPTY pots
        OFFSET_CORRECTION = 2,          // 'advance' skips first 2 pots
};

const Int part1_gens = 20;
const Int part2_gens = 50000000000;
const char empty_pad[PAD_BUFF] = "....";

struct note {
        char pat[PAT_BUFF];
        char res;
};

struct row {
        struct cgs_string pots;
        Int offset;
        Int gen;
};

void row_free(struct row* row)
{
        cgs_string_free(&row->pots);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* read_input_and_allocate(struct row* row, struct cgs_array* notes)
{
        char* p = NULL;
        if (scanf(" initial state: %ms ", &p) != 1)
                return NULL;

        if (!cgs_string_new_from_string(&row->pots, p))
                goto error_cleanup;
        free(p);

        if (!cgs_array_new(notes, sizeof(struct note)))
                return NULL;

        for (struct note n; scanf(" %s => %c", n.pat, &n.res) == 2; )
                if (!cgs_array_push(notes, &n))
                        return NULL;

        return row;

error_cleanup:
        free(p);
        return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - Detect pattern stasis and project future
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void project_future_growth(struct row* row, Int old_offset)
{
        Int offset_diff = row->offset - old_offset;
        Int gen_diff = part2_gens - row->gen;
        row->offset += offset_diff * gen_diff;
        row->gen = part2_gens;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Simulate plant growth
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
const void* prepare_row(struct cgs_string* pots, Int* offset)
{
        const char* b = cgs_string_data(pots);
        const char* e = b + cgs_string_length(pots) - 1;

        Int num_pre = PAD_LEN;
        for ( ; *b == '.'; ++b)
                --num_pre;

        Int num_app = PAD_LEN;
        for ( ; *e == '.'; --e)
                --num_app;

        if (num_pre > 0) {
                *offset += num_pre;
                if (!cgs_string_prepend(pots, empty_pad, num_pre))
                        return NULL;
        }
        if (num_app > 0 && !cgs_string_append(pots, empty_pad, num_app))
                return NULL;

        return pots;
}

int note_match_cb(const void* elem, const void* data)
{
        const struct note* n = elem;
        const char* p = data;

        return strncmp(n->pat, p, PAT_LEN) == 0;
}

char* advance_pots(struct cgs_string* pots, struct cgs_array* notes)
{
        const size_t len = cgs_string_length(pots) - PAD_LEN;
        char* p = cgs_string_xfer(pots);

        if (!cgs_string_new(pots))
                return NULL;

        for (size_t i = 0; i < len; ++i) {
                struct note* n = cgs_array_find(notes, note_match_cb, &p[i]);
                if (!n || !cgs_string_push(pots, n->res))
                        return NULL;            // should always find match
        }

        return p;
}

void* advance_generations(struct row* row, Int num_generations,
                struct cgs_array* notes)
{
        if (!prepare_row(&row->pots, &row->offset)) // Initial prep
                return NULL;

        while (row->gen < num_generations) {
                ++row->gen;                     // key: THIS generation is set
                Int prev_offset = row->offset;

                char* prev = advance_pots(&row->pots, notes);
                if (!prev)
                        return NULL;
                row->offset -= OFFSET_CORRECTION;

                if (!prepare_row(&row->pots, &row->offset))
                        return NULL;
                if (strcmp(prev, cgs_string_data(&row->pots)) == 0)
                        project_future_growth(row, prev_offset);
                free(prev);
        }
        return row;
}

Int score_plants(const char* p, Int offset)
{
        Int score = 0;
        for (Int i = 0; p[i]; ++i)
                if (p[i] == PLANT)
                        score += i - offset;
        return score;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 12: Subterranean Sustainability\n");

        struct row row = { 0 };
        struct cgs_array notes = { 0 };
        if (!read_input_and_allocate(&row, &notes))
                return EXIT_FAILURE;

        if (!advance_generations(&row, part1_gens, &notes))
                return EXIT_FAILURE;
        Int part1 = score_plants(cgs_string_data(&row.pots), row.offset);

        if (!advance_generations(&row, part2_gens, &notes))
                return EXIT_FAILURE;
        Int part2 = score_plants(cgs_string_data(&row.pots), row.offset);

        printf("Part 1: %"PRId64"\n", part1);
        printf("Part 2: %"PRId64"\n", part2);

        row_free(&row);
        cgs_array_free(&notes);

        return EXIT_SUCCESS;
}

