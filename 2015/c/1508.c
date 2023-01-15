#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

struct char_counts {
        int code;
        int mem;
        int encode;
};

void
count_chars(const struct cgs_string* line, struct char_counts* cc)
{
        cc->encode += 2;
        for (const char* s = cgs_string_data(line); *s; ++s) {
                if (*s == '"') {
                        cc->code += 1;
                        cc->encode += 2;
                } else if (*s == '\\') {
                        if (*(s+1) == '"' || *(s+1) == '\\') {
                                cc->code += 2;
                                cc->mem += 1;
                                cc->encode += 4;
                                s += 1;
                        } else {
                                cc->code += 4;
                                cc->mem += 1;
                                cc->encode += 5;
                                s += 3;
                        }
                } else {
                        ++cc->code;
                        ++cc->mem;
                        ++cc->encode;
                }
        }
}

void
read_and_count_chars(struct char_counts* cc)
{
        struct cgs_string line = cgs_string_new();
        for ( ; cgs_io_getline(stdin, &line) > 0; cgs_string_clear(&line)) {
                count_chars(&line, cc);
        }
        cgs_string_free(&line);
}

int main(void)
{
        struct char_counts cc = { 0 };
        read_and_count_chars(&cc);

        printf("%d\n", cc.code - cc.mem);
        printf("%d\n", cc.encode - cc.code);

        return EXIT_SUCCESS;
}
