#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum {
        WORKING = -2,
        COMPLETE = -1,
        READY = 0,
        FIRST_STEP = 'A',
        MAX_STEPS = 26,
        BUFFSIZE = MAX_STEPS + 1,
        STATIC_TIME = 60,
        NUM_WORKERS = 5,
};

struct sub;

struct step {
        char id;
        int p1_deps;
        int p2_deps;
        struct sub* subs;
};

struct sub {
        struct step* step;
        struct sub* next;
};

struct input {
        struct step steps[MAX_STEPS];
        int num_steps;
};

struct worker {
        int secs;
        struct step* step;
};

void sub_free(struct sub* sub)
{
        if (!sub)
                return;
        sub_free(sub->next);
        free(sub);
}

void step_free(struct step* step)
{
        if (!step)
                return;

        sub_free(step->subs);
}

void input_free(struct input* input)
{
        for (int i = 0; i < MAX_STEPS; ++i)
                step_free(&input->steps[i]);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct step* get_step(struct step* steps, char id)
{
        struct step* p = &steps[id - FIRST_STEP];
        if (!p->id) {
                p->id = id;
                p->p1_deps = 0;
                p->p2_deps = 0;
                p->subs = NULL;
        }
        return p;
}

void* read_input_and_parse_steps(struct input* input)
{
        const char* fmt = "Step %c must be finished before step %c can begin. ";
        struct step*const steps = input->steps;

        for (char need, step; scanf(fmt, &need, &step) == 2; ) {
                struct step* ps = get_step(steps, step);
                ++ps->p1_deps;
                ++ps->p2_deps;

                struct sub* sub = malloc(sizeof(struct sub));
                if (!sub)
                        return NULL;
                sub->step = ps;

                struct step* pn = get_step(steps, need);
                sub->next = pn->subs;
                pn->subs = sub;
        }

        for (int i = 0; i < MAX_STEPS; ++i)
                if (steps[i].id)
                        ++input->num_steps;

        return input;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - What order must the steps be executed synchronusly?
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void get_synchronus_step_order(struct input* input, char* buff)
{
        for (int i = 0; i < input->num_steps; ++i) {

                for (int j = 0; j < MAX_STEPS; ++j) {
                        struct step* p = &input->steps[j];

                        if (!p->id || p->p1_deps != READY)
                                continue;

                        p->p1_deps = COMPLETE;

                        for (struct sub* sub = p->subs; sub; sub = sub->next)
                                --sub->step->p1_deps;

                        *buff++ = p->id;
                        break;
                }
        }

        *buff = '\0';
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - What order may the steps be executed in parallel?
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_work(struct worker* w, struct input* input)
{
        for (struct step* b = input->steps, *e = b + MAX_STEPS; b != e; ++b) {
                if (!b->id || b->p2_deps != READY)
                        continue;
                b->p2_deps = WORKING;
                w->step = b;
                w->secs = b->id - FIRST_STEP + 1 + STATIC_TIME;
                return 1;
        }
        return 0;
}

void complete_step(struct worker* w, int* count)
{
        w->step->p2_deps = COMPLETE;

        for (struct sub* sub = w->step->subs; sub; sub = sub->next)
                --sub->step->p2_deps;

        w->step = NULL;
        w->secs = 0;
        ++*count;
}

int get_parallel_work_time(struct input* input)
{
        struct worker workers[NUM_WORKERS] = { { 0 } };

        for (int i = 0, t = 0; ; ++t) {
                for (int j = 0; j < NUM_WORKERS; ++j) {
                        struct worker* w = &workers[j];
                        if (!w->step && !get_work(w, input)) {
                                continue;
                        } else if (w->secs == 0) {
                                complete_step(w, &i);
                                if (i == input->num_steps)
                                        return t;
                                j = 0;  // reset loop to assign open workers
                        }
                }
                // Separate for potential loop resets
                for (int k = 0; k < NUM_WORKERS; ++k)
                        --workers[k].secs;
        }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 7: The Sum of Its Parts\n");

        struct input input = { 0 };
        if (!read_input_and_parse_steps(&input))
                return EXIT_FAILURE;

        char part1[BUFFSIZE] = "";
        get_synchronus_step_order(&input, part1);

        int part2 = get_parallel_work_time(&input);

        printf("Part 1: %s\n", part1);
        printf("Part 2: %d\n", part2);

        input_free(&input);

        return EXIT_SUCCESS;
}

