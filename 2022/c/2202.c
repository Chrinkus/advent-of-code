#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

enum scores { ROCK = 1, PAPER = 2, SCISSORS = 3, WIN = 6, LOSE = 0, DRAW = 3 };
enum choice {
        OP_ROCK = 'A',
        OP_PAPER = 'B',
        OP_SCISSORS = 'C',
        MY_ROCK = 'X',
        MY_PAPER = 'Y',
        MY_SCISSORS = 'Z',
        RES_LOSE = 'X',
        RES_DRAW = 'Y',
        RES_WIN = 'Z',
};

int
err_char(const char ch)
{
        printf("Uknown char: '%c'\n", ch);
        return -1;
}

int
score_system_1(char them, char me)
{
        int score = 0;
        switch (me) {
        case MY_ROCK:           score += ROCK;          break;
        case MY_PAPER:          score += PAPER;         break;
        case MY_SCISSORS:       score += SCISSORS;      break;
        default:                return err_char(me);
        }

        switch (them) {
        case OP_ROCK:
                if      (me == MY_PAPER)        score += WIN;
                else if (me == MY_ROCK)         score += DRAW;
                break;
        case OP_PAPER:
                if      (me == MY_SCISSORS)     score += WIN;
                else if (me == MY_PAPER)        score += DRAW;
                break;
        case OP_SCISSORS:
                if      (me == MY_ROCK)         score += WIN;
                else if (me == MY_SCISSORS)     score += DRAW;
                break;
        default:
                return err_char(them);
        }
        return score;
}

int
get_winner(const char opp)
{
        switch (opp) {
        case OP_ROCK:           return PAPER;
        case OP_PAPER:          return SCISSORS;
        case OP_SCISSORS:       return ROCK;
        default:                return err_char(opp);
        }
}

int get_loser(const char opp)
{
        switch (opp) {
        case OP_ROCK:           return SCISSORS;
        case OP_PAPER:          return ROCK;
        case OP_SCISSORS:       return PAPER;
        default:                return err_char(opp);
        }
}

int get_drawer(const char opp)
{
        switch (opp) {
        case OP_ROCK:           return ROCK;
        case OP_PAPER:          return PAPER;
        case OP_SCISSORS:       return SCISSORS;
        default:                return err_char(opp);
        }
}

int
score_system_2(char them, char res)
{
        int score = 0;
        switch (res) {
        case RES_WIN:   score += WIN  + get_winner(them);       break;
        case RES_LOSE:  score += LOSE + get_loser(them);        break;
        case RES_DRAW:  score += DRAW + get_drawer(them);       break;
        default:        return err_char(res);
        }
        return score;
}

void
play_rps(int* p1, int* p2)
{
        for (char them, mys; scanf("%c %c\n", &them, &mys) == 2; ) {
                *p1 += score_system_1(them, mys);
                *p2 += score_system_2(them, mys);
        }
}

int main(void)
{
        int part1 = 0;
        int part2 = 0;

        play_rps(&part1, &part2);

        printf("%d\n", part1);
        printf("%d\n", part2);

        return EXIT_SUCCESS;
}
