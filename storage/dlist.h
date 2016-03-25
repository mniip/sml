#include <stdlib.h>
/*
 * dlist<T>
 * Doubly linked list of items of type T.
 *
 * dlist_iterator<T>
 * Iterator over a dlist<T>. Allows access to and modification of elements of
 * the list. Can point either to a specific element, or after the end of the
 * list, or before its beginning. The iterator becomes invalid if the element it
 * points at is removed from the list.
 *
 * dlist<T> new()
 * Allocate and return an empty list.
 *
 * void destroy(dlist<T> l)
 * Destroy the list l.
 *
 * dlist_iterator<T> begin(dlist<T> l)
 * Iterator pointing to the first element of l.
 *
 * dlist_iterator<T> end(dlist<T> l)
 * Iterator pointing to the last element of l.
 *
 * int iter_good(dlist_iterator<T> i)
 * Returns zero if i points past the end of the list or before its beginning,
 * and a non-zero value otherwise.
 *
 * dlist_iterator<T> iter_next(dlist_iterator<T> i)
 * Returns an iterator pointing at the position directly after i. i should not 
 * point past the end of the list.
 *
 * dlist_iterator<T> iter_prev(dlist_iterator<T> i)
 * Returns an iterator pointing at the position directly before i. i should not 
 * point before the beginning of the list.
 *
 * T *iter_at(dlist_iterator<T> i)
 * Returns a pointer to the element contained at the position i points at,
 * which can be used to access and modify the said element. The returned pointer
 * becomes invalid when the element is removed from the list. i should point at
 * an element of the list.
 *
 * void iter_insert_before(dlist_iterator<T> i, T x)
 * Inserts the value x directly before the position i points at.
 *
 * void iter_insert_after(dlist_iterator<T> i, T x)
 * Inserts the value x directly after the position i points at.
 * 
 * dlist_iterator<T> iter_delete_next(dlist_iterator<T> i)
 * Removes the value pointed at by the iterator i. Returns an iterator that
 * points at the element after the one that was removed. i should point at an
 * element of the list.
 * 
 * dlist_iterator<T> iter_delete_prev(dlist_iterator<T> i)
 * Removes the value pointed at by the iterator i. Returns an iterator that
 * points at the element before the one that was removed. i should point at an
 * element of the list.
 */

#define SML_DLIST_TYPE_DECL(T, TYPE, NS) typedef struct TYPE##_desc *TYPE
#define SML_DLIST_TYPE_ITERATOR_DECL(T, TYPE, NS) typedef struct { struct TYPE##_node *current; struct TYPE##_desc *desc; } TYPE##_iterator
#define SML_DLIST_NEW_DECL(T, TYPE, NS) TYPE NS##new()
#define SML_DLIST_DESTROY_DECL(T, TYPE, NS) void NS##destroy(TYPE)
#define SML_DLIST_BEGIN_DECL(T, TYPE, NS) TYPE##_iterator NS##begin(TYPE)
#define SML_DLIST_END_DECL(T, TYPE, NS) TYPE##_iterator NS##end(TYPE)
#define SML_DLIST_ITER_GOOD_DECL(T, TYPE, NS) int NS##iter_good(TYPE##_iterator)
#define SML_DLIST_ITER_NEXT_DECL(T, TYPE, NS) TYPE##_iterator NS##iter_next(TYPE##_iterator)
#define SML_DLIST_ITER_PREV_DECL(T, TYPE, NS) TYPE##_iterator NS##iter_prev(TYPE##_iterator)
#define SML_DLIST_ITER_AT_DECL(T, TYPE, NS) T *NS##iter_at(TYPE##_iterator)
#define SML_DLIST_ITER_INSERT_BEFORE_DECL(T, TYPE, NS) void NS##iter_insert_before(TYPE##_iterator, T)
#define SML_DLIST_ITER_INSERT_AFTER_DECL(T, TYPE, NS) void NS##iter_insert_after(TYPE##_iterator, T)
#define SML_DLIST_ITER_DELETE_NEXT_DECL(T, TYPE, NS) TYPE##_iterator NS##iter_delete_next(TYPE##_iterator)
#define SML_DLIST_ITER_DELETE_PREV_DECL(T, TYPE, NS) TYPE##_iterator NS##iter_delete_prev(TYPE##_iterator)

#define SML_DLIST_DECL(T, TYPE, NS) \
	SML_DLIST_TYPE_DECL(T, TYPE, NS); \
	SML_DLIST_TYPE_ITERATOR_DECL(T, TYPE, NS); \
	SML_DLIST_NEW_DECL(T, TYPE, NS); \
	SML_DLIST_DESTROY_DECL(T, TYPE, NS); \
	SML_DLIST_BEGIN_DECL(T, TYPE, NS); \
	SML_DLIST_END_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_GOOD_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_NEXT_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_PREV_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_AT_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_INSERT_BEFORE_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_INSERT_AFTER_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_DELETE_NEXT_DECL(T, TYPE, NS); \
	SML_DLIST_ITER_DELETE_PREV_DECL(T, TYPE, NS)

#define SML_DLIST_DESC_IMPL(T, TYPE, NS) \
	struct TYPE##_desc { \
		struct TYPE##_node *first, *last; \
	}

#define SML_DLIST_NODE_IMPL(T, TYPE, NS) \
	struct TYPE##_node { \
		T data; \
		struct TYPE##_node *next, *prev; \
	}

#define SML_DLIST_NEW_IMPL(T, TYPE, NS) \
	TYPE NS##new() \
	{ \
		struct TYPE##_desc *desc = malloc(sizeof(struct TYPE##_desc)); \
		desc->first = NULL; \
		desc->last = NULL; \
		return desc; \
	}

#define SML_DLIST_DESTROY_IMPL(T, TYPE, NS) \
	void NS##destroy(TYPE desc) \
	{ \
		struct TYPE##_node *cur = desc->first; \
		free(desc); \
		while(cur) \
		{ \
			struct TYPE##_node *next = cur->next; \
			free(cur); \
			cur = next; \
		} \
	}

#define SML_DLIST_BEGIN_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##begin(TYPE desc) \
	{ \
		TYPE##_iterator i; \
		i.current = desc->first; \
		i.desc = desc; \
		return i; \
	}

#define SML_DLIST_END_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##end(TYPE desc) \
	{ \
		TYPE##_iterator i; \
		i.current = desc->last; \
		i.desc = desc; \
		return i; \
	}

#define SML_DLIST_ITER_GOOD_IMPL(T, TYPE, NS) \
	int NS##iter_good(TYPE##_iterator i) \
	{ \
		return !!i.current; \
	}

#define SML_DLIST_ITER_NEXT_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##iter_next(TYPE##_iterator i) \
	{ \
		if(i.current) \
			i.current = i.current->next; \
		else \
			i.current = i.desc->first; \
		return i; \
	}

#define SML_DLIST_ITER_PREV_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##iter_prev(TYPE##_iterator i) \
	{ \
		if(i.current) \
			i.current = i.current->prev; \
		else \
			i.current = i.desc->last; \
		return i; \
	}

#define SML_DLIST_ITER_AT_IMPL(T, TYPE, NS) \
	T *NS##iter_at(TYPE##_iterator i) \
	{ \
		return &i.current->data; \
	}

#define SML_DLIST_ITER_INSERT_BEFORE_IMPL(T, TYPE, NS) \
	void NS##iter_insert_before(TYPE##_iterator i, T data) \
	{ \
		struct TYPE##_node *node = malloc(sizeof(struct TYPE##_node)); \
		node->data = data; \
		node->next = i.current; \
		if(i.current) \
		{ \
			node->prev = i.current->prev; \
			if(i.current->prev) \
				i.current->prev->next = node; \
			else \
				i.desc->first = node; \
			i.current->prev = node; \
		} \
		else \
		{ \
			node->prev = i.desc->last; \
			if(i.desc->last) \
				i.desc->last->next = node; \
			else \
				i.desc->first = node; \
			i.desc->last = node; \
		} \
	}

#define SML_DLIST_ITER_INSERT_AFTER_IMPL(T, TYPE, NS) \
	void NS##iter_insert_after(TYPE##_iterator i, T data) \
	{ \
		struct TYPE##_node *node = malloc(sizeof(struct TYPE##_node)); \
		node->data = data; \
		node->prev = i.current; \
		if(i.current) \
		{ \
			node->next = i.current->next; \
			i.current->next = node; \
		} \
		else \
		{ \
			node->next = i.desc->first; \
			i.desc->first = node; \
			if(!i.desc->last) \
				i.desc->last = node; \
		} \
	}

#define SML_DLIST_ITER_DELETE_NEXT_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##iter_delete_next(TYPE##_iterator i) \
	{ \
		if(i.current->next) \
			i.current->next->prev = i.current->prev; \
		else \
			i.desc->last = i.current->prev; \
		if(i.current->prev) \
			i.current->prev->next = i.current->next; \
		else \
			i.desc->first = i.current->next; \
		struct TYPE##_node *next = i.current->next; \
		free(i.current); \
		i.current = next; \
		return i; \
	}

#define SML_DLIST_ITER_DELETE_PREV_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##iter_delete_prev(TYPE##_iterator i) \
	{ \
		if(i.current->next) \
			i.current->next->prev = i.current->prev; \
		else \
			i.desc->last = i.current->prev; \
		if(i.current->prev) \
			i.current->prev->next = i.current->next; \
		else \
			i.desc->first = i.current->next; \
		struct TYPE##_node *prev = i.current->prev; \
		free(i.current); \
		i.current = prev; \
		return i; \
	}

#define SML_DLIST_IMPL(T, TYPE, NS) \
	SML_DLIST_DESC_IMPL(T, TYPE, NS); \
	SML_DLIST_NODE_IMPL(T, TYPE, NS); \
	SML_DLIST_NEW_IMPL(T, TYPE, NS); \
	SML_DLIST_DESTROY_IMPL(T, TYPE, NS); \
	SML_DLIST_BEGIN_IMPL(T, TYPE, NS); \
	SML_DLIST_END_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_GOOD_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_NEXT_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_PREV_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_AT_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_INSERT_BEFORE_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_INSERT_AFTER_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_DELETE_NEXT_IMPL(T, TYPE, NS); \
	SML_DLIST_ITER_DELETE_PREV_IMPL(T, TYPE, NS)
