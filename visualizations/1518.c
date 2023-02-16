#include <stdlib.h>
#include <stdio.h>

#include "fruity.h"
#include "fruity_io.h"

#include <ncurses.h>
#include <unistd.h>

enum { STEPS = 1000, ON = '#', OFF = '.', BLANK = ' ', };

const unsigned usecs = 1000000 / 15;    // framerate

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

static void
print_display(const struct fruity_2d* screen)
{
        const char* const* pp = fruity_data(screen);
        const int starty = (LINES - fruity_rows(screen)) / 2;
        const int startx = (COLS - fruity_cols(screen)) / 2;

        for (int i = 0; i < fruity_rows(screen); ++i) {
                move(starty + i, startx);
                for (int j = 0; j < fruity_cols(screen); ++j) {
                        addch(pp[i][j] == ON ? ON : BLANK);
                }
        }
}

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
run_animations(struct screen* scr, int steps)
{
        initscr();
        clear();
        noecho();
        cbreak();
        print_display(&scr->display);
        refresh();
        while (steps--) {
                usleep(usecs);
                fruity_foreach(&scr->display,
                                NULL, NULL,
                                animate_pixel, scr);
                fruity_swap(&scr->display, &scr->buffer);

                print_display(&scr->display);
                refresh();
        }
        getch();
        endwin();
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

        run_animations(&scr1, STEPS);
        printf("%d\n", count_lights(&scr1));

        screen_free(&scr1);
        return EXIT_SUCCESS;
}
