// 03 環狀佇列示範程式（C）/ Circular queue demo program (C).  // Bilingual file header.

#include "circular_queue.h"  // Use the circular queue API for this unit.

#include <stdio.h>  // Provide printf for console output.
#include <stdlib.h>  // Provide atoi for parsing CLI args.

typedef struct enqueue_summary {  // Summarize a sequence of enqueues (growth behavior).
    int m;  // Number of enqueues performed.
    int final_size;  // Final size after enqueues.
    int final_capacity;  // Final capacity after growth.
    long long total_copies;  // Total copied elements due to resizes.
    long long total_actual_cost;  // Total cost if each enqueue costs 1 write + copied.
    int max_copied_in_one_op;  // Maximum copies in any single enqueue.
} enqueue_summary;  // End of enqueue_summary.

static enqueue_summary simulate_enqueues(int m) {  // Simulate m enqueues and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        m = 0;  // Clamp for demo simplicity (tests validate inputs separately).
    }  // Close validation.
    circular_queue q;  // Allocate queue struct on the stack frame.
    if (!circular_queue_init(&q)) {  // Initialize backing buffer.
        fprintf(stderr, "allocation failed\n");  // Print error.
        exit(1);  // Exit demo with failure.
    }  // Close init check.

    long long total_actual = 0;  // Accumulate total cost (1 write + copied).
    int max_copied = 0;  // Track max copied in one enqueue.
    for (int i = 0; i < m; i++) {  // Perform m enqueues.
        queue_cost cost;  // Per-enqueue cost record.
        if (!circular_queue_enqueue(&q, i, &cost)) {  // Enqueue deterministic value.
            fprintf(stderr, "allocation failed\n");  // Print error.
            exit(1);  // Exit demo with failure.
        }  // Close enqueue check.
        total_actual += 1 + cost.copied;  // Add write cost + resize copy cost.
        if (cost.copied > max_copied) {  // Update max copied.
            max_copied = cost.copied;  // Store new max.
        }  // Close max update.
    }  // Close loop.

    enqueue_summary summary = {  // Build summary record.
        .m = m,  // Store operation count.
        .final_size = q.size,  // Store final size.
        .final_capacity = q.capacity,  // Store final capacity.
        .total_copies = q.total_copies,  // Store total copies.
        .total_actual_cost = total_actual,  // Store total actual cost.
        .max_copied_in_one_op = max_copied,  // Store max per-op copied.
    };  // Close summary init.

    circular_queue_destroy(&q);  // Free queue resources.
    return summary;  // Return computed summary.
}  // Close simulate_enqueues().

static int simulate_dequeue_moved_at_size(int n) {  // Build size n queue then dequeue once and return moved.
    if (n <= 0) {  // Dequeue on empty is invalid.
        return 0;  // Return 0 for demo placeholder.
    }  // Close validation.
    circular_queue q;  // Allocate queue struct.
    if (!circular_queue_init(&q)) {  // Initialize queue.
        fprintf(stderr, "allocation failed\n");  // Print error.
        exit(1);  // Exit failure.
    }  // Close init check.
    queue_cost c;  // Reusable cost record for enqueues.
    for (int i = 0; i < n; i++) {  // Enqueue 0..n-1.
        if (!circular_queue_enqueue(&q, i, &c)) {  // Enqueue one value.
            fprintf(stderr, "allocation failed\n");  // Print error.
            exit(1);  // Exit failure.
        }  // Close enqueue check.
    }  // Close loop.
    queue_dequeue_result r;  // Dequeue result record.
    if (!circular_queue_dequeue(&q, &r)) {  // Dequeue once.
        fprintf(stderr, "dequeue failed\n");  // Print error.
        exit(1);  // Exit failure.
    }  // Close dequeue check.
    int moved = r.cost.moved;  // Capture moved count (should be 0).
    circular_queue_destroy(&q);  // Free queue resources.
    return moved;  // Return moved count.
}  // Close simulate_dequeue_moved_at_size().

static void print_enqueue_growth_table(const int* ms, int ms_len) {  // Print enqueue-growth summary table.
    printf("%6s | %6s | %7s | %6s | %6s\n", "m", "cap", "copies", "avg", "maxCp");  // Print header.
    printf("----------------------------------------\n");  // Print separator (fixed width).
    for (int i = 0; i < ms_len; i++) {  // Render one row per m.
        enqueue_summary s = simulate_enqueues(ms[i]);  // Simulate and summarize.
        double avg = (s.m > 0) ? ((double)s.total_actual_cost / (double)s.m) : 0.0;  // Average actual cost per op.
        printf("%6d | %6d | %7lld | %6.2f | %6d\n", s.m, s.final_capacity, s.total_copies, avg, s.max_copied_in_one_op);  // Print row.
    }  // Close loop.
}  // Close print_enqueue_growth_table().

static void print_dequeue_cost_table(const int* ns, int ns_len) {  // Print dequeue moved-cost table (should be 0).
    printf("%6s | %6s\n", "n", "moved");  // Print header.
    printf("-----------------\n");  // Print separator.
    for (int i = 0; i < ns_len; i++) {  // Render one row per n.
        int moved = simulate_dequeue_moved_at_size(ns[i]);  // moved should always be 0 in circular queue.
        printf("%6d | %6d\n", ns[i], moved);  // Print row.
    }  // Close loop.
}  // Close print_dequeue_cost_table().

static void print_fifo_demo(void) {  // Print a small FIFO example with peek/dequeue.
    circular_queue q;  // Allocate queue struct.
    if (!circular_queue_init(&q)) {  // Initialize queue.
        fprintf(stderr, "allocation failed\n");  // Print error.
        exit(1);  // Exit failure.
    }  // Close init check.

    queue_cost c;  // Reusable cost record for enqueues.
    circular_queue_enqueue(&q, 3, &c);  // Enqueue 3.
    circular_queue_enqueue(&q, 7, &c);  // Enqueue 7.
    circular_queue_enqueue(&q, 1, &c);  // Enqueue 1.

    int front = 0;  // Store peeked front value.
    circular_queue_peek(&q, &front);  // Peek current front.
    queue_dequeue_result d1;  // First dequeue result.
    queue_dequeue_result d2;  // Second dequeue result.
    queue_dequeue_result d3;  // Third dequeue result.
    circular_queue_dequeue(&q, &d1);  // Dequeue first (should be 3).
    circular_queue_dequeue(&q, &d2);  // Dequeue second (should be 7).
    circular_queue_dequeue(&q, &d3);  // Dequeue third (should be 1).

    printf("enqueue [3,7,1] => peek=%d, dequeues=[%d,%d,%d]\n", front, d1.value, d2.value, d3.value);  // Print demo line.
    circular_queue_destroy(&q);  // Free queue resources.
}  // Close print_fifo_demo().

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

    int ns[] = {0, 1, 2, 4, 8, 16};  // Fixed n list for dequeue moved costs.
    int ns_len = (int)(sizeof(ns) / sizeof(ns[0]));  // Length of ns list.

    printf("=== Enqueue Growth (m enqueues) ===\n");  // Print section title.
    print_enqueue_growth_table(ms, ms_len);  // Print enqueue growth table.
    printf("\n");  // Blank line.
    printf("=== Dequeue Shift Cost (should be 0) ===\n");  // Print section title.
    print_dequeue_cost_table(ns, ns_len);  // Print dequeue moved table.
    printf("\n");  // Blank line.
    printf("=== FIFO demo ===\n");  // Print section title.
    print_fifo_demo();  // Print FIFO demo line.

    if (ms != defaults) {  // Free allocated m list when CLI args were used.
        free(ms);  // Release memory.
    }  // Close free branch.
    return 0;  // Exit success.
}  // Close main().

