/* 05 環狀鏈結串列（C）/ Circular linked list (C). */  // Bilingual file header.
#include "circular_linked_list.h"  // Include public API declarations.

#include <stdlib.h>  // Use malloc/free for manual memory management.

static bool require_valid_list(const circular_linked_list* list) {  // Validate pointer and basic invariants.
    if (!list) {  // Reject null pointer to avoid undefined behavior.
        return false;  // Signal invalid input.
    }  // Close null check.
    if (list->size < 0) {  // Size should never be negative.
        return false;  // Signal invalid state.
    }  // Close size check.
    if (list->size == 0 && list->tail != NULL) {  // Empty list must have null tail.
        return false;  // Signal invalid state.
    }  // Close empty invariant.
    if (list->size > 0 && list->tail == NULL) {  // Non-empty list must have non-null tail.
        return false;  // Signal invalid state.
    }  // Close non-empty invariant.
    if (list->tail != NULL && list->tail->next == NULL) {  // In a circular list, tail->next must exist.
        return false;  // Signal invalid state.
    }  // Close next invariant.
    return true;  // Signal valid state.
}  // End require_valid_list.

static cll_node* make_node(int value, cll_node* next) {  // Allocate one node with value and next pointer.
    cll_node* node = (cll_node*)malloc(sizeof(cll_node));  // Allocate node on heap.
    if (!node) {  // Handle allocation failure.
        return NULL;  // Signal failure.
    }  // Close allocation check.
    node->value = value;  // Store value.
    node->next = next;  // Store next pointer.
    return node;  // Return allocated node.
}  // End make_node.

bool circular_linked_list_init(circular_linked_list* list) {  // Initialize an empty list.
    if (!list) {  // Reject null pointer.
        return false;  // Signal failure.
    }  // Close null check.
    list->tail = NULL;  // Empty list has null tail.
    list->size = 0;  // Empty list has size 0.
    return true;  // Signal success.
}  // End circular_linked_list_init.

void circular_linked_list_destroy(circular_linked_list* list) {  // Free all nodes and reset list.
    if (!list) {  // Allow destroying a null pointer safely.
        return;  // Nothing to do.
    }  // Close null check.
    if (list->tail == NULL) {  // Empty list has nothing to free.
        list->size = 0;  // Ensure size is consistent.
        return;  // Done.
    }  // Close empty case.

    cll_node* head = list->tail->next;  // Head is tail->next.
    cll_node* current = head;  // Start from head.
    for (int i = 0; i < list->size; i++) {  // Free exactly size nodes to avoid infinite loops.
        cll_node* next = current->next;  // Save next pointer before freeing.
        free(current);  // Free current node.
        current = next;  // Move to next node.
    }  // Close loop.
    list->tail = NULL;  // Reset tail.
    list->size = 0;  // Reset size.
}  // End circular_linked_list_destroy.

bool circular_linked_list_push_back(circular_linked_list* list, int value, cll_cost* out_cost) {  // Insert at tail.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    if (list->tail == NULL) {  // Empty list case.
        cll_node* node = make_node(value, NULL);  // Create node with temporary next.
        if (!node) {  // Handle allocation failure.
            return false;  // Signal failure.
        }  // Close allocation check.
        node->next = node;  // Point to itself to form a 1-node circle.
        list->tail = node;  // Tail becomes node.
        list->size = 1;  // Size becomes 1.
        out_cost->hops = 0;  // No traversal.
        return true;  // Signal success.
    }  // Close empty case.

    cll_node* head = list->tail->next;  // Read current head.
    cll_node* node = make_node(value, head);  // New node points to head.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    list->tail->next = node;  // Old tail points to new node.
    list->tail = node;  // New node becomes tail.
    list->size += 1;  // Update size.
    out_cost->hops = 0;  // No traversal.
    return true;  // Signal success.
}  // End circular_linked_list_push_back.

bool circular_linked_list_push_front(circular_linked_list* list, int value, cll_cost* out_cost) {  // Insert at head.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    if (list->tail == NULL) {  // Empty list case.
        return circular_linked_list_push_back(list, value, out_cost);  // Reuse push_back logic.
    }  // Close empty case.

    cll_node* head = list->tail->next;  // Read current head.
    cll_node* node = make_node(value, head);  // New node points to current head.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    list->tail->next = node;  // Tail now points to new head.
    list->size += 1;  // Update size.
    out_cost->hops = 0;  // No traversal.
    return true;  // Signal success.
}  // End circular_linked_list_push_front.

bool circular_linked_list_pop_front(circular_linked_list* list, cll_remove_result* out_result) {  // Remove head.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (list->tail == NULL) {  // Reject pop from empty list.
        return false;  // Signal empty.
    }  // Close empty check.

    cll_node* head = list->tail->next;  // Head is tail->next.
    out_result->value = head->value;  // Store removed value.
    out_result->cost.hops = 0;  // No traversal for head removal.
    if (list->size == 1) {  // Removing the only node empties the list.
        free(head);  // Free node.
        list->tail = NULL;  // Clear tail.
        list->size = 0;  // Reset size.
        return true;  // Signal success.
    }  // Close single-node case.

    list->tail->next = head->next;  // Bypass head by linking tail to new head.
    list->size -= 1;  // Update size.
    free(head);  // Free removed node.
    return true;  // Signal success.
}  // End circular_linked_list_pop_front.

bool circular_linked_list_pop_back(circular_linked_list* list, cll_remove_result* out_result) {  // Remove tail and count hops.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (list->tail == NULL) {  // Reject pop from empty list.
        return false;  // Signal empty.
    }  // Close empty check.

    cll_node* old_tail = list->tail;  // Capture node to remove.
    out_result->value = old_tail->value;  // Store removed value.
    if (list->size == 1) {  // Removing the only node empties the list.
        out_result->cost.hops = 0;  // No traversal.
        free(old_tail);  // Free node.
        list->tail = NULL;  // Clear tail.
        list->size = 0;  // Reset size.
        return true;  // Signal success.
    }  // Close single-node case.

    cll_node* head = old_tail->next;  // Head is tail->next.
    cll_node* current = head;  // Start from head to find predecessor.
    int hops = 0;  // Count next traversals.
    while (current->next != old_tail) {  // Stop when current is predecessor of tail.
        current = current->next;  // Traverse one next pointer.
        hops += 1;  // Count one hop.
    }  // Close loop.
    current->next = old_tail->next;  // Predecessor now points to head.
    list->tail = current;  // Predecessor becomes new tail.
    list->size -= 1;  // Update size.
    out_result->cost.hops = hops;  // Store traversal cost.
    free(old_tail);  // Free removed node.
    return true;  // Signal success.
}  // End circular_linked_list_pop_back.

bool circular_linked_list_rotate(circular_linked_list* list, int steps, cll_cost* out_cost) {  // Rotate by moving tail forward.
    if (!out_cost) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (steps < 0) {  // Reject negative rotations for simplicity.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (list->tail == NULL || list->size <= 1) {  // Rotating empty/1-node list changes nothing.
        out_cost->hops = 0;  // No traversal.
        return true;  // Signal success.
    }  // Close trivial case.
    int steps_mod = steps % list->size;  // Reduce steps to avoid unnecessary cycles.
    int hops = 0;  // Count traversals.
    for (int i = 0; i < steps_mod; i++) {  // Advance tail steps_mod times.
        list->tail = list->tail->next;  // Move tail forward one node.
        hops += 1;  // Count one hop.
    }  // Close loop.
    out_cost->hops = hops;  // Store traversal cost.
    return true;  // Signal success.
}  // End circular_linked_list_rotate.

bool circular_linked_list_get(const circular_linked_list* list, int index, cll_get_result* out_result) {  // Get value at index and cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= list->size) {  // Validate index.
        return false;  // Signal invalid index.
    }  // Close index check.
    if (list->tail == NULL) {  // Defensive check for empty list.
        return false;  // Signal failure.
    }  // Close empty check.

    cll_node* current = list->tail->next;  // Start from head.
    int hops = 0;  // Count next traversals.
    for (int i = 0; i < index; i++) {  // Move forward index times.
        current = current->next;  // Traverse one next pointer.
        hops += 1;  // Count one hop.
    }  // Close loop.
    out_result->value = current->value;  // Store fetched value.
    out_result->cost.hops = hops;  // Store traversal cost.
    return true;  // Signal success.
}  // End circular_linked_list_get.

bool circular_linked_list_index_of(const circular_linked_list* list, int value, cll_find_result* out_result) {  // Find first index of value and cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (list->tail == NULL) {  // Empty list cannot contain the value.
        out_result->index = -1;  // Not found sentinel.
        out_result->cost.hops = 0;  // No traversal.
        return true;  // Signal success.
    }  // Close empty case.

    cll_node* current = list->tail->next;  // Start at head.
    int hops = 0;  // Count next traversals.
    for (int index = 0; index < list->size; index++) {  // Traverse exactly size nodes.
        if (current->value == value) {  // Check for match.
            out_result->index = index;  // Store found index.
            out_result->cost.hops = hops;  // Store traversal cost.
            return true;  // Signal success.
        }  // Close match branch.
        current = current->next;  // Move to next node.
        hops += 1;  // Count one hop.
    }  // Close loop.
    out_result->index = -1;  // Not found sentinel.
    out_result->cost.hops = hops;  // Store traversal cost (equals size).
    return true;  // Signal success.
}  // End circular_linked_list_index_of.

bool circular_linked_list_to_array(const circular_linked_list* list, int* out_values, int out_capacity) {  // Copy list contents to an array.
    if (!out_values) {  // Require output buffer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (out_capacity < list->size) {  // Require sufficient capacity.
        return false;  // Signal insufficient capacity.
    }  // Close capacity check.
    if (list->tail == NULL) {  // Empty list copies nothing.
        return true;  // Signal success.
    }  // Close empty case.

    cll_node* current = list->tail->next;  // Start at head.
    for (int i = 0; i < list->size; i++) {  // Copy exactly size nodes.
        out_values[i] = current->value;  // Copy one value.
        current = current->next;  // Move to next node.
    }  // Close loop.
    return true;  // Signal success.
}  // End circular_linked_list_to_array.

