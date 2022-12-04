#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <cgs/cgs.h>

enum magic {
        PRIO_LOWER_A = 1,
        PRIO_UPPER_A = 27,
        COUNT_BUFF = 53,        // 26 + 26 + 1 (ignore the zero-index)
        GROUP_SIZE = 3,
};

struct Charcount {
        int a[COUNT_BUFF];
};

static void*
read_lines(FILE* file, struct cgs_vector* lines)
{
        struct cgs_string buff = cgs_string_new();
        while (cgs_io_getline(file, &buff) > 0) {
                struct cgs_string line = cgs_string_new();
                cgs_string_move(&buff, &line);
                if (!cgs_vector_push(lines, &line))
                        return cgs_error_retnull("cgs_vector_push");
        }
        cgs_string_free(&buff);
        return lines;
}

static int
get_priority(char ch)
{
        return islower(ch)
                ? ch - 'a' + PRIO_LOWER_A
                : ch - 'A' + PRIO_UPPER_A;
}

static struct Charcount
count_chars(const char* s, size_t len)
{
        struct Charcount chars = { 0 };
        for (size_t i = 0; i < len; ++i)
                ++chars.a[get_priority(s[i])];
        return chars;
}

static int
get_repeat_priority(const struct cgs_string* ruck)
{
        const size_t half = cgs_string_length(ruck) / 2;
        const struct Charcount chars = count_chars(cgs_string_data(ruck), half);

        for (size_t i = half; i < cgs_string_length(ruck); ++i) {
                const int prio = get_priority(cgs_string_char(ruck, i));
                if (chars.a[prio] > 0)
                        return prio;
        }
        return 0;
}

static int
get_repeat_priority_sum(const struct cgs_vector* rucks)
{
        int sum = 0;
        for (size_t i = 0; i < cgs_vector_length(rucks); ++i) {
                const struct cgs_string* ruck = cgs_vector_get(rucks, i);
                sum += get_repeat_priority(ruck);
        }
        return sum;
}

static int
get_group_badge_priority(const struct cgs_string* group[])
{
        const struct Charcount chars1 = count_chars(cgs_string_data(group[0]),
                        cgs_string_length(group[0]));
        const struct Charcount chars2 = count_chars(cgs_string_data(group[1]),
                        cgs_string_length(group[1]));

        for (size_t i = 0; i < cgs_string_length(group[2]); ++i) {
                const int prio = get_priority(cgs_string_char(group[2], i));
                if (chars1.a[prio] > 0 && chars2.a[prio] > 0)
                        return prio;
        }
        return 0;
}

static int
get_group_badge_priority_sum(const struct cgs_vector* rucks)
{
        if (cgs_vector_length(rucks) % 3 != 0)
                return -1;

        int sum = 0;
        const struct cgs_string* group[GROUP_SIZE];
        for (size_t i = 0; i < cgs_vector_length(rucks); i += GROUP_SIZE) {
                for (size_t j = 0; j < GROUP_SIZE; ++j)
                        group[j] = cgs_vector_get(rucks, i+j);

                sum += get_group_badge_priority(group);
        }
        return sum;
}

int main(void)
{
        struct cgs_vector rucks = cgs_vector_new(sizeof(struct cgs_string));
        if (!read_lines(stdin, &rucks))
                return EXIT_FAILURE;

        const int part1 = get_repeat_priority_sum(&rucks);
        const int part2 = get_group_badge_priority_sum(&rucks);

        printf("%d\n", part1);
        printf("%d\n", part2);

        cgs_vector_free_all_with(&rucks, cgs_string_free);
        return EXIT_SUCCESS;
}
