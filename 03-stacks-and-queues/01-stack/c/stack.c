// 01 堆疊示範（C）/ Stack demo (C).  // Bilingual file header.

#include "stack.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static bool resize(array_stack* stack, int new_capacity, int* out_copied) {  // Resize buffer and report copied count.
    if (new_capacity < stack->size) {  // Ensure new buffer can hold all elements.
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
    for (int i = 0; i < stack->size; i++) {  // Copy exactly the used portion.
        new_data[i] = stack->data[i];  // Copy one element.
        copied += 1;  // Count one copy.
    }  // Close copy loop.

    free(stack->data);  // Free old buffer.
    stack->data = new_data;  // Swap buffer pointer.
    stack->capacity = new_capacity;  // Update capacity.
    stack->total_copies += copied;  // Accumulate total copies.
    *out_copied = copied;  // Output copied count.
    return true;  // Report success.
}  // Close resize().

static bool ensure_capacity_for_one_more(array_stack* stack, int* out_copied) {  // Ensure capacity for one more push.
    if (stack->size < stack->capacity) {  // Fast path: free space exists.
        *out_copied = 0;  // No resize means no copies.
        return true;  // Report success.
    }  // Close fast path.
    return resize(stack, stack->capacity * 2, out_copied);  // Double capacity and return copies.
}  // Close ensure_capacity_for_one_more().

bool array_stack_init(array_stack* stack) {  // Initialize empty stack with capacity 1.
    stack->size = 0;  // Empty stack has size 0.
    stack->capacity = 1;  // Start with capacity 1 for deterministic doubling.
    stack->total_copies = 0;  // Start with zero total copies.
    stack->data = (int*)malloc((size_t)stack->capacity * sizeof(int));  // Allocate backing buffer.
    return stack->data != NULL;  // Report allocation success.
}  // Close array_stack_init().

void array_stack_destroy(array_stack* stack) {  // Free resources held by stack.
    free(stack->data);  // Free backing buffer.
    stack->data = NULL;  // Clear pointer to avoid dangling references.
    stack->size = 0;  // Reset size for safety.
    stack->capacity = 0;  // Reset capacity for safety.
    stack->total_copies = 0;  // Reset totals for safety.
}  // Close array_stack_destroy().

bool array_stack_push(array_stack* stack, int value, stack_cost* out_cost) {  // Push to top and return resize-copy cost.
    int copied = 0;  // Prepare copied count output.
    if (!ensure_capacity_for_one_more(stack, &copied)) {  // Resize if needed.
        return false;  // Signal allocation failure.
    }  // Close resize branch.
    stack->data[stack->size] = value;  // Write new value at top slot.
    stack->size += 1;  // Increase size.
    out_cost->copied = copied;  // Store copied count in output cost.
    return true;  // Report success.
}  // Close array_stack_push().

bool array_stack_peek(const array_stack* stack, int* out_value) {  // Read top value without removing it.
    if (stack->size == 0) {  // Reject peeking an empty stack.
        return false;  // Signal invalid operation.
    }  // Close validation.
    *out_value = stack->data[stack->size - 1];  // Read top slot.
    return true;  // Report success.
}  // Close array_stack_peek().

bool array_stack_pop(array_stack* stack, stack_pop_result* out_result) {  // Pop top value (no shrinking in this unit).
    if (stack->size == 0) {  // Reject popping an empty stack.
        return false;  // Signal invalid operation.
    }  // Close validation.
    stack->size -= 1;  // Decrease size first so top index becomes size.
    out_result->value = stack->data[stack->size];  // Read removed slot.
    out_result->cost.copied = 0;  // Pop does not resize/copy in this unit.
    return true;  // Report success.
}  // Close array_stack_pop().

bool array_stack_to_array(const array_stack* stack, int* out, int out_len) {  // Copy used portion (bottom->top) into out.
    if (out_len < stack->size) {  // Ensure output buffer is large enough.
        return false;  // Signal insufficient output size.
    }  // Close validation.
    for (int i = 0; i < stack->size; i++) {  // Copy used portion.
        out[i] = stack->data[i];  // Copy one element.
    }  // Close copy loop.
    return true;  // Report success.
}  // Close array_stack_to_array().

