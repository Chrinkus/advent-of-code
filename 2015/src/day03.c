#include <stdlib.h>
#include <stdio.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct point {
	int x;
	int y;
};

void point_move(struct point* p, int dx, int dy)
{
	p->x += dx;
	p->y += dy;
}

int point_cmp(const struct point* a, const struct point* b)
{
	int cond = a->x - b->x;
	return cond == 0 ? a->y - b->y : cond;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct node {
	struct point point;
	int count;
	struct node* left;
	struct node* right;
};

struct binary_tree {
	struct node* root;
	size_t size;
};

struct node* bt_add(struct binary_tree* bt, struct node* n, struct point* p)
{
	int cond;

	if (n == NULL) {
		n = (struct node*)malloc(sizeof(struct node));
		n->point = *p;
		n->count = 1;
		n->left = n->right = NULL;
		bt->size++;
	} else if ((cond = point_cmp(p, &n->point)) == 0) {
		n->count++;
	} else if (cond < 0) {
		n->left = bt_add(bt, n->left, p);
	} else {
		n->right = bt_add(bt, n->right, p);
	}
	return n;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

void santa_visit(struct binary_tree* bt, struct point* p)
{
	bt->root = bt_add(bt, bt->root, p);
}

void move_santa(struct point* santa, int dir)
{
	switch (dir) {
	case '^':	point_move(santa, 0, 1);	break;
	case 'v':	point_move(santa, 0, -1);	break;
	case '<':	point_move(santa, -1, 0);	break;
	case '>':	point_move(santa, 1, 0);	break;
	}
}

int main()
{
	const struct point origin = { .x = 0, .y = 0 };

	// Part 1
	struct point santa = origin;
	struct binary_tree houses_y1 = { .root = NULL, .size = 0 };
	santa_visit(&houses_y1, &santa);

	// Part 2
	struct point santa2 = origin;
	struct point robo_santa = origin;
	struct binary_tree houses_y2 = { .size = 0, .root = NULL };
	santa_visit(&houses_y2, &santa2);
	santa_visit(&houses_y2, &robo_santa);

	for (int c, i = 0; (c = getchar()) != EOF && c != '\n'; ++i) {
		move_santa(&santa, c);
		santa_visit(&houses_y1, &santa);

		if (i % 2 == 0) {
			move_santa(&santa2, c);
			santa_visit(&houses_y2, &santa2);
		} else {
			move_santa(&robo_santa, c);
			santa_visit(&houses_y2, &robo_santa);
		}
	}

	printf("Part 1: %zu\n", houses_y1.size);
	printf("Part 2: %zu\n", houses_y2.size);

	return EXIT_SUCCESS;
}
