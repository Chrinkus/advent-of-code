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

struct extents extents_init(void)
{
        return (struct extents){ .min = INT_MAX, .max = INT_MIN, };
}

int extents_diff(const struct extents* ext)
{
        return ext->max - ext->min;
}

struct frame {
        struct extents h;
        struct extents w;
};

struct frame frame_init(void)
{
        return (struct frame){ .h = extents_init(), .w = extents_init(), };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* read_input(struct cgs_array* stars)
{
        const char* fmt = " position=< %d, %d> velocity=< %d, %d> ";
        for (struct light l; scanf(fmt, &l.x, &l.y, &l.dx, &l.dy) == 4; )
                if (!cgs_array_push(stars, &l))
                        return cgs_error_retnull("cgs_array_push");
        return stars;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 and 2 - Increment lights until message appears
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void increment_and_track_positions(struct light* l, struct extents* ex)
{
        l->x += l->dx;
        l->y += l->dy;

        ex->min = CGS_MIN(l->y, ex->min);
        ex->max = CGS_MAX(l->y, ex->max);
}

void increment_and_track_cb(void* ele, size_t i, void* data)
{
        struct light* l = ele;
        struct extents* ex = data;
        (void)i;
        increment_and_track_positions(l, ex);
}

void decrement_positions(struct light* l)
{
        l->x -= l->dx;
        l->y -= l->dy;
}

void decrement_cb(void* ele, size_t i, void* data)
{
        struct light* l = ele;
        (void)i;
        (void)data;
        decrement_positions(l);
}

int wait_for_message(struct cgs_array* stars)
{
        int seconds = 0;
        for (int hprev = INT_MAX, hcurr = hprev - 1; hcurr < hprev; ) {
                struct extents hext = extents_init();
                cgs_array_transform(stars, increment_and_track_cb, &hext);
                hprev = hcurr;
                hcurr = extents_diff(&hext);
                ++seconds;
        }
        cgs_array_transform(stars, decrement_cb, NULL); // back up 1s
        return --seconds;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Print the sky
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void set_frame(struct frame* f, const struct light* l)
{
        f->h.min = CGS_MIN(l->y, f->h.min);
        f->h.max = CGS_MAX(l->y, f->h.max);
        f->w.min = CGS_MIN(l->x, f->w.min);
        f->w.max = CGS_MAX(l->x, f->w.max);
}

void set_frame_cb(const void* ele, size_t i, void* data)
{
        const struct light* light = ele;
        struct frame* frame = data;
        (void)i;
        set_frame(frame, light);
}

void* init_sky(Fruity2D* sky, const struct frame* frame)
{
        int rows = extents_diff(&frame->h) + 1;
        int cols = extents_diff(&frame->w) + 1;
        void* res = fruity_new(sky, rows, cols, sizeof(char));
        if (!res)
                return cgs_error_retnull("fruity_new %d x %d", rows, cols);

        char space = SPACE;
        fruity_init(sky, &space);

        return res;
}

void* plot_stars(Fruity2D* sky, const struct cgs_array* stars)
{
        struct frame frame = frame_init();
        cgs_array_foreach(stars, set_frame_cb, &frame);

        if (!init_sky(sky, &frame))
                return cgs_error_retnull("init_sky");

        int xoff = frame.w.min;
        int yoff = frame.h.min;

        char** s = fruity_data_mutable(sky);
        for (size_t i = 0; i < cgs_array_length(stars); ++i) {
                const struct light* l = cgs_array_get(stars, i);
                s[l->y - yoff][l->x - xoff] = STAR;
        }
        return sky;
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
                return cgs_error_retfail("cgs_array_new");

        if (!read_input(&stars))
                return cgs_error_retfail("read_input");

        int part2 = wait_for_message(&stars);

        Fruity2D sky = { 0 };
        if (!plot_stars(&sky, &stars))
                return cgs_error_retfail("plot_stars");

        printf("Part 1:\n");
        print_sky(&sky);
        printf("Part 2: %d\n", part2);

        cgs_array_free(&stars);
        fruity_free(&sky);

        return EXIT_SUCCESS;
}

