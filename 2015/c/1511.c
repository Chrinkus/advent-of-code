#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum ice { PASSWD_LEN = 8, };

static void
iterate_passwd(char* pw, int len)
{
        for (int i = len - 1; i >= 0; --i) {
                if (pw[i] == 'z') {
                        pw[i] = 'a';
                        continue;
                } else {
                        ++pw[i];
                        break;
                }
        }
}

static int
is_valid_passwd(char* pw, int len)
{
        // Second Requirement
        if (strpbrk(pw, "iol"))
                return 0;

        // First Requirement
        int straight = 0;
        for (int i = 0; i < len; ++i) {
                char ch = pw[i];
                if (i < len - 2 && pw[i+1] == ch + 1 && pw[i+2] == ch + 2) {
                        straight = 1;
                        break;
                }
        }
        if (!straight)
                return 0;

        // Third Requirement
        int pairs = 0;
        while (*pw) {
                if (*pw == *(pw+1)) {
                        pairs += 1;
                        pw += 2;
                } else {
                        pw += 1;
                }
        }
        return pairs >= 2;
}

static void
get_next_passwd(char* pw, int len)
{
        do {
                iterate_passwd(pw, len);
        } while (!is_valid_passwd(pw, len));
}

int main(void)
{
        char passwd[PASSWD_LEN + 1];
        scanf("%s ", passwd);

        get_next_passwd(passwd, PASSWD_LEN);
        printf("%s\n", passwd);

        get_next_passwd(passwd, PASSWD_LEN);
        printf("%s\n", passwd);

        return EXIT_SUCCESS;
}
