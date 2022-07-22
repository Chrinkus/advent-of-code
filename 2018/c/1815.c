#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cgs/cgs.h>
#include <fruity.h>
#include <fruity_io.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data structures and constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum default_values { DEFAULT_HP = 200, DEFAULT_ATK = 3, };

enum cave_symbol { ELF = 'E', GOBLIN = 'G', WALL = '#', OPEN = '.', };

enum entity_type { TYPE_ELF, TYPE_GOBLIN, };

struct entity {
        enum entity_type type;
        int hp;
        int atk;
        int x;
        int y;
};

int entity_cmp(const void* a, const void* b)
{
        const struct entity* e1 = a;
        const struct entity* e2 = b;

        int res = e1->y - e2->y;
        return (res == 0) ? e1->x - e2->x : res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing and Setup
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* read_input(struct cgs_array* input)
{
        if (!cgs_array_new(input, sizeof(char*)))
                return cgs_error_retnull("cgs_array_new: input");

        if (!cgs_io_readlines(stdin, input))
                return cgs_error_retnull("cgs_io_readlines");

        return input;
}

void* init_cave_and_ents(Fruity2D* cave, struct cgs_array* ents,
                const struct cgs_array* input)
{
        char** ppc = fruity_data_mutable(cave);

        for (size_t i = 0; i < cgs_array_length(input); ++i) {
                const char*const s = *(CgsStrIter)cgs_array_get(input, i);
                for (size_t j = 0; j < strlen(s); ++j) {
                        ppc[i][j] = s[j];
                        if (s[j] == OPEN || s[j] == WALL)
                                continue;
                        struct entity ent = {
                                .type = s[j] == ELF ? TYPE_ELF : TYPE_GOBLIN,
                                .hp = DEFAULT_HP,
                                .atk = DEFAULT_ATK,
                                .x = j,
                                .y = i,
                        };
                        if (!cgs_array_push(ents, &ent))
                                return cgs_error_retnull("cgs_array_push");
                }
        }
        return cave;
}

void* read_input_and_init_structs(Fruity2D* cave, struct cgs_array* ents)
{
        struct cgs_array input = { 0 };
        if (!read_input(&input))
                return cgs_error_retnull("read_input");

        if (!cgs_array_new(ents, sizeof(struct entity))) {
                cgs_error_msg("cgs_array_new: ents");
                goto error_cleanup;
        }

        int rows = cgs_array_length(&input);
        int cols = strlen(*(const char*const*)cgs_array_get(&input, 0));
        if (!fruity_new(cave, rows, cols, sizeof(char))) {
                cgs_error_msg("fruity_new");
                goto error_cleanup;
        }

        if (!init_cave_and_ents(cave, ents, &input)) {
                cgs_error_msg("init_cave_and_ents");
                goto error_cleanup;
        }

        cgs_array_free_all(&input);
        return ents;

error_cleanup:
        cgs_array_free_all(&input);
        return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Print the Cave
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void print_cave(const Fruity2D* cave)
{
        fruity_foreach(cave,
                       fruity_io_newline, NULL,
                       fruity_io_print_char, NULL);
}

void print_entity(const void* a, size_t i, void* data)
{
        (void)i;
        (void)data;
        const struct entity* e = a;

        printf("%s (%2d,%2d) hp = %d\n",
                        e->type == TYPE_ELF ? "ELF" : "GOB",
                        e->x, e->y, e->hp);
}

void print_entities(const struct cgs_array* ents)
{
        cgs_array_foreach(ents, print_entity, NULL);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Run simulation
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void battle(Fruity2D* cave, struct cgs_array* ents)
{
        cgs_array_sort(ents, entity_cmp);
        print_entities(ents);
        print_cave(cave);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
	printf("Advent of Code 2018 Day 15: Beverage Bandits\n");

        Fruity2D cave = { 0 };
        struct cgs_array entities = { 0 };
        if (!read_input_and_init_structs(&cave, &entities))
                return cgs_error_retfail("read_input_and_init_structs");

        battle(&cave, &entities);

	int part1 = 0;
	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

        cgs_array_free(&entities);
        fruity_free(&cave);

	return EXIT_SUCCESS;
}
