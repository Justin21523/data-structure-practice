/* 03 單向鏈結串列（C）/ Singly linked list (C). */  // Bilingual header for this unit's public API.
#ifndef SINGLY_LINKED_LIST_H  // Header guard to prevent multiple inclusion.
#define SINGLY_LINKED_LIST_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct singly_list_cost {  // Record deterministic traversal costs for this unit.
    int hops;  // Number of `next` pointer traversals performed (current = current->next).
} singly_list_cost;  // End of cost struct.

/* get 結果 / Result of a get operation. */  // Bilingual struct purpose.
typedef struct singly_list_get_result {  // Return fetched value plus its traversal cost.
    int value;  // The fetched element value.
    singly_list_cost cost;  // Traversal cost record.
} singly_list_get_result;  // End get result struct.

/* remove 結果 / Result of a removeAt operation. */  // Bilingual struct purpose.
typedef struct singly_list_remove_result {  // Return removed value plus its traversal cost.
    int value;  // The removed element value.
    singly_list_cost cost;  // Traversal cost record.
} singly_list_remove_result;  // End remove result struct.

/* find 結果 / Result of an indexOf operation. */  // Bilingual struct purpose.
typedef struct singly_list_find_result {  // Return found index plus traversal cost.
    int index;  // Found index, or -1 if not found.
    singly_list_cost cost;  // Traversal cost record.
} singly_list_find_result;  // End find result struct.

/* 節點 / Node. */  // Bilingual struct purpose.
typedef struct singly_list_node {  // A singly linked list node (value + next pointer).
    int value;  // Stored element value.
    struct singly_list_node* next;  // Next pointer (NULL means end of list).
} singly_list_node;  // End node struct.

/* 單向鏈結串列 / Singly linked list. */  // Bilingual struct purpose.
typedef struct singly_linked_list {  // A head-only singly linked list (no tail pointer).
    singly_list_node* head;  // Head pointer (NULL means empty).
    int size;  // Number of stored nodes.
} singly_linked_list;  // End list struct.

bool singly_linked_list_init(singly_linked_list* list);  // Initialize an empty list; returns false on invalid pointer.
void singly_linked_list_destroy(singly_linked_list* list);  // Free all nodes and reset list to empty state.

bool singly_linked_list_push_front(singly_linked_list* list, int value, singly_list_cost* out_cost);  // Insert at head; hops=0.
bool singly_linked_list_push_back(singly_linked_list* list, int value, singly_list_cost* out_cost);  // Insert at tail; hops=max(0,size-1) without tail pointer.

bool singly_linked_list_get(const singly_linked_list* list, int index, singly_list_get_result* out_result);  // Get value at index and hops; returns false on invalid index.
bool singly_linked_list_insert_at(singly_linked_list* list, int index, int value, singly_list_cost* out_cost);  // Insert at index; returns false on invalid index or allocation failure.
bool singly_linked_list_remove_at(singly_linked_list* list, int index, singly_list_remove_result* out_result);  // Remove at index; returns false on invalid index.

bool singly_linked_list_index_of(const singly_linked_list* list, int value, singly_list_find_result* out_result);  // Find first index of value; returns true with index=-1 when not found.

bool singly_linked_list_to_array(const singly_linked_list* list, int* out_values, int out_capacity);  // Copy list to array; returns false when capacity is insufficient.

#endif  // SINGLY_LINKED_LIST_H  // End of header guard.

