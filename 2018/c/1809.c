/*
 * Another solution that doesn't need my libraries. This time I will work with
 * a circular linked-list.
 *
 * I'm not sure if there's a way to find the final answer without actually
 * playing through the games so I'll just be running them. There is a slight
 * optimization to be gained from storing the last marble and player of the
 * first game and continuing from there but it seems like it would just be
 * a 1% improvement.
 */
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
typedef int64_t Int;    // potentially big scores

enum { MAGIC_VALUE = 23, MAGIC_STEPS = -7, NORMAL_STEPS = 2, P2_FACTOR = 100 };

struct input {
        int players;
        int marbles;
};

struct marble {
        int value;
        struct marble* next;
        struct marble* prev;
};

struct marble* marble_new(int val)
{
        struct marble* m = malloc(sizeof(struct marble));
        if (!m)
                return NULL;
        m->value = val;
        m->next = m->prev = m;  // circular list, all marbles are round!
        return m;
}

struct marble_game {
        struct marble* current;
        Int* scores;
        int num_players;
        int max_marble;
};

void* marble_game_new(struct marble_game* mg, const struct input* input)
{
        struct marble* m = marble_new(0);
        if (!m)
                return NULL;

        Int* scores = calloc(input->players, sizeof(Int));
        if (!scores) {
                free(m);
                return NULL;
        }
        mg->current = m;
        mg->scores = scores;
        mg->num_players = input->players;
        mg->max_marble = input->marbles;
        return mg;
}

void marble_game_free(struct marble_game* mg)
{
        struct marble* curr = mg->current;
        curr->prev->next = NULL;        // break circle

        while (curr) {
                struct marble* p = curr->next;
                free(curr);
                curr = p;
        }
        free(mg->scores);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int read_input(struct input* input)
{
        const char* fmt = "%d players; last marble is worth %d points";
        return scanf(fmt, &input->players, &input->marbles) == 2;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 & 2 - Play the game, find the winner
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct marble* marble_advance(struct marble* m, int i)
{
        if (i >= 0) {
                while (i--)
                        m = m->next;
        } else {
                while (i++)
                        m = m->prev;
        }
        return m;
}

struct marble* marble_insert(struct marble* curr, struct marble* m)
        // inserts m before curr and returns m (to be assigned as new curr)
{
        m->next = curr;
        m->prev = curr->prev;
        curr->prev->next = m;
        curr->prev = m;
        return m;
}

struct marble* marble_remove(struct marble* m)
        // removes given marble and returns new current marble
{
        m->prev->next = m->next;
        m->next->prev = m->prev;
        return m->next;
}

void* play_game(struct marble_game* mg)
{
        struct marble* curr = mg->current;

        for (int i = 0, m = 1; m <= mg->max_marble; ++m) {
                if (m % MAGIC_VALUE == 0) {     // Handle the magic case
                        curr = marble_advance(curr, MAGIC_STEPS);
                        struct marble* p = curr;
                        curr = marble_remove(p);
                        mg->scores[i] += m + p->value;
                        free(p);
                } else {                        // Handle normal case
                        struct marble* p = marble_new(m);
                        if (!p)
                                return NULL;
                        curr = marble_advance(curr, NORMAL_STEPS);
                        curr = marble_insert(curr, p);
                }
                i = (i + 1) % mg->num_players;
        }
        mg->current = curr;
        return mg;
}

Int get_max_score(const struct marble_game* mg)
{
        Int max = 0;
        for (int i = 0; i < mg->num_players; ++i)
                max = max > mg->scores[i] ? max : mg->scores[i];
        return max;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 9: Marble Mania\n");

        struct input input = { 0 };
        if (!read_input(&input))
                return EXIT_FAILURE;

        struct marble_game game1 = { 0 };
        if (!marble_game_new(&game1, &input) || !play_game(&game1))
                return EXIT_FAILURE;

        Int part1 = get_max_score(&game1);
        printf("Part 1: %"PRId64"\n", part1);
        marble_game_free(&game1);

        input.marbles *= P2_FACTOR;

        struct marble_game game2 = { 0 };
        if (!marble_game_new(&game2, &input) || !play_game(&game2))
                return EXIT_FAILURE;

        Int part2 = get_max_score(&game2);
        printf("Part 2: %"PRId64"\n", part2);
        marble_game_free(&game2);

        return EXIT_SUCCESS;
}

