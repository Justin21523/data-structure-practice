/* 04 雙向鏈結串列示範 CLI（C）/ Doubly linked list demo CLI (C). */  // Bilingual file header for the executable entry point.
#include "doubly_linked_list.h"  // Import list helpers for simulation.

#include <errno.h>  // Use errno to validate numeric parsing via strtol.
#include <limits.h>  // Use INT_MIN/INT_MAX to validate parsed values fit into int.
#include <stdio.h>  // Use printf/fprintf for CLI output.
#include <stdlib.h>  // Use strtol for CLI parsing and exit codes.

static bool parse_int(const char* s, int* out_value) {  // Parse an int from a string with basic validation.
    if (!s || !out_value) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal failure.
    }  // Close pointer check.

    errno = 0;  // Reset errno so we can detect conversion errors reliably.
    char* end = NULL;  // strtol will set this to the first non-parsed character.
    long value = strtol(s, &end, 10);  // Convert string to long.
    if (errno != 0) {  // Detect overflow/underflow.
        return false;  // Signal failure.
    }  // Close errno check.
    if (end == s || *end != '\0') {  // Reject empty strings and trailing garbage.
        return false;  // Signal failure.
    }  // Close format check.
    if (value < INT_MIN || value > INT_MAX) {  // Ensure value fits into int.
        return false;  // Signal failure.
    }  // Close range check.

    *out_value = (int)value;  // Store parsed integer.
    return true;  // Signal success.
}  // End parse_int.

static void print_usage(const char* program_name) {  // Print a short usage message.
    fprintf(stderr, "Usage: %s [n1 n2 ...] (each n must be >= 0)\n", program_name);  // Explain expected arguments.
}  // End print_usage.

static bool build_ordered_list(int n, doubly_linked_list* out_list) {  // Build list containing [0..n-1] in order.
    if (!out_list) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0) {  // Reject invalid sizes.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (!doubly_linked_list_init(out_list)) {  // Initialize list.
        return false;  // Signal failure.
    }  // Close init failure.

    for (int v = 0; v < n; v++) {  // Append in increasing order (push_back is O(1) with tail).
        dll_cost cost;  // Per-op cost (ignored for building).
        if (!doubly_linked_list_push_back(out_list, v, &cost)) {  // Append one value.
            doubly_linked_list_destroy(out_list);  // Clean up on allocation failure.
            return false;  // Signal failure.
        }  // Close failure check.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_ordered_list.

static void print_push_pop_table(const int* ns, size_t ns_count) {  // Print push/pop costs table.
    printf("=== push/pop hops at both ends ===\n");  // Print section title.
    printf("%6s | %6s | %6s | %6s | %6s\n", "n", "pushB", "popB", "pushF", "popF");  // Print header.
    printf("-------------------------------------------------\n");  // Print separator.
    for (size_t i = 0; i < ns_count; i++) {  // Print one row per n.
        int n = ns[i];  // Read n.

        doubly_linked_list a;  // List for back operations.
        if (!build_ordered_list(n, &a)) {  // Build list.
            fprintf(stderr, "Build failed for n=%d\n", n);  // Report error.
            return;  // Stop printing on error.
        }  // Close failure check.
        dll_cost push_back_cost;  // Cost record.
        doubly_linked_list_push_back(&a, 999, &push_back_cost);  // Measure pushBack hops (should be 0).
        int pop_back_hops = 0;  // popBack hops (0 for n=0 because we skip pop).
        if (a.size > 0) {  // Only pop when list is non-empty.
            dll_remove_result rr;  // Remove result.
            doubly_linked_list_pop_back(&a, &rr);  // Pop tail.
            pop_back_hops = rr.cost.hops;  // Read cost.
        }  // Close branch.
        doubly_linked_list_destroy(&a);  // Clean up.

        doubly_linked_list b;  // List for front operations.
        if (!build_ordered_list(n, &b)) {  // Build list.
            fprintf(stderr, "Build failed for n=%d\n", n);  // Report error.
            return;  // Stop printing on error.
        }  // Close failure check.
        dll_cost push_front_cost;  // Cost record.
        doubly_linked_list_push_front(&b, 999, &push_front_cost);  // Measure pushFront hops (should be 0).
        int pop_front_hops = 0;  // popFront hops (0 for n=0 because we skip pop).
        if (b.size > 0) {  // Only pop when list is non-empty.
            dll_remove_result rr;  // Remove result.
            doubly_linked_list_pop_front(&b, &rr);  // Pop head.
            pop_front_hops = rr.cost.hops;  // Read cost.
        }  // Close branch.
        doubly_linked_list_destroy(&b);  // Clean up.

        printf("%6d | %6d | %6d | %6d | %6d\n", n, push_back_cost.hops, pop_back_hops, push_front_cost.hops, pop_front_hops);  // Print row.
    }  // Close loop.
}  // End print_push_pop_table.

static void print_get_hops_table(void) {  // Print get(i) hops table for a fixed list size.
    printf("\n");  // Blank line.
    printf("=== get(i) hops on list of size 16 ===\n");  // Print section title.
    printf("%6s | %6s | %6s\n", "i", "value", "hops");  // Print header.
    printf("------------------------------\n");  // Print separator.

    doubly_linked_list lst;  // List for get demonstration.
    if (!build_ordered_list(16, &lst)) {  // Build list of size 16.
        fprintf(stderr, "Build failed for n=16\n");  // Report error.
        return;  // Stop printing on error.
    }  // Close failure check.
    const int indices[] = {0, 1, 2, 4, 8, 15};  // Fixed indices for readability.
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {  // Iterate indices.
        int idx = indices[i];  // Read index.
        dll_get_result r;  // Result record.
        if (!doubly_linked_list_get(&lst, idx, &r)) {  // Fetch value and cost.
            fprintf(stderr, "get failed for i=%d\n", idx);  // Report error.
            break;  // Stop printing rows.
        }  // Close failure check.
        printf("%6d | %6d | %6d\n", idx, r.value, r.cost.hops);  // Print row.
    }  // Close loop.
    doubly_linked_list_destroy(&lst);  // Clean up.
}  // End print_get_hops_table.

int main(int argc, char** argv) {  // CLI entry point that prints demo tables.
    const int default_ns[] = {0, 1, 2, 4, 8, 16};  // Default n values include boundary case.
    int ns[64];  // Fixed buffer for parsed n values.
    size_t ns_count = 0;  // Track how many n values we will print.

    if (argc <= 1) {  // Use defaults when no args are provided.
        for (size_t i = 0; i < sizeof(default_ns) / sizeof(default_ns[0]); i++) {  // Copy defaults.
            ns[ns_count++] = default_ns[i];  // Store one n.
        }  // Close loop.
    } else {  // Parse user-provided n values.
        for (int i = 1; i < argc; i++) {  // Parse each token.
            int value = 0;  // Parsed integer.
            if (!parse_int(argv[i], &value) || value < 0) {  // Validate and require n >= 0.
                print_usage(argv[0]);  // Print usage.
                return 2;  // Invalid usage.
            }  // Close validation.
            if (ns_count < sizeof(ns) / sizeof(ns[0])) {  // Avoid overflow.
                ns[ns_count++] = value;  // Store value.
            }  // Close capacity check.
        }  // Close parse loop.
    }  // Close branch.

    print_push_pop_table(ns, ns_count);  // Print push/pop table.
    print_get_hops_table();  // Print get hops table.
    return 0;  // Indicate success.
}  // End main.

