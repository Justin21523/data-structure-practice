/* 01 靜態陣列示範 CLI（C）/ Static array demo CLI (C). */  // Bilingual file header for the executable entry point.
#include "fixed_array.h"  // Import fixed array helpers for simulation.

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
}  // End of parse_int.

static void print_usage(const char* program_name) {  // Print a short usage message.
    fprintf(stderr, "Usage: %s [n1 n2 ...] (each n must be >= 0)\\n", program_name);  // Explain expected arguments.
}  // End of print_usage.

static bool build_filled_array(int n, int extra_capacity, fixed_array* out_array) {  // Build array filled with 0..n-1.
    if (!out_array) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0 || extra_capacity < 0) {  // Reject invalid parameters.
        return false;  // Signal invalid input.
    }  // Close validation.

    if (!fixed_array_init(out_array, n + extra_capacity)) {  // Initialize fixed array with requested capacity.
        return false;  // Signal allocation failure.
    }  // Close init check.

    for (int i = 0; i < n; i++) {  // Fill with deterministic values.
        int moved = 0;  // Append should always move 0 elements.
        if (!fixed_array_append(out_array, i, &moved)) {  // Append one value.
            fixed_array_destroy(out_array);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close append failure branch.
    }  // Close fill loop.
    return true;  // Signal success.
}  // End of build_filled_array.

static int simulate_insert_moves(int n, int index) {  // Simulate insert and return moved count (or -1 on failure).
    fixed_array a;  // Local array used for simulation.
    if (!build_filled_array(n, 1, &a)) {  // Build size n with one extra slot.
        return -1;  // Signal failure.
    }  // Close build check.

    int moved = 0;  // Output moved count from insert.
    bool ok = fixed_array_insert_at(&a, index, 999, &moved);  // Insert sentinel and capture move count.
    fixed_array_destroy(&a);  // Clean up resources.
    return ok ? moved : -1;  // Return moved on success, -1 on failure.
}  // End of simulate_insert_moves.

static int simulate_remove_moves(int n, int index) {  // Simulate remove and return moved count (or -1 on failure).
    fixed_array a;  // Local array used for simulation.
    if (!build_filled_array(n, 0, &a)) {  // Build size n exactly.
        return -1;  // Signal failure.
    }  // Close build check.

    fixed_array_remove_result result;  // Output result from remove.
    bool ok = fixed_array_remove_at(&a, index, &result);  // Remove and capture moved count.
    fixed_array_destroy(&a);  // Clean up resources.
    return ok ? result.moved : -1;  // Return moved on success, -1 on failure.
}  // End of simulate_remove_moves.

int main(int argc, char** argv) {  // CLI entry point that prints move-count table.
    const int default_ns[] = {0, 1, 2, 4, 8, 16};  // Default sizes include boundary case n=0.
    int ns[64];  // Fixed buffer for parsed n values (teaching demo simplicity).
    size_t ns_count = 0;  // Track how many n values we will print.

    if (argc <= 1) {  // Use defaults when no args are provided.
        for (size_t i = 0; i < sizeof(default_ns) / sizeof(default_ns[0]); i++) {  // Copy defaults into ns.
            ns[ns_count++] = default_ns[i];  // Store one default n.
        }  // Close copy loop.
    } else {  // Parse user-provided n values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token.
            int value = 0;  // Temporary storage for parsed integer.
            if (!parse_int(argv[i], &value) || value < 0) {  // Validate integer and require n >= 0.
                print_usage(argv[0]);  // Print usage guidance.
                return 2;  // Return non-zero for invalid usage.
            }  // Close validation block.
            if (ns_count < sizeof(ns) / sizeof(ns[0])) {  // Avoid overflowing fixed buffer.
                ns[ns_count++] = value;  // Store validated n.
            }  // Close capacity check.
        }  // Close parse loop.
    }  // Close parsing branch.

    printf("%6s | %6s | %7s | %7s | %6s | %7s | %7s\\n", "n", "ins@0", "ins@mid", "ins@end", "rm@0", "rm@mid", "rm@end");  // Print header.
    printf("---------------------------------------------------------------\\n");  // Print separator line.

    for (size_t i = 0; i < ns_count; i++) {  // Print one row per n value.
        int n = ns[i];  // Read current n.
        int mid = n / 2;  // Choose deterministic middle index.

        int ins0 = simulate_insert_moves(n, 0);  // Moves for head insert.
        int insm = simulate_insert_moves(n, mid);  // Moves for middle insert.
        int inse = simulate_insert_moves(n, n);  // Moves for tail insert.

        if (n == 0) {  // Removal is not defined for empty arrays.
            printf("%6d | %6d | %7d | %7d | %6s | %7s | %7s\\n", n, ins0, insm, inse, "n/a", "n/a", "n/a");  // Print row with n/a.
        } else {  // For n>0 we can remove at valid indices.
            int rm0 = simulate_remove_moves(n, 0);  // Moves for head removal.
            int rmm = simulate_remove_moves(n, mid);  // Moves for middle removal.
            int rme = simulate_remove_moves(n, n - 1);  // Moves for tail removal.
            printf("%6d | %6d | %7d | %7d | %6d | %7d | %7d\\n", n, ins0, insm, inse, rm0, rmm, rme);  // Print numeric row.
        }  // Close branch.
    }  // Close loop.

    return 0;  // Indicate success.
}  // End of main.

