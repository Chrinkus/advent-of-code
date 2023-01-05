#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <openssl/md5.h>

bool test_md5(const char* result, size_t zeros)
{
	for (size_t i = 0; i < zeros; ++i)
		if (result[i] != '0')
			return false;
	return true;
}

int main()
{
	const char* input = "bgvyzdsv";

	size_t part1 = 0;
	size_t part2 = 0;
	for (size_t i = 0; ; ++i) {
		char message[32];
		sprintf(message, "%s%zu", input, i);

		unsigned char digest[MD5_DIGEST_LENGTH];
		MD5((unsigned char*)message, strlen(message), digest);

		char result[64];
		for (size_t j = 0; j < MD5_DIGEST_LENGTH; ++j)
			sprintf(&result[j*2], "%02x", digest[j]);

		//printf("[%zu] %s\n", i, result);

		if (!part1 && test_md5(result, 5))
			part1 = i;
		if (!part2 && test_md5(result, 6))
			part2 = i;
		if (part1 && part2)
			break;
	}

	printf("Part 1: %zu\n", part1);
	printf("Part 2: %zu\n", part2);

	return EXIT_SUCCESS;
}
