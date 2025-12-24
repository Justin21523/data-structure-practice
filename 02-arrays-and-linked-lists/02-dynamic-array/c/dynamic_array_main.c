/* 02 動態陣列示範 CLI（C）/ Dynamic array demo CLI (C). */  // Bilingual file header for the executable entry point.
#include "dynamic_array.h"  // Import dynamic array helpers for simulation.

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
    fprintf(stderr, "Usage: %s [m1 m2 ...] (each m must be >= 0)\n", program_name);  // Explain expected arguments.
}  // End print_usage.

typedef struct append_summary {  // Summarize a sequence of appends (growth behavior).
    int m;  // Number of appends performed.
    int final_size;  // Final size after appends.
    int final_capacity;  // Final capacity after growth.
    long long total_copies;  // Total copies due to resizes.
    long long total_actual_cost;  // Total cost (1 write + copies per append).
    int max_copied_in_one_op;  // Maximum copies in any single append.
} append_summary;  // End summary struct.

static bool simulate_appends(int m, append_summary* out_summary) {  // Simulate m appends and fill summary.
    if (!out_summary) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (m < 0) {  // Reject invalid negative counts.
        return false;  // Signal invalid input.
    }  // Close validation.

    dynamic_array a;  // Fresh array for simulation.
    if (!dynamic_array_init(&a)) {  // Initialize array.
        return false;  // Signal allocation failure.
    }  // Close init failure.

    long long total_actual = 0;  // Accumulate total actual cost.
    int max_copied = 0;  // Track max copied per append.
    for (int i = 0; i < m; i++) {  // Perform m appends.
        dynamic_array_cost cost;  // Per-append cost record.
        if (!dynamic_array_append(&a, i, &cost)) {  // Append deterministic value.
            dynamic_array_destroy(&a);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close append failure.
        total_actual += 1 + (long long)cost.copied;  // Add write cost + copy cost.
        if (cost.copied > max_copied) {  // Update max copied if needed.
            max_copied = cost.copied;  // Store new max.
        }  // Close update branch.
    }  // Close loop.

    out_summary->m = m;  // Store m.
    out_summary->final_size = a.size;  // Store final size.
    out_summary->final_capacity = a.capacity;  // Store final capacity.
    out_summary->total_copies = a.total_copies;  // Store total copies.
    out_summary->total_actual_cost = total_actual;  // Store total actual cost.
    out_summary->max_copied_in_one_op = max_copied;  // Store max copied.

    dynamic_array_destroy(&a);  // Clean up resources.
    return true;  // Signal success.
}  // End simulate_appends.

static bool build_filled_array(int n, dynamic_array* out_array) {  // Build dynamic array filled with 0..n-1.
    if (!out_array) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0) {  // Reject invalid sizes.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (!dynamic_array_init(out_array)) {  // Initialize array.
        return false;  // Signal allocation failure.
    }  // Close init failure.
    for (int i = 0; i < n; i++) {  // Fill with deterministic values.
        dynamic_array_cost cost;  // Per-append cost (ignored for fill).
        if (!dynamic_array_append(out_array, i, &cost)) {  // Append one value.
            dynamic_array_destroy(out_array);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close append failure.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_filled_array.

static bool simulate_append_cost_at_size(int n, dynamic_array_cost* out_cost) {  // Build size n then append once and return cost.
    if (!out_cost) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    dynamic_array a;  // Local array.
    if (!build_filled_array(n, &a)) {  // Build filled array.
        return false;  // Signal failure.
    }  // Close build check.
    bool ok = dynamic_array_append(&a, 999, out_cost);  // Append sentinel and capture cost.
    dynamic_array_destroy(&a);  // Clean up.
    return ok;  // Return status.
}  // End simulate_append_cost_at_size.

static bool simulate_insert0_cost_at_size(int n, dynamic_array_cost* out_cost) {  // Build size n then insert at head and return cost.
    if (!out_cost) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    dynamic_array a;  // Local array.
    if (!build_filled_array(n, &a)) {  // Build filled array.
        return false;  // Signal failure.
    }  // Close build check.
    bool ok = dynamic_array_insert_at(&a, 0, 999, out_cost);  // Insert sentinel at head and capture cost.
    dynamic_array_destroy(&a);  // Clean up.
    return ok;  // Return status.
}  // End simulate_insert0_cost_at_size.

int main(int argc, char** argv) {  // CLI entry point that prints demo tables.
    const int default_ms[] = {0, 1, 2, 4, 8, 16, 32};  // Default m values include boundary case.
    int ms[64];  // Fixed buffer for parsed m values.
    size_t ms_count = 0;  // Track how many m values we will print.

    if (argc <= 1) {  // Use defaults when no args are provided.
        for (size_t i = 0; i < sizeof(default_ms) / sizeof(default_ms[0]); i++) {  // Copy defaults.
            ms[ms_count++] = default_ms[i];  // Store one m.
        }  // Close loop.
    } else {  // Parse user-provided m values.
        for (int i = 1; i < argc; i++) {  // Parse each token.
            int value = 0;  // Parsed integer.
            if (!parse_int(argv[i], &value) || value < 0) {  // Validate and require m >= 0.
                print_usage(argv[0]);  // Print usage.
                return 2;  // Invalid usage.
            }  // Close validation.
            if (ms_count < sizeof(ms) / sizeof(ms[0])) {  // Avoid overflow.
                ms[ms_count++] = value;  // Store value.
            }  // Close capacity check.
        }  // Close parse loop.
    }  // Close branch.

    printf("=== Append Growth (m appends) ===\n");  // Print section title.
    printf("%6s | %6s | %7s | %6s | %6s\n", "m", "cap", "copies", "avg", "maxCp");  // Print header.
    printf("-------------------------------------------\n");  // Print separator.
    for (size_t i = 0; i < ms_count; i++) {  // Print one row per m.
        int m = ms[i];  // Read m.
        append_summary s;  // Summary struct.
        if (!simulate_appends(m, &s)) {  // Simulate appends.
            fprintf(stderr, "Simulation failed for m=%d\n", m);  // Report error.
            return 1;  // Exit failure.
        }  // Close failure check.
        double avg = (m > 0) ? ((double)s.total_actual_cost / (double)m) : 0.0;  // Average cost per op.
        printf("%6d | %6d | %7lld | %6.2f | %6d\n", s.m, s.final_capacity, s.total_copies, avg, s.max_copied_in_one_op);  // Print row.
    }  // Close loop.

    printf("\n");  // Print blank line between sections.
    printf("=== Append vs insertAt(0) at size n ===\n");  // Print section title.
    printf("%6s | %6s | %6s | %7s | %7s\n", "n", "appCp", "appMv", "ins0Cp", "ins0Mv");  // Print header.
    printf("---------------------------------------------\n");  // Print separator.
    const int ns[] = {0, 1, 2, 4, 8, 16};  // Fixed n list for readability.
    for (size_t i = 0; i < sizeof(ns) / sizeof(ns[0]); i++) {  // Iterate n list.
        int n = ns[i];  // Read n.
        dynamic_array_cost app_cost;  // Append cost at size n.
        dynamic_array_cost ins0_cost;  // Insert@0 cost at size n.
        if (!simulate_append_cost_at_size(n, &app_cost) || !simulate_insert0_cost_at_size(n, &ins0_cost)) {  // Compute costs.
            fprintf(stderr, "Simulation failed for n=%d\n", n);  // Report error.
            return 1;  // Exit failure.
        }  // Close failure check.
        printf("%6d | %6d | %6d | %7d | %7d\n", n, app_cost.copied, app_cost.moved, ins0_cost.copied, ins0_cost.moved);  // Print row.
    }  // Close loop.

    return 0;  // Indicate success.
}  // End main.
