#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

enum scores { ROCK = 1, PAPER = 2, SCISSORS = 3, WIN = 6, LOSE = 0, DRAW = 3 };
enum choice {
        OPP_ROCK = 'A',
        OPP_PAPER = 'B',
        OPP_SCISSORS = 'C',
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
score_round_1(char them, char me)
{
        int score = 0;
        switch (me) {
        case MY_ROCK:           score += ROCK;          break;
        case MY_PAPER:          score += PAPER;         break;
        case MY_SCISSORS:       score += SCISSORS;      break;
        default:                return err_char(me);
        }

        switch (them) {
        case OPP_ROCK:
                if      (me == MY_PAPER)        score += WIN;
                else if (me == MY_ROCK)         score += DRAW;
                break;
        case OPP_PAPER:
                if      (me == MY_SCISSORS)     score += WIN;
                else if (me == MY_PAPER)        score += DRAW;
                break;
        case OPP_SCISSORS:
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
        case OPP_ROCK:          return PAPER;
        case OPP_PAPER:         return SCISSORS;
        case OPP_SCISSORS:      return ROCK;
        default:                return err_char(opp);
        }
}

int get_loser(const char opp)
{
        switch (opp) {
        case OPP_ROCK:          return SCISSORS;
        case OPP_PAPER:         return ROCK;
        case OPP_SCISSORS:      return PAPER;
        default:                return err_char(opp);
        }
}

int get_draw(const char opp)
{
        switch (opp) {
        case OPP_ROCK:          return ROCK;
        case OPP_PAPER:         return PAPER;
        case OPP_SCISSORS:      return SCISSORS;
        default:                return err_char(opp);
        }
}

int
score_round_2(char them, char res)
{
        int score = 0;
        switch (res) {
        case RES_WIN:   score += WIN  + get_winner(them);       break;
        case RES_LOSE:  score += LOSE + get_loser(them);        break;
        case RES_DRAW:  score += DRAW + get_draw(them);         break;
        default:        return err_char(res);
        }
        return score;
}

void
play_rps(int* p1, int* p2)
{
        for (char them, me; scanf("%c %c\n", &them, &me) == 2; ) {
                *p1 += score_round_1(them, me);
                *p2 += score_round_2(them, me);
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
