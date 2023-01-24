#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "cgs.h"
#include "fruity.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Debug
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
/*
#include "fruity_io.h"

static void
print_happys(const struct fruity_2d* happys, int width)
{
        fruity_foreach(happys, fruity_io_newline, NULL,
                        fruity_io_print_int, &width);
}
*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Read
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum buffs { NAMEMAX = 16, FEELMAX = 8, };

const char* gfmt = "%s would %*s %*d happiness units by sitting next to %*s.";
const char* hfmt = "%s would %s %d happiness units " \
                   "by sitting next to %[A-Za-z].";
const char* lose = "lose";

static void*
insert_guest(struct cgs_vector* guests, const char* guest)
{
        if (!cgs_vector_find(guests, cgs_string_eq_str, guest)) {
                struct cgs_string buff = cgs_string_new();
                cgs_string_from(guest, &buff);
                cgs_vector_push(guests, &buff);
        }
        return guests;
}

static void*
parse_guests(const struct cgs_vector* lines, struct cgs_vector* guests)
{
        char guest[NAMEMAX];
        for (size_t i = 0; i < cgs_vector_length(lines); ++i) {
                const char* s = cgs_string_data(cgs_vector_get(lines, i));
                if (sscanf(s, gfmt, guest) != 1)
                        return cgs_error_retnull("sscanf gfmt: %s", s);
                insert_guest(guests, guest);
        }
        return guests;
}

static int
get_dimension(const struct cgs_vector* guests, const char* guest)
{
        for (size_t i = 0; i < cgs_vector_length(guests); ++i)
                if (cgs_string_eq_str(cgs_vector_get(guests, i), guest))
                        return (int)i;
        return -1;
}

static void*
build_and_parse_happys(const struct cgs_vector* guests,
                const struct cgs_vector* lines, struct fruity_2d* happys)
{
        int dim = (int)cgs_vector_length(guests);
        int init = 0;
        fruity_build(happys, dim, dim, &init, sizeof(init));
        int** aai = fruity_data_mut(happys);

        char guest[NAMEMAX];
        char other[NAMEMAX];
        char feels[FEELMAX];
        int happy;
        for (size_t i = 0; i < cgs_vector_length(lines); ++i) {
                const char* s = cgs_string_data(cgs_vector_get(lines, i));
                if (sscanf(s, hfmt, guest, feels, &happy, other) != 4)
                        return cgs_error_retnull("sscanf hfmt: %s", s);

                int d1 = get_dimension(guests, guest);
                int d2 = get_dimension(guests, other);
                if (d1 < 0 || d2 < 0)
                        return cgs_error_retnull("get_dimension: %s,%s",
                                        guest, other);

                if (strcmp(feels, lose) == 0)
                        happy *= -1;
                aai[d1][d2] = happy;
        }
        return happys;
}

static void*
read_and_parse_data(struct cgs_vector* guests, struct fruity_2d* happys)
{
        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        cgs_io_readlines(stdin, &lines);

        parse_guests(&lines, guests);
        build_and_parse_happys(guests, &lines, happys);

        cgs_vector_free_all_with(&lines, cgs_string_free);
        return guests;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Solution
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct seat {
        int prev;
        int happy;
        unsigned seated;
};

static int
seat_cmp(const void* a, const void* b)
{
        const struct seat* s1 = a;
        const struct seat* s2 = b;

        return cgs_int_cmp(&s2->happy, &s1->happy);
}

static int
get_2way_happy(const int* const* aai, int d1, int d2)
{
        return aai[d1][d2] + aai[d2][d1];
}

static void*
load_seat_queue(const struct fruity_2d* happys, struct cgs_heap* pq,
                const struct seat* prev)
{
        const int* const* aai = fruity_data(happys);

        for (int i = 0; i < fruity_rows(happys); ++i) {
                if (1u << i & prev->seated)
                        continue;
                struct seat next = {
                        .prev = i,
                        .seated = prev->seated | 1u << i,
                        .happy = prev->happy + get_2way_happy(aai,
                                        i, prev->prev),
                };
                cgs_heap_push(pq, &next);
        }
        return pq;
}

static int
get_happiest_plan(const struct fruity_2d* happys)
{
        int happy = INT_MIN;
        struct cgs_heap pq = cgs_heap_new(sizeof(struct seat), seat_cmp);

        struct seat seat = {
                .prev = 0,
                .seated = 1u,
                .happy = 0,
        };

        const unsigned ALL_SEATED = (1u << fruity_rows(happys)) - 1;
        do {
                load_seat_queue(happys, &pq, &seat);
                if ((seat.seated & ALL_SEATED) == ALL_SEATED) {
                        seat.happy += get_2way_happy(fruity_data(happys),
                                        0, seat.prev);
                        happy = CGS_MAX(happy, seat.happy);
                }
        } while (cgs_heap_pop(&pq, &seat));

        cgs_heap_free(&pq);
        return happy;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
        struct cgs_vector guests = cgs_vector_new(sizeof(struct cgs_string));
        struct fruity_2d happys = { 0 };
        read_and_parse_data(&guests, &happys);

        //print_happys(&happys, 5);

        printf("%d\n", get_happiest_plan(&happys));

        fruity_grow(&happys, 1, 1, 0, 0, NULL);
        printf("%d\n", get_happiest_plan(&happys));

        cgs_vector_free_all_with(&guests, cgs_string_free);
        fruity_free(&happys);
        return EXIT_SUCCESS;
}
