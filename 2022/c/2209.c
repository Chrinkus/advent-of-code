#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum magic { SHORTLEN = 2, LONGLEN = 10 };

struct Move {
        char dir;
        int len;
};

struct Point {
        int x;
        int y;
};

static int
point_cmp(const void* a, const void* b)
{
        const struct Point* p1 = a;
        const struct Point* p2 = b;
        return p1->x - p2->x || p1->y - p2->y;
}

static void*
read_input(struct cgs_vector* moves)
{
        for (struct Move m; scanf("%c %d ", &m.dir, &m.len) == 2; )
                if (!cgs_vector_push(moves, &m))
                        return cgs_error_retnull("cgs_vector_push");
        return moves;
}

static struct Point*
point_copy(const struct Point* pt)
{
        struct Point* p = malloc(sizeof(struct Point));
        if (!p)
                return NULL;

        p->x = pt->x;
        p->y = pt->y;
        return p;
}

static void
move_head(struct Point* head, char dir)
{
        switch (dir) {
        case 'U':       head->y -= 1;           break;
        case 'D':       head->y += 1;           break;
        case 'L':       head->x -= 1;           break;
        case 'R':       head->x += 1;           break;
        }
}

static void
move_curr(const struct Point* prev, struct Point* curr)
{
        int dx = prev->x - curr->x;
        int dy = prev->y - curr->y;

        if (dx == 0) {                  // same x, move y
                curr->y += dy > 0 ? 1 : -1;
        } else if (dy == 0) {           // same y, move x
                curr->x += dx > 0 ? 1 : -1;
        } else {
                curr->x += dx > 0 ? 1 : -1;
                curr->y += dy > 0 ? 1 : -1;
        }
}

static int
is_curr_touching_prev(const struct Point* prev, const struct Point* curr)
{
        if (abs(prev->x - curr->x) <= 1 &&
            abs(prev->y - curr->y) <= 1)
                return 1;
        return 0;
}

static void
move_mid(struct Point* rope, int len)
{
        struct Point* prev = &rope[0];
        for (int i = 1; i < len - 1; ++i) {
                struct Point* curr = &rope[i];
                if (!is_curr_touching_prev(prev, curr)) {
                        move_curr(prev, curr);
                }
                prev = curr;
        }
}

static void*
move_rope(const struct cgs_vector* moves, struct Point* rope, int len,
                struct cgs_bst* map)
{
        struct cgs_variant v = { 0 };
        struct Point* tail = &rope[len-1];
        cgs_variant_set_data(&v, point_copy(tail));
        cgs_bst_insert(map, &v);

        for (size_t i = 0; i < cgs_vector_length(moves); ++i) {
                const struct Move* m = cgs_vector_get(moves, i);
                for (int n = m->len; n != 0; --n) {
                        move_head(&rope[0], m->dir);
                        move_mid(rope, len);
                        const struct Point* prev = &rope[len-2];
                        if (!is_curr_touching_prev(prev, tail)) {
                                move_curr(prev, tail);
                                cgs_variant_set_data(&v, point_copy(tail));
                                cgs_bst_insert(map, &v);
                        }
                }
        }
        return map;
}

int main(void)
{
        struct cgs_vector input = cgs_vector_new(sizeof(struct Move));
        if (!read_input(&input))
                return cgs_error_retfail("read_input");

        // Part 1
        struct Point shorty[SHORTLEN] = { 0 };
        struct cgs_bst short_map = cgs_bst_new(point_cmp);

        if (!move_rope(&input, shorty, SHORTLEN, &short_map))
                return cgs_error_retfail("move_rope: short");

        int part1 = cgs_bst_length(&short_map);
        printf("%d\n", part1);

        // Part 2
        struct Point longer[LONGLEN] = { 0 };
        struct cgs_bst long_map = cgs_bst_new(point_cmp);

        if (!move_rope(&input, longer, LONGLEN, &long_map))
                return cgs_error_retfail("move_rope: long");

        int part2 = cgs_bst_length(&long_map);
        printf("%d\n", part2);

        cgs_vector_free(&input);
        cgs_bst_free(&short_map);
        cgs_bst_free(&long_map);
        return EXIT_SUCCESS;
}
