#include <stdlib.h>
/*
 * segment_tree<T>
 * Segment tree of items of type T.
 *
 * segment_tree<T> new(size_t size, T (*op)(T, T), T id)
 * Allocates and returns a segment tree of size items. op should be an
 * associative operation with the left and right identity id. Elements of the
 * tree will be initialized with id.
 *
 * void destroy(segment_tree<T> t)
 * Destroy the tree t.
 *
 */

#define SML_SEGMENT_TREE_TYPE_DECL(T, TYPE, NS) typedef struct TYPE##_desc *TYPE
#define SML_SEGMENT_TREE_NEW_DECL(T, TYPE, NS) TYPE NS##new(size_t, T (*)(T, T), T)
#define SML_SEGMENT_TREE_DESTROY_DECL(T, TYPE, NS) void NS##destroy(TYPE)
#define SML_SEGMENT_TREE_FILL_DECL(T, TYPE, NS) void NS##fill(TYPE, size_t, size_t, T *)
#define SML_SEGMENT_TREE_FOLD_DECL(T, TYPE, NS) T NS##fold(TYPE, size_t, size_t)
#define SML_SEGMENT_TREE_SET_DECL(T, TYPE, NS) void NS##set(TYPE, size_t, T)

#define SML_SEGMENT_TREE_DECL(T, TYPE, NS) \
	SML_SEGMENT_TREE_TYPE_DECL(T, TYPE, NS); \
	SML_SEGMENT_TREE_NEW_DECL(T, TYPE, NS); \
	SML_SEGMENT_TREE_DESTROY_DECL(T, TYPE, NS); \
	SML_SEGMENT_TREE_FILL_DECL(T, TYPE, NS); \
	SML_SEGMENT_TREE_FOLD_DECL(T, TYPE, NS); \
	SML_SEGMENT_TREE_SET_DECL(T, TYPE, NS)

#define SML_SEGMENT_TREE_DESC_IMPL(T, TYPE, NS) \
	struct TYPE##_desc { \
		size_t size; \
		T (*op)(T, T); \
		T id; \
		T *array; \
	}

#define SML_SEGMENT_TREE_BUILD_IMPL(T, TYPE, NS) \
	void NS##build(TYPE desc, T *data, size_t i, size_t left, size_t right) \
	{ \
		if(left == right) \
			desc->array[i] = data[left]; \
		else \
		{ \
			size_t middle = (left + right) >> 1; \
			NS##build(desc, data, i * 2, left, middle); \
			NS##build(desc, data, i * 2 + 1, middle + 1, right); \
			desc->array[i] = desc->op(desc->array[i * 2], desc->array[i * 2 + 1]); \
		} \
	}

#define SML_SEGMENT_TREE_NEW_IMPL(T, TYPE, NS) \
	TYPE NS##new(size_t size, T (*op)(T, T), T id) \
	{ \
		struct TYPE##_desc *desc = malloc(sizeof(struct TYPE##_desc)); \
		desc->size = size; \
		desc->op = op; \
		desc->id = id; \
		desc->array = malloc(sizeof(T) * size * 4); \
		size_t i; \
		for(i = 0; i < size * 4; i++) \
			desc->array[i] = id; \
		return desc; \
	}

#define SML_SEGMENT_TREE_DESTROY_IMPL(T, TYPE, NS) \
	void NS##destroy(TYPE desc) \
	{ \
		free(desc->array); \
		free(desc); \
	}

#define SML_SEGMENT_TREE_FILL_AT_IMPL(T, TYPE, NS) \
	void NS##fill_at(TYPE desc, size_t i, size_t left, size_t right, size_t from, size_t to, T *data) \
	{ \
		if(left > to || right < from) \
			return; \
		if(left == right) \
			desc->array[i] = data[left - from]; \
		else \
		{ \
			size_t middle = (left + right) >> 1; \
			NS##fill_at(desc, 2 * i, left, middle, from, to < middle ? to : middle, data); \
			NS##fill_at(desc, 2 * i + 1, middle + 1, right, from > middle + 1 ? from : middle + 1, to, data + (middle + 1 - from)); \
			desc->array[i] = desc->op(desc->array[2 * i], desc->array[2 * i + 1]); \
		} \
	}

#define SML_SEGMENT_TREE_FILL_IMPL(T, TYPE, NS) \
	void NS##fill(TYPE desc, size_t from, size_t to, T *data) \
	{ \
		NS##fill_at(desc, 1, 0, desc->size - 1, from, to, data); \
	}

#define SML_SEGMENT_TREE_FOLD_AT_IMPL(T, TYPE, NS) \
	T NS##fold_at(TYPE desc, size_t i, size_t left, size_t right, size_t from, size_t to) \
	{ \
		if(from > to) \
			return desc->id; \
		if(left == from && right == to) \
			return desc->array[i]; \
		size_t middle = (left + right) >> 1; \
		return desc->op(NS##fold_at(desc, 2 * i, left, middle, from, to < middle ? to : middle), NS##fold_at(desc, 2 * i + 1, middle + 1, right, from > middle + 1 ? from : middle + 1, to)); \
	}

#define SML_SEGMENT_TREE_FOLD_IMPL(T, TYPE, NS) \
	T NS##fold(TYPE desc, size_t from, size_t to) \
	{ \
		return NS##fold_at(desc, 1, 0, desc->size - 1, from, to); \
	}
	
#define SML_SEGMENT_TREE_SET_AT_IMPL(T, TYPE, NS) \
	void NS##set_at(TYPE desc, size_t i, size_t left, size_t right, size_t at, T value) \
	{ \
		if(left == right) \
			desc->array[i] = value; \
		else \
		{ \
			size_t middle = (left + right) >> 1; \
			if(at <= middle) \
				return NS##set_at(desc, 2 * i, left, middle, at, value); \
			else \
				return NS##set_at(desc, 2 * i + 1, middle + 1, right, at, value); \
		} \
	}

#define SML_SEGMENT_TREE_SET_IMPL(T, TYPE, NS) \
	void NS##set(TYPE desc, size_t at, T value) \
	{ \
		NS##set_at(desc, 1, 0, desc->size - 1, at, value); \
	}

#define SML_SEGMENT_TREE_IMPL(T, TYPE, NS) \
	SML_SEGMENT_TREE_DESC_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_BUILD_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_NEW_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_DESTROY_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_FILL_AT_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_FILL_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_FOLD_AT_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_FOLD_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_SET_AT_IMPL(T, TYPE, NS); \
	SML_SEGMENT_TREE_SET_IMPL(T, TYPE, NS)
