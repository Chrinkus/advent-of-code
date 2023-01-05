#include <stdlib.h>
#include <stdio.h>

enum magic { UP = '(', DOWN = ')', };

int main(void)
{
        int floor = 0;
        int basement = 0;
        for (int c, count = 0; (c = getchar()) != EOF && c != '\n'; ) {
                switch (c) {
                case UP:        floor += 1;     break;
                case DOWN:      floor -= 1;     break;
                }
                if (!basement && ++count && floor < 0)
                        basement = count;
        }
        printf("%d\n", floor);
        printf("%d\n", basement);

        return EXIT_SUCCESS;
}
