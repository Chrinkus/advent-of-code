#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <cgs/cgs.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum {
        FIRST = '3',
        SECOND = '7',

        ZERO = '0',
        ONE = '1',

        BUFFLEN = 10,
        BUFFMAX = BUFFLEN + 1,
};

#define ntod(n) (ZERO + (n))
#define dton(d) ((d) - ZERO)

struct kitchen {
        size_t e1;              // first elf
        size_t e2;              // second elf
        int r1;                 // first elf's recipe
        int r2;                 // second elf's recipe
        struct cgs_string sb;   // scoreboard(?)
};


struct kitchen*
kitchen_init(struct kitchen* k)
{
        if (!cgs_string_new(&k->sb))
                return cgs_error_retnull("cgs_string_new");

        if (!cgs_string_push(&k->sb, FIRST)
                        || !cgs_string_push(&k->sb, SECOND)) {
                cgs_string_free(&k->sb);
                return cgs_error_retnull("cgs_string_push");
        }

        k->e1 = 0;
        k->e2 = 1;
        k->r1 = k->r2 = 0;

        return k;
}

void
kitchen_free(struct kitchen* k)
{
        if (!k)
                return;

        cgs_string_free(&k->sb);
}

struct kitchen*
kitchen_add_recipe(struct kitchen* k, char recipe)
{
        if (!cgs_string_push(&k->sb, recipe))
                return cgs_error_retnull("cgs_string_push");
        return k;
}

int
kitchen_cook(struct kitchen* k)
{
        k->r1 = dton(cgs_string_char(&k->sb, k->e1));
        k->r2 = dton(cgs_string_char(&k->sb, k->e2));
        return k->r1 + k->r2;
}

size_t
kitchen_scoreboard_length(const struct kitchen* k)
{
        return cgs_string_length(&k->sb);
}

void
kitchen_update_elves(struct kitchen* k)
{
        k->e1 = (k->e1 + k->r1 + 1) % cgs_string_length(&k->sb);
        k->e2 = (k->e2 + k->r2 + 1) % cgs_string_length(&k->sb);
}

char*
kitchen_get_m_after_n(const struct kitchen* k, int m, int n)
{
        char* ret = malloc(m + 1);
        if (!ret)
                return cgs_error_retnull("malloc");

        strncpy(ret, cgs_string_get(&k->sb, n), m);
        ret[m] = '\0';
        return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int read_input(char* buff, int len)
{
        int n = 0;
        for (int c; n < len && (c = fgetc(stdin)) != EOF && c != '\n'; )
                if (isdigit(c))
                        buff[n++] = c;
        buff[n] = '\0';
        return n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Get 10 recipes after input number of recipes
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct cgs_string* init_scoreboard(struct cgs_string* sb)
{
        if (!cgs_string_new(sb))
                return cgs_error_retnull("cgs_string_new");

        if (!cgs_string_push(sb, FIRST) || !cgs_string_push(sb, SECOND)) {
                cgs_string_free(sb);
                return cgs_error_retnull("cgs_string_push");
        }

        return sb;
}

size_t get_next_recipe_index(size_t prev, int val, const size_t len)
{
        return (prev + val + 1) % len;
}

char* get_next_10_after(const char* input)
{
        const int n = strtol(input, NULL, 10);
        const size_t max = n + BUFFLEN;

        struct kitchen k = { 0 };
        if (!kitchen_init(&k))
                return cgs_error_retnull("kitchen_init");

        while (kitchen_scoreboard_length(&k) < max) {
                int sum = kitchen_cook(&k);

                if (sum >= 10) {
                        sum %= 10;
                        if (!kitchen_add_recipe(&k, ONE))
                                goto add_error;
                }
                if (!kitchen_add_recipe(&k, ntod(sum)))
                        goto add_error;

                kitchen_update_elves(&k);
        }

        char* ret = kitchen_get_m_after_n(&k, BUFFLEN, n);

        kitchen_free(&k);
        return ret;

add_error:
        kitchen_free(&k);
        return cgs_error_retnull("kitchen_add_recipe");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - Find how many recipes appear before input string
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int kitchen_check_end(const struct kitchen* k, const char* s)
{
        const size_t ilen = strlen(s);
        const size_t klen = kitchen_scoreboard_length(k);

        if (klen >= ilen && strcmp(s, cgs_string_end(&k->sb) - ilen) == 0)
                return klen - ilen;

        return 0;
}

int kitchen_add_and_check(struct kitchen* k, int c, const char* s, int last)
        // improvement? combined two calls that were used identically in two
        // places but had a three-way result to deal with..
{
        if (!kitchen_add_recipe(k, c))
                return -1;                      // push error
        if (c == last)
                return kitchen_check_end(k, s); // check for match
        return 0;                               // no match possible
}

int get_num_recipes_till(const char* input)
{
        const size_t len = strlen(input);
        const char last = input[len - 1];

        struct kitchen k = { 0 };
        if (!kitchen_init(&k))
                return cgs_error_retbool("kitchen_init");

        int n = 0;
        while (n == 0) {
                int sum = kitchen_cook(&k);
                if (sum >= 10) {
                        n = kitchen_add_and_check(&k, ONE, input, last);
                        if (n < 0)
                                goto add_error;
                        else if (n > 0)
                                break;
                        sum %= 10;
                }
                n = kitchen_add_and_check(&k, ntod(sum), input, last);
                if (n < 0)
                        goto add_error;

                kitchen_update_elves(&k);
        }

        kitchen_free(&k);
        return n;
add_error:
        kitchen_free(&k);
        return cgs_error_retbool("kitchen_add_recipe");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 14: Chocolate Charts\n");

        char input[BUFFMAX];
        if (read_input(input, BUFFLEN) == 0)
                return cgs_error_retfail("read_input");

        char* part1 = get_next_10_after(input);
        if (!part1)
                return cgs_error_retfail("get_next_10_after");
        int part2 = get_num_recipes_till(input);

        printf("Part 1: %s\n", part1);
        printf("Part 2: %d\n", part2);

        free(part1);

        return EXIT_SUCCESS;
}

