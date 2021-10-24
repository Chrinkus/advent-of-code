#ifndef AOC_VECTOR_H
#define AOC_VECTOR_H

/**
 * Generic Vector
 *
 * All of these macros expect a vector struct of the following definition:
 *
 * struct TYPE_vector {
 * 	size_t size;
 * 	size_t cap;
 * 	TYPE* vec;
 * };
 */

enum AOC_Vector_scalars {
	AOC_VECTOR_INITIAL_CAP = 8,
	AOC_VECTOR_GROWTH_RATE = 2
};

#define aoc_vector_init(v)						\
	do {								\
		(v).size = 0;						\
		(v).cap = AOC_VECTOR_INITIAL_CAP;			\
		(v).vec = malloc(sizeof(*(v).vec) * (v).cap);		\
	} while (0)

#define aoc_vector_free(v)						\
	do {								\
		(v).size = 0;						\
		(v).cap = 0;						\
		free((v).vec);						\
	} while (0)

#define aoc_vector_grow(v)						\
	do {								\
		size_t new_cap = (v).cap * AOC_VECTOR_GROWTH_RATE;	\
		(v).vec = realloc((v).vec, sizeof(*(v).vec) * new_cap);	\
		(v).cap = new_cap;					\
	} while (0)

#define aoc_vector_size(v) (v).size

#define aoc_vector_place(v, val)					\
	do {								\
		(v).vec[(v).size++] = (val);				\
	} while (0)			

#define aoc_vector_push(v, val)						\
	do {								\
		if ((v).size == (v).cap)				\
			aoc_vector_grow((v));				\
		aoc_vector_place((v), (val));				\
	} while (0)

#define aoc_vector_copy(vdst, vsrc)					\
	do {								\
		(vdst).vec = malloc(sizeof(*(vsrc).vec) * (vsrc).size);	\
		(vdst).cap = (vsrc).size;				\
		(vdst).size = 0;					\
		for (size_t i = 0; i < (vsrc).size; ++i)		\
			aoc_vector_place((vdst), (vsrc).vec[i]);	\
	} while (0)

#define aoc_vector_find(v, tar, cmp, found)				\
	do {								\
		for (size_t i = 0; i < (v).size; ++i) {			\
			if (cmp( &((v).vec[i]), tar)) {			\
				found = &((v).vec[i]);			\
				break;					\
			}						\
		}							\
	} while (0)

#define aoc_vector_foreach(v, func)					\
	do {								\
		for (size_t i = 0; i < (v).size; ++i) 			\
			func(&(v).vec[i]);				\
	} while (0)

#endif//AOC_VECTOR_H
