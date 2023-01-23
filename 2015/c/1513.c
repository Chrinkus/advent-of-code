#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"
#include "fruity.h"
#include "fruity_io.h"

enum buffs { NAMEMAX = 16, FEELMAX = 8, };

const char* gfmt = "%s would %*s %*d happiness units by sitting next to %*s.";
const char* hfmt = "%s would %s %d happiness units by sitting next to %[A-Za-z].";
const char* lose = "lose";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Read
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

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

static void
print_happys(const struct fruity_2d* happys, int width)
{
        fruity_foreach(happys, fruity_io_newline, NULL,
                        fruity_io_print_int, &width);
}

int main(void)
{
        struct cgs_vector guests = cgs_vector_new(sizeof(struct cgs_string));
        struct fruity_2d happys = { 0 };
        read_and_parse_data(&guests, &happys);

        print_happys(&happys, 5);

        int part1 = 0;
        printf("%d\n", part1);

        cgs_vector_free_all_with(&guests, cgs_string_free);
        fruity_free(&happys);
        return EXIT_SUCCESS;
}
