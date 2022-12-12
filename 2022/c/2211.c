#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

typedef int Int;

enum magic { SELF = -1, RELIEF = 3, PART1_ROUNDS = 20, };
const char* old = "old";

struct Operation {
        char tok;
        Int n;
};

struct Monkey {
        size_t id;
        struct cgs_vector items;
        struct Operation op;
        int test;
        size_t t;
        size_t f;
        Int inspect_count;
};

/*
static void
monkey_print(const struct Monkey* m)
{
        printf("Monkey %zu\n", m->id);
        printf("\tHolding: ");
        for (size_t i = 0; i < cgs_vector_length(&m->items); ++i) {
                const Int* item = cgs_vector_get(&m->items, i);
                printf("%d, ", *item);
        }
        printf("\n");
        printf("\tOperation: %c %d\n", m->op.tok, m->op.n);
        printf("\tTest: %d\n", m->test);
        printf("\tTrue: %zu\n", m->t);
        printf("\tFalse: %zu\n", m->f);
        printf("\tCount: %d\n", m->inspect_count);
}
*/

static struct Monkey
monkey_new(void)
{
        return (struct Monkey){
                .id = 0,
                .items = cgs_vector_new(sizeof(Int)),
                .op = { .tok = '\0', .n = 0 },
                .test = 0,
                .t = 0,
                .f = 0,
                .inspect_count = 0,
        };
}

static void
monkey_free(void* p)
{
        struct Monkey* m = p;
        cgs_vector_free(&m->items);
}

static void*
parse_starting_items(const struct cgs_string* s, struct Monkey* m)
{
        struct cgs_vector subs = cgs_vector_new(sizeof(struct cgs_strsub));
        if (!cgs_string_split(s, ' ', &subs))
                return cgs_error_retnull("string_split");
        for (size_t i = 2; i < cgs_vector_length(&subs); ++i) {
                Int x = 0;
                if (!cgs_strsub_to_int(cgs_vector_get(&subs, i), &x))
                        return cgs_error_retnull("strsub_to_int");
                if (!cgs_vector_push(&m->items, &x))
                        return cgs_error_retnull("vector_push");
        }

        cgs_vector_free(&subs);
        return m;
}

static void*
parse_operation(const struct cgs_string* s, struct Monkey* m)
{
        struct cgs_vector subs = cgs_vector_new(sizeof(struct cgs_strsub));
        if (!cgs_string_split(s, ' ', &subs))
                return cgs_error_retnull("string_split");

        const struct cgs_strsub* tok = cgs_vector_get(&subs, 4);
        if (tok->length != 1)
                return cgs_error_retnull("Token read");
        m->op.tok = tok->data[0];

        const struct cgs_strsub* ss = cgs_vector_get(&subs, 5);
        if (cgs_strsub_eq_str(ss, old))
                m->op.n = SELF;
        else if (!cgs_strsub_to_int(ss, &m->op.n))
                return cgs_error_retnull("Operand read");


        cgs_vector_free(&subs);
        return m;
}

static void*
read_monkeys(struct cgs_vector* vm)
{
        struct cgs_string buff = cgs_string_new();
        while (!feof(stdin)) {
                struct Monkey m = monkey_new();
                if (scanf(" Monkey %zu: ", &m.id) != 1)
                        return cgs_error_retnull("scanf: Monkey id");

                if (cgs_io_getline(stdin, &buff) == 0)
                        return cgs_error_retnull("getline 1");
                if (!parse_starting_items(&buff, &m))
                        return cgs_error_retnull("starting_items");
                cgs_string_clear(&buff);

                if (cgs_io_getline(stdin, &buff) == 0)
                        return cgs_error_retnull("getline 2");
                if (!parse_operation(&buff, &m))
                        return cgs_error_retnull("operation");
                cgs_string_clear(&buff);

                if (scanf(" Test: divisible by %d ", &m.test) != 1)
                        return cgs_error_retnull("scanf: Test");
                if (scanf(" If true: throw to monkey %zu ", &m.t) != 1)
                        return cgs_error_retnull("scanf: true");
                if (scanf(" If false: throw to monkey %zu ", &m.f) != 1)
                        return cgs_error_retnull("scanf: false");

                if (!cgs_vector_push(vm, &m))
                        return cgs_error_retnull("vector_push");
        }

        cgs_string_free(&buff);
        return vm;
}

static Int
perform_operation(Int item, struct Operation* op)
{
        Int n = op->n == SELF ? item : op->n;
        switch (op->tok) {
        case '*':       return item * n;
        case '+':       return item + n;
        default:
                cgs_error_msg("Unknown token %c", op->tok);
                return 0;
        }
}

static void*
take_turn(struct Monkey* m, struct cgs_vector* vm)
{
        for (size_t i = 0; i < cgs_vector_length(&m->items); ++i) {
                Int item = *(Int*)cgs_vector_get_mut(&m->items, i);
                m->inspect_count += 1;
                item = perform_operation(item, &m->op);
                item /= RELIEF;

                struct Monkey* p = NULL;
                if (item % m->test == 0)
                        p = cgs_vector_get_mut(vm, m->t);
                else
                        p = cgs_vector_get_mut(vm, m->f);
                if (!cgs_vector_push(&p->items, &item))
                        return cgs_error_retnull("vector_push");
        }
        cgs_vector_clear(&m->items);
        return m;
}

static void*
monkey_around(struct cgs_vector* vm, const int rounds)
{
        for (int i = 0; i < rounds; ++i) {
                for (size_t j = 0; j < cgs_vector_length(vm); ++j) {
                        struct Monkey* m = cgs_vector_get_mut(vm, j);
                        if (!take_turn(m, vm))
                                return cgs_error_retnull("take turn");
                }
        }
        return vm;
}

static void
swap_int(int* a, int* b)
{
        int tmp = *a;
        *a = *b;
        *b = tmp;
}

static Int
get_monkey_business(const struct cgs_vector* vm)
{
        Int top2[2] = { 0 };
        for (size_t i = 0; i < cgs_vector_length(vm); ++i) {
                const struct Monkey* m = cgs_vector_get(vm, i);
                Int count = m->inspect_count;
                if (count > top2[0]) {
                        swap_int(&count, &top2[0]);
                }
                if (count > top2[1])
                        top2[1] = count;
        }
        return top2[0] * top2[1];
}

/*
static void
print_all_monkeys(const struct cgs_vector* vm)
{
        for (size_t i = 0; i < cgs_vector_length(vm); ++i) {
                const struct Monkey* m = cgs_vector_get(vm, i);
                monkey_print(m);
        }
}
*/

int main(void)
{
        struct cgs_vector monkeys = cgs_vector_new(sizeof(struct Monkey));
        if (!read_monkeys(&monkeys))
                return cgs_error_retfail("read_monkeys");

        if (!monkey_around(&monkeys, PART1_ROUNDS))
                return cgs_error_retfail("monkey around %d", PART1_ROUNDS);

        //print_all_monkeys(&monkeys);

        Int part1 = get_monkey_business(&monkeys);
        printf("%d\n", part1);

        cgs_vector_free_all_with(&monkeys, monkey_free);
        return EXIT_SUCCESS;
}
