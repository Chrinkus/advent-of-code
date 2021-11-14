#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <sxc_string.h>

#include <openssl/md5.h>

enum { PASSWORD_LEN = 8, ZERO_COUNT = 5, BUFFMAX = 64 };

const char* test_md5(const char* hash, char ch, int count);
int hchtoi(const char ch);
double diffclock(const clock_t t1, const clock_t t2);
double sinceclock(const clock_t t);

int main()
{
	printf("Advent of Code 2016\n");
	printf("Day 5: How About a Nice Game of Chess?\n");

	String input;
	sxc_string_init(&input);
	for (int c; (c = getchar()) != EOF; )
		if (isalpha(c))
			sxc_string_push(&input, c);
	char part1[PASSWORD_LEN+1];
	char part2[PASSWORD_LEN+1];

	memset(part2, '_', PASSWORD_LEN);
	part2[PASSWORD_LEN] = '\0';

	printf("\nInput: %s\n\n", input.str);
	printf("Password     ID     Time (s)\n");
	printf("======== ========== ========\n");
	const char* fmt = "\r%s %10d %8.3f";

	clock_t start = clock();
	for (int i = 0, k = 0, l = PASSWORD_LEN; l; ++i) {
		char message[BUFFMAX];
		sprintf(message, "%s%d", input.str, i);

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)message, strlen(message), digest);

		char result[BUFFMAX];
		for (int j = 0; j < MD5_DIGEST_LENGTH; ++j)
			sprintf(&result[j*2], "%02x", digest[j]);

		printf(fmt, part2, i, sinceclock(start));
		fflush(stdout);
		if (test_md5(result, '0', ZERO_COUNT)) {
			if (k < PASSWORD_LEN)
				part1[k++] = result[ZERO_COUNT];

			int n = hchtoi(result[ZERO_COUNT]);
			if (n < PASSWORD_LEN && part2[n] == '_') {
				part2[n] = result[ZERO_COUNT+1];
				--l;

				printf(fmt, part2, i, sinceclock(start));
				printf("\n");
			}
		}
	}

	part1[PASSWORD_LEN] = '\0';

	printf("\n");
	printf("Part 1: %s\n", part1);
	printf("Part 2: %s\n", part2);

	sxc_string_free(&input);

	return EXIT_SUCCESS;
}

const char* test_md5(const char* hash, char ch, int count)
{
	for (int i = 0; i < count; ++i)
		if (hash[i] != ch)
			return NULL;
	return hash;
}

int hchtoi(const char ch)
{
	int n = 0;
	if ('0' <= ch && ch <= '9') {
		n = ch - '0';
	} else {
		switch (ch) {
		case 'A':
		case 'a':	n = 10;	break;
		case 'B':
		case 'b':	n = 11;	break;
		case 'C':
		case 'c':	n = 12;	break;
		case 'D':
		case 'd':	n = 13;	break;
		case 'E':
		case 'e':	n = 14;	break;
		case 'F':
		case 'f':	n = 15;	break;
		default:	n = 0;	break;
		}
	}

	return n;
}

double diffclock(const clock_t t1, const clock_t t2)
{
	return (double)(t1 - t2) / CLOCKS_PER_SEC;
}

double sinceclock(const clock_t t)
{
	return (double)(clock() - t) / CLOCKS_PER_SEC;
}

