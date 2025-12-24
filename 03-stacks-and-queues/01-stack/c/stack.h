/* 01 堆疊示範（C）/ Stack demo (C). */  // Bilingual header for this unit's public API.
#ifndef STACK_H  // Header guard to prevent multiple inclusion.
#define STACK_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct stack_cost {  // Record deterministic costs for this unit.
    int copied;  // Number of elements copied due to resize (0 if no resize).
} stack_cost;  // End of cost struct.

/* pop 結果 / Result of a pop operation. */  // Bilingual struct purpose.
typedef struct stack_pop_result {  // Return popped value plus its operation cost.
    int value;  // The popped element value.
    stack_cost cost;  // Resize-copy cost for this pop (always 0 in this unit).
} stack_pop_result;  // End of pop result struct.

/* 陣列堆疊 / Array-backed stack. */  // Bilingual struct purpose.
typedef struct array_stack {  // Represent a growable stack with doubling capacity.
    int size;  // Number of stored elements.
    int capacity;  // Allocated slots (always >= 1 in this unit).
    int* data;  // Backing buffer allocated on the heap.
    long long total_copies;  // Total elements copied due to resizes (for analysis/tests).
} array_stack;  // End of array_stack.

bool array_stack_init(array_stack* stack);  // Initialize empty stack with capacity 1; returns false on allocation failure.
void array_stack_destroy(array_stack* stack);  // Free resources held by stack.

bool array_stack_push(array_stack* stack, int value, stack_cost* out_cost);  // Push to top; grows when full; returns false on allocation failure.
bool array_stack_peek(const array_stack* stack, int* out_value);  // Read top value; returns false on empty stack.
bool array_stack_pop(array_stack* stack, stack_pop_result* out_result);  // Pop top value; returns false on empty stack.

bool array_stack_to_array(const array_stack* stack, int* out, int out_len);  // Copy used portion (bottom->top) into out; returns false if out_len < size.

#endif  // STACK_H  // End of header guard.

