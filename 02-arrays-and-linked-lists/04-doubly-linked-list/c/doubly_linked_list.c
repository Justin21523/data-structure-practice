/* 04 雙向鏈結串列（C）/ Doubly linked list (C). */  // Bilingual file header.
#include "doubly_linked_list.h"  // Include public API declarations.

#include <stdlib.h>  // Use malloc/free for manual memory management.

static bool require_valid_list(const doubly_linked_list* list) {  // Validate pointer and basic invariants.
    if (!list) {  // Reject null pointer to avoid undefined behavior.
        return false;  // Signal invalid input.
    }  // Close null check.
    if (list->size < 0) {  // Size should never be negative.
        return false;  // Signal invalid state.
    }  // Close size check.
    if (list->size == 0 && (list->head != NULL || list->tail != NULL)) {  // Empty list must have null head and tail.
        return false;  // Signal invalid state.
    }  // Close empty invariant.
    if (list->size > 0 && (list->head == NULL || list->tail == NULL)) {  // Non-empty list must have non-null head and tail.
        return false;  // Signal invalid state.
    }  // Close non-empty invariant.
    return true;  // Signal valid state.
}  // End require_valid_list.

static dll_node* make_node(int value, dll_node* prev, dll_node* next) {  // Allocate one node with value and neighbors.
    dll_node* node = (dll_node*)malloc(sizeof(dll_node));  // Allocate node on heap.
    if (!node) {  // Handle allocation failure.
        return NULL;  // Signal failure.
    }  // Close allocation check.
    node->value = value;  // Store value.
    node->prev = prev;  // Store prev pointer.
    node->next = next;  // Store next pointer.
    return node;  // Return allocated node.
}  // End make_node.

static bool node_at_with_cost(const doubly_linked_list* list, int index, dll_node** out_node, int* out_hops) {  // Locate node at index and count hops.
    if (!out_node || !out_hops) {  // Validate output pointers.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= list->size) {  // Validate index.
        return false;  // Signal invalid index.
    }  // Close index check.

    if (index < list->size / 2) {  // Traverse from head when index is in the first half.
        int hops = 0;  // Count pointer traversals.
        dll_node* current = list->head;  // Start from head.
        for (int i = 0; i < index; i++) {  // Move forward index times.
            current = current->next;  // Traverse one next pointer.
            hops += 1;  // Count one hop.
        }  // Close loop.
        *out_node = current;  // Return node.
        *out_hops = hops;  // Return hop count.
        return true;  // Signal success.
    }  // Close head traversal branch.

    int steps = list->size - 1 - index;  // How many prev steps needed from tail.
    int hops = 0;  // Count pointer traversals.
    dll_node* current = list->tail;  // Start from tail.
    for (int i = 0; i < steps; i++) {  // Move backward `steps` times.
        current = current->prev;  // Traverse one prev pointer.
        hops += 1;  // Count one hop.
    }  // Close loop.
    *out_node = current;  // Return node.
    *out_hops = hops;  // Return hop count.
    return true;  // Signal success.
}  // End node_at_with_cost.

bool doubly_linked_list_init(doubly_linked_list* list) {  // Initialize an empty list.
    if (!list) {  // Reject null pointer.
        return false;  // Signal failure.
    }  // Close null check.
    list->head = NULL;  // Empty list has null head.
    list->tail = NULL;  // Empty list has null tail.
    list->size = 0;  // Empty list has size 0.
    return true;  // Signal success.
}  // End doubly_linked_list_init.

void doubly_linked_list_destroy(doubly_linked_list* list) {  // Free all nodes and reset list.
    if (!list) {  // Allow destroying a null pointer safely.
        return;  // Nothing to do.
    }  // Close null check.
    dll_node* current = list->head;  // Start at head.
    while (current != NULL) {  // Traverse until end.
        dll_node* next = current->next;  // Save next pointer before freeing.
        free(current);  // Free current node.
        current = next;  // Move to next node.
    }  // Close traversal loop.
    list->head = NULL;  // Reset head.
    list->tail = NULL;  // Reset tail.
    list->size = 0;  // Reset size.
}  // End doubly_linked_list_destroy.

bool doubly_linked_list_push_front(doubly_linked_list* list, int value, dll_cost* out_cost) {  // Insert at head.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    dll_node* node = make_node(value, NULL, list->head);  // New node points forward to old head.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.

    if (list->head == NULL) {  // Empty list case.
        list->head = node;  // Head becomes node.
        list->tail = node;  // Tail becomes node.
        list->size = 1;  // Size becomes 1.
        out_cost->hops = 0;  // No traversal.
        return true;  // Signal success.
    }  // Close empty case.

    list->head->prev = node;  // Old head points back to new head.
    list->head = node;  // Update head pointer.
    list->size += 1;  // Update size.
    out_cost->hops = 0;  // Head insertion does not traverse.
    return true;  // Signal success.
}  // End doubly_linked_list_push_front.

bool doubly_linked_list_push_back(doubly_linked_list* list, int value, dll_cost* out_cost) {  // Insert at tail.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    dll_node* node = make_node(value, list->tail, NULL);  // New node points back to old tail.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.

    if (list->tail == NULL) {  // Empty list case.
        list->head = node;  // Head becomes node.
        list->tail = node;  // Tail becomes node.
        list->size = 1;  // Size becomes 1.
        out_cost->hops = 0;  // No traversal.
        return true;  // Signal success.
    }  // Close empty case.

    list->tail->next = node;  // Old tail points forward to new tail.
    list->tail = node;  // Update tail pointer.
    list->size += 1;  // Update size.
    out_cost->hops = 0;  // Tail insertion does not traverse.
    return true;  // Signal success.
}  // End doubly_linked_list_push_back.

bool doubly_linked_list_pop_front(doubly_linked_list* list, dll_remove_result* out_result) {  // Remove head.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (list->head == NULL) {  // Reject pop from empty list.
        return false;  // Signal empty.
    }  // Close empty check.

    dll_node* removed = list->head;  // Capture node to remove.
    out_result->value = removed->value;  // Store removed value.
    out_result->cost.hops = 0;  // No traversal for head removal.

    dll_node* new_head = removed->next;  // Candidate new head.
    if (new_head == NULL) {  // Removing the only node empties the list.
        list->head = NULL;  // Clear head.
        list->tail = NULL;  // Clear tail.
        list->size = 0;  // Reset size.
        free(removed);  // Free removed node.
        return true;  // Signal success.
    }  // Close single-node case.

    new_head->prev = NULL;  // New head has no previous node.
    list->head = new_head;  // Update head pointer.
    list->size -= 1;  // Update size.
    free(removed);  // Free removed node.
    return true;  // Signal success.
}  // End doubly_linked_list_pop_front.

bool doubly_linked_list_pop_back(doubly_linked_list* list, dll_remove_result* out_result) {  // Remove tail.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (list->tail == NULL) {  // Reject pop from empty list.
        return false;  // Signal empty.
    }  // Close empty check.

    dll_node* removed = list->tail;  // Capture node to remove.
    out_result->value = removed->value;  // Store removed value.
    out_result->cost.hops = 0;  // No traversal for tail removal.

    dll_node* new_tail = removed->prev;  // Candidate new tail.
    if (new_tail == NULL) {  // Removing the only node empties the list.
        list->head = NULL;  // Clear head.
        list->tail = NULL;  // Clear tail.
        list->size = 0;  // Reset size.
        free(removed);  // Free removed node.
        return true;  // Signal success.
    }  // Close single-node case.

    new_tail->next = NULL;  // New tail has no next node.
    list->tail = new_tail;  // Update tail pointer.
    list->size -= 1;  // Update size.
    free(removed);  // Free removed node.
    return true;  // Signal success.
}  // End doubly_linked_list_pop_back.

bool doubly_linked_list_get(const doubly_linked_list* list, int index, dll_get_result* out_result) {  // Get value at index and cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.

    dll_node* node = NULL;  // Node pointer to fill.
    int hops = 0;  // Hop count to fill.
    if (!node_at_with_cost(list, index, &node, &hops)) {  // Locate node and cost.
        return false;  // Signal failure.
    }  // Close failure check.
    out_result->value = node->value;  // Store fetched value.
    out_result->cost.hops = hops;  // Store traversal cost.
    return true;  // Signal success.
}  // End doubly_linked_list_get.

bool doubly_linked_list_insert_at(doubly_linked_list* list, int index, int value, dll_cost* out_cost) {  // Insert at index and return traversal cost.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index > list->size) {  // Validate insert index in [0, size].
        return false;  // Signal invalid index.
    }  // Close index check.

    if (index == 0) {  // Head insertion is O(1).
        return doubly_linked_list_push_front(list, value, out_cost);  // Delegate to push_front.
    }  // Close head case.
    if (index == list->size) {  // Tail insertion is O(1).
        return doubly_linked_list_push_back(list, value, out_cost);  // Delegate to push_back.
    }  // Close tail case.

    dll_node* target = NULL;  // Target node currently at index.
    int hops = 0;  // Traversal cost.
    if (!node_at_with_cost(list, index, &target, &hops)) {  // Locate target and hops.
        return false;  // Signal failure.
    }  // Close failure check.

    dll_node* before = target->prev;  // Predecessor node.
    dll_node* node = make_node(value, before, target);  // New node sits before target.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.

    before->next = node;  // Predecessor points forward to new node.
    target->prev = node;  // Target points back to new node.
    list->size += 1;  // Update size.
    out_cost->hops = hops;  // Store traversal cost.
    return true;  // Signal success.
}  // End doubly_linked_list_insert_at.

bool doubly_linked_list_remove_at(doubly_linked_list* list, int index, dll_remove_result* out_result) {  // Remove at index and return removed value + cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= list->size) {  // Validate index.
        return false;  // Signal invalid index.
    }  // Close index check.

    if (index == 0) {  // Head removal is O(1).
        return doubly_linked_list_pop_front(list, out_result);  // Delegate to pop_front.
    }  // Close head case.
    if (index == list->size - 1) {  // Tail removal is O(1).
        return doubly_linked_list_pop_back(list, out_result);  // Delegate to pop_back.
    }  // Close tail case.

    dll_node* target = NULL;  // Node to remove.
    int hops = 0;  // Traversal cost.
    if (!node_at_with_cost(list, index, &target, &hops)) {  // Locate target and hops.
        return false;  // Signal failure.
    }  // Close failure check.

    dll_node* before = target->prev;  // Predecessor.
    dll_node* after = target->next;  // Successor.
    before->next = after;  // Bypass target from the left.
    after->prev = before;  // Bypass target from the right.
    list->size -= 1;  // Update size.
    out_result->value = target->value;  // Store removed value.
    out_result->cost.hops = hops;  // Store traversal cost.
    free(target);  // Free removed node.
    return true;  // Signal success.
}  // End doubly_linked_list_remove_at.

bool doubly_linked_list_index_of(const doubly_linked_list* list, int value, dll_find_result* out_result) {  // Find first index of value and cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    int index = 0;  // Track index.
    int hops = 0;  // Count next traversals.
    dll_node* current = list->head;  // Start from head.
    while (current != NULL) {  // Traverse until end.
        if (current->value == value) {  // Check for match.
            out_result->index = index;  // Store found index.
            out_result->cost.hops = hops;  // Store traversal cost.
            return true;  // Signal success.
        }  // Close match branch.
        current = current->next;  // Traverse one next pointer.
        hops += 1;  // Count one hop.
        index += 1;  // Advance index.
    }  // Close traversal loop.
    out_result->index = -1;  // Not found sentinel.
    out_result->cost.hops = hops;  // Store traversal cost (equals number of traversals performed).
    return true;  // Signal success (function executed).
}  // End doubly_linked_list_index_of.

bool doubly_linked_list_to_array(const doubly_linked_list* list, int* out_values, int out_capacity) {  // Copy list contents to an array.
    if (!out_values) {  // Require output buffer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (out_capacity < list->size) {  // Require sufficient capacity.
        return false;  // Signal insufficient capacity.
    }  // Close capacity check.
    dll_node* current = list->head;  // Start from head.
    int i = 0;  // Track write index.
    while (current != NULL) {  // Traverse until end.
        out_values[i] = current->value;  // Copy one value.
        i += 1;  // Advance index.
        current = current->next;  // Move to next node.
    }  // Close traversal loop.
    return true;  // Signal success.
}  // End doubly_linked_list_to_array.

