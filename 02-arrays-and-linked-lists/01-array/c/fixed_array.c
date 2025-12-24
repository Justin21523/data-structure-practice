/* 01 靜態陣列示範（C）/ Static array demo (C). */  // Bilingual file header.
#include "fixed_array.h"  // Include public API declarations.

#include <stdlib.h>  // Use malloc/free for manual memory management.

static bool require_valid_array(const fixed_array* array) {  // Validate that array pointer and internal invariants are sane.
    if (!array) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal invalid input.
    }  // Close null check.
    if (array->capacity < 0) {  // Capacity should never be negative.
        return false;  // Signal invalid state.
    }  // Close capacity check.
    if (array->size < 0 || array->size > array->capacity) {  // Size must be within [0, capacity].
        return false;  // Signal invalid state.
    }  // Close size check.
    if (array->capacity > 0 && !array->data) {  // When capacity>0, data pointer must be non-null.
        return false;  // Signal invalid state.
    }  // Close data check.
    return true;  // Signal valid state.
}  // End of require_valid_array.

bool fixed_array_init(fixed_array* array, int capacity) {  // Initialize fixed array with given capacity.
    if (!array) {  // Reject null pointers.
        return false;  // Signal failure.
    }  // Close null check.
    if (capacity < 0) {  // Reject negative capacity.
        return false;  // Signal invalid input.
    }  // Close validation block.

    array->size = 0;  // Start empty.
    array->capacity = capacity;  // Store fixed capacity.
    array->data = NULL;  // Initialize pointer so destroy is always safe.

    if (capacity == 0) {  // A zero-capacity array is allowed (always full).
        return true;  // Initialization succeeds with no allocation.
    }  // Close zero-capacity branch.

    array->data = (int*)malloc((size_t)capacity * sizeof(int));  // Allocate contiguous backing buffer.
    if (!array->data) {  // Handle allocation failure.
        array->capacity = 0;  // Reset to a safe state.
        return false;  // Signal allocation failure.
    }  // Close allocation check.
    return true;  // Signal success.
}  // End of fixed_array_init.

void fixed_array_destroy(fixed_array* array) {  // Free resources held by array.
    if (!array) {  // Allow destroying null pointers for convenience.
        return;  // Nothing to do.
    }  // Close null check.
    free(array->data);  // Free backing buffer (free(NULL) is safe).
    array->data = NULL;  // Clear pointer to avoid accidental reuse.
    array->size = 0;  // Reset size to safe value.
    array->capacity = 0;  // Reset capacity to safe value.
}  // End of fixed_array_destroy.

bool fixed_array_get(const fixed_array* array, int index, int* out_value) {  // Read element at index.
    if (!out_value) {  // Reject null output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array pointer and invariants.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= array->size) {  // Validate index in [0, size-1].
        return false;  // Signal invalid index.
    }  // Close index check.
    *out_value = array->data[index];  // Read value from backing buffer.
    return true;  // Signal success.
}  // End of fixed_array_get.

bool fixed_array_set(fixed_array* array, int index, int value) {  // Write element at index.
    if (!require_valid_array(array)) {  // Validate array pointer and invariants.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= array->size) {  // Validate index in [0, size-1].
        return false;  // Signal invalid index.
    }  // Close index check.
    array->data[index] = value;  // Write directly into backing buffer.
    return true;  // Signal success.
}  // End of fixed_array_set.

bool fixed_array_append(fixed_array* array, int value, int* out_moved) {  // Append value at end.
    return fixed_array_insert_at(array, array ? array->size : 0, value, out_moved);  // Delegate to insert_at (moves will be 0).
}  // End of fixed_array_append.

bool fixed_array_insert_at(fixed_array* array, int index, int value, int* out_moved) {  // Insert value at index.
    if (!out_moved) {  // Require out_moved so caller can observe cost.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array pointer and invariants.
        return false;  // Signal failure.
    }  // Close validation.
    if (array->size >= array->capacity) {  // Reject insertion when array is full.
        return false;  // Signal overflow.
    }  // Close overflow check.
    if (index < 0 || index > array->size) {  // Validate insert index in [0, size].
        return false;  // Signal invalid index.
    }  // Close index check.

    int moved = array->size - index;  // Shifting right moves exactly (size - index) elements.
    for (int i = array->size; i > index; i--) {  // Shift right from tail toward index.
        array->data[i] = array->data[i - 1];  // Move one element one step to the right.
    }  // Close shift loop.
    array->data[index] = value;  // Write new value into gap.
    array->size += 1;  // Increase size after insertion.
    *out_moved = moved;  // Return moved elements count to the caller.
    return true;  // Signal success.
}  // End of fixed_array_insert_at.

bool fixed_array_remove_at(fixed_array* array, int index, fixed_array_remove_result* out_result) {  // Remove element at index.
    if (!out_result) {  // Reject null output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (!require_valid_array(array)) {  // Validate array pointer and invariants.
        return false;  // Signal failure.
    }  // Close validation.
    if (index < 0 || index >= array->size) {  // Validate index in [0, size-1].
        return false;  // Signal invalid index.
    }  // Close index check.

    int removed_value = array->data[index];  // Capture removed value before shifting.
    int moved = array->size - index - 1;  // Shifting left moves exactly (size - index - 1) elements.
    for (int i = index; i < array->size - 1; i++) {  // Shift left to fill removed slot.
        array->data[i] = array->data[i + 1];  // Move one element one step to the left.
    }  // Close shift loop.
    array->size -= 1;  // Decrease size after removal.

    out_result->value = removed_value;  // Store removed value.
    out_result->moved = moved;  // Store moved count.
    return true;  // Signal success.
}  // End of fixed_array_remove_at.

int fixed_array_index_of(const fixed_array* array, int value) {  // Return first index of value or -1.
    if (!require_valid_array(array)) {  // Validate array before searching.
        return -1;  // Signal not found / invalid state.
    }  // Close validation.
    for (int i = 0; i < array->size; i++) {  // Scan from left to right (linear search).
        if (array->data[i] == value) {  // Compare each element with target value.
            return i;  // Return first matching index.
        }  // Close match check.
    }  // Close loop.
    return -1;  // Return not found sentinel.
}  // End of fixed_array_index_of.

