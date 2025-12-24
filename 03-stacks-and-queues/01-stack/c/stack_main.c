// 01 堆疊示範程式（C）/ Stack demo program (C).  // Bilingual file header.

#include "stack.h"  // Use the stack API for this unit.

#include <stdio.h>  // Provide printf for console output.
#include <stdlib.h>  // Provide atoi for parsing CLI args.

typedef struct push_summary {  // Summarize a sequence of pushes (growth behavior).
    int m;  // Number of pushes performed.
    int final_size;  // Final size after pushes.
    int final_capacity;  // Final capacity after growth.
    long long total_copies;  // Total copied elements due to resizes.
    long long total_actual_cost;  // Total cost if each push costs 1 write + copied.
    int max_copied_in_one_op;  // Maximum copies in any single push.
} push_summary;  // End of push_summary.

static push_summary simulate_pushes(int m) {  // Simulate m pushes and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        m = 0;  // Clamp for demo simplicity (tests validate inputs separately).
    }  // Close validation.
    array_stack s;  // Allocate stack struct on the stack frame.
    if (!array_stack_init(&s)) {  // Initialize backing buffer.
        fprintf(stderr, "allocation failed\n");  // Print error.
        exit(1);  // Exit demo with failure.
    }  // Close init check.

    long long total_actual = 0;  // Accumulate total cost (1 write + copied).
    int max_copied = 0;  // Track max copied in one push.
    for (int i = 0; i < m; i++) {  // Perform m pushes.
        stack_cost cost;  // Per-push cost record.
        if (!array_stack_push(&s, i, &cost)) {  // Push deterministic value.
            fprintf(stderr, "allocation failed\n");  // Print error.
            exit(1);  // Exit demo with failure.
        }  // Close push check.
        total_actual += 1 + cost.copied;  // Add write cost + resize copy cost.
        if (cost.copied > max_copied) {  // Update max copied.
            max_copied = cost.copied;  // Store new max.
        }  // Close max update.
    }  // Close loop.

    push_summary summary = {  // Build summary record.
        .m = m,  // Store operation count.
        .final_size = s.size,  // Store final size.
        .final_capacity = s.capacity,  // Store final capacity.
        .total_copies = s.total_copies,  // Store total copies.
        .total_actual_cost = total_actual,  // Store total actual cost.
        .max_copied_in_one_op = max_copied,  // Store max per-op copied.
    };  // Close summary init.

    array_stack_destroy(&s);  // Free stack resources.
    return summary;  // Return computed summary.
}  // Close simulate_pushes().

static void print_push_growth_table(const int* ms, int ms_len) {  // Print push-growth summary table.
    printf("%6s | %6s | %7s | %6s | %6s\n", "m", "cap", "copies", "avg", "maxCp");  // Print header.
    printf("----------------------------------------\n");  // Print separator (fixed width for readability).
    for (int i = 0; i < ms_len; i++) {  // Render one row per m.
        push_summary s = simulate_pushes(ms[i]);  // Simulate and summarize.
        double avg = (s.m > 0) ? ((double)s.total_actual_cost / (double)s.m) : 0.0;  // Average actual cost per op.
        printf("%6d | %6d | %7lld | %6.2f | %6d\n", s.m, s.final_capacity, s.total_copies, avg, s.max_copied_in_one_op);  // Print row.
    }  // Close loop.
}  // Close print_push_growth_table().

static void print_lifo_demo(void) {  // Print a small LIFO example with peek/pop.
    array_stack s;  // Allocate stack struct.
    if (!array_stack_init(&s)) {  // Initialize stack.
        fprintf(stderr, "allocation failed\n");  // Print error.
        exit(1);  // Exit failure.
    }  // Close init check.

    stack_cost c;  // Reusable cost record for pushes.
    array_stack_push(&s, 3, &c);  // Push 3.
    array_stack_push(&s, 7, &c);  // Push 7.
    array_stack_push(&s, 1, &c);  // Push 1 (becomes top).

    int top = 0;  // Store peeked top value.
    array_stack_peek(&s, &top);  // Peek current top.
    stack_pop_result p1;  // First pop result.
    stack_pop_result p2;  // Second pop result.
    stack_pop_result p3;  // Third pop result.
    array_stack_pop(&s, &p1);  // Pop first (should be 1).
    array_stack_pop(&s, &p2);  // Pop second (should be 7).
    array_stack_pop(&s, &p3);  // Pop third (should be 3).

    printf("push [3,7,1] => peek=%d, pops=[%d,%d,%d]\n", top, p1.value, p2.value, p3.value);  // Print demo line.
    array_stack_destroy(&s);  // Free stack resources.
}  // Close print_lifo_demo().

int main(int argc, char** argv) {  // Entry point for demo CLI.
    int defaults[] = {0, 1, 2, 4, 8, 16, 32};  // Default m values.
    int* ms = defaults;  // Use defaults by default.
    int ms_len = (int)(sizeof(defaults) / sizeof(defaults[0]));  // Default length.

    if (argc > 1) {  // Use CLI args as m values when provided.
        ms_len = argc - 1;  // Number of provided m values.
        ms = (int*)malloc((size_t)ms_len * sizeof(int));  // Allocate m list.
        if (ms == NULL) {  // Handle allocation failure.
            fprintf(stderr, "allocation failed\n");  // Print error.
            return 1;  // Exit failure.
        }  // Close allocation check.
        for (int i = 0; i < ms_len; i++) {  // Parse each CLI token.
            ms[i] = atoi(argv[i + 1]);  // Convert to int (demo-only parsing).
        }  // Close parse loop.
    }  // Close CLI override.

    printf("=== Push Growth (m pushes) ===\n");  // Print section title.
    print_push_growth_table(ms, ms_len);  // Print growth table.
    printf("\n");  // Blank line.
    printf("=== LIFO demo ===\n");  // Print section title.
    print_lifo_demo();  // Print LIFO demo line.

    if (ms != defaults) {  // Free allocated m list when CLI args were used.
        free(ms);  // Release memory.
    }  // Close free branch.
    return 0;  // Exit success.
}  // Close main().

