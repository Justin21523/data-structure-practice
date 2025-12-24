/* 02 攤銷分析示範（C）/ Amortized analysis demo (C). */  // Bilingual file header.
#include "amortized_demo.h"  // Include the public API for this unit.

#include <stdlib.h>  // Use malloc/free for manual memory management.

int amortized_potential(int size, int capacity) {  // Compute Φ used by the potential method.
    if (capacity < 1) {  // Reject invalid capacities so callers can detect misuse.
        return -1;  // Return -1 as a simple sentinel for invalid input in C.
    }  // Close validation block.
    return (2 * size) - capacity + 1;  // Use Φ = 2·size - capacity + 1 so Φ(0,1)=0.
}  // End of amortized_potential.

static bool amortized_resize(amortized_table* table, int new_capacity, int* out_copied) {  // Resize backing buffer and count copies.
    if (!table || !out_copied) {  // Validate pointers to avoid undefined behavior.
        return false;  // Signal failure to the caller.
    }  // Close pointer validation.
    if (new_capacity < table->size) {  // Ensure the new buffer can hold all existing elements.
        return false;  // Signal failure (invalid request).
    }  // Close size check.
    if (new_capacity < 1) {  // Keep capacity positive to simplify invariants.
        return false;  // Signal failure (invalid request).
    }  // Close capacity check.

    int* new_data = (int*)malloc((size_t)new_capacity * sizeof(int));  // Allocate a new buffer of the requested capacity.
    if (!new_data) {  // Handle allocation failure gracefully.
        return false;  // Signal allocation failure to the caller.
    }  // Close allocation check.

    int copied = 0;  // Count how many elements we copy into the new buffer.
    for (int i = 0; i < table->size; i++) {  // Copy exactly the stored elements.
        new_data[i] = table->data[i];  // Copy one element (one "move" in our cost model).
        copied += 1;  // Increment copy counter.
    }  // Close copy loop.

    free(table->data);  // Free the old buffer after copying completes.
    table->data = new_data;  // Install the new buffer.
    table->capacity = new_capacity;  // Update capacity to match the new allocation.
    *out_copied = copied;  // Return how many copies were performed.
    return true;  // Signal success to the caller.
}  // End of amortized_resize.

bool amortized_table_init(amortized_table* table) {  // Initialize an empty table with capacity 1.
    if (!table) {  // Validate pointer to avoid undefined behavior.
        return false;  // Signal failure to the caller.
    }  // Close pointer check.

    table->size = 0;  // Start with no elements stored.
    table->capacity = 1;  // Start with one slot to keep the model simple.
    table->data = (int*)malloc((size_t)table->capacity * sizeof(int));  // Allocate initial backing buffer.
    if (!table->data) {  // Handle allocation failure.
        table->capacity = 0;  // Reset capacity so destroy remains safe.
        return false;  // Signal allocation failure to the caller.
    }  // Close allocation check.

    table->bank = 0;  // Start with zero credit for the accounting method.
    table->total_actual_cost = 0;  // Start with zero total cost.
    table->total_copies = 0;  // Start with zero total copies.
    table->max_actual_cost = 0;  // Start max at zero.
    return true;  // Signal successful initialization.
}  // End of amortized_table_init.

void amortized_table_destroy(amortized_table* table) {  // Free resources held by the table.
    if (!table) {  // Allow destroying a null pointer for convenience.
        return;  // Nothing to do.
    }  // Close null check.
    free(table->data);  // Free backing buffer (free(NULL) is safe).
    table->data = NULL;  // Clear pointer to avoid accidental reuse.
    table->size = 0;  // Reset fields to a safe state.
    table->capacity = 0;  // Reset fields to a safe state.
    table->bank = 0;  // Reset bank for completeness.
    table->total_actual_cost = 0;  // Reset totals for completeness.
    table->total_copies = 0;  // Reset totals for completeness.
    table->max_actual_cost = 0;  // Reset max for completeness.
}  // End of amortized_table_destroy.

bool amortized_table_append(amortized_table* table, int value, amortized_step* out_step) {  // Append one element and return a per-step record.
    if (!table || !out_step) {  // Validate pointers to avoid undefined behavior.
        return false;  // Signal failure to the caller.
    }  // Close pointer validation.
    if (table->capacity < 1 || !table->data) {  // Ensure table has been initialized.
        return false;  // Signal failure for invalid table state.
    }  // Close state check.

    int phi_before = amortized_potential(table->size, table->capacity);  // Capture Φ before mutation.
    int size_before = table->size;  // Capture size before insertion.
    int capacity_before = table->capacity;  // Capture capacity before possible resize.

    int copied = 0;  // Default: no resize means no copies.
    int actual_cost = 1;  // Always pay 1 for writing the new element.
    if (table->size == table->capacity) {  // Resize exactly when full (doubling rule).
        int new_capacity = table->capacity * 2;  // Double capacity to achieve amortized O(1).
        if (!amortized_resize(table, new_capacity, &copied)) {  // Resize and count copies.
            return false;  // Signal allocation failure to the caller.
        }  // Close resize failure branch.
        actual_cost += copied;  // Add copy cost to this operation's actual cost.
    }  // Close resize branch.

    table->data[table->size] = value;  // Write the new element into the first free slot.
    table->size += 1;  // Increase logical size after write.

    int phi_after = amortized_potential(table->size, table->capacity);  // Capture Φ after mutation.
    int amortized_cost = actual_cost + (phi_after - phi_before);  // Compute amortized cost via potential method.

    table->bank += 3 - (long long)actual_cost;  // Accounting method: charge 3 and pay actual cost.
    if (table->bank < 0) {  // bank should never be negative for this classic analysis.
        return false;  // Signal failure (invariant violation).
    }  // Close invariant check.

    table->total_actual_cost += (long long)actual_cost;  // Accumulate total actual cost.
    table->total_copies += (long long)copied;  // Accumulate total copies.
    if ((long long)actual_cost > table->max_actual_cost) {  // Update maximum cost if this step is larger.
        table->max_actual_cost = (long long)actual_cost;  // Store new maximum.
    }  // Close max update.

    out_step->index = size_before + 1;  // Store 1-based index (append #k increases size from k-1 to k).
    out_step->size_before = size_before;  // Store size before.
    out_step->capacity_before = capacity_before;  // Store capacity before.
    out_step->copied = copied;  // Store copies.
    out_step->actual_cost = actual_cost;  // Store actual cost.
    out_step->phi_before = phi_before;  // Store Φ before.
    out_step->phi_after = phi_after;  // Store Φ after.
    out_step->amortized_cost = amortized_cost;  // Store amortized cost.
    out_step->bank_after = table->bank;  // Store bank after.
    return true;  // Signal success.
}  // End of amortized_table_append.

bool amortized_simulate_appends(int m, amortized_summary* out_summary) {  // Simulate m appends and return a summary.
    if (!out_summary) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (m < 0) {  // Reject negative operation counts.
        return false;  // Signal invalid input.
    }  // Close validation block.

    amortized_table table;  // Create a local table so we can clean it up deterministically.
    if (!amortized_table_init(&table)) {  // Initialize backing storage.
        return false;  // Signal allocation failure.
    }  // Close init failure branch.

    for (int i = 0; i < m; i++) {  // Perform m appends.
        amortized_step step;  // Temporary step record (not stored, only used to ensure append succeeds).
        if (!amortized_table_append(&table, i, &step)) {  // Append value and track success.
            amortized_table_destroy(&table);  // Clean up on failure.
            return false;  // Signal failure to the caller.
        }  // Close append failure branch.
    }  // Close loop.

    out_summary->m = m;  // Store operation count.
    out_summary->final_size = table.size;  // Store final size.
    out_summary->final_capacity = table.capacity;  // Store final capacity.
    out_summary->total_actual_cost = table.total_actual_cost;  // Store total cost.
    out_summary->total_copies = table.total_copies;  // Store total copies.
    out_summary->max_actual_cost = table.max_actual_cost;  // Store max cost.
    out_summary->final_bank = table.bank;  // Store final bank.

    amortized_table_destroy(&table);  // Free resources before returning.
    return true;  // Signal success.
}  // End of amortized_simulate_appends.
