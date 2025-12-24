/* 02 佇列示範（C）/ Queue demo (C). */  // Bilingual header for this unit's public API.
#ifndef QUEUE_H  // Header guard to prevent multiple inclusion.
#define QUEUE_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct queue_cost {  // Record deterministic costs for this unit.
    int copied;  // Number of elements copied due to resize (0 if no resize).
    int moved;  // Number of elements shifted left due to dequeue (0 for enqueue).
} queue_cost;  // End of cost struct.

/* dequeue 結果 / Result of a dequeue operation. */  // Bilingual struct purpose.
typedef struct queue_dequeue_result {  // Return dequeued value plus its operation cost.
    int value;  // The dequeued element value.
    queue_cost cost;  // Copy/shift cost for this dequeue.
} queue_dequeue_result;  // End of dequeue result struct.

/* 陣列佇列 / Array-backed queue. */  // Bilingual struct purpose.
typedef struct array_queue {  // Represent a naive queue with doubling capacity and shift-on-dequeue.
    int size;  // Number of stored elements.
    int capacity;  // Allocated slots (always >= 1 in this unit).
    int* data;  // Backing buffer allocated on the heap.
    long long total_copies;  // Total elements copied due to resizes (for analysis/tests).
} array_queue;  // End of array_queue.

bool array_queue_init(array_queue* queue);  // Initialize empty queue with capacity 1; returns false on allocation failure.
void array_queue_destroy(array_queue* queue);  // Free resources held by queue.

bool array_queue_enqueue(array_queue* queue, int value, queue_cost* out_cost);  // Enqueue at rear; grows when full; returns false on allocation failure.
bool array_queue_peek(const array_queue* queue, int* out_value);  // Read front value; returns false on empty queue.
bool array_queue_dequeue(array_queue* queue, queue_dequeue_result* out_result);  // Dequeue from front; shifts left; returns false on empty queue.

bool array_queue_to_array(const array_queue* queue, int* out, int out_len);  // Copy used portion (front->rear) into out; returns false if out_len < size.

#endif  // QUEUE_H  // End of header guard.

