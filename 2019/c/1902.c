#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "vector.h"

#define MAXBUFF 8
#define ungetchar(v) ungetc(v, stdin)

int getint(int* pn)
{
    int c, sign;

    while (isspace(c = getchar()))
        ;
    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetchar(c);
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
        c = getchar();
    for (*pn = 0; isdigit(c); c = getchar())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;
    if (c != EOF)
        ungetchar(c);
    return c;
}

int main(void)
{
    printf("Advent of Code 2019 - Day 2\n");
    printf("1202 Program Alarm\n");

    vector v;
    vector_init(&v);

    /*
    for (int val = 0, c = 0; c != EOF && getint(&val) != EOF; ) {
        vector_push(&v, val);
        c = getchar();
    }
    */
    char buff[MAXBUFF];
    while (fgets(buff, MAXBUFF, stdin))
        vector_push(&v, atoi(buff));

	int32_t part1 = vector_size(&v);
	int32_t part2 = 0;

    for (int i = 0; i < vector_size(&v); ++i)
        printf("%d ", vector_get(&v, i));
    printf("\n");

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

    vector_free(&v);
}
