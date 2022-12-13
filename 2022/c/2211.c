#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "cgs.h"

typedef int64_t Int;

enum magic {
        SELF = -1,
        RELIEF = 3,
        PART1_ROUNDS = 20,
        PART2_ROUNDS = 10000,
};

const char* old = "old";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct Operation {
        char tok;
        int n;
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

static void*
monkey_copy(const void* src, void* dst)
{
        const struct Monkey* m1 = src;
        struct Monkey* m2 = dst;

        if (!cgs_vector_copy(&m1->items, &m2->items))
                return cgs_error_retnull("vector_copy");

        m2->id = m1->id;
        m2->op = m1->op;
        m2->test = m1->test;
        m2->t = m1->t;
        m2->f = m1->f;
        m2->inspect_count = m1->inspect_count;
        return m2;
}

static void
monkey_free(void* p)
{
        struct Monkey* m = p;
        cgs_vector_free(&m->items);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static void*
parse_starting_items(const struct cgs_string* s, struct Monkey* m)
{
        struct cgs_vector subs = cgs_vector_new(sizeof(struct cgs_strsub));
        if (!cgs_string_split(s, ' ', &subs))
                return cgs_error_retnull("string_split");
        for (size_t i = 2; i < cgs_vector_length(&subs); ++i) {
                int x = 0;
                if (!cgs_strsub_to_int(cgs_vector_get(&subs, i), &x))
                        return cgs_error_retnull("strsub_to_int");
                Int xx = x;
                if (!cgs_vector_push(&m->items, &xx))
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
read_monkeys(struct cgs_vector* vm, Int* lcm)
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
                *lcm *= m.test;

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
perform_operation(Int item, struct Operation* op, const Int lcm)
{
        Int n = op->n == SELF ? item : op->n;
        switch (op->tok) {
        case '*':       item *= n;      break;
        case '+':       item += n;      break;
        default:
                cgs_error_msg("Unknown token %c", op->tok);
                return 0;
        }
        return lcm == 0 ? item : item % lcm;
}

static void*
take_turn(struct Monkey* m, struct cgs_vector* vm, const Int lcm)
{
        for (size_t i = 0; i < cgs_vector_length(&m->items); ++i) {
                Int item = *(Int*)cgs_vector_get_mut(&m->items, i);
                m->inspect_count += 1;
                item = perform_operation(item, &m->op, lcm);
                if (lcm == 0)
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
monkey_around(struct cgs_vector* vm, const int rounds, const Int lcm)
{
        for (int i = 0; i < rounds; ++i) {
                for (size_t j = 0; j < cgs_vector_length(vm); ++j) {
                        struct Monkey* m = cgs_vector_get_mut(vm, j);
                        if (!take_turn(m, vm, lcm))
                                return cgs_error_retnull("take turn");
                }
        }
        return vm;
}

static Int
get_monkey_business(const struct cgs_vector* vm)
{
        Int top2[2] = { 0 };
        for (size_t i = 0; i < cgs_vector_length(vm); ++i) {
                const struct Monkey* m = cgs_vector_get(vm, i);
                Int count = m->inspect_count;
                if (count > top2[0]) {
                        CGS_SWAP(count, top2[0], Int);
                }
                if (count > top2[1])
                        top2[1] = count;
        }
        return top2[0] * top2[1];
}

int main(void)
{
        Int worry_lcm = 1;
        struct cgs_vector monkeys1 = cgs_vector_new(sizeof(struct Monkey));
        if (!read_monkeys(&monkeys1, &worry_lcm))
                return cgs_error_retfail("read_monkeys");

        struct cgs_vector monkeys2 = cgs_vector_new(0);
        if (!cgs_vector_copy_with(&monkeys1, &monkeys2, monkey_copy))
                return cgs_error_retfail("vector_copy_with");

        // Part 1
        if (!monkey_around(&monkeys1, PART1_ROUNDS, 0))
                return cgs_error_retfail("monkey around %d", PART1_ROUNDS);
        Int part1 = get_monkey_business(&monkeys1);
        printf("%"PRId64"\n", part1);

        // Part 2
        if (!monkey_around(&monkeys2, PART2_ROUNDS, worry_lcm))
                return cgs_error_retfail("monkey around %d", PART2_ROUNDS);
        Int part2 = get_monkey_business(&monkeys2);
        printf("%"PRId64"\n", part2);

        cgs_vector_free_all_with(&monkeys1, monkey_free);
        cgs_vector_free_all_with(&monkeys2, monkey_free);
        return EXIT_SUCCESS;
}
