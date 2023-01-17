#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"

enum ice { PART1_ROUNDS = 40, PART2_ROUNDS = 50, };

static size_t
play_look_and_say(struct cgs_string* input, int rounds)
{
        struct cgs_string buffer = cgs_string_new();

        struct cgs_string* b1 = input;
        struct cgs_string* b2 = &buffer;

        for (int i = 0; i < rounds; ++i) {
                for (const char* s = cgs_string_data(b1); *s; ) {
                        char ch = *s;
                        int count = 0;
                        while (*s == ch) {
                                ++s;
                                ++count;
                        }
                        cgs_string_push(b2, '0' + count);
                        cgs_string_push(b2, ch);
                }
                CGS_SWAP(b1, b2, struct cgs_string*);
                cgs_string_clear(b2);
        }

        if (rounds % 2 != 0) {
                cgs_string_free(input);
                cgs_string_move(&buffer, input);
        } else {
                cgs_string_free(&buffer);
        }

        return cgs_string_length(input);
}

int main(void)
{
        struct cgs_string input = cgs_string_new();
        cgs_io_getline(stdin, &input);

        size_t part1 = play_look_and_say(&input, PART1_ROUNDS);
        printf("%zu\n", part1);

        size_t part2 = play_look_and_say(&input, PART2_ROUNDS - PART1_ROUNDS);
        printf("%zu\n", part2);

        cgs_string_free(&input);
        return EXIT_SUCCESS;
}
