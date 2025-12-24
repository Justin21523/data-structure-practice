/* 05 環狀鏈結串列示範 CLI（C）/ Circular linked list demo CLI (C). */  // Bilingual file header for the executable entry point.
#include "circular_linked_list.h"  // Import list helpers for simulation.

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

static bool build_ordered_list(int n, circular_linked_list* out_list) {  // Build list containing [0..n-1] in order.
    if (!out_list) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0) {  // Reject invalid sizes.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (!circular_linked_list_init(out_list)) {  // Initialize list.
        return false;  // Signal failure.
    }  // Close init failure.

    for (int v = 0; v < n; v++) {  // Append in increasing order.
        cll_cost cost;  // Per-op cost (ignored for building).
        if (!circular_linked_list_push_back(out_list, v, &cost)) {  // Append one value.
            circular_linked_list_destroy(out_list);  // Clean up on allocation failure.
            return false;  // Signal failure.
        }  // Close failure check.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_ordered_list.

static void print_basic_cost_table(const int* ns, size_t ns_count) {  // Print push/pop costs table.
    printf("=== basic costs (push/pop) ===\n");  // Print section title.
    printf("%6s | %6s | %6s | %6s | %6s\n", "n", "pushB", "pushF", "popF", "popB");  // Print header.
    printf("-------------------------------------------\n");  // Print separator.
    for (size_t i = 0; i < ns_count; i++) {  // Print one row per n.
        int n = ns[i];  // Read n.

        circular_linked_list a;  // List for pushBack.
        if (!build_ordered_list(n, &a)) {  // Build list.
            fprintf(stderr, "Build failed for n=%d\n", n);  // Report error.
            return;  // Stop printing.
        }  // Close failure check.
        cll_cost push_b;  // Cost record.
        circular_linked_list_push_back(&a, 999, &push_b);  // Measure pushBack cost (0).
        circular_linked_list_destroy(&a);  // Clean up.

        circular_linked_list b;  // List for pushFront.
        if (!build_ordered_list(n, &b)) {  // Build list.
            fprintf(stderr, "Build failed for n=%d\n", n);  // Report error.
            return;  // Stop printing.
        }  // Close failure check.
        cll_cost push_f;  // Cost record.
        circular_linked_list_push_front(&b, 999, &push_f);  // Measure pushFront cost (0).
        circular_linked_list_destroy(&b);  // Clean up.

        circular_linked_list c;  // List for popFront.
        if (!build_ordered_list(n, &c)) {  // Build list.
            fprintf(stderr, "Build failed for n=%d\n", n);  // Report error.
            return;  // Stop printing.
        }  // Close failure check.
        int pop_f_hops = 0;  // popFront hops (0; handle n=0).
        if (c.size > 0) {  // Only pop when list is non-empty.
            cll_remove_result rr;  // Remove result.
            circular_linked_list_pop_front(&c, &rr);  // Pop head.
            pop_f_hops = rr.cost.hops;  // Read cost.
        }  // Close branch.
        circular_linked_list_destroy(&c);  // Clean up.

        circular_linked_list d;  // List for popBack.
        if (!build_ordered_list(n, &d)) {  // Build list.
            fprintf(stderr, "Build failed for n=%d\n", n);  // Report error.
            return;  // Stop printing.
        }  // Close failure check.
        int pop_b_hops = 0;  // popBack hops (n-2; handle n=0).
        if (d.size > 0) {  // Only pop when list is non-empty.
            cll_remove_result rr;  // Remove result.
            circular_linked_list_pop_back(&d, &rr);  // Pop tail.
            pop_b_hops = rr.cost.hops;  // Read cost.
        }  // Close branch.
        circular_linked_list_destroy(&d);  // Clean up.

        printf("%6d | %6d | %6d | %6d | %6d\n", n, push_b.hops, push_f.hops, pop_f_hops, pop_b_hops);  // Print row.
    }  // Close loop.
}  // End print_basic_cost_table.

static void print_rotate_demo(void) {  // Print a small rotate demo on n=5.
    printf("\n");  // Blank line.
    printf("=== rotate demo on n=5 ===\n");  // Print section title.
    printf("%6s | %6s | %6s | %20s\n", "steps", "hops", "head", "list");  // Print header.
    printf("-------------------------------------------\n");  // Print separator.

    circular_linked_list lst;  // Demo list.
    if (!build_ordered_list(5, &lst)) {  // Build [0..4].
        fprintf(stderr, "Build failed for n=5\n");  // Report error.
        return;  // Stop demo.
    }  // Close failure check.

    int steps_list[] = {0, 1, 2, 5, 7};  // Steps list (cumulative).
    for (size_t i = 0; i < sizeof(steps_list) / sizeof(steps_list[0]); i++) {  // Iterate steps.
        int steps = steps_list[i];  // Read steps.
        cll_cost cost;  // Cost record.
        if (!circular_linked_list_rotate(&lst, steps, &cost)) {  // Rotate list.
            fprintf(stderr, "rotate failed for steps=%d\n", steps);  // Report error.
            break;  // Stop demo.
        }  // Close failure check.
        int buffer[64];  // Output buffer for snapshot.
        circular_linked_list_to_array(&lst, buffer, 64);  // Copy list to array.
        int head_value = (lst.size > 0) ? buffer[0] : -1;  // Read head value.
        printf("%6d | %6d | %6d | %20s\n", steps, cost.hops, head_value, "");  // Print prefix (list printed below).
        printf("                     %s", "[");  // Print opening bracket aligned.
        for (int j = 0; j < lst.size; j++) {  // Print array values.
            printf("%d%s", buffer[j], (j + 1 == lst.size) ? "" : ", ");  // Print one value.
        }  // Close loop.
        printf("]\n");  // Print closing bracket.
    }  // Close loop.
    circular_linked_list_destroy(&lst);  // Clean up.
}  // End print_rotate_demo.

int main(int argc, char** argv) {  // CLI entry point that prints demo tables.
    const int default_ns[] = {0, 1, 2, 4, 8};  // Default n values include boundary case.
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

    print_basic_cost_table(ns, ns_count);  // Print basic cost table.
    print_rotate_demo();  // Print rotate demo.
    return 0;  // Indicate success.
}  // End main.

