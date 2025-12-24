/* 03 單向鏈結串列（C）/ Singly linked list (C). */  // Bilingual file header.
#include "singly_linked_list.h"  // Include public API declarations.

#include <stdlib.h>  // Use malloc/free for manual memory management.

static bool require_valid_list(const singly_linked_list* list) {  // Validate pointer and basic invariants.
    if (!list) {  // Reject null pointer to avoid undefined behavior.
        return false;  // Signal invalid input.
    }  // Close null check.
    if (list->size < 0) {  // Size should never be negative.
        return false;  // Signal invalid state.
    }  // Close size check.
    if (list->size == 0 && list->head != NULL) {  // Empty list should have NULL head.
        return false;  // Signal invalid state.
    }  // Close empty invariant.
    if (list->size > 0 && list->head == NULL) {  // Non-empty list must have non-null head.
        return false;  // Signal invalid state.
    }  // Close non-empty invariant.
    return true;  // Signal valid state.
}  // End require_valid_list.

static singly_list_node* make_node(int value, singly_list_node* next) {  // Allocate one node with value and next.
    singly_list_node* node = (singly_list_node*)malloc(sizeof(singly_list_node));  // Allocate node on heap.
    if (!node) {  // Handle allocation failure.
        return NULL;  // Signal failure.
    }  // Close allocation check.
    node->value = value;  // Store value.
    node->next = next;  // Store next pointer.
    return node;  // Return allocated node.
}  // End make_node.

bool singly_linked_list_init(singly_linked_list* list) {  // Initialize an empty list.
    if (!list) {  // Reject null pointer.
        return false;  // Signal failure.
    }  // Close null check.
    list->head = NULL;  // Empty list has null head.
    list->size = 0;  // Empty list has size 0.
    return true;  // Signal success.
}  // End singly_linked_list_init.

void singly_linked_list_destroy(singly_linked_list* list) {  // Free all nodes and reset list.
    if (!list) {  // Allow destroying a null pointer safely.
        return;  // Nothing to do.
    }  // Close null check.
    singly_list_node* current = list->head;  // Start at head.
    while (current != NULL) {  // Traverse until end.
        singly_list_node* next = current->next;  // Save next pointer before freeing.
        free(current);  // Free current node.
        current = next;  // Move to next node.
    }  // Close traversal loop.
    list->head = NULL;  // Reset head.
    list->size = 0;  // Reset size.
}  // End singly_linked_list_destroy.

bool singly_linked_list_push_front(singly_linked_list* list, int value, singly_list_cost* out_cost) {  // Insert at head.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    singly_list_node* node = make_node(value, list->head);  // Allocate node and link it before head.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    list->head = node;  // Update head to new node.
    list->size += 1;  // Update size.
    out_cost->hops = 0;  // Head insertion does not traverse `next`.
    return true;  // Signal success.
}  // End singly_linked_list_push_front.

bool singly_linked_list_push_back(singly_linked_list* list, int value, singly_list_cost* out_cost) {  // Insert at tail.
    if (!out_cost) {  // Require cost output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    if (list->head == NULL) {  // Special-case empty list.
        singly_list_node* node = make_node(value, NULL);  // Allocate new head node.
        if (!node) {  // Handle allocation failure.
            return false;  // Signal failure.
        }  // Close allocation check.
        list->head = node;  // Set head.
        list->size = 1;  // Size becomes 1.
        out_cost->hops = 0;  // No traversal needed.
        return true;  // Signal success.
    }  // Close empty case.

    int hops = 0;  // Count `next` traversals.
    singly_list_node* current = list->head;  // Start from head.
    while (current->next != NULL) {  // Walk until last node.
        current = current->next;  // Traverse one link.
        hops += 1;  // Count one hop.
    }  // Close traversal loop.

    singly_list_node* node = make_node(value, NULL);  // Allocate new tail node.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    current->next = node;  // Link new node after last.
    list->size += 1;  // Update size.
    out_cost->hops = hops;  // Record traversal hops.
    return true;  // Signal success.
}  // End singly_linked_list_push_back.

bool singly_linked_list_get(const singly_linked_list* list, int index, singly_list_get_result* out_result) {  // Get value at index and cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= list->size) {  // Validate index.
        return false;  // Signal invalid index.
    }  // Close index check.

    int hops = 0;  // Count `next` traversals.
    singly_list_node* current = list->head;  // Start from head.
    for (int i = 0; i < index; i++) {  // Move forward index times.
        current = current->next;  // Traverse one link.
        hops += 1;  // Count one hop.
    }  // Close loop.
    out_result->value = current->value;  // Store fetched value.
    out_result->cost.hops = hops;  // Store traversal cost.
    return true;  // Signal success.
}  // End singly_linked_list_get.

bool singly_linked_list_insert_at(singly_linked_list* list, int index, int value, singly_list_cost* out_cost) {  // Insert at index and return cost.
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
        return singly_linked_list_push_front(list, value, out_cost);  // Delegate to push_front.
    }  // Close head case.

    int hops = 0;  // Count traversal hops.
    singly_list_node* prev = list->head;  // Start from head (need predecessor).
    for (int i = 0; i < index - 1; i++) {  // Move to predecessor.
        prev = prev->next;  // Traverse one link.
        hops += 1;  // Count one hop.
    }  // Close loop.

    singly_list_node* node = make_node(value, prev->next);  // Allocate new node and point it to successor.
    if (!node) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    prev->next = node;  // Splice new node after predecessor.
    list->size += 1;  // Update size.
    out_cost->hops = hops;  // Store traversal cost.
    return true;  // Signal success.
}  // End singly_linked_list_insert_at.

bool singly_linked_list_remove_at(singly_linked_list* list, int index, singly_list_remove_result* out_result) {  // Remove at index and return removed value + cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= list->size) {  // Validate index.
        return false;  // Signal invalid index.
    }  // Close index check.

    if (index == 0) {  // Special-case head removal.
        singly_list_node* removed = list->head;  // Capture node to remove.
        list->head = removed->next;  // Bypass old head.
        list->size -= 1;  // Update size.
        out_result->value = removed->value;  // Store removed value.
        out_result->cost.hops = 0;  // No traversal needed.
        free(removed);  // Free removed node.
        return true;  // Signal success.
    }  // Close head case.

    int hops = 0;  // Count traversal hops.
    singly_list_node* prev = list->head;  // Start from head (need predecessor).
    for (int i = 0; i < index - 1; i++) {  // Move to predecessor.
        prev = prev->next;  // Traverse one link.
        hops += 1;  // Count one hop.
    }  // Close loop.

    singly_list_node* removed = prev->next;  // Node to remove.
    prev->next = removed->next;  // Bypass removed node.
    list->size -= 1;  // Update size.
    out_result->value = removed->value;  // Store removed value.
    out_result->cost.hops = hops;  // Store traversal cost.
    free(removed);  // Free removed node.
    return true;  // Signal success.
}  // End singly_linked_list_remove_at.

bool singly_linked_list_index_of(const singly_linked_list* list, int value, singly_list_find_result* out_result) {  // Find first index of value and cost.
    if (!out_result) {  // Require output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.

    int index = 0;  // Track index.
    int hops = 0;  // Count `next` traversals.
    singly_list_node* current = list->head;  // Start from head.
    while (current != NULL) {  // Traverse until end.
        if (current->value == value) {  // Check for match.
            out_result->index = index;  // Store found index.
            out_result->cost.hops = hops;  // Store traversal cost.
            return true;  // Signal success.
        }  // Close match branch.
        current = current->next;  // Traverse one link.
        hops += 1;  // Count one hop.
        index += 1;  // Advance index.
    }  // Close traversal loop.
    out_result->index = -1;  // Not found sentinel.
    out_result->cost.hops = hops;  // Store traversal cost (equals number of traversals performed).
    return true;  // Signal success (function executed).
}  // End singly_linked_list_index_of.

bool singly_linked_list_to_array(const singly_linked_list* list, int* out_values, int out_capacity) {  // Copy list contents to an array.
    if (!out_values) {  // Require output buffer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_list(list)) {  // Validate list state.
        return false;  // Signal failure.
    }  // Close validation.
    if (out_capacity < list->size) {  // Require sufficient capacity.
        return false;  // Signal insufficient capacity.
    }  // Close capacity check.
    singly_list_node* current = list->head;  // Start from head.
    int i = 0;  // Track write index.
    while (current != NULL) {  // Traverse until end.
        out_values[i] = current->value;  // Copy one value.
        i += 1;  // Advance index.
        current = current->next;  // Move to next node.
    }  // Close traversal loop.
    return true;  // Signal success.
}  // End singly_linked_list_to_array.

