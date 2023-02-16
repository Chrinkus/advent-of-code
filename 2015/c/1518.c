#include <stdlib.h>
#include <stdio.h>

#include "fruity.h"
#include "fruity_io.h"

enum { STEPS = 100, ON = '#', OFF = '.', };

struct screen {
        struct fruity_2d display;
        struct fruity_2d buffer;
};

static void*
screen_copy(const struct screen* s1, struct screen* s2)
{
        fruity_copy(&s1->display, &s2->display);
        fruity_copy(&s1->buffer, &s2->buffer);
        return s2;
}

static void
screen_free(struct screen* scr)
{
        fruity_free(&scr->display);
        fruity_free(&scr->buffer);
}

/*
static void
print_screen(const struct fruity_2d* screen)
{
        fruity_foreach(screen,
                        fruity_io_newline, NULL,
                        fruity_io_print_char, NULL);
}
*/

static void*
read_and_set_screen(struct screen* scr)
{
        fruity_io_read_chars(&scr->display, stdin);
        fruity_copy(&scr->display, &scr->buffer);

        return scr;
}

static void
animate_pixel(struct f2d_cell cell, void* data)
{
        struct screen* scr = data;
        const int r = cell.pt.y;
        const int c = cell.pt.x;

        struct f2d_cell adj[8];
        int n = fruity_adjacent_8(&scr->display, r, c, adj);

        int count = 0;
        for (int i = 0; i < n; ++i)
                count += *(const char*)adj[i].data == ON;

        const char* pd = cell.data;
        char* pb = fruity_get_mut(&scr->buffer, r, c);
        if (*pd == ON) {
                *pb = count == 2 || count == 3 ? ON : OFF;
        } else {
                *pb = count == 3 ? ON : OFF;
        }
}

static void
set_corners(struct fruity_2d* f2d)
{
        const int r = fruity_rows(f2d);
        const int c = fruity_cols(f2d);
        char** pp = fruity_data_mut(f2d);

        pp[0][0] = ON;
        pp[0][c-1] = ON;
        pp[r-1][0] = ON;
        pp[r-1][c-1] = ON;
}

static void
run_animations(struct screen* scr, int steps, int part2)
{
        while (steps--) {
                fruity_foreach(&scr->display,
                                NULL, NULL,
                                animate_pixel, scr);
                fruity_swap(&scr->display, &scr->buffer);
                if (part2)
                        set_corners(&scr->display);
        }
}

static int
is_light_on(struct f2d_cell cell, void* data)
{
        (void)data;
        return *(const char*)cell.data == ON;
}

static int
count_lights(const struct screen* scr)
{
        return fruity_count_if(&scr->display, is_light_on, NULL);
}

int main(void)
{
        struct screen scr1 = { 0 };
        read_and_set_screen(&scr1);

        struct screen scr2 = { 0 };
        screen_copy(&scr1, &scr2);
        set_corners(&scr2.display);

        run_animations(&scr1, STEPS, 0);
        printf("%d\n", count_lights(&scr1));

        run_animations(&scr2, STEPS, 1);
        printf("%d\n", count_lights(&scr2));

        screen_free(&scr1);
        screen_free(&scr2);
        return EXIT_SUCCESS;
}
