#include <stdlib.h>
#include <stdio.h>

#include "fruity_set.h"

enum magic { NORTH = '^', SOUTH = 'v', EAST = '>', WEST = '<', };

static void
move_sleigh(struct f2d_point* pt, char ch)
{
        int dx = 0;
        int dy = 0;
        switch (ch) {
        case NORTH:     dy = -1;        break;
        case SOUTH:     dy =  1;        break;
        case EAST:      dx = -1;        break;
        case WEST:      dx =  1;        break;
        }
        f2d_point_move(pt, dx, dy);
}

int main(void)
{
        struct f2d_set h1 = f2d_set_new();
        struct f2d_set h2 = f2d_set_new();

        struct f2d_point s1 = { 0 };
        f2d_set_insert(&h1, &s1);

        struct f2d_point s2 = { 0 };
        struct f2d_point rb = { 0 };
        f2d_set_insert(&h2, &s2);
        f2d_set_insert(&h2, &rb);

        for (int c, n = 0; (c = getchar()) != EOF && c != '\n'; ++n) {
                move_sleigh(&s1, c);
                f2d_set_insert(&h1, &s1);

                struct f2d_point* pt = n % 2 == 0 ? &s2 : &rb;
                move_sleigh(pt, c);
                f2d_set_insert(&h2, pt);
        }

        printf("%zu\n", f2d_set_length(&h1));
        printf("%zu\n", f2d_set_length(&h2));

        f2d_set_free(&h1);
        f2d_set_free(&h2);
        return EXIT_SUCCESS;
}
