#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

struct parts {
        int p1;
        int p2;
};

void*
parse_nums_and_sort(struct cgs_array* a, char* s)
{
        for (char* p = NULL; *s != '\0'; s = p) {
                int n = strtol(s, &p, 10);
                if (!cgs_array_push(a, &n))
                        return cgs_error_retnull("cgs_array_push");
        }
        cgs_array_sort(a, cgs_int_cmp);
        return a;
}

int
get_checksum(const int* arr, const size_t len)
{
        return arr[len-1] - arr[0];
}

int
get_even_div(const int* arr, const size_t len)
{
        for (size_t i = len-1; i != 0; --i) {
                int half = arr[i] / 2;
                for (size_t j = 0; arr[j] <= half; ++j)
                        if (arr[i] % arr[j] == 0)
                                return arr[i] / arr[j];
        }
        return 0;
}

void*
process_input(struct cgs_string* buff, struct cgs_array* nums, struct parts* ps)
{
        while (cgs_io_getline(stdin, buff) > 0) {
                if (!parse_nums_and_sort(nums, cgs_string_data_mutable(buff)))
                        return cgs_error_retnull("parse_nums_and_sort");

                const int* arr = cgs_array_data(nums);
                const size_t len = cgs_array_length(nums);

                ps->p1 += get_checksum(arr, len);
                ps->p2 += get_even_div(arr, len);

                cgs_string_clear(buff);
                cgs_array_clear(nums);
        }
        return ps;
}

int main(void)
{
        struct cgs_string buff = { 0 };
        if (!cgs_string_new(&buff))
                return cgs_error_retfail("cgs_string_new");

        struct cgs_array nums = { 0 };
        if (!cgs_array_new(&nums, sizeof(int)))
                return cgs_error_retfail("cgs_array_new");

        struct parts ps = { 0 };
        if (!process_input(&buff, &nums, &ps))
                return cgs_error_retfail("process_input");

        printf("%d\n%d\n", ps.p1, ps.p2);

        cgs_string_free(&buff);
        cgs_array_free(&nums);
        return EXIT_SUCCESS;
}
