#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"
#include "fruity.h"
#include "fruity_io.h"

enum magic { DELIM = ' ', LIT = '#', DARK = ' ', };
enum screen { WIDTH = 40, HEIGHT = 6, };

const char* noop = "noop";
const char* addx = "addx";
const size_t signals[] = { 20, 60, 100, 140, 180, 220, };

static void*
read_program(struct cgs_vector* prog)
{
        if (!cgs_io_readlines(stdin, prog))
                return cgs_error_retnull("readlines");
        return prog;
}

static void
draw_to_screen(Fruity2D* screen, size_t cycle, int x)
{
        size_t row = (cycle - 1) / WIDTH;
        size_t col = (cycle - 1) % WIDTH;

        char** buff = fruity_data_mut(screen);
        if (x-1 <= (int)col && (int)col <= x+1)
                buff[row][col] = LIT;
}

static void*
load_instruction(const struct cgs_vector* prog, size_t* i, int* add)
{
        struct cgs_vector subs = cgs_vector_new(sizeof(struct cgs_strsub));

        if (!cgs_string_split(cgs_vector_get(prog, *i), DELIM, &subs))
                return cgs_error_retnull("string_split");

        const struct cgs_strsub* ss = cgs_vector_get(&subs, 0);
        if (cgs_strsub_eq_str(ss, noop)) {
                ;
        } else if (cgs_strsub_eq_str(ss, addx)) {
                if (!cgs_strsub_to_int(cgs_vector_get(&subs, 1), add)) {
                        cgs_error_msg("strsub_to_int");
                        goto error_cleanup;
                }
        } else {
                cgs_error_msg("Unknown instruction");
                goto error_cleanup;
        }

        *i += 1;
        cgs_vector_free(&subs);
        return add;

error_cleanup:
        cgs_vector_free(&subs);
        return NULL;
}

static int
run_program(const struct cgs_vector* prog, Fruity2D* screen)
{
        int sum = 0;

        int x = 1;
        int add = 0;
        for (size_t cycle = 1, i = 0, sig = 0; i < cgs_vector_length(prog);
                        ++cycle) {
                // Part 2
                draw_to_screen(screen, cycle, x);

                // Part 1
                if (cycle == signals[sig])
                        sum += signals[sig++] * x;
                if (add) {
                        x += add;
                        add = 0;
                        continue;
                }
                if (!load_instruction(prog, &i, &add))
                        return cgs_error_retfail("load_instruction");
        }
        return sum;
}

static void*
init_screen(Fruity2D* screen)
{
        if (!fruity_new(screen, HEIGHT, WIDTH, sizeof(char)))
                return cgs_error_retnull("fruity_new");

        char pix = DARK;
        fruity_init(screen, &pix);

        return screen;
}

int main(void)
{
        struct cgs_vector prog = cgs_vector_new(sizeof(struct cgs_string));
        if (!read_program(&prog))
                return cgs_error_retfail("read_program");

        Fruity2D screen = { 0 };
        if (!init_screen(&screen))
                return cgs_error_retfail("init_screen");

        int part1 = run_program(&prog, &screen);
        printf("%d\n", part1);

        // Part 2
        fruity_foreach(&screen,
                        fruity_io_newline, NULL,
                        fruity_io_print_char, NULL);

        cgs_vector_free_all_with(&prog, cgs_string_free);
        fruity_free(&screen);
        return EXIT_SUCCESS;
}
