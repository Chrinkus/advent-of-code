#include <stdlib.h>
#include <stdio.h>

enum magic { PKT_LEN = 4, MSG_LEN = 14 };

static int
all_diff(const int* buff, const int len)
{
        for (int i = 0; i < len - 1; ++i)
                for (int j = i + 1; j < len; ++j)
                        if (buff[i] == buff[j])
                                return 0;
        return 1;
}

static void
count_till_start_of_packet(int* p1, int* p2)
{
        int b1[PKT_LEN] = { 0 };
        int b2[MSG_LEN] = { 0 };

        int count = 0;
        for (int i=0, c; (c = getchar()) != EOF && c != '\n'; ++i) {
                ++count;
                if (*p1 == 0) {
                        b1[i % PKT_LEN] = c;
                        if (i >= PKT_LEN && all_diff(b1, PKT_LEN))
                                *p1 = count;
                }
                if (*p2 == 0) {
                        b2[i % MSG_LEN] = c;
                        if (i >= MSG_LEN && all_diff(b2, MSG_LEN))
                                *p2 = count;
                }
                if (*p1 > 0 && *p2 > 0)
                        return;
        }
}

int main(void)
{
        int part1 = 0;
        int part2 = 0;
        count_till_start_of_packet(&part1, &part2);

        printf("%d\n", part1);
        printf("%d\n", part2);
}
