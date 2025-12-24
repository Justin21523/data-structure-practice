// 02 佇列示範（C）/ Queue demo (C).  // Bilingual file header.

#include "queue.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static bool resize(array_queue* queue, int new_capacity, int* out_copied) {  // Resize buffer and report copied count.
    if (new_capacity < queue->size) {  // Ensure new buffer can hold all elements.
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
    for (int i = 0; i < queue->size; i++) {  // Copy exactly the used portion.
        new_data[i] = queue->data[i];  // Copy one element.
        copied += 1;  // Count one copy.
    }  // Close copy loop.

    free(queue->data);  // Free old buffer.
    queue->data = new_data;  // Swap buffer pointer.
    queue->capacity = new_capacity;  // Update capacity.
    queue->total_copies += copied;  // Accumulate total copies.
    *out_copied = copied;  // Output copied count.
    return true;  // Report success.
}  // Close resize().

static bool ensure_capacity_for_one_more(array_queue* queue, int* out_copied) {  // Ensure capacity for one more enqueue.
    if (queue->size < queue->capacity) {  // Fast path: free space exists.
        *out_copied = 0;  // No resize means no copies.
        return true;  // Report success.
    }  // Close fast path.
    return resize(queue, queue->capacity * 2, out_copied);  // Double capacity and return copies.
}  // Close ensure_capacity_for_one_more().

bool array_queue_init(array_queue* queue) {  // Initialize empty queue with capacity 1.
    queue->size = 0;  // Empty queue has size 0.
    queue->capacity = 1;  // Start with capacity 1 for deterministic doubling.
    queue->total_copies = 0;  // Start with zero total copies.
    queue->data = (int*)malloc((size_t)queue->capacity * sizeof(int));  // Allocate backing buffer.
    return queue->data != NULL;  // Report allocation success.
}  // Close array_queue_init().

void array_queue_destroy(array_queue* queue) {  // Free resources held by queue.
    free(queue->data);  // Free backing buffer.
    queue->data = NULL;  // Clear pointer to avoid dangling references.
    queue->size = 0;  // Reset size for safety.
    queue->capacity = 0;  // Reset capacity for safety.
    queue->total_copies = 0;  // Reset totals for safety.
}  // Close array_queue_destroy().

bool array_queue_enqueue(array_queue* queue, int value, queue_cost* out_cost) {  // Enqueue at rear and return copy/shift cost.
    int copied = 0;  // Prepare copied count output.
    if (!ensure_capacity_for_one_more(queue, &copied)) {  // Resize if needed.
        return false;  // Signal allocation failure.
    }  // Close resize branch.
    queue->data[queue->size] = value;  // Write new value at rear slot.
    queue->size += 1;  // Increase size.
    out_cost->copied = copied;  // Store copied count.
    out_cost->moved = 0;  // Enqueue does not shift left.
    return true;  // Report success.
}  // Close array_queue_enqueue().

bool array_queue_peek(const array_queue* queue, int* out_value) {  // Read front value without removing it.
    if (queue->size == 0) {  // Reject peeking an empty queue.
        return false;  // Signal invalid operation.
    }  // Close validation.
    *out_value = queue->data[0];  // Read front slot.
    return true;  // Report success.
}  // Close array_queue_peek().

bool array_queue_dequeue(array_queue* queue, queue_dequeue_result* out_result) {  // Dequeue from front (shifts left).
    if (queue->size == 0) {  // Reject dequeuing an empty queue.
        return false;  // Signal invalid operation.
    }  // Close validation.

    out_result->value = queue->data[0];  // Capture front value.
    int moved = queue->size - 1;  // Shifting left moves (size - 1) elements.
    for (int i = 1; i < queue->size; i++) {  // Shift elements left by one.
        queue->data[i - 1] = queue->data[i];  // Move one element left.
    }  // Close shift loop.
    queue->size -= 1;  // Decrease size after shifting.
    out_result->cost.copied = 0;  // Dequeue does not resize/copy in this unit.
    out_result->cost.moved = moved;  // Store moved count.
    return true;  // Report success.
}  // Close array_queue_dequeue().

bool array_queue_to_array(const array_queue* queue, int* out, int out_len) {  // Copy used portion (front->rear) into out.
    if (out_len < queue->size) {  // Ensure output buffer is large enough.
        return false;  // Signal insufficient output size.
    }  // Close validation.
    for (int i = 0; i < queue->size; i++) {  // Copy used portion.
        out[i] = queue->data[i];  // Copy one element.
    }  // Close copy loop.
    return true;  // Report success.
}  // Close array_queue_to_array().

