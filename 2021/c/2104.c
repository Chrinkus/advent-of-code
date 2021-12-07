#include <stdlib.h>
#include <stdio.h>

#include <sxc_vector.h>
#include <sxc_vector_types.h>

#include "aoc_banner.h"

enum { ROWLEN = 5, NUMROWS = 5, CARDSIZE = ROWLEN * NUMROWS, MARKED = -1 };

struct Card_vector {
	size_t siz;
	size_t cap;
	int** vec;
};

const int* check_rows(const int* card)
{
	for (int i = 0; i < NUMROWS; ++i) {
		int marked = 0;
		for (int j = 0; j < ROWLEN; ++j) {
			if (card[i * ROWLEN + j] == MARKED)
				++marked;
			else
				break;
		}
		if (marked == ROWLEN)
			return card;
	}
	return NULL;
}

const int* check_cols(const int* card)
{
	for (int i = 0; i < ROWLEN; ++i) {
		int marked = 0;
		for (int j = 0; j < NUMROWS; ++j) {
			if (card[j * ROWLEN + i] == MARKED)
				++marked;
			else
				break;
		}
		if (marked == NUMROWS)
			return card;
	}
	return NULL;
}

int* check_cards(struct Card_vector* v)
{
	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		int* card = sxc_vector_get(v, i);
		if (!card) continue;	// winning cards are removed

		if (check_rows(card) || check_cols(card)) {
			sxc_vector_get(v, i) = NULL;
			return card;
		}
	}
	return NULL;
}

void mark_cards(struct Card_vector* v, int ball)
{
	for (size_t i = 0; i < sxc_vector_size(v); ++i) {
		int* card = sxc_vector_get(v, i);
		if (!card) continue;	// winning cards are removed

		for (int j = 0; j < CARDSIZE; ++j)
			if (card[j] == ball)
				card[j] = MARKED;
	}
}

int tally_score(const int* card, const int ball)
{
	int score = 0;
	for (int i = 0; i < CARDSIZE; ++i)
		if (card[i] != MARKED)
			score += card[i];
	return score * ball;
}

int main()
{
	aoc_banner_2021("04", "Giant Squid");

	// Read balls
	struct Int_vector balls;
	sxc_vector_init(&balls);
	for (int n, c = ','; c != '\n' && scanf("%d", &n) == 1; c = getchar())
		sxc_vector_push(&balls, n);

	// Read cards
	struct Card_vector cards;
	sxc_vector_init(&cards);
	while (!feof(stdin)) {
		int* card = malloc(sizeof(int) * CARDSIZE);
		for (int i = 0, n; i < CARDSIZE && scanf("%d ", &n) == 1; ++i)
			card[i] = n;
		sxc_vector_push(&cards, card);
	}

	// PLAY!!
	int part1 = 0;
	int part2 = 0;
	for (size_t i = 0, cards_in_play = sxc_vector_size(&cards);
			i < sxc_vector_size(&balls); ++i) {
		int ball = sxc_vector_get(&balls, i);
		mark_cards(&cards, ball);

		for (int* bingo = NULL; (bingo = check_cards(&cards));
				--cards_in_play) {
			if (!part1)
				part1 = tally_score(bingo, ball);
			if (!part2 && cards_in_play == 1)
				part2 = tally_score(bingo, ball);

			free(bingo);
		}
	}

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	sxc_vector_free(&balls);
	for (size_t i = 0; i < sxc_vector_size(&cards); ++i) {
		free(sxc_vector_get(&cards, i));
	}
	sxc_vector_free(&cards);

	return EXIT_SUCCESS;
}

