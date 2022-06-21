#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <cgs/cgs.h>
#include <fruity.h>
#include <fruity_io.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum { SPACE = ' ', STAR = '#', };

struct light {
        int x, y, dx, dy;
};

struct extents {
        int min, max;
};

struct frame {
        struct extents h;
        struct extents w;
};

void frame_init(struct frame* f)
{
        f->h.min = f->w.min = INT_MAX;
        f->h.max = f->w.max = INT_MIN;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* read_input(struct cgs_array* stars)
{
        const char* fmt = " position=< %d, %d> velocity=< %d, %d> ";
        for (struct light v; scanf(fmt, &v.x, &v.y, &v.dx, &v.dy) == 4; )
                if (!cgs_array_push(stars, &v))
                        return NULL;
        return stars;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 and 2 - Increment lights until message appears
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void increment_positions(struct light* v, struct extents* ex)
{
        v->x += v->dx;
        v->y += v->dy;

        ex->min = CGS_MIN(v->y, ex->min);
        ex->max = CGS_MAX(v->y, ex->max);
}

void decrement_positions(struct light* v, struct frame* f)
{
        v->x -= v->dx;
        v->y -= v->dy;

        f->h.min = CGS_MIN(v->y, f->h.min);
        f->h.max = CGS_MAX(v->y, f->h.max);
        f->w.min = CGS_MIN(v->x, f->w.min);
        f->w.max = CGS_MAX(v->x, f->w.max);
}

void increment_cb(void* ele, size_t i, void* data)
{
        struct light* v = ele;
        struct extents* ex = data;
        (void)i;
        increment_positions(v, ex);
}

void decrement_cb(void* ele, size_t i, void* data)
{
        struct light* v = ele;
        struct frame* f = data;
        (void)i;
        decrement_positions(v, f);
}

int wait_for_message(struct cgs_array* stars, struct frame* frame)
{
        int seconds = 0;
        for (int hp = INT_MAX, hc = hp - 1; hc < hp; ++seconds) {
                struct extents ex = { 0 };
                cgs_array_transform(stars, increment_cb, &ex);
                hp = hc;
                hc = ex.max - ex.min;
        }
        cgs_array_transform(stars, decrement_cb, frame);
        return --seconds;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Print the sky
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* init_sky(Fruity2D* sky, const struct frame* frame)
{
        int rows = frame->h.max - frame->h.min + 1;
        int cols = frame->w.max - frame->w.min + 1;
        void* res = fruity_new(sky, rows, cols, sizeof(char));
        if (!res)
                return NULL;

        char space = SPACE;
        fruity_init(sky, &space);

        return res;
}

void plot_stars(Fruity2D* sky, const struct cgs_array* stars,
                const struct frame* f)
{
        int xoff = f->w.min;
        int yoff = f->h.min;

        char** s = fruity_data(sky);
        for (size_t i = 0; i < cgs_array_length(stars); ++i) {
                const struct light* l = cgs_array_get(stars, i);
                s[l->y - yoff][l->x - xoff] = STAR;
        }
}

void print_sky(const Fruity2D* sky)
{
        fruity_foreach(sky,
                       fruity_io_newline, NULL,
                       fruity_io_print_char, NULL);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 10: The Stars Align\n");

        struct cgs_array stars = { 0 };
        if (!cgs_array_new(&stars, sizeof(struct light)))
                return EXIT_FAILURE;

        if (!read_input(&stars))
                return EXIT_FAILURE;

        struct frame frame = { 0 };
        frame_init(&frame);
        int part2 = wait_for_message(&stars, &frame);

        Fruity2D sky = { 0 };
        if (!init_sky(&sky, &frame))
                return EXIT_FAILURE;

        plot_stars(&sky, &stars, &frame);

        printf("Part 1:\n");
        print_sky(&sky);
        printf("\n");
        printf("Part 2: %d\n", part2);

        cgs_array_free(&stars);
        fruity_free(&sky);

        return EXIT_SUCCESS;
}

