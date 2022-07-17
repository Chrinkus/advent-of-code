#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cgs/cgs.h>
#include <fruity.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum buffers { BUFFMAX = 16, };

enum direction { UP, RIGHT, DOWN, LEFT, NUM_DIRS };
const char dirs[NUM_DIRS] = {
        [UP]    = '^',
        [RIGHT] = '>',
        [DOWN]  = 'v',
        [LEFT]  = '<',
};

int left_turn(enum direction d)
{
        return d == UP ? LEFT : --d;
}

int right_turn(enum direction d)
{
        return d == LEFT ? UP : ++d;
}

enum track {
        DASH = '-',
        PIPE = '|',
        BACK = '\\',
        FORWARD = '/',
        INTER = '+',
        SPACE = ' ',
        CART_UP = '^',
        CART_DOWN = 'v',
        CART_LEFT = '<',
        CART_RIGHT = '>',
};

enum turn {
        GO_LEFT,
        GO_STRAIGHT,
        GO_RIGHT,
        NUM_TURNS
};

struct cart {
        int x;
        int y;
        enum direction dir;
        enum turn t;
};

int
cart_cmp(const void* a, const void* b)
{
        const struct cart* c1 = a;
        const struct cart* c2 = b;

        return (c1->y == c2->y) ? c1->x - c2->x : c1->y - c2->y;
}

char*
cart_to_string(const struct cart* c)
{
        char* out = malloc(BUFFMAX);
        if (!out)
                return cgs_error_retnull("malloc");

        int n = snprintf(out, BUFFMAX, "%d,%d", c->x, c->y);
        if (n >= BUFFMAX)
                return cgs_error_retnull("snprintf");
        return out;
}

int
cart_collision_detect(const struct cart* a, const struct cart* b)
{
        return a->x == b->x && a->y == b->y;
}

const void*
add_cart(struct cgs_array* carts, int x, int y, int cart)
{
        int d = 0;
        while (dirs[d] != cart)
                ++d;
        struct cart c = { .x = x, .y = y, .dir = d, .t = GO_LEFT };
        if (!cgs_array_push(carts, &c))
                return cgs_error_retnull("cgs_array_push");
        return carts;
}

void
handle_back_slash(struct cart* c)
{
        switch (c->dir) {
        case UP:        c->dir = LEFT;  --c->x;         break;
        case DOWN:      c->dir = RIGHT; ++c->x;         break;
        case LEFT:      c->dir = UP;    --c->y;         break;
        case RIGHT:     c->dir = DOWN;  ++c->y;         break;
        default:                                        break;
        }
}

void
handle_forward_slash(struct cart* c)
{
        switch (c->dir) {
        case UP:        c->dir = RIGHT; ++c->x;         break;
        case DOWN:      c->dir = LEFT;  --c->x;         break;
        case LEFT:      c->dir = DOWN;  ++c->y;         break;
        case RIGHT:     c->dir = UP;    --c->y;         break;
        default:                                        break;
        }
}

void
handle_intersection(struct cart* c)
{
        switch (c->t) {
        case GO_LEFT:           c->dir = left_turn(c->dir);     break;
        case GO_STRAIGHT:       /* no change */                 break;
        case GO_RIGHT:          c->dir = right_turn(c->dir);    break;
        default:                                                break;
        }
        switch (c->dir) {
        case UP:                --c->y;                 break;
        case DOWN:              ++c->y;                 break;
        case LEFT:              --c->x;                 break;
        case RIGHT:             ++c->x;                 break;
        default:                                        break;
        }
        c->t = (c->t + 1) % NUM_TURNS;
}

void
move_cart(struct cart* c, Fruity2D* track)
{
        const char*const* pt = fruity_data(track);

        switch (pt[c->y][c->x]) {
        case PIPE:      c->y += c->dir == DOWN ? 1 : -1;        break;
        case DASH:      c->x += c->dir == RIGHT ? 1 : -1;       break;
        case BACK:      handle_back_slash(c);                   break;
        case FORWARD:   handle_forward_slash(c);                break;
        case INTER:     handle_intersection(c);                 break;
        }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_array*
read_input_and_init_structs(Fruity2D* track, struct cgs_array* carts,
                struct cgs_array* input)
{
        if (!cgs_array_new(input, sizeof(char*)))
                return cgs_error_retnull("cgs_array_new: input");

        if (!cgs_io_readlines(stdin, input)) {
                return cgs_error_retnull("cgs_io_readlines");
        }

        int rows = cgs_array_length(input);
        int cols = strlen(*(const char*const*)cgs_array_get(input, 0));
        if (!fruity_new(track, rows, cols, sizeof(char))) {
                return cgs_error_retnull("fruity_new");
        }

        if (!cgs_array_new(carts, sizeof(struct cart)))
                return cgs_error_retnull("cgs_array_new: carts");

        return input;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Build track, set carts, find first collision
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
const struct cgs_array*
set_track_and_log_carts(Fruity2D* track, struct cgs_array* carts,
                const struct cgs_array* input)
{
        char** fp = fruity_data(track);
        for (size_t i = 0; i <  cgs_array_length(input); ++i) {
                // This is a problem..
                const char*const s = *(CgsStrIter)cgs_array_get(input, i);

                for (size_t j = 0; j < strlen(s); ++j) {
                        switch (s[j]) {
                        case DASH:      // fallthrough
                        case PIPE:      // fallthrough
                        case BACK:      // fallthrough
                        case FORWARD:   // fallthrough
                        case INTER:     // fallthrough
                        case SPACE:
                                fp[i][j] = s[j];
                                break;
                        case CART_UP:   // fallthrough
                        case CART_DOWN:
                                fp[i][j] = PIPE;
                                if (!add_cart(carts, j, i, s[j]))
                                        return cgs_error_retnull("add_cart");
                                break;
                        case CART_LEFT: // fallthrough
                        case CART_RIGHT:
                                fp[i][j] = DASH;
                                if (!add_cart(carts, j, i, s[j]))
                                        return cgs_error_retnull("add_cart");
                                break;
                        default:
                                const char* fmt = "Unknown: %c @ %d,%d";
                                return cgs_error_retnull(fmt, s[j], j, i);
                        }
                }
        }
        return input;
}

char*
sim_carts_till_first_crash(const struct cgs_array* carts, Fruity2D* track)
{
        struct cart* c = NULL;

        struct cgs_array tmp = { 0 };
        if (!cgs_array_copy(&tmp, carts))
                return cgs_error_retnull("cgs_array_copy");

        while (1) {
                cgs_array_sort(&tmp, cart_cmp);
                for (size_t i = 0, n = cgs_array_length(&tmp); i < n; ++i) {
                        c = cgs_array_get_mutable(&tmp, i);
                        move_cart(c, track);
                        for (size_t j = 0; j < n; ++j) {
                                if (i == j)
                                        continue;
                                const struct cart* d = cgs_array_get(&tmp, j);
                                if (cart_collision_detect(c, d))
                                        goto collision_detected;
                        }
                }
        }
collision_detected:
        char* ret = cart_to_string(c);
        cgs_array_free(&tmp);
        return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - Sim carts, remove crashes, find last one
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t remove_collision(struct cgs_array* carts, size_t i, const struct cart* c)
{
        size_t n = 0;
        for (size_t j = 0, l = cgs_array_length(carts); j < l; ++j) {
                if (i == j)
                        continue;
                const struct cart* d = cgs_array_get(carts, j);
                if (cart_collision_detect(c, d)) {
                        if (j < i) {
                                cgs_array_remove(carts, i);
                                cgs_array_remove(carts, j);
                                n = 2;
                        } else {
                                cgs_array_remove(carts, j);
                                cgs_array_remove(carts, i);
                                n = 1;
                        }
                        break;
                }
        }
        return n;
}

char* sim_carts_till_last_cart(const struct cgs_array* carts, Fruity2D* track)
{
        struct cgs_array tmp = { 0 };
        if (!cgs_array_copy(&tmp, carts))
                return cgs_error_retnull("cgs_array_copy");

        while (cgs_array_length(&tmp) > 1) {
                cgs_array_sort(&tmp, cart_cmp);
                for (size_t i = 0; i < cgs_array_length(&tmp); ++i) {
                        struct cart* c = cgs_array_get_mutable(&tmp, i);
                        move_cart(c, track);
                        i -= remove_collision(&tmp, i, c);
                }
        }
        const struct cart* c = cgs_array_get(&tmp, 0);
        char* ret = cart_to_string(c);
        cgs_array_free(&tmp);
        return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 13: Mine Cart Madness\n");

        Fruity2D track = { 0 };
        struct cgs_array carts = { 0 };
        struct cgs_array input = { 0 };
        if (!read_input_and_init_structs(&track, &carts, &input))
                return cgs_error_retfail("read_input_and_init_structs");

        if (!set_track_and_log_carts(&track, &carts, &input))
                return cgs_error_retfail("set_track_and_log_carts");

        char* part1 = sim_carts_till_first_crash(&carts, &track);
        if (!part1)
                return cgs_error_retfail("sim_carts_till_first_crash");

        char* part2 = sim_carts_till_last_cart(&carts, &track);
        if (!part2)
                return cgs_error_retfail("sim_carts_till_last_one");

        printf("Part 1: %s\n", part1);
        printf("Part 2: %s\n", part2);

        free(part1);
        free(part2);

        cgs_array_free_all(&input);
        cgs_array_free(&carts);
        fruity_free(&track);

        return EXIT_SUCCESS;
}

