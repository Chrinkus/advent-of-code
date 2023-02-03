#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum ice { COMPBUFF = 16, COMPCOUNT = 3, NUM_SUES = 500, UNKNOWN = -1 };

enum compound_constants {
        CHLD,
        CATS,
        SAMO,
        POMR,
        AKIT,
        VIZS,
        GOLD,
        TREE,
        CARS,
        PERF,
        NUM_COMPOUNDS,
};

const char* const compounds[NUM_COMPOUNDS] = {
        [CHLD] = "children",
        [CATS] = "cats",
        [SAMO] = "samoyeds",
        [POMR] = "pomeranians",
        [AKIT] = "akitas",
        [VIZS] = "vizslas",
        [GOLD] = "goldfish",
        [TREE] = "trees",
        [CARS] = "cars",
        [PERF] = "perfumes",
};

const int sue_data[NUM_COMPOUNDS] = {
        [CHLD] = 3,
        [CATS] = 7,
        [SAMO] = 2,
        [POMR] = 3,
        [AKIT] = 0,
        [VIZS] = 0,
        [GOLD] = 5,
        [TREE] = 3,
        [CARS] = 2,
        [PERF] = 1,
};

static int
get_comp_index(const char* s)
{
        for (int i = 0; i < NUM_COMPOUNDS; ++i)
                if (strcmp(compounds[i], s) == 0)
                        return i;
        return -1;
}

static void
init_all_the_sues(int sues[NUM_SUES][NUM_COMPOUNDS])
{
        for (int i = 0; i < NUM_SUES; ++i)
                for (int j = 0; j < NUM_COMPOUNDS; ++j)
                        sues[i][j] = UNKNOWN;
}

static void
add_sue(int sue[], const char comps[][COMPBUFF], int counts[], int len)
{
        for (int i = 0; i < len; ++i) {
                int j = get_comp_index(comps[i]);
                if (j < 0) {
                        cgs_error_msg("add_sue: %s", comps[i]);
                        return;
                }
                sue[j] = counts[i];
        }
}

static void*
read_all_the_sues(int sues[NUM_SUES][NUM_COMPOUNDS])
{
        const char* const fmt = "Sue %d: %[a-z]: %d, %[a-z]: %d, %[a-z]: %d";
        struct cgs_string line = cgs_string_new();
        for (int i = 0; i < NUM_SUES && cgs_io_getline(stdin, &line) > 0;
                        ++i, cgs_string_clear(&line)) {
                char comps[COMPCOUNT][COMPBUFF];
                int counts[COMPCOUNT];
                int sue;

                if (sscanf(cgs_string_data(&line), fmt, &sue,
                                        comps[0], &counts[0],
                                        comps[1], &counts[1],
                                        comps[2], &counts[2]) != 7)
                        goto bad_read;

                add_sue(sues[sue-1], comps, counts, COMPCOUNT);
        }
        cgs_string_free(&line);
        return sues;

bad_read:
        cgs_error_msg("Bad read: %s", cgs_string_data(&line));
        cgs_string_free(&line);
        return NULL;
}

typedef int (*Checker)(const int* p, int len);

static int
check_sue_1(const int* sue, int len)
{
        for (int i = 0; i < len; ++i)
                if (sue[i] != UNKNOWN && sue[i] != sue_data[i])
                        return 0;
        return 1;
}

static int
check_sue_2(const int* sue, int len)
{
        for (int i = 0; i < len; ++i) {
                if (sue[i] == UNKNOWN)
                        continue;
                switch (i) {
                case CATS:
                case TREE:
                        if (!(sue[i] > sue_data[i]))
                                return 0;
                        break;
                case POMR:
                case GOLD:
                        if (!(sue[i] < sue_data[i]))
                                return 0;
                        break;
                default:
                        if (sue_data[i] != sue[i])
                                return 0;
                }
        }
        return 1;
}

static int
find_aunt_sue(const int sues[][NUM_COMPOUNDS], int len, Checker chk)
{
        for (int i = 0; i < len; ++i)
                if (chk(sues[i], NUM_COMPOUNDS))
                        return i + 1;
        return UNKNOWN;
}

int main(void)
{
        int sues[NUM_SUES][NUM_COMPOUNDS];
        init_all_the_sues(sues);
        read_all_the_sues(sues);

        printf("%d\n", find_aunt_sue(sues, NUM_SUES, check_sue_1));
        printf("%d\n", find_aunt_sue(sues, NUM_SUES, check_sue_2));

        return EXIT_SUCCESS;
}
