/* 03 環狀佇列示範（C）/ Circular queue demo (C). */  // Bilingual header for this unit's public API.
#ifndef CIRCULAR_QUEUE_H  // Header guard to prevent multiple inclusion.
#define CIRCULAR_QUEUE_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct queue_cost {  // Record deterministic costs for this unit.
    int copied;  // Number of elements copied due to resize (0 if no resize).
    int moved;  // Number of elements shifted due to dequeue (should be 0 here).
} queue_cost;  // End of cost struct.

/* dequeue 結果 / Result of a dequeue operation. */  // Bilingual struct purpose.
typedef struct queue_dequeue_result {  // Return dequeued value plus its operation cost.
    int value;  // The dequeued element value.
    queue_cost cost;  // Copy/shift cost for this dequeue.
} queue_dequeue_result;  // End of dequeue result struct.

/* 環狀佇列 / Circular queue. */  // Bilingual struct purpose.
typedef struct circular_queue {  // Represent a circular queue with doubling capacity (no shift on dequeue).
    int size;  // Number of stored elements.
    int capacity;  // Allocated slots (always >= 1 in this unit).
    int head;  // Head index (front position) within the buffer.
    int* data;  // Backing buffer allocated on the heap.
    long long total_copies;  // Total elements copied due to resizes (for analysis/tests).
} circular_queue;  // End of circular_queue.

bool circular_queue_init(circular_queue* queue);  // Initialize empty queue with capacity 1; returns false on allocation failure.
void circular_queue_destroy(circular_queue* queue);  // Free resources held by queue.

bool circular_queue_enqueue(circular_queue* queue, int value, queue_cost* out_cost);  // Enqueue at rear; grows when full; returns false on allocation failure.
bool circular_queue_peek(const circular_queue* queue, int* out_value);  // Read front value; returns false on empty queue.
bool circular_queue_dequeue(circular_queue* queue, queue_dequeue_result* out_result);  // Dequeue from front; no shifting; returns false on empty queue.

bool circular_queue_to_array(const circular_queue* queue, int* out, int out_len);  // Copy used portion (front->rear) into out; returns false if out_len < size.

#endif  // CIRCULAR_QUEUE_H  // End of header guard.

