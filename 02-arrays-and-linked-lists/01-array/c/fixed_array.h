/* 01 靜態陣列示範（C）/ Static array demo (C). */  // Bilingual header for this unit's public API.
#ifndef FIXED_ARRAY_H  // Header guard to prevent multiple inclusion.
#define FIXED_ARRAY_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 固定容量陣列 / Fixed-capacity array. */  // Bilingual struct purpose.
typedef struct fixed_array {  // Represent a static array with explicit size and capacity.
    int size;  // Number of elements currently stored (valid range: 0..capacity).
    int capacity;  // Fixed capacity of the backing buffer.
    int* data;  // Backing buffer allocated once during init (conceptually contiguous).
} fixed_array;  // End of fixed_array.

/* remove 結果 / Result of a removeAt operation. */  // Bilingual struct purpose.
typedef struct fixed_array_remove_result {  // Return both removed value and moved elements count.
    int value;  // The removed element.
    int moved;  // How many elements were shifted left after removal.
} fixed_array_remove_result;  // End of remove result struct.

bool fixed_array_init(fixed_array* array, int capacity);  // Initialize array with fixed capacity; returns false on invalid input or allocation failure.
void fixed_array_destroy(fixed_array* array);  // Free resources held by array (safe to call on partially initialized array).

bool fixed_array_get(const fixed_array* array, int index, int* out_value);  // Read element at index; returns false on invalid index.
bool fixed_array_set(fixed_array* array, int index, int value);  // Write element at index; returns false on invalid index.

bool fixed_array_append(fixed_array* array, int value, int* out_moved);  // Append at end; returns false if full; moved will be 0 on success.
bool fixed_array_insert_at(fixed_array* array, int index, int value, int* out_moved);  // Insert at index; returns false on invalid index or full; moved = size-index.
bool fixed_array_remove_at(fixed_array* array, int index, fixed_array_remove_result* out_result);  // Remove at index; returns false on invalid index; moved = size-index-1.

int fixed_array_index_of(const fixed_array* array, int value);  // Return first index of value or -1 if not found.

#endif  // FIXED_ARRAY_H  // End of header guard.

