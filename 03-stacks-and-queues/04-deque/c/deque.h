/* 04 雙端佇列示範（C）/ Deque demo (C). */  // Bilingual header for this unit's public API.
#ifndef DEQUE_H  // Header guard to prevent multiple inclusion.
#define DEQUE_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct deque_cost {  // Record deterministic costs for this unit.
    int copied;  // Number of elements copied due to resize (0 if no resize).
    int moved;  // Number of elements shifted due to an operation (should be 0 here).
} deque_cost;  // End of cost struct.

/* pop 結果 / Result of a pop operation. */  // Bilingual struct purpose.
typedef struct deque_pop_result {  // Return popped value plus its operation cost.
    int value;  // The popped element value.
    deque_cost cost;  // Copy/shift cost for this pop.
} deque_pop_result;  // End of pop result struct.

/* 雙端佇列（環狀陣列實作）/ Circular-buffer deque. */  // Bilingual struct purpose.
typedef struct circular_deque {  // Represent a deque with doubling capacity (no shifting).
    int size;  // Number of stored elements.
    int capacity;  // Allocated slots (always >= 1 in this unit).
    int head;  // Head index (front position) within the buffer.
    int* data;  // Backing buffer allocated on the heap.
    long long total_copies;  // Total elements copied due to resizes (for analysis/tests).
} circular_deque;  // End of circular_deque.

bool circular_deque_init(circular_deque* deque);  // Initialize empty deque with capacity 1; returns false on allocation failure.
void circular_deque_destroy(circular_deque* deque);  // Free resources held by deque.

bool circular_deque_push_back(circular_deque* deque, int value, deque_cost* out_cost);  // Push value at back; grows when full; returns false on allocation failure.
bool circular_deque_push_front(circular_deque* deque, int value, deque_cost* out_cost);  // Push value at front; grows when full; returns false on allocation failure.
bool circular_deque_peek_front(const circular_deque* deque, int* out_value);  // Read front value; returns false on empty deque.
bool circular_deque_peek_back(const circular_deque* deque, int* out_value);  // Read back value; returns false on empty deque.
bool circular_deque_pop_front(circular_deque* deque, deque_pop_result* out_result);  // Pop value from front; returns false on empty deque.
bool circular_deque_pop_back(circular_deque* deque, deque_pop_result* out_result);  // Pop value from back; returns false on empty deque.

bool circular_deque_to_array(const circular_deque* deque, int* out, int out_len);  // Copy used portion (front->back) into out; returns false if out_len < size.

#endif  // DEQUE_H  // End of header guard.

