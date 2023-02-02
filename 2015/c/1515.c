#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"
#include "fruity.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Debug
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
/*
#include "fruity_io.h"
static void
print_props(const struct fruity_2d* props, int weight)
{
        fruity_foreach(props, fruity_io_newline, NULL,
                        fruity_io_print_int, &weight);
}
*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum ice { TSP = 100, CAL_TARGET = 500, };

enum properties {
        CAP,
        DUR,
        FLV,
        TEX,
        CAL,
        NUM_PROPS,
        WO_CALS = NUM_PROPS - 1,
};

static void*
read_and_parse_ingredients(struct fruity_2d* props)
{
        const char* ifmt = "%*s capacity %d, durability %d, "   \
                           "flavor %d, texture %d, calories %d";

        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        cgs_io_readlines(stdin, &lines);

        fruity_build(props, cgs_vector_length(&lines), NUM_PROPS, NULL,
                        sizeof(int));

        int** aai = fruity_data_mut(props);
        for (size_t i = 0; i < cgs_vector_length(&lines); ++i) {
                const char* s = cgs_string_data(cgs_vector_get(&lines, i));
                if (sscanf(s, ifmt, &aai[i][CAP], &aai[i][DUR], &aai[i][FLV],
                                        &aai[i][TEX], &aai[i][CAL]) != 5)
                        return cgs_error_retnull("sscanf: %s", s);
        }

        cgs_vector_free_all_with(&lines, cgs_string_free);
        return props;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

typedef int (*Checker)(const struct fruity_2d*, int*, int*);

static int
check_prop_score(const struct fruity_2d* props, int* m, int* t)
{
        const int* const* aai = fruity_data(props);

        for (int j = 0; j < WO_CALS; ++j) {
                t[j] = 0;
                for (int i = 0; i < fruity_rows(props); ++i) {
                        t[j] += aai[i][j] * m[i];
                }
                if (t[j] <= 0)
                        return 0;
        }
        int score = 1;
        for (int i = 0; i < WO_CALS; ++i)
                score *= t[i];
        return score;
}

static int
check_cals_score(const struct fruity_2d* props, int* m, int* t)
{
        const int* const* aai = fruity_data(props);

        for (int j = 0; j < NUM_PROPS; ++j) {
                t[j] = 0;
                for (int i = 0; i < fruity_rows(props); ++i)
                        t[j] += aai[i][j] * m[i];
                if (t[j] <= 0)
                        return 0;
        }

        int score = 1;
        for (int i = 0; i < WO_CALS; ++i)
                score *= t[i];
        return t[CAL] == CAL_TARGET ? score : 0;
}

static void
run_loops(const struct fruity_2d* props, int* m, int len, int* t, int i,
                Checker chk, int* max)
{
        int sum = 0;
        for (int j = 0; j < i; ++j)
                sum += m[j];

        if (i == len - 1) {
                m[i] = TSP - sum;
                *max = CGS_MAX(chk(props, m, t), *max);
                return;
        }

        for (m[i] = 1; m[i] + sum < TSP; ++m[i])
                run_loops(props, m, len, t, i + 1, chk, max);
}

static int
get_highest_score(const struct fruity_2d* props, Checker chk)
{
        int rows = fruity_rows(props);
        int* m = malloc(sizeof(int) * rows);
        int t[NUM_PROPS];

        int max = 0;
        run_loops(props, m, rows, t, 0, chk, &max);

        free(m);
        return max;
}

int main(void)
{
        struct fruity_2d props = { 0 };
        read_and_parse_ingredients(&props);

        //print_props(&props, 3);

        printf("%d\n", get_highest_score(&props, check_prop_score));
        printf("%d\n", get_highest_score(&props, check_cals_score));

        fruity_free(&props);
        return EXIT_SUCCESS;
}
