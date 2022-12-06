#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <cgs/cgs.h>

enum magic { START = 1, STRIDE = 4 };

struct Stack {
        struct cgs_vector crates;
};

static struct Stack
stack_new()
{
        struct cgs_vector crates = cgs_vector_new(sizeof(char));
        return (struct Stack){ .crates = crates };
}

static void*
stack_copy(const struct Stack* s1, struct Stack* s2)
{
        return cgs_vector_copy(&s1->crates, &s2->crates);
}

static void
stack_free(void* p)
{
        struct Stack* stack = p;
        cgs_vector_free(&stack->crates);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct Move {
        int n;          // number
        int f;          // from
        int t;          // to
};

static void*
read_moves(struct cgs_vector* moves)
{
        for (struct Move m;
                        scanf("move %d from %d to %d ", &m.n,&m.f,&m.t) == 3; )
                if (!cgs_vector_push(moves, &m))
                        return NULL;
        return moves;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static void*
init_stacks(const struct cgs_vector* lines, struct cgs_vector* stacks)
{
        if (cgs_vector_length(lines) == 0)
                return NULL;

        const struct cgs_string* s = cgs_vector_get(lines, 0);
        for (size_t i = START; i < cgs_string_length(s); i += STRIDE) {
                struct Stack stack = stack_new();
                if (!cgs_vector_push(stacks, &stack))
                        return NULL;
        }
        return stacks;
}

static void*
dup_stacks(const struct cgs_vector* src, struct cgs_vector* dst)
{
        for (size_t i = 0; i < cgs_vector_length(src); ++i) {
                const struct Stack* s1 = cgs_vector_get(src, i);
                struct Stack s2 = stack_new();
                if (!stack_copy(s1, &s2))
                        return NULL;
                if (!cgs_vector_push(dst, &s2))
                        return NULL;
        }
        return dst;
}

static void*
push_crates(const struct cgs_string* s, struct cgs_vector* stacks)
{
        for (size_t i = START, j = 0; i < cgs_string_length(s);
                        i += STRIDE, ++j) {
                char ch = cgs_string_char(s, i);
                if (!isalpha(ch))
                        continue;
                struct Stack* p = cgs_vector_get_mut(stacks, j);
                if (!cgs_vector_push(&p->crates, &ch))
                        return NULL;
        }
        return stacks;
}

static void*
read_stacks(struct cgs_vector* stacks)
{
        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        struct cgs_string buff = cgs_string_new();
        while (cgs_io_getline(stdin, &buff) > 0) {
                struct cgs_string line = cgs_string_new();
                cgs_string_move(&buff, &line);
                if (!cgs_vector_push(&lines, &line))
                        return NULL;
        }

        if (!init_stacks(&lines, stacks))
                return NULL;

        for (size_t l = cgs_vector_length(&lines), i = l - 2; i < l; --i) {
                const struct cgs_string* s = cgs_vector_get(&lines, i);
                if (!push_crates(s, stacks))
                        return NULL;
        }

        cgs_vector_free_all_with(&lines, cgs_string_free);
        return stacks;
}

static void
print_stack_tops(const struct cgs_vector* stacks)
{
        for (size_t i = 0; i < cgs_vector_length(stacks); ++i) {
                const struct Stack* st = cgs_vector_get(stacks, i);
                const char* ch = cgs_vector_last(&st->crates);
                putchar(*ch);
        }
        putchar('\n');
}

static size_t
ctoi(int col)
{
        return (size_t)col - 1;
}

static void*
move_crates_1(const struct cgs_vector* moves, struct cgs_vector* stacks)
{
        for (size_t i = 0; i < cgs_vector_length(moves); ++i) {
                const struct Move* m = cgs_vector_get(moves, i);

                struct Stack* src = cgs_vector_get_mut(stacks, ctoi(m->f));
                struct Stack* dst = cgs_vector_get_mut(stacks, ctoi(m->t));
                for (int j = 0; j < m->n; ++j) {
                        char ch = '\0';
                        if (!cgs_vector_pop(&src->crates, &ch))
                                return NULL;
                        if (!cgs_vector_push(&dst->crates, &ch))
                                return NULL;
                }
        }
        return stacks;
}

static void*
move_crates_2(const struct cgs_vector* moves, struct cgs_vector* stacks)
{
        struct cgs_vector buff = cgs_vector_new(sizeof(char));

        for (size_t i = 0; i < cgs_vector_length(moves); ++i) {
                const struct Move* m = cgs_vector_get(moves, i);

                struct Stack* src = cgs_vector_get_mut(stacks, ctoi(m->f));
                struct Stack* dst = cgs_vector_get_mut(stacks, ctoi(m->t));
                for (int j = 0; j < m->n; ++j) {
                        char ch = '\0';
                        if (!cgs_vector_pop(&src->crates, &ch))
                                return NULL;
                        if (!cgs_vector_push(&buff, &ch))
                                return NULL;
                }
                for (int j = 0; j < m->n; ++j) {
                        char ch = '\0';
                        if (!cgs_vector_pop(&buff, &ch))
                                return NULL;
                        if (!cgs_vector_push(&dst->crates, &ch))
                                return NULL;
                }
        }
        cgs_vector_free(&buff);
        return stacks;
}

int main(void)
{
        struct cgs_vector stacks1 = cgs_vector_new(sizeof(struct Stack));
        if (!read_stacks(&stacks1))
                return cgs_error_retfail("read_stacks");

        struct cgs_vector moves = cgs_vector_new(sizeof(struct Move));
        if (!read_moves(&moves))
                return cgs_error_retfail("read_moves");

        struct cgs_vector stacks2 = cgs_vector_new(sizeof(struct Stack));
        if (!dup_stacks(&stacks1, &stacks2))
                return cgs_error_retfail("dup_stacks");

        if (!move_crates_1(&moves, &stacks1))
                return cgs_error_retfail("move_crates_1");
        print_stack_tops(&stacks1);

        if (!move_crates_2(&moves, &stacks2))
                return cgs_error_retfail("move_crates_2");
        print_stack_tops(&stacks2);

        cgs_vector_free_all_with(&stacks1, stack_free);
        cgs_vector_free_all_with(&stacks2, stack_free);
        cgs_vector_free(&moves);
        return EXIT_SUCCESS;
}
