// 04 雙端佇列示範（C）/ Deque demo (C).  // Bilingual file header.

#include "deque.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static int index_at(const circular_deque* deque, int offset) {  // Map logical offset [0..size) to physical buffer index.
    return (deque->head + offset) % deque->capacity;  // Wrap around using modulo.
}  // Close index_at().

static bool resize(circular_deque* deque, int new_capacity, int* out_copied) {  // Resize buffer and report copied count.
    if (new_capacity < deque->size) {  // Ensure new buffer can hold all elements.
        return false;  // Reject invalid resize request.
    }  // Close validation.
    if (new_capacity < 1) {  // Keep capacity positive for simplicity.
        return false;  // Reject invalid capacity.
    }  // Close validation.

    int* new_data = (int*)malloc((size_t)new_capacity * sizeof(int));  // Allocate new buffer.
    if (new_data == NULL) {  // Handle allocation failure.
        return false;  // Signal failure to caller.
    }  // Close allocation check.

    int copied = 0;  // Count copied elements deterministically.
    for (int i = 0; i < deque->size; i++) {  // Copy elements in deque order (front -> back).
        new_data[i] = deque->data[index_at(deque, i)];  // Copy one element into compact layout.
        copied += 1;  // Count one copy.
    }  // Close copy loop.

    free(deque->data);  // Free old buffer.
    deque->data = new_data;  // Swap buffer pointer.
    deque->capacity = new_capacity;  // Update capacity.
    deque->head = 0;  // Reset head so deque starts at index 0 after resize.
    deque->total_copies += copied;  // Accumulate total copies.
    *out_copied = copied;  // Output copied count.
    return true;  // Report success.
}  // Close resize().

static bool ensure_capacity_for_one_more(circular_deque* deque, int* out_copied) {  // Ensure capacity for one more push.
    if (deque->size < deque->capacity) {  // Fast path: free space exists.
        *out_copied = 0;  // No resize means no copies.
        return true;  // Report success.
    }  // Close fast path.
    return resize(deque, deque->capacity * 2, out_copied);  // Double capacity and return copies.
}  // Close ensure_capacity_for_one_more().

bool circular_deque_init(circular_deque* deque) {  // Initialize empty deque with capacity 1.
    deque->size = 0;  // Empty deque has size 0.
    deque->capacity = 1;  // Start with capacity 1 for deterministic doubling.
    deque->head = 0;  // Head starts at index 0.
    deque->total_copies = 0;  // Start with zero total copies.
    deque->data = (int*)malloc((size_t)deque->capacity * sizeof(int));  // Allocate backing buffer.
    return deque->data != NULL;  // Report allocation success.
}  // Close circular_deque_init().

void circular_deque_destroy(circular_deque* deque) {  // Free resources held by deque.
    free(deque->data);  // Free backing buffer.
    deque->data = NULL;  // Clear pointer to avoid dangling references.
    deque->size = 0;  // Reset size for safety.
    deque->capacity = 0;  // Reset capacity for safety.
    deque->head = 0;  // Reset head for safety.
    deque->total_copies = 0;  // Reset totals for safety.
}  // Close circular_deque_destroy().

bool circular_deque_push_back(circular_deque* deque, int value, deque_cost* out_cost) {  // Push at back and return copy/shift cost.
    int copied = 0;  // Prepare copied count output.
    if (!ensure_capacity_for_one_more(deque, &copied)) {  // Resize if needed.
        return false;  // Signal allocation failure.
    }  // Close resize branch.
    int tail_index = index_at(deque, deque->size);  // Compute tail index where new element is written.
    deque->data[tail_index] = value;  // Store value at tail position.
    deque->size += 1;  // Increase size.
    out_cost->copied = copied;  // Store copied count.
    out_cost->moved = 0;  // moved stays 0 (no shifting).
    return true;  // Report success.
}  // Close circular_deque_push_back().

bool circular_deque_push_front(circular_deque* deque, int value, deque_cost* out_cost) {  // Push at front and return copy/shift cost.
    int copied = 0;  // Prepare copied count output.
    if (!ensure_capacity_for_one_more(deque, &copied)) {  // Resize if needed.
        return false;  // Signal allocation failure.
    }  // Close resize branch.
    deque->head = (deque->head - 1 + deque->capacity) % deque->capacity;  // Move head left by one with wrap-around.
    deque->data[deque->head] = value;  // Store value at new head position.
    deque->size += 1;  // Increase size.
    out_cost->copied = copied;  // Store copied count.
    out_cost->moved = 0;  // moved stays 0 (no shifting).
    return true;  // Report success.
}  // Close circular_deque_push_front().

bool circular_deque_peek_front(const circular_deque* deque, int* out_value) {  // Read front value without removing it.
    if (deque->size == 0) {  // Reject peeking an empty deque.
        return false;  // Signal invalid operation.
    }  // Close validation.
    *out_value = deque->data[deque->head];  // Read front slot directly.
    return true;  // Report success.
}  // Close circular_deque_peek_front().

bool circular_deque_peek_back(const circular_deque* deque, int* out_value) {  // Read back value without removing it.
    if (deque->size == 0) {  // Reject peeking an empty deque.
        return false;  // Signal invalid operation.
    }  // Close validation.
    int tail_index = index_at(deque, deque->size - 1);  // Compute index of last element.
    *out_value = deque->data[tail_index];  // Read back slot directly.
    return true;  // Report success.
}  // Close circular_deque_peek_back().

bool circular_deque_pop_front(circular_deque* deque, deque_pop_result* out_result) {  // Pop from front (no shifting).
    if (deque->size == 0) {  // Reject popping an empty deque.
        return false;  // Signal invalid operation.
    }  // Close validation.
    out_result->value = deque->data[deque->head];  // Capture front value.
    out_result->cost.copied = 0;  // Pop does not resize/copy in this unit.
    out_result->cost.moved = 0;  // moved stays 0 (no shifting).
    deque->head = (deque->head + 1) % deque->capacity;  // Advance head by one with wrap-around.
    deque->size -= 1;  // Decrease size.
    if (deque->size == 0) {  // Normalize head for deterministic state when deque becomes empty.
        deque->head = 0;  // Reset head (optional but stable).
    }  // Close normalize branch.
    return true;  // Report success.
}  // Close circular_deque_pop_front().

bool circular_deque_pop_back(circular_deque* deque, deque_pop_result* out_result) {  // Pop from back (no shifting).
    if (deque->size == 0) {  // Reject popping an empty deque.
        return false;  // Signal invalid operation.
    }  // Close validation.
    int tail_index = index_at(deque, deque->size - 1);  // Compute index of last element.
    out_result->value = deque->data[tail_index];  // Capture back value.
    out_result->cost.copied = 0;  // Pop does not resize/copy in this unit.
    out_result->cost.moved = 0;  // moved stays 0 (no shifting).
    deque->size -= 1;  // Decrease size.
    if (deque->size == 0) {  // Normalize head for deterministic state when deque becomes empty.
        deque->head = 0;  // Reset head (optional but stable).
    }  // Close normalize branch.
    return true;  // Report success.
}  // Close circular_deque_pop_back().

bool circular_deque_to_array(const circular_deque* deque, int* out, int out_len) {  // Copy used portion (front->back) into out.
    if (out_len < deque->size) {  // Ensure output buffer is large enough.
        return false;  // Signal insufficient output size.
    }  // Close validation.
    for (int i = 0; i < deque->size; i++) {  // Copy elements in deque order.
        out[i] = deque->data[index_at(deque, i)];  // Copy one element.
    }  // Close copy loop.
    return true;  // Report success.
}  // Close circular_deque_to_array().

