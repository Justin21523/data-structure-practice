/* 05 環狀鏈結串列（C）/ Circular linked list (C). */  // Bilingual header for this unit's public API.
#ifndef CIRCULAR_LINKED_LIST_H  // Header guard to prevent multiple inclusion.
#define CIRCULAR_LINKED_LIST_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct cll_cost {  // Record deterministic traversal costs for this unit.
    int hops;  // Number of `next` pointer traversals performed.
} cll_cost;  // End of cost struct.

/* get 結果 / Result of a get operation. */  // Bilingual struct purpose.
typedef struct cll_get_result {  // Return fetched value plus traversal cost.
    int value;  // The fetched element value.
    cll_cost cost;  // Traversal cost record.
} cll_get_result;  // End get result struct.

/* remove 結果 / Result of a pop operation. */  // Bilingual struct purpose.
typedef struct cll_remove_result {  // Return removed value plus traversal cost.
    int value;  // The removed element value.
    cll_cost cost;  // Traversal cost record.
} cll_remove_result;  // End remove result struct.

/* find 結果 / Result of an indexOf operation. */  // Bilingual struct purpose.
typedef struct cll_find_result {  // Return found index plus traversal cost.
    int index;  // Found index, or -1 if not found.
    cll_cost cost;  // Traversal cost record.
} cll_find_result;  // End find result struct.

/* 節點 / Node. */  // Bilingual struct purpose.
typedef struct cll_node {  // A singly circular linked list node (value + next).
    int value;  // Stored element value.
    struct cll_node* next;  // Next pointer (circular in non-empty list).
} cll_node;  // End node struct.

/* 環狀鏈結串列 / Circular linked list. */  // Bilingual struct purpose.
typedef struct circular_linked_list {  // A singly circular linked list implemented with a tail pointer.
    cll_node* tail;  // Tail pointer (NULL means empty).
    int size;  // Number of stored nodes.
} circular_linked_list;  // End list struct.

bool circular_linked_list_init(circular_linked_list* list);  // Initialize an empty list; returns false on invalid pointer.
void circular_linked_list_destroy(circular_linked_list* list);  // Free all nodes and reset list to empty state.

bool circular_linked_list_push_back(circular_linked_list* list, int value, cll_cost* out_cost);  // Insert at tail; hops=0.
bool circular_linked_list_push_front(circular_linked_list* list, int value, cll_cost* out_cost);  // Insert at head; hops=0.
bool circular_linked_list_pop_front(circular_linked_list* list, cll_remove_result* out_result);  // Remove head; hops=0; returns false on empty.
bool circular_linked_list_pop_back(circular_linked_list* list, cll_remove_result* out_result);  // Remove tail; hops=max(0,n-2); returns false on empty.

bool circular_linked_list_rotate(circular_linked_list* list, int steps, cll_cost* out_cost);  // Rotate by moving tail forward; hops=steps%size; returns false on invalid steps.

bool circular_linked_list_get(const circular_linked_list* list, int index, cll_get_result* out_result);  // Get value at index; hops=index; returns false on invalid index.
bool circular_linked_list_index_of(const circular_linked_list* list, int value, cll_find_result* out_result);  // Find first index of value; returns true with index=-1 when not found.

bool circular_linked_list_to_array(const circular_linked_list* list, int* out_values, int out_capacity);  // Copy list to array; returns false when capacity is insufficient.

#endif  // CIRCULAR_LINKED_LIST_H  // End of header guard.

