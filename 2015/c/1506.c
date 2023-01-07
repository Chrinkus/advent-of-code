#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cgs.h"

typedef uint8_t Bit;

struct point {
        int x, y;
};

static void
toggle_1(Bit screen[][1000], struct point tl, struct point br)
{
        for (int i = tl.y; i <= br.y; ++i)
                for (int j = tl.x; j <= br.x; ++j)
                        screen[i][j] = screen[i][j] == 0 ? 1 : 0;
}

static void
turn_1(Bit screen[][1000], const char* s, struct point tl, struct point br)
{
        Bit n = strcmp(s, "on") == 0;
        for (int i = tl.y; i <= br.y; ++i)
                for (int j = tl.x; j <= br.x; ++j)
                        screen[i][j] = n;
}

static void
toggle_2(int screen[][1000], struct point tl, struct point br)
{
        for (int i = tl.y; i <= br.y; ++i)
                for (int j = tl.x; j <= br.x; ++j)
                        screen[i][j] += 2;
}

static void
turn_2(int screen[][1000], const char* s, struct point tl, struct point br)
{
        int n = strcmp(s, "on") == 0 ? 1 : -1;
        for (int i = tl.y; i <= br.y; ++i)
                for (int j = tl.x; j <= br.x; ++j)
                        if (n > 0 || screen[i][j] > 0)
                                screen[i][j] += n;
}

static int
count_lit(Bit screen[][1000])
{
        int sum = 0;
        for (int i = 0; i < 1000; ++i)
                for (int j = 0; j < 1000; ++j)
                        sum += screen[i][j] == 1;
        return sum;
}

static int
sum_lights(int screen[][1000])
{
        int sum = 0;
        for (int i = 0; i < 1000; ++i)
                for (int j = 0; j < 1000; ++j)
                        sum += screen[i][j];
        return sum;
}

int main(void)
{
        Bit screen_1[1000][1000] = { 0 };
        int screen_2[1000][1000] = { 0 };
        const char* togg = "toggle %d,%d through %d,%d ";
        const char* turn = "turn %s %d,%d through %d,%d ";

        struct cgs_string line = cgs_string_new();
        for ( ; cgs_io_getline(stdin, &line); cgs_string_clear(&line)) {
                char buff[8];
                struct point tl, br;
                const char* s = cgs_string_data(&line);
                if (sscanf(s, togg, &tl.x, &tl.y, &br.x, &br.y) == 4) {
                        toggle_1(screen_1, tl, br);
                        toggle_2(screen_2, tl, br);
                } else if (sscanf(s, turn, buff,
                                        &tl.x, &tl.y, &br.x, &br.y) == 5) {
                        turn_1(screen_1, buff, tl, br);
                        turn_2(screen_2, buff, tl, br);
                } else {
                        cgs_error_msg("Bad read: %s", s);
                        goto bad_read;
                }
        }

        int part1 = count_lit(screen_1);
        int part2 = sum_lights(screen_2);
        printf("%d\n", part1);
        printf("%d\n", part2);

        cgs_string_free(&line);
        return EXIT_SUCCESS;
bad_read:
        cgs_string_free(&line);
        return EXIT_FAILURE;
}
