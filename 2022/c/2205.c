#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <cgs/cgs.h>

struct Stack {
        struct cgs_vector crates;
};

static struct Stack
stack_new()
{
        struct cgs_vector crates = cgs_vector_new(sizeof(char));
        return (struct Stack){ .crates = crates };
}

static void
stack_free(void* p)
{
        struct Stack* stack = p;
        cgs_vector_free(&stack->crates);
}

static void*
init_stacks(const struct cgs_vector* lines, struct cgs_vector* stacks)
{
        if (cgs_vector_length(lines) == 0)
                return NULL;

        const struct cgs_string* s = cgs_vector_get(lines, 0);
        for (size_t i = 1; i < cgs_string_length(s); i += 4) {
                struct Stack stack = stack_new();
                if (!cgs_vector_push(stacks, &stack))
                        return NULL;
        }
        return stacks;
}

static void*
push_crates(const struct cgs_string* s, struct cgs_vector* stacks)
{
        for (size_t i = 1, j = 0; i < cgs_string_length(s); i += 4, ++j) {
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
print_stacks(const struct cgs_vector* stacks)
{
        for (size_t i = 0; i < cgs_vector_length(stacks); ++i) {
                printf("%zu: ", i + 1);
                const struct Stack* st = cgs_vector_get(stacks, i);
                for (size_t j = 0; j < cgs_vector_length(&st->crates); ++j) {
                        const char* ch = cgs_vector_get(&st->crates, j);
                        printf("%c", *ch);
                }
                printf("\n");
        }
}

int main(void)
{
        struct cgs_vector stacks = cgs_vector_new(sizeof(struct Stack));
        if (!read_stacks(&stacks))
                return cgs_error_retfail("read_stacks");

        print_stacks(&stacks);

        cgs_vector_free_all_with(&stacks, stack_free);
        return EXIT_SUCCESS;
}
