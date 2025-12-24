/* 02 動態陣列示範（C）/ Dynamic array demo (C). */  // Bilingual file header.
#include "dynamic_array.h"  // Include public API declarations.

#include <stdlib.h>  // Use malloc/free for manual memory management.

static bool require_valid_array(const dynamic_array* array) {  // Validate pointer and internal invariants.
    if (!array) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal invalid input.
    }  // Close null check.
    if (array->capacity < 1) {  // Capacity should be at least 1 in this unit.
        return false;  // Signal invalid state.
    }  // Close capacity check.
    if (array->size < 0 || array->size > array->capacity) {  // Size must be within [0, capacity].
        return false;  // Signal invalid state.
    }  // Close size check.
    if (!array->data) {  // Backing buffer must be non-null after init.
        return false;  // Signal invalid state.
    }  // Close data check.
    return true;  // Signal valid state.
}  // End require_valid_array.

static bool resize(dynamic_array* array, int new_capacity, int* out_copied) {  // Resize backing buffer and count copied elements.
    if (!out_copied) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.
    if (new_capacity < array->size) {  // Ensure new buffer can hold all elements.
        return false;  // Signal invalid request.
    }  // Close size check.
    if (new_capacity < 1) {  // Keep capacity positive.
        return false;  // Signal invalid request.
    }  // Close capacity check.

    int* new_data = (int*)malloc((size_t)new_capacity * sizeof(int));  // Allocate new buffer.
    if (!new_data) {  // Handle allocation failure.
        return false;  // Signal allocation failure.
    }  // Close allocation check.

    int copied = 0;  // Count copied elements deterministically.
    for (int i = 0; i < array->size; i++) {  // Copy used portion.
        new_data[i] = array->data[i];  // Copy one element.
        copied += 1;  // Count one copy.
    }  // Close copy loop.

    free(array->data);  // Free old buffer after copying.
    array->data = new_data;  // Swap buffer pointer.
    array->capacity = new_capacity;  // Update capacity.
    array->total_copies += (long long)copied;  // Accumulate total copies.
    *out_copied = copied;  // Return copy count for this resize.
    return true;  // Signal success.
}  // End resize.

static bool ensure_capacity_for_one_more(dynamic_array* array, int* out_copied) {  // Ensure capacity for one more element.
    if (!out_copied) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.
    if (array->size < array->capacity) {  // Fast path: there is free space.
        *out_copied = 0;  // No resize needed.
        return true;  // Signal success.
    }  // Close fast path.
    return resize(array, array->capacity * 2, out_copied);  // Double capacity and return copies count.
}  // End ensure_capacity_for_one_more.

bool dynamic_array_init(dynamic_array* array) {  // Initialize empty array with capacity 1.
    if (!array) {  // Reject null pointers.
        return false;  // Signal failure.
    }  // Close null check.
    array->size = 0;  // Start empty.
    array->capacity = 1;  // Start with capacity 1 for deterministic doubling.
    array->total_copies = 0;  // Start with zero total copies.
    array->data = (int*)malloc(sizeof(int) * (size_t)array->capacity);  // Allocate backing buffer.
    if (!array->data) {  // Handle allocation failure.
        array->capacity = 0;  // Reset to safe state for destroy.
        return false;  // Signal allocation failure.
    }  // Close allocation check.
    return true;  // Signal success.
}  // End dynamic_array_init.

void dynamic_array_destroy(dynamic_array* array) {  // Free resources held by array.
    if (!array) {  // Allow destroying null pointers.
        return;  // Nothing to do.
    }  // Close null check.
    free(array->data);  // Free backing buffer (free(NULL) is safe).
    array->data = NULL;  // Clear pointer.
    array->size = 0;  // Reset fields.
    array->capacity = 0;  // Reset fields.
    array->total_copies = 0;  // Reset fields.
}  // End dynamic_array_destroy.

bool dynamic_array_get(const dynamic_array* array, int index, int* out_value) {  // Read element at index.
    if (!out_value) {  // Reject null output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= array->size) {  // Validate index in [0, size-1].
        return false;  // Signal invalid index.
    }  // Close index check.
    *out_value = array->data[index];  // Read value.
    return true;  // Signal success.
}  // End dynamic_array_get.

bool dynamic_array_set(dynamic_array* array, int index, int value) {  // Write element at index.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= array->size) {  // Validate index in [0, size-1].
        return false;  // Signal invalid index.
    }  // Close index check.
    array->data[index] = value;  // Write value.
    return true;  // Signal success.
}  // End dynamic_array_set.

bool dynamic_array_append(dynamic_array* array, int value, dynamic_array_cost* out_cost) {  // Append at end and return cost.
    if (!out_cost) {  // Require out_cost so caller can observe costs.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.

    int copied = 0;  // Copied elements due to resize.
    if (!ensure_capacity_for_one_more(array, &copied)) {  // Resize if needed.
        return false;  // Signal allocation failure.
    }  // Close resize failure.

    array->data[array->size] = value;  // Write new element.
    array->size += 1;  // Increase size.
    out_cost->copied = copied;  // Record copied elements.
    out_cost->moved = 0;  // Append does not shift elements.
    return true;  // Signal success.
}  // End dynamic_array_append.

bool dynamic_array_insert_at(dynamic_array* array, int index, int value, dynamic_array_cost* out_cost) {  // Insert at index and return cost.
    if (!out_cost) {  // Require out_cost so caller can observe costs.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index > array->size) {  // Validate insert index in [0, size].
        return false;  // Signal invalid index.
    }  // Close index check.

    int copied = 0;  // Copied elements due to resize.
    if (!ensure_capacity_for_one_more(array, &copied)) {  // Resize if needed before shifting.
        return false;  // Signal allocation failure.
    }  // Close resize failure.

    int moved = array->size - index;  // Shifting right moves (size - index) elements.
    for (int i = array->size; i > index; i--) {  // Shift right from tail toward index.
        array->data[i] = array->data[i - 1];  // Move one element right.
    }  // Close shift loop.
    array->data[index] = value;  // Write new value.
    array->size += 1;  // Increase size.
    out_cost->copied = copied;  // Record copied elements.
    out_cost->moved = moved;  // Record shifted elements.
    return true;  // Signal success.
}  // End dynamic_array_insert_at.

bool dynamic_array_remove_at(dynamic_array* array, int index, dynamic_array_remove_result* out_result) {  // Remove at index and return removed value + cost.
    if (!out_result) {  // Reject null output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array state.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= array->size) {  // Validate index in [0, size-1].
        return false;  // Signal invalid index.
    }  // Close index check.

    int removed_value = array->data[index];  // Capture removed value.
    int moved = array->size - index - 1;  // Shifting left moves (size - index - 1) elements.
    for (int i = index; i < array->size - 1; i++) {  // Shift left to fill gap.
        array->data[i] = array->data[i + 1];  // Move one element left.
    }  // Close shift loop.
    array->size -= 1;  // Decrease size.

    out_result->value = removed_value;  // Store removed value.
    out_result->cost.copied = 0;  // Remove does not resize in this unit.
    out_result->cost.moved = moved;  // Store moved count.
    return true;  // Signal success.
}  // End dynamic_array_remove_at.

int dynamic_array_index_of(const dynamic_array* array, int value) {  // Return first index of value or -1.
    if (!require_valid_array(array)) {  // Validate array state.
        return -1;  // Signal not found / invalid state.
    }  // Close validation.
    for (int i = 0; i < array->size; i++) {  // Linear scan.
        if (array->data[i] == value) {  // Compare element with target.
            return i;  // Return first match.
        }  // Close match check.
    }  // Close loop.
    return -1;  // Not found sentinel.
}  // End dynamic_array_index_of.

