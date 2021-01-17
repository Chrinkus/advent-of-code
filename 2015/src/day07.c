#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <inttypes.h>

enum op { AND, OR, NOT, LSHIFT, RSHIFT, ASSIGN, NUM_OP };

char const*const operations[NUM_OP] = {
	[AND] 	 = "AND",
	[OR] 	 = "OR",
	[NOT] 	 = "NOT",
	[LSHIFT] = "LSHIFT",
	[RSHIFT] = "RSHIFT",
	[ASSIGN] = "ASSIGN"
};

enum op get_op(const char* s)
{
	for (int i = 0; i < NUM_OP; ++i)
		if (strcmp(s, operations[i]) == 0)
			return i;
	return NUM_OP;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

enum data_sizes { id_max = 8, op_max = 8, var_max = 8, num_reg = 3 };

enum { hashsize = 199 };

static struct nlist* hashtab[hashsize];

unsigned hash(char* s)
{
	unsigned hashval = 1;
	while (*s != '\0')
		hashval = *s++ + 31 * hashval;

	return hashval % hashsize;
}

struct nlist {
	struct nlist* next;
	char id[id_max];
	uint16_t signal;
};

struct nlist* hash_lookup(char* s)
{
	for (struct nlist* np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->id) == 0)
			return np;
	return NULL;
}

struct nlist* hash_access(char* id)
{
	struct nlist* np;

	if ((np = hash_lookup(id)) == NULL) {
		np = malloc(sizeof(*np));
		if (np == NULL)
			return NULL;
		strcpy(np->id, id);
		unsigned hashval = hash(id);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	return np;
}

void print_hash_table(struct nlist* ht[], int n)
{
	for (int i = 0; i < n; ++i) {
		printf("[%d] ", i);
		struct nlist* np = ht[i];
		while (np) {
			printf("%s = %" PRIu16 ", ", np->id, np->signal);
			np = np->next;
		}
		printf("NULL\n");
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

enum type { UINT, STRING };

struct reg {		// a reg is either a value or a variable name
	enum type t;
	union {
		uint16_t uval;
		char sval[id_max];
	} u;
};

struct instruction {	// instructions have 3 regs and an operator
	struct reg r[num_reg];
	char op[op_max];
};

void instruction_init(struct instruction* ins)
{
	ins->r[0].t = UINT;
	ins->r[0].u.uval = 0;
	ins->r[1].t = UINT;
	ins->r[1].u.uval = 0;
	ins->r[2].t = STRING;
	ins->r[2].u.sval[0] = '\0';
}

void print_instruction(struct instruction* ins)
{
	for (int i = 0; i < num_reg; ++i) {
		if (ins->r[i].t == UINT)
			printf("r%d: %" PRIu16 ", ", i, ins->r[i].u.uval);
		else
			printf("r%d: %s, ", i, ins->r[i].u.sval);
	}
	printf("%s\n", ins->op);
}

void parse_instruction(const char* s, struct instruction* ins)
{
	char var[num_reg][var_max];

	if (isalpha(s[0]) && isupper(s[0])) {	// a NOT unary statement
		ins->r[1].t = STRING;
		ins->r[2].t = STRING;
		sscanf(s, "%s %s -> %s\n",
				ins->op, ins->r[1].u.sval, ins->r[2].u.sval);
	} else if (sscanf(s, "%s %s %s -> %s\n",
				var[0], ins->op, var[1], var[2]) == 4) {

		for (int i = 0; i < num_reg; ++i) {
			if (isdigit(var[i][0])) {
				ins->r[i].t = UINT;
				ins->r[i].u.uval = (uint16_t)atoi(var[i]);
			} else {
				ins->r[i].t = STRING;
				strcpy(ins->r[i].u.sval, var[i]);
			}
		}
	} else {
		sscanf(s, "%s -> %s\n", var[1], var[2]);
		if (isdigit(var[1][0])) {
			ins->r[1].t = UINT;
			ins->r[1].u.uval = (uint16_t)atoi(var[1]);
		} else {
			ins->r[1].t = STRING;
			strcpy(ins->r[1].u.sval, var[1]);
		}
		strcpy(ins->r[2].u.sval, var[2]);
		strcpy(ins->op, operations[ASSIGN]);
	}
}

int execute_instruction(struct instruction* ins)
{
	struct reg* r0 = &ins->r[0];
	struct reg* r1 = &ins->r[1];

	if (r0->t == STRING && !hash_lookup(r0->u.sval))
		return 0;
	if (r1->t == STRING && !hash_lookup(r1->u.sval))
		return 0;

	uint16_t val1 = r0->t == UINT ? r0->u.uval
		: hash_access(r0->u.sval)->signal;

	uint16_t val2 = r1->t == UINT ? r1->u.uval
		: hash_access(r1->u.sval)->signal;

	struct nlist* np = hash_access(ins->r[2].u.sval);

	switch (get_op(ins->op)) {
	case ASSIGN:	np->signal = val2;		break;
	case NOT:	np->signal = ~val2;		break;
	case AND:	np->signal = val1 & val2;	break;
	case OR:	np->signal = val1 | val2;	break;
	case LSHIFT:	np->signal = val1 << val2;	break;
	case RSHIFT:	np->signal = val1 >> val2;	break;
	default:
		fprintf(stderr, "Invalid Op: %s\n", ins->op);
		break;
	}
	return 1;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Queue

enum queue_sizes { queue_base = 8, queue_growth = 2 };

struct queue {
	struct instruction** data;
	size_t head;
	size_t tail;
	size_t size;
	size_t capacity;
};

struct queue* queue_init(struct queue* q)
{
	q->data = malloc(sizeof(struct instruction*) * queue_base);
	if (!q->data)
		return NULL;

	q->head = 0;
	q->tail = 0;
	q->size = 0;
	q->capacity = queue_base;
	return q;
}

void queue_free(struct queue* q)
{
	free(q->data);
	q->data = NULL;
}

struct queue* queue_resize(struct queue* q)
{
	size_t new_cap = q->capacity * 2;
	struct instruction** new_data = malloc(sizeof(struct instruction*) * new_cap);
	if (!new_data)
		return NULL;

	for (size_t i = 0, j = q->head;
			i == 0 || (j != q->tail);
			++i, j = (j+1) % q->size)
		new_data[i] = q->data[j];

	free(q->data);
	q->data = new_data;
	q->head = 0;
	q->tail = q->size;
	q->capacity = new_cap;
	return q;
}

struct instruction* queue_push(struct queue* q, struct instruction* np)
{
	if (q->size == q->capacity && !queue_resize(q))
		return NULL;

	q->data[q->tail++] = np;
	q->tail %= q->capacity;
	++q->size;

	return np;		// more for testing success
}

struct instruction* queue_pop(struct queue* q)
{
	if (q->size == 0)
		return NULL;

	struct instruction* popped = q->data[q->head];
	q->data[q->head++] = NULL;
	q->head %= q->capacity;
	--q->size;

	return popped;
}

void queue_debug(struct queue* q)
{
	printf("QUEUE DEBUG: size = %zu, cap = %zu, head = %zu, tail = %zu\n",
			q->size, q->capacity, q->head, q->tail);
}

void print_queue(struct queue* q)
{
	for (size_t i = q->head; i != q->tail; i = (i + 1) % q->capacity) {
		printf("[%zu] ", i);
		print_instruction(q->data[i]);
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main()
{
	struct queue qins;
	if (!queue_init(&qins))
		return EXIT_FAILURE;

	size_t len = 32;
	char* line = malloc(len);
	for (ssize_t nread; (nread = getline(&line, &len, stdin)) != -1; ) {
		struct instruction* ins = malloc(sizeof(struct instruction));
		if (!ins)
			return EXIT_FAILURE;
		instruction_init(ins);

		parse_instruction(line, ins);
		if (execute_instruction(ins))
			free(ins);
		else
			queue_push(&qins, ins);
	}
	free(line);

	for (struct instruction* ins; (ins = queue_pop(&qins)) != NULL; ) {
		if (execute_instruction(ins))
			free(ins);
		else
			queue_push(&qins, ins);
	}
	queue_free(&qins);

	//print_hash_table(hashtab, hashsize);

	uint16_t part1 = hash_access("a")->signal;
	printf("Part 1: %" PRIu16 "\n", part1);

	return EXIT_SUCCESS;
}
