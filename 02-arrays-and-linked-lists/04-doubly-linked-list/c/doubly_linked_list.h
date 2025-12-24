/* 04 雙向鏈結串列（C）/ Doubly linked list (C). */  // Bilingual header for this unit's public API.
#ifndef DOUBLY_LINKED_LIST_H  // Header guard to prevent multiple inclusion.
#define DOUBLY_LINKED_LIST_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct dll_cost {  // Record deterministic traversal costs for this unit.
    int hops;  // Number of pointer traversals performed (next or prev).
} dll_cost;  // End of cost struct.

/* get 結果 / Result of a get operation. */  // Bilingual struct purpose.
typedef struct dll_get_result {  // Return fetched value plus traversal cost.
    int value;  // The fetched element value.
    dll_cost cost;  // Traversal cost record.
} dll_get_result;  // End get result struct.

/* remove 結果 / Result of a remove/pop operation. */  // Bilingual struct purpose.
typedef struct dll_remove_result {  // Return removed value plus traversal cost.
    int value;  // The removed element value.
    dll_cost cost;  // Traversal cost record.
} dll_remove_result;  // End remove result struct.

/* find 結果 / Result of an indexOf operation. */  // Bilingual struct purpose.
typedef struct dll_find_result {  // Return found index plus traversal cost.
    int index;  // Found index, or -1 if not found.
    dll_cost cost;  // Traversal cost record.
} dll_find_result;  // End find result struct.

/* 節點 / Node. */  // Bilingual struct purpose.
typedef struct dll_node {  // A doubly linked list node (value + prev/next pointers).
    int value;  // Stored element value.
    struct dll_node* prev;  // Previous pointer (NULL means head).
    struct dll_node* next;  // Next pointer (NULL means tail).
} dll_node;  // End node struct.

/* 雙向鏈結串列 / Doubly linked list. */  // Bilingual struct purpose.
typedef struct doubly_linked_list {  // A head+tail doubly linked list.
    dll_node* head;  // Head pointer (NULL means empty).
    dll_node* tail;  // Tail pointer (NULL means empty).
    int size;  // Number of stored nodes.
} doubly_linked_list;  // End list struct.

bool doubly_linked_list_init(doubly_linked_list* list);  // Initialize an empty list; returns false on invalid pointer.
void doubly_linked_list_destroy(doubly_linked_list* list);  // Free all nodes and reset list to empty state.

bool doubly_linked_list_push_front(doubly_linked_list* list, int value, dll_cost* out_cost);  // Insert at head; hops=0.
bool doubly_linked_list_push_back(doubly_linked_list* list, int value, dll_cost* out_cost);  // Insert at tail; hops=0.
bool doubly_linked_list_pop_front(doubly_linked_list* list, dll_remove_result* out_result);  // Remove head; hops=0; returns false on empty.
bool doubly_linked_list_pop_back(doubly_linked_list* list, dll_remove_result* out_result);  // Remove tail; hops=0; returns false on empty.

bool doubly_linked_list_get(const doubly_linked_list* list, int index, dll_get_result* out_result);  // Get value at index; hops=min(i,size-1-i); returns false on invalid index.
bool doubly_linked_list_insert_at(doubly_linked_list* list, int index, int value, dll_cost* out_cost);  // Insert at index; returns false on invalid index or allocation failure.
bool doubly_linked_list_remove_at(doubly_linked_list* list, int index, dll_remove_result* out_result);  // Remove at index; returns false on invalid index.

bool doubly_linked_list_index_of(const doubly_linked_list* list, int value, dll_find_result* out_result);  // Find first index of value; returns true with index=-1 when not found.

bool doubly_linked_list_to_array(const doubly_linked_list* list, int* out_values, int out_capacity);  // Copy list to array; returns false when capacity is insufficient.

#endif  // DOUBLY_LINKED_LIST_H  // End of header guard.

