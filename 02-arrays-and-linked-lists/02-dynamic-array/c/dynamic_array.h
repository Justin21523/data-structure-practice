/* 02 動態陣列示範（C）/ Dynamic array demo (C). */  // Bilingual header for this unit's public API.
#ifndef DYNAMIC_ARRAY_H  // Header guard to prevent multiple inclusion.
#define DYNAMIC_ARRAY_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* 每次操作的成本 / Per-operation cost. */  // Bilingual struct purpose.
typedef struct dynamic_array_cost {  // Record deterministic costs for this unit.
    int copied;  // Number of elements copied due to resize (0 if no resize).
    int moved;  // Number of elements shifted due to insert/remove (0 for append).
} dynamic_array_cost;  // End of cost struct.

/* remove 結果 / Result of a removeAt operation. */  // Bilingual struct purpose.
typedef struct dynamic_array_remove_result {  // Return removed value plus its operation cost.
    int value;  // The removed element value.
    dynamic_array_cost cost;  // Copy/shift cost for this removal.
} dynamic_array_remove_result;  // End of remove result struct.

/* 動態陣列 / Dynamic array. */  // Bilingual struct purpose.
typedef struct dynamic_array {  // Represent a growable array with doubling capacity.
    int size;  // Number of stored elements.
    int capacity;  // Allocated slots (always >= 1 in this unit).
    int* data;  // Backing buffer allocated on the heap.
    long long total_copies;  // Total elements copied due to resizes (for analysis/tests).
} dynamic_array;  // End of dynamic_array.

bool dynamic_array_init(dynamic_array* array);  // Initialize empty array with capacity 1; returns false on allocation failure.
void dynamic_array_destroy(dynamic_array* array);  // Free resources held by array.

bool dynamic_array_get(const dynamic_array* array, int index, int* out_value);  // Read element at index; returns false on invalid index.
bool dynamic_array_set(dynamic_array* array, int index, int value);  // Write element at index; returns false on invalid index.

bool dynamic_array_append(dynamic_array* array, int value, dynamic_array_cost* out_cost);  // Append at end; grows when full; returns false on allocation failure.
bool dynamic_array_insert_at(dynamic_array* array, int index, int value, dynamic_array_cost* out_cost);  // Insert at index; grows when full; returns false on invalid index or allocation failure.
bool dynamic_array_remove_at(dynamic_array* array, int index, dynamic_array_remove_result* out_result);  // Remove at index; returns false on invalid index.

int dynamic_array_index_of(const dynamic_array* array, int value);  // Return first index of value or -1 if not found.

#endif  // DYNAMIC_ARRAY_H  // End of header guard.

