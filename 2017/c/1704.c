#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

int
check_duplicates(const struct cgs_array* phrases)
{
        const struct cgs_strsub* prev = cgs_array_get(phrases, 0);
        for (size_t i = 1, len = cgs_array_length(phrases); i < len; ++i) {
                const struct cgs_strsub* curr = cgs_array_get(phrases, i);
                if (cgs_strsub_cmp(prev, curr) == 0)
                        return 0;
                prev = curr;
        }
        return 1;
}

int
check_anagrams(const struct cgs_array* phrases)
{
        (void)phrases;
        return 1;
}

void*
count_valid_passphrases(int* dups, int* anas)
{
        struct cgs_string buff = { 0 };
        if (!cgs_string_new(&buff))
                return NULL;

        struct cgs_array splits = { 0 };
        if (!cgs_array_new(&splits, sizeof(struct cgs_strsub)))
                return NULL;

        while (cgs_io_getline(stdin, &buff) > 0) {
                if (!cgs_str_split(cgs_string_data(&buff), ' ', &splits))
                        return NULL;

                cgs_array_sort(&splits, cgs_strsub_cmp);

                *dups += check_duplicates(&splits);
                *anas += check_anagrams(&splits);
                cgs_string_clear(&buff);
                cgs_array_clear(&splits);
        }

        cgs_string_free(&buff);
        cgs_array_free(&splits);

        return dups;
}

int main(void)
{
        int part1 = 0;
        int part2 = 0;

        if (!count_valid_passphrases(&part1, &part2))
                return cgs_error_retfail("count_valid_passphrases");

        printf("%d\n", part1);
        printf("%d\n", part2);

        return EXIT_SUCCESS;
}
