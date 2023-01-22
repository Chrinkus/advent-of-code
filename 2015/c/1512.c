#include <stdlib.h>
#include <stdio.h>

#include "cgs.h"
#include "cJSON.h"

static int
sum_all_numbers(const cJSON* json)
{
        if (cJSON_IsNumber(json))
                return json->valueint;

        int sum = 0;
        if (cJSON_IsArray(json) || cJSON_IsObject(json))
                for (const cJSON* it = json->child; it; it = it->next)
                        sum += sum_all_numbers(it);
        return sum;
}

static int
sum_non_red_numbers(const cJSON* json)
{
        if (cJSON_IsNumber(json))
                return json->valueint;

        if (cJSON_IsObject(json))
                for (const cJSON* it = json->child; it; it = it->next)
                        if (cJSON_IsString(it) &&
                                        strcmp(it->valuestring, "red") == 0)
                                return 0;

        int sum = 0;
        if (cJSON_IsArray(json) || cJSON_IsObject(json))
                for (const cJSON* it = json->child; it; it = it->next)
                        sum += sum_non_red_numbers(it);
        return sum;
}

int main(void)
{
        struct cgs_string buffer = cgs_string_new();
        cgs_io_getline(stdin, &buffer);

        cJSON* json = cJSON_Parse(cgs_string_data(&buffer));

        int part1 = sum_all_numbers(json);
        printf("%d\n", part1);

        int part2 = sum_non_red_numbers(json);
        printf("%d\n", part2);

        cJSON_Delete(json);
        cgs_string_free(&buffer);
        return EXIT_SUCCESS;
}
