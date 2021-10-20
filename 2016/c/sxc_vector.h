#ifndef SXC_VECTOR_H
#define SXC_VECTOR_H

/**
 * Genderic Vector
 *
 * All of these macros expect a vector struct of the following definition:
 *
 * struct TYPE_vector {
 * 	size_t size;
 * 	size_t cap;
 * 	size_t elem_size;
 * 	TYPE* vec;
 * };
 */

enum SXC_Vector_scalars {
	SXC_VECTOR_INITIAL_CAP = 8,
	SXC_VECTOR_GROWTH_RATE = 2
};

#define sxc_vector_init(v, type)					\
	do {								\
		(v).size = 0;						\
		(v).cap = SXC_VECTOR_INITIAL_CAP;			\
		(v).elem_size = sizeof(type);				\
		(v).vec = malloc((v).elem_size * (v).cap);		\
	} while (0)

#define sxc_vector_grow(v)						\
	do {								\
		size_t new_cap = (v).cap * SXC_VECTOR_GROWTH_RATE;	\
		(v).vec = realloc((v).vec, (v).elem_size * new_cap);	\
		(v).cap = new_cap;					\
	} while (0)

#define sxc_vector_place(v, val)					\
	do {								\
		(v).vec[(v).size++] = (val);				\
	} while (0)			

#define sxc_vector_push(v, val)						\
	do {								\
		if ((v).size == (v).cap)				\
			sxc_vector_grow((v));				\
		sxc_vector_place((v), (val));				\
	} while (0)

#define sxc_vector_find(v, tar, cmp, found)				\
	do {								\
		for (size_t i = 0; i < (v).size; ++i) {			\
			if (cmp( &((v).vec[i]), tar)) {			\
				found = &((v).vec[i]);			\
				break;					\
			}						\
		}							\
	} while (0)

#endif//SXC_VECTOR_H
