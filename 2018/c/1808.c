/*
 * Experimenting with non-string NULL terminated arrays. There are plenty of
 * these in API's so I'm taking this chance to work with managing them.
 *
 * The children array will be terminated by NULL. The meta data array will
 * be terminated by -1 since all input values are positive.
 *
 * Aaaannd part 2 requires knowledge of the number of children so I have to
 * store that value anyways. Oh well, leaving part 1 implementation as is but
 * adding a children length to node struct.
 *
 * Interesting to note that the decision to NULL-terminate is not space-saving,
 * an extra NULL entry will take up just as much memory as storing the length.
 * The only apparent advantage is the ability to just handle an array rather
 * than a structure of an array and its length.
 *
 * Added a value member to save myself from re-calculating the same number
 * more than once (part 2).
 */
#include <stdlib.h>
#include <stdio.h>

#include <cgs/cgs.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Data Structures and Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum { META_END = -1, };

struct node {
        struct node** children; /* Terminated by NULL entry */
        int* meta;              /* Terminated by META_END */
        int num_children;       /* Sadly needed for part 2 */
        int value;              /* Memoized value calculation */
};

struct node* node_new(void)
{
        struct node* p = malloc(sizeof(struct node));
        if (!p)
                return cgs_error_retnull("malloc: %s", cgs_error_sys());
        p->children = NULL;
        p->meta = NULL;
        p->value = META_END;

        return p;
}

void node_free(struct node* n)
{
        if (!n)
                return;

        for (int i = 0; n->children[i]; ++i)
                node_free(n->children[i]);

        free(n->children);
        free(n->meta);
        free(n);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Input Processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void* read_node(struct node* n)
{
        int c, m;
        if (scanf(" %d %d", &c, &m) != 2)
                return cgs_error_retnull("scanf: %s", cgs_error_sys());

        struct node** pn = malloc(sizeof(struct node*) * (c+1));
        if (!pn)
                return cgs_error_retnull("malloc: %s", cgs_error_sys());
        n->children = pn;
        n->num_children = c;

        for (int i = 0; i < c; ++i) {
                pn[i] = node_new();
                if (!pn[i])
                        return cgs_error_retnull("node_new");
                if (!read_node(pn[i]))
                        return cgs_error_retnull("read_node");
        }
        pn[c] = NULL;           /* Terminate */

        int* pi = malloc(sizeof(int) * (m+1));
        if (!pi)
                return cgs_error_retnull("malloc: %s", cgs_error_sys());
        n->meta = pi;

        for (int i = 0; i < m; ++i) {
                if (scanf(" %d", &pi[i]) != 1)
                        return cgs_error_retnull("scanf: %s", cgs_error_sys());
        }
        pi[m] = META_END;       /* Terminate */

        return n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 1 - Add up all meta entries
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_meta_sum(const struct node* node)
{
        int sum = 0;
        for (int i = 0; node->children[i]; ++i)
                sum += get_meta_sum(node->children[i]);

        for (int i = 0; node->meta[i] != META_END; ++i)
                sum += node->meta[i];

        return sum;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Part 2 - Get the "value" of the root node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int get_node_value(struct node* node)
{
        if (node->value != META_END)
                return node->value;

        int value = 0;
        if (node->num_children > 0) {
                for (int i = 0; node->meta[i] != META_END; ++i) {
                        int x = node->meta[i];
                        if (x == 0 || x > node->num_children)
                                continue;
                        value += get_node_value(node->children[x-1]);
                }
        } else {
                for (int i = 0; node->meta[i] != META_END; ++i)
                        value += node->meta[i];
        }

        node->value = value;    // store value to save re-calculating
        return value;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        printf("Advent of Code 2018 Day 8: Memory Maneuver\n");

        struct node* root = node_new();
        if (!root)
                return cgs_error_retfail("node_new");
        if (!read_node(root))
                return cgs_error_retfail("read_node");

        int part1 = get_meta_sum(root);
        int part2 = get_node_value(root);

        printf("Part 1: %d\n", part1);
        printf("Part 2: %d\n", part2);

        node_free(root);

        return EXIT_SUCCESS;
}

