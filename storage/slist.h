#include <stdlib.h>

/*
 * slist<T>
 * Singly linked list of items of type T.
 *
 * slist_iterator<T>
 * Iterator over a slist<T>. Allows access to and modification of elements of
 * the list. Can point either to a specific element, or after the end of the
 * list. The iterator is actually bound to the preceeding element of the list,
 * so if the previous element is removed, the iterator becomes invalid. And if
 * an element is inserted inbetween the iterator's position and the previous
 * element, the it then points at the newly inserted element.
 *
 * slist<T> new()
 * Allocate and return an empty list.
 *
 * void destroy(slist<T> l)
 * Destroy the list l.
 *
 * slist_iterator<T> begin(slist<T> l)
 * Iterator pointing to the beginning of l.
 *
 * int iter_good(slist_iterator<T> i)
 * Returns zero if i points past the end of the list, and a non-zero value
 * otherwise.
 *
 * slist_iterator<T> iter_next(slist_iterator<T> i)
 * Returns an iterator pointing at the position directly after i. i should point
 * at an element of the list.
 *
 * T *iter_at(slist_iterator<T> i)
 * Returns a pointer to the element contained at the position i points at,
 * which can be used to access and modify the said element. The returned pointer
 * becomes invalid when the element is removed from the list. i should point at
 * an element of the list.
 *
 * slist_iterator<T> iter_insert_advance(slist_iterator<T> i, T x)
 * Inserts the value x directly before the position i points at, and returns an
 * iterator that points at the new position of the element i pointed at.
 *
 * void iter_insert_before(slist_iterator<T> i, T x)
 * Inserts the value x directly before the position i points at. i then points
 * at the position of the inserted value.
 * 
 * T iter_remove_at(slist_iterator<T> i)
 * Removes and returns the value pointed at by the iterator i. i then points at
 * the next element. i should not initially point past the end of the list.
 */

#define SML_SLIST_TYPE_DECL(T, TYPE, NS) typedef struct TYPE##_node **TYPE
#define SML_SLIST_TYPE_ITERATOR_DECL(T, TYPE, NS) typedef struct { struct TYPE##_node **current; } TYPE##_iterator
#define SML_SLIST_NEW_DECL(T, TYPE, NS) TYPE NS##new()
#define SML_SLIST_DESTROY_DECL(T, TYPE, NS) void NS##destroy(TYPE)
#define SML_SLIST_BEGIN_DECL(T, TYPE, NS) TYPE##_iterator NS##begin(TYPE)
#define SML_SLIST_ITER_GOOD_DECL(T, TYPE, NS) int NS##iter_good(TYPE##_iterator)
#define SML_SLIST_ITER_NEXT_DECL(T, TYPE, NS) TYPE##_iterator NS##iter_next(TYPE##_iterator)
#define SML_SLIST_ITER_AT_DECL(T, TYPE, NS) T *NS##iter_at(TYPE##_iterator)
#define SML_SLIST_ITER_INSERT_ADVANCE_DECL(T, TYPE, NS) TYPE##_iterator NS##iter_insert_advance(TYPE##_iterator, T)
#define SML_SLIST_ITER_INSERT_BEFORE_DECL(T, TYPE, NS) void NS##iter_insert_before(TYPE##_iterator, T)
#define SML_SLIST_ITER_REMOVE_AT_DECL(T, TYPE, NS) T NS##iter_remove_at(TYPE##_iterator)

#define SML_SLIST_DECL(T, TYPE, NS) \
	SML_SLIST_TYPE_DECL(T, TYPE, NS); \
	SML_SLIST_TYPE_ITERATOR_DECL(T, TYPE, NS); \
	SML_SLIST_NEW_DECL(T, TYPE, NS); \
	SML_SLIST_DESTROY_DECL(T, TYPE, NS); \
	SML_SLIST_BEGIN_DECL(T, TYPE, NS); \
	SML_SLIST_ITER_GOOD_DECL(T, TYPE, NS); \
	SML_SLIST_ITER_NEXT_DECL(T, TYPE, NS); \
	SML_SLIST_ITER_AT_DECL(T, TYPE, NS); \
	SML_SLIST_ITER_INSERT_ADVANCE_DECL(T, TYPE, NS); \
	SML_SLIST_ITER_INSERT_BEFORE_DECL(T, TYPE, NS); \
	SML_SLIST_ITER_REMOVE_AT_DECL(T, TYPE, NS)

#define SML_SLIST_NODE_IMPL(T, TYPE, NS) \
	struct TYPE##_node { \
		T data; \
		struct TYPE##_node *next; \
	}

#define SML_SLIST_NEW_IMPL(T, TYPE, NS) \
	TYPE NS##new() \
	{ \
		struct TYPE##_node **head = malloc(sizeof(struct TYPE##_node *)); \
		*head = NULL; \
		return head; \
	}

#define SML_SLIST_DESTROY_IMPL(T, TYPE, NS) \
	void NS##destroy(TYPE head) \
	{ \
		struct TYPE##_node *cur = *head; \
		free(head); \
		while(cur) \
		{ \
			struct TYPE##_node *next = cur->next; \
			free(cur); \
			cur = next; \
		} \
	}

#define SML_SLIST_BEGIN_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##begin(TYPE head) \
	{ \
		TYPE##_iterator i; \
		i.current = head; \
		return i; \
	}

#define SML_SLIST_ITER_GOOD_IMPL(T, TYPE, NS) \
	int NS##iter_good(TYPE##_iterator i) \
	{ \
		return !!*i.current; \
	}

#define SML_SLIST_ITER_NEXT_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##iter_next(TYPE##_iterator i) \
	{ \
		if(*i.current) \
			i.current = &(*i.current)->next; \
		return i; \
	}

#define SML_SLIST_ITER_AT_IMPL(T, TYPE, NS) \
	T *NS##iter_at(TYPE##_iterator i) \
	{ \
		return &(*i.current)->data; \
	}

#define SML_SLIST_ITER_INSERT_ADVANCE_IMPL(T, TYPE, NS) \
	TYPE##_iterator NS##iter_insert_advance(TYPE##_iterator i, T data) \
	{ \
		struct TYPE##_node *node = malloc(sizeof(struct TYPE##_node)); \
		node->data = data; \
		node->next = *i.current; \
		*i.current = node; \
		i.current = &node->next; \
		return i; \
	}

#define SML_SLIST_ITER_INSERT_BEFORE_IMPL(T, TYPE, NS) \
	void NS##iter_insert_before(TYPE##_iterator i, T data) \
	{ \
		struct TYPE##_node *node = malloc(sizeof(struct TYPE##_node)); \
		node->data = data; \
		node->next = *i.current; \
		*i.current = node; \
	}

#define SML_SLIST_ITER_REMOVE_AT_IMPL(T, TYPE, NS) \
	T NS##iter_remove_at(TYPE##_iterator i) \
	{ \
		T data = (*i.current)->data;\
		struct TYPE##_node *next = (*i.current)->next; \
		free(*i.current); \
		*i.current = next; \
		return data; \
	}

#define SML_SLIST_IMPL(T, TYPE, NS) \
	SML_SLIST_NODE_IMPL(T, TYPE, NS); \
	SML_SLIST_NEW_IMPL(T, TYPE, NS); \
	SML_SLIST_DESTROY_IMPL(T, TYPE, NS); \
	SML_SLIST_BEGIN_IMPL(T, TYPE, NS); \
	SML_SLIST_ITER_GOOD_IMPL(T, TYPE, NS); \
	SML_SLIST_ITER_NEXT_IMPL(T, TYPE, NS); \
	SML_SLIST_ITER_AT_IMPL(T, TYPE, NS); \
	SML_SLIST_ITER_INSERT_ADVANCE_IMPL(T, TYPE, NS); \
	SML_SLIST_ITER_INSERT_BEFORE_IMPL(T, TYPE, NS); \
	SML_SLIST_ITER_REMOVE_AT_IMPL(T, TYPE, NS)
