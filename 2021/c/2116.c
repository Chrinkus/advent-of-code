#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aoc_banner.h"

enum constants {
	BUFFLEN = 2,	// all examples are in multiples of '2' hex-digits
	HEX_BITS = 4,
	// header
	VERSION_LEN = 3,
	TYPE_ID_LEN = 3,
	// literal
	PREFIX_LEN = 1,
	LITERAL_LEN = 4,
	// operator
	MODE_LEN = 1,
	LENGTH_TYPE_0 = 15,
	LENGTH_TYPE_1 = 11,
};

struct packet;

struct sub_packet {
	struct packet* sub;
	struct sub_packet* next;
};

struct packet {
	uint8_t version;
	uint8_t type_id;
	union {
		uint64_t literal;
		struct sub_packet* subs;
	} payload;
};

uint64_t extract_len(uint64_t* bitbuff, int* numbits, int len, int64_t* count)
{
	uint64_t ret = 0;
	int offset = *numbits - len;
	uint64_t tmpmask = (1U << offset) - 1;

	ret |= *bitbuff >> offset;
	*bitbuff &= tmpmask;
	*numbits -= len;
	if (count && *count)
		*count -= len;

	return ret;
}

int get_bits(uint64_t* bitbuff, int* numbits)
{
	if (*numbits < 0) exit(1);
	char buffer[BUFFLEN+1];
	int i = 0;
	for (int c; i < BUFFLEN && (c = getchar()) != EOF; )
		if (isxdigit(c))
			buffer[i++] = c;
	buffer[i] = '\0';

	int new_bits = strlen(buffer) * HEX_BITS;
	if (new_bits) {
		*numbits += new_bits;
		*bitbuff <<= new_bits;
		*bitbuff |= strtoul(buffer, NULL, 16);
	}

	return new_bits;
}

void extract_header(struct packet* p, uint64_t* bitbuff, int* numbits,
		int64_t* count)
{
	if (*numbits < VERSION_LEN + TYPE_ID_LEN)
		get_bits(bitbuff, numbits);

	p->version = extract_len(bitbuff, numbits, VERSION_LEN, count);
	p->type_id = extract_len(bitbuff, numbits, TYPE_ID_LEN, count);
}

uint64_t extract_literal(uint64_t* bitbuff, int* numbits, int64_t* count)
{
	uint64_t lit = 0;
	uint64_t prefix = 0;
	do {
		if (*numbits < PREFIX_LEN + LITERAL_LEN)
			get_bits(bitbuff, numbits);

		prefix = extract_len(bitbuff, numbits, PREFIX_LEN, count);

		lit <<= LITERAL_LEN;
		lit |= extract_len(bitbuff, numbits, LITERAL_LEN, count);

	} while (prefix);

	return lit;
}

struct packet* extract_packet(uint64_t* bitbuff, int* numbits, int* sum,
		int64_t* count);
struct sub_packet* extract_sub_packets(uint64_t* bitbuff, int* numbits,
		int* sum, int64_t* count)
{
	// Get length mode
	if (*numbits < MODE_LEN)
		get_bits(bitbuff, numbits);

	uint8_t mode = extract_len(bitbuff, numbits, MODE_LEN, count);
	int len_len = mode == 0 ? LENGTH_TYPE_0 : LENGTH_TYPE_1;

	// Get sub-packets length
	while (*numbits < len_len)
		get_bits(bitbuff, numbits);
	int64_t len = extract_len(bitbuff, numbits, len_len, count);

	struct sub_packet* list = NULL;//malloc(sizeof(struct sub_packet));

	if (mode == 0) {
		while (!mode && *numbits < 8)	// load up on bits
			get_bits(bitbuff, numbits);
		if (count && *count)
			*count -= len;
		for (struct sub_packet* node, * parent = NULL;
				len > 0; ) {
			node = malloc(sizeof(struct sub_packet));
			if (!list)	list = node;
			node->sub = extract_packet(bitbuff, numbits, sum, &len);
			node->next = NULL;
			if (parent)
				parent->next = node;
			parent = node;
			node = node->next;
		}
	} else {
		for (struct sub_packet* node, * parent = NULL;
				len--; ) {
			node = malloc(sizeof(struct sub_packet));
			if (!list)	list = node;
			node->sub = extract_packet(bitbuff, numbits, sum, count);
			node->next = NULL;
			if (parent)
				parent->next = node;
			parent = node;
			node = node->next;
		}
	}

	return list;
}

struct packet* extract_packet(uint64_t* bitbuff, int* numbits, int* sum,
		int64_t* count)
{
	struct packet* p = malloc(sizeof(struct packet));
	extract_header(p, bitbuff, numbits, count);
	*sum += p->version;


	switch (p->type_id) {
	case 4:		// type is literal
		p->payload.literal = extract_literal(bitbuff, numbits, count);
		break;
	default:	// type is operator (recurse!)
		p->payload.subs = extract_sub_packets(bitbuff, numbits, sum,
				count);
		break;
	}
	return p;
}

int64_t minimum(int64_t a, int64_t b)
{
	return a < b ? a : b;
}

int64_t maximum(int64_t a, int64_t b)
{
	return a > b ? a : b;
}

int64_t evaluate_packet(const struct packet* packet)
{
	int64_t res = 0;

	switch (packet->type_id) {
	case 0:	// sum
		for (const struct sub_packet* p = packet->payload.subs; p;
				p = p->next) {
			res += evaluate_packet(p->sub);
		}

		break;
	case 1:	// product
		res = 1;
		for (const struct sub_packet* p = packet->payload.subs; p;
				p = p->next) {
			res *= evaluate_packet(p->sub);
		}
		break;
	case 2:	// minimum
		{
			const struct sub_packet* p = packet->payload.subs;
			res = evaluate_packet(p->sub);
			while ((p = p->next))
				res = minimum(res, evaluate_packet(p->sub));
		}
		break;
	case 3:	// max
		{
			const struct sub_packet* p = packet->payload.subs;
			res = evaluate_packet(p->sub);
			while ((p = p->next))
				res = maximum(res, evaluate_packet(p->sub));
		}
		break;
	case 4:	// literal
		res = packet->payload.literal;
		break;
	case 5:	// greater than
		{
			const struct sub_packet* p = packet->payload.subs;
			int64_t a = evaluate_packet(p->sub);
			p = p->next;
			int64_t b = evaluate_packet(p->sub);
			res = a > b;
		}
		break;
	case 6:	// less than
		{
			const struct sub_packet* p = packet->payload.subs;
			int64_t a = evaluate_packet(p->sub);
			p = p->next;
			int64_t b = evaluate_packet(p->sub);
			res = a < b;
		}
		break;
	case 7:	// equal to
		{
			const struct sub_packet* p = packet->payload.subs;
			int64_t a = evaluate_packet(p->sub);
			p = p->next;
			int64_t b = evaluate_packet(p->sub);
			res = a == b;
		}
		break;
	}

	return res;
}

int main()
{
	aoc_banner_2021("16", "Packet Decoder");

	int part1 = 0;
	int numbits = 0;
	uint64_t bitbuff = 0;
	struct packet* p = extract_packet(&bitbuff, &numbits, &part1, NULL);

	int64_t part2 = evaluate_packet(p);

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %ld\n", part2);

	free(p);	// can do better!

	return EXIT_SUCCESS;
}

