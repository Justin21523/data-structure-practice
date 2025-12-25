// 03 環狀佇列示範（C）/ Circular queue demo (C).  // Bilingual file header.

#include "circular_queue.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static int index_at(const circular_queue* queue, int offset) {  // Map logical offset [0..size) to physical buffer index.
    return (queue->head + offset) % queue->capacity;  // Wrap around using modulo.
}  // Close index_at().

static bool resize(circular_queue* queue, int new_capacity, int* out_copied) {  // Resize buffer and report copied count.
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
    for (int i = 0; i < queue->size; i++) {  // Copy elements in queue order.
        new_data[i] = queue->data[index_at(queue, i)];  // Copy one element into compact layout.
        copied += 1;  // Count one copy.
    }  // Close copy loop.

    free(queue->data);  // Free old buffer.
    queue->data = new_data;  // Swap buffer pointer.
    queue->capacity = new_capacity;  // Update capacity.
    queue->head = 0;  // Reset head so queue starts at index 0 after resize.
    queue->total_copies += copied;  // Accumulate total copies.
    *out_copied = copied;  // Output copied count.
    return true;  // Report success.
}  // Close resize().

static bool ensure_capacity_for_one_more(circular_queue* queue, int* out_copied) {  // Ensure capacity for one more enqueue.
    if (queue->size < queue->capacity) {  // Fast path: free space exists.
        *out_copied = 0;  // No resize means no copies.
        return true;  // Report success.
    }  // Close fast path.
    return resize(queue, queue->capacity * 2, out_copied);  // Double capacity and return copies.
}  // Close ensure_capacity_for_one_more().

bool circular_queue_init(circular_queue* queue) {  // Initialize empty queue with capacity 1.
    queue->size = 0;  // Empty queue has size 0.
    queue->capacity = 1;  // Start with capacity 1 for deterministic doubling.
    queue->head = 0;  // Head starts at index 0.
    queue->total_copies = 0;  // Start with zero total copies.
    queue->data = (int*)malloc((size_t)queue->capacity * sizeof(int));  // Allocate backing buffer.
    return queue->data != NULL;  // Report allocation success.
}  // Close circular_queue_init().

void circular_queue_destroy(circular_queue* queue) {  // Free resources held by queue.
    free(queue->data);  // Free backing buffer.
    queue->data = NULL;  // Clear pointer to avoid dangling references.
    queue->size = 0;  // Reset size for safety.
    queue->capacity = 0;  // Reset capacity for safety.
    queue->head = 0;  // Reset head for safety.
    queue->total_copies = 0;  // Reset totals for safety.
}  // Close circular_queue_destroy().

bool circular_queue_enqueue(circular_queue* queue, int value, queue_cost* out_cost) {  // Enqueue at rear and return copy/shift cost.
    int copied = 0;  // Prepare copied count output.
    if (!ensure_capacity_for_one_more(queue, &copied)) {  // Resize if needed.
        return false;  // Signal allocation failure.
    }  // Close resize branch.
    int tail_index = index_at(queue, queue->size);  // Compute tail index where new element is written.
    queue->data[tail_index] = value;  // Store value at tail position.
    queue->size += 1;  // Increase size.
    out_cost->copied = copied;  // Store copied count.
    out_cost->moved = 0;  // moved stays 0 (no shifting).
    return true;  // Report success.
}  // Close circular_queue_enqueue().

bool circular_queue_peek(const circular_queue* queue, int* out_value) {  // Read front value without removing it.
    if (queue->size == 0) {  // Reject peeking an empty queue.
        return false;  // Signal invalid operation.
    }  // Close validation.
    *out_value = queue->data[queue->head];  // Read front slot directly.
    return true;  // Report success.
}  // Close circular_queue_peek().

bool circular_queue_dequeue(circular_queue* queue, queue_dequeue_result* out_result) {  // Dequeue from front (no shifting).
    if (queue->size == 0) {  // Reject dequeuing an empty queue.
        return false;  // Signal invalid operation.
    }  // Close validation.

    out_result->value = queue->data[queue->head];  // Capture front value.
    out_result->cost.copied = 0;  // Dequeue does not resize/copy in this unit.
    out_result->cost.moved = 0;  // moved stays 0 (no shift).
    queue->head = (queue->head + 1) % queue->capacity;  // Advance head by one with wrap-around.
    queue->size -= 1;  // Decrease size.
    if (queue->size == 0) {  // Normalize head for deterministic state when queue becomes empty.
        queue->head = 0;  // Reset head (optional but stable).
    }  // Close normalize branch.
    return true;  // Report success.
}  // Close circular_queue_dequeue().

bool circular_queue_to_array(const circular_queue* queue, int* out, int out_len) {  // Copy used portion (front->rear) into out.
    if (out_len < queue->size) {  // Ensure output buffer is large enough.
        return false;  // Signal insufficient output size.
    }  // Close validation.
    for (int i = 0; i < queue->size; i++) {  // Copy elements in queue order.
        out[i] = queue->data[index_at(queue, i)];  // Copy one element.
    }  // Close copy loop.
    return true;  // Report success.
}  // Close circular_queue_to_array().

