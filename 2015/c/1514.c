#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum magic { SECONDS = 2503, };
enum state { FLYING, RESTING,  };

struct deer {
        int speed;
        int fly;
        int rest;
        enum state state;
        int distance;
        int count;
        int points;
};

static int
deer_distance_cmp(const void* a, const void* b)
{
        const struct deer* d1 = a;
        const struct deer* d2 = b;

        return cgs_int_cmp(&d1->distance, &d2->distance);
}

static int
deer_points_cmp(const void* a, const void* b)
{
        const struct deer* d1 = a;
        const struct deer* d2 = b;

        return cgs_int_cmp(&d1->points, &d2->points);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void*
read_and_parse_reindeer(struct cgs_vector* team)
{
        const char* dfmt = "%*s can fly %d km/s for %d seconds," \
                           " but then must rest for %d seconds.";

        for (struct deer d = { .state = FLYING };
                        scanf(dfmt, &d.speed, &d.fly, &d.rest) == 3; ) {
                d.count = d.fly;
                cgs_vector_push(team, &d);
        }

        return team;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int
get_max_distance(const struct cgs_vector* deer)
{
        const struct deer* d = cgs_vector_max(deer, deer_distance_cmp);
        return d->distance;
}

static int
get_max_points(const struct cgs_vector* deer)
{
        const struct deer* d = cgs_vector_max(deer, deer_points_cmp);
        return d->points;
}

static void
award_points(struct cgs_vector* team, int value)
{
        for (size_t i = 0; i < cgs_vector_length(team); ++i) {
                struct deer* d = cgs_vector_get_mut(team, i);
                if (d->distance == value)
                        ++d->points;
        }
}

static void
run_race(struct cgs_vector* team, int seconds)
{
        while (--seconds) {
                for (size_t i = 0; i < cgs_vector_length(team); ++i) {
                        struct deer* d = cgs_vector_get_mut(team, i);
                        if (d->state == FLYING)
                                d->distance += d->speed;
                        if (--d->count == 0) {
                                d->state = d->state == FLYING
                                        ? RESTING : FLYING;
                                d->count = d->state == FLYING
                                        ? d->fly : d->rest;
                        }
                }
                const int dist = get_max_distance(team);
                award_points(team, dist);
        }
}

int main(void)
{
        struct cgs_vector team = cgs_vector_new(sizeof(struct deer));
        read_and_parse_reindeer(&team);

        run_race(&team, SECONDS);

        printf("%d\n", get_max_distance(&team));
        printf("%d\n", get_max_points(&team));

        cgs_vector_free(&team);
        return EXIT_SUCCESS;
}
