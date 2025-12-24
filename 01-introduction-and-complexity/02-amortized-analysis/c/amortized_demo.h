/* 02 攤銷分析示範（C）/ Amortized analysis demo (C). */  // Bilingual header for this unit's public API.
#ifndef AMORTIZED_DEMO_H  // Header guard to prevent multiple inclusion.
#define AMORTIZED_DEMO_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure signals.

/* 位能函數 Φ / Potential function Φ. */  // Bilingual description for the potential method helper.
int amortized_potential(int size, int capacity);  // Return Φ = 2*size - capacity + 1; return -1 if capacity < 1.

/* 一次 append 的成本紀錄 / Per-append cost record. */  // Bilingual struct purpose.
typedef struct amortized_step {  // Bundle all per-operation metrics needed for tests and debugging.
    int index;  // 1-based operation index in the sequence.
    int size_before;  // Size before append.
    int capacity_before;  // Capacity before append.
    int copied;  // Number of elements copied during resize (0 if no resize).
    int actual_cost;  // Actual cost: 1 write + copied moves.
    int phi_before;  // Φ before operation.
    int phi_after;  // Φ after operation.
    int amortized_cost;  // Amortized cost: actual + ΔΦ (should be 3).
    long long bank_after;  // Accounting credit after charging 3 and paying actual cost.
} amortized_step;  // End of per-step record struct.

/* 動態陣列狀態 / Dynamic array state. */  // Bilingual struct purpose.
typedef struct amortized_table {  // Represent a growing array that doubles when full (insert-only).
    int size;  // Number of stored elements.
    int capacity;  // Number of allocated slots.
    int* data;  // Backing buffer (manually resized to count copies deterministically).

    long long bank;  // Accounting-method credit balance.
    long long total_actual_cost;  // Aggregate total actual cost.
    long long total_copies;  // Aggregate total copies during resizes.
    long long max_actual_cost;  // Maximum single-operation actual cost.
} amortized_table;  // End of table struct.

bool amortized_table_init(amortized_table* table);  // Initialize an empty table (capacity=1); returns false on allocation failure.
void amortized_table_destroy(amortized_table* table);  // Free resources held by the table (safe to call on partially-initialized tables).
bool amortized_table_append(amortized_table* table, int value, amortized_step* out_step);  // Append value and write a step record; returns false on allocation failure.

/* 一段操作序列的摘要 / Summary for a sequence of appends. */  // Bilingual struct purpose.
typedef struct amortized_summary {  // Summarize costs for m consecutive appends.
    int m;  // Number of appends performed.
    int final_size;  // Final size after simulation.
    int final_capacity;  // Final capacity after simulation.
    long long total_actual_cost;  // Total actual cost (writes + copies).
    long long total_copies;  // Total copy count.
    long long max_actual_cost;  // Maximum single-operation actual cost.
    long long final_bank;  // Final bank balance (3m - total_actual_cost).
} amortized_summary;  // End of summary struct.

bool amortized_simulate_appends(int m, amortized_summary* out_summary);  // Simulate m appends using a fresh table; returns false on allocation failure.

#endif  // AMORTIZED_DEMO_H  // End of header guard.

