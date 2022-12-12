#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum magic { DELIM = ' ', };

const size_t signals[] = { 20, 60, 100, 140, 180, 220, };
const size_t siglen = CGS_ARRAY_LENGTH(signals);

static void*
read_program(struct cgs_vector* prog)
{
        if (!cgs_io_readlines(stdin, prog))
                return cgs_error_retnull("readlines");
        return prog;
}

static int
run_program(const struct cgs_vector* prog)
{
        int sum = 0;
        struct cgs_vector subs = cgs_vector_new(sizeof(struct cgs_strsub));

        int x = 1;
        int n = 0;
        for (size_t cycle = 1, i = 0, sig = 0;
                        i < cgs_vector_length(prog) && sig < siglen;
                        ++cycle, cgs_vector_clear(&subs)) {
                if (cycle == signals[sig]) {
                        sum += signals[sig] * x;
                        ++sig;
                }
                if (n) {
                        x += n;
                        n = 0;
                        ++i;
                        continue;
                }
                const struct cgs_string* line = cgs_vector_get(prog, i);
                if (!cgs_string_split(line, DELIM, &subs))
                        return cgs_error_retfail("string_split");
                const struct cgs_strsub* ss = cgs_vector_get(&subs, 0);
                if (cgs_strsub_eq_str(ss, "noop")) {
                        ++i;
                        continue;
                }
                if (cgs_strsub_eq_str(ss, "addx")) {
                        ss = cgs_vector_get(&subs, 1);
                        if (!cgs_strsub_to_int(ss, &n))
                                return cgs_error_retfail("to_int");
                }
        }

        cgs_vector_free(&subs);
        return sum;
}

int main(void)
{
        struct cgs_vector prog = cgs_vector_new(sizeof(struct cgs_string));
        if (!read_program(&prog))
                return cgs_error_retfail("read_program");

        int part1 = run_program(&prog);
        printf("%d\n", part1);

        cgs_vector_free_all_with(&prog, cgs_string_free);
        return EXIT_SUCCESS;
}
