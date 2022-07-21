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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
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

        if (!cgs_array_new(ents, sizeof(struct entity)))
                goto error_cleanup;

        int rows = cgs_array_length(&input);
        int cols = strlen(*(const char*const*)cgs_array_get(&input, 0));
        if (!fruity_new(cave, rows, cols, sizeof(char)))
                goto error_cleanup;

        if (!init_cave_and_ents(cave, ents, &input))
                goto error_cleanup;

        cgs_array_free_all(&input);
        return ents;

error_cleanup:
        cgs_array_free_all(&input);
        return NULL;
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

	int part1 = 0;
	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

        cgs_array_free(&entities);
        fruity_free(&cave);

	return EXIT_SUCCESS;
}
