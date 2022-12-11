#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

struct Point {
        int x;
        int y;
};

/*
static void
point_move(struct Point* pt, int dx, int dy)
{
        pt->x += dx;
        pt->y += dy;
}
*/

static int
point_cmp(const void* a, const void* b)
{
        const struct Point* p1 = a;
        const struct Point* p2 = b;
        return p1->x - p2->x || p1->y - p2->y;
}

struct Rope {
        struct Point head;
        struct Point tail;
};

static struct Point*
rope_copy_tail(const struct Rope* rope)
{
        struct Point* pt = malloc(sizeof(struct Point));
        if (!pt)
                return NULL;

        pt->x = rope->tail.x;
        pt->y = rope->tail.y;
        return pt;
}

static void
rope_move_head(struct Rope* rope, int dir)
{
        switch (dir) {
        case 'U':       rope->head.y -= 1;      break;
        case 'D':       rope->head.y += 1;      break;
        case 'L':       rope->head.x -= 1;      break;
        case 'R':       rope->head.x += 1;      break;
        }
}

static void
rope_move_tail(struct Rope* rope)
{
        int dx = rope->head.x - rope->tail.x;
        int dy = rope->head.y - rope->tail.y;

        if (dx == 0) {                  // same x, move y
                rope->tail.y += dy > 0 ? 1 : -1;
        } else if (dy == 0) {           // same y, move x
                rope->tail.x += dx > 0 ? 1 : -1;
        } else {
                rope->tail.x += dx > 0 ? 1 : -1;
                rope->tail.y += dy > 0 ? 1 : -1;
        }
}

static int
rope_is_tail_touching(const struct Rope* rope)
{
        if (abs(rope->head.x - rope->tail.x) <= 1 &&
            abs(rope->head.y - rope->tail.y) <= 1)
                return 1;
        return 0;
}

static void*
read_and_move_rope(struct Rope* rope, struct cgs_bst* map)
{
        struct cgs_variant v = { 0 };
        cgs_variant_set_data(&v, rope_copy_tail(rope));
        cgs_bst_insert(map, &v);

        for (int dir = 0, n = 0; scanf("%c %d ", (char*)&dir, &n) == 2; ) {
                while (n--) {
                        rope_move_head(rope, dir);
                        if (!rope_is_tail_touching(rope)) {
                                rope_move_tail(rope);
                                cgs_variant_set_data(&v, rope_copy_tail(rope));
                                cgs_bst_insert(map, &v);
                        }
                }
        }
        return map;
}

int main(void)
{
        struct Rope rope = { .head = { 0 }, .tail = { 0 } };
        struct cgs_bst map = { 0 };
        cgs_bst_new(&map, point_cmp);

        if (!read_and_move_rope(&rope, &map))
                return cgs_error_retfail("read_and_move_rope");

        int part1 = cgs_bst_length(&map);
        printf("%d\n", part1);

        cgs_bst_free(&map);
        return EXIT_SUCCESS;
}
