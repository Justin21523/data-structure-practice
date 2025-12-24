/* 02 攤銷分析 CLI（C）/ Amortized analysis CLI (C). */  // Bilingual file header for the executable entry point.
#include "amortized_demo.h"  // Import simulation helpers for printing summaries.

#include <errno.h>  // Use errno to validate numeric parsing via strtol.
#include <limits.h>  // Use INT_MIN/INT_MAX to validate parsed values fit into int.
#include <stdio.h>  // Use printf/fprintf for CLI output.
#include <stdlib.h>  // Use strtol for CLI parsing and exit codes.

static bool parse_int(const char* s, int* out_value) {  // Parse an int from a string with basic validation.
    if (!s || !out_value) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal parsing failure.
    }  // Close pointer check.

    errno = 0;  // Reset errno so we can detect conversion errors.
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

    *out_value = (int)value;  // Store parsed value.
    return true;  // Signal success.
}  // End of parse_int.

static void print_usage(const char* program_name) {  // Print a short usage message.
    fprintf(stderr, "Usage: %s [m1 m2 ...] (each m must be >= 0)\\n", program_name);  // Explain expected arguments.
}  // End of print_usage.

int main(int argc, char** argv) {  // CLI entry point that prints a summary table for multiple m values.
    const int default_ms[] = {1, 2, 4, 8, 16, 32};  // Default operation counts for quick demos.
    int ms[64];  // Fixed buffer for parsed m values (teaching demo simplicity).
    size_t ms_count = 0;  // Track how many m values we will print.

    if (argc <= 1) {  // Use defaults when no args are provided.
        for (size_t i = 0; i < sizeof(default_ms) / sizeof(default_ms[0]); i++) {  // Copy defaults.
            ms[ms_count++] = default_ms[i];  // Store one default m.
        }  // Close copy loop.
    } else {  // Parse user-provided m values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token.
            int value = 0;  // Temporary storage for parsed integer.
            if (!parse_int(argv[i], &value) || value < 0) {  // Validate integer and require m >= 0.
                print_usage(argv[0]);  // Print usage guidance.
                return 2;  // Return non-zero exit code for invalid usage.
            }  // Close validation block.
            if (ms_count < sizeof(ms) / sizeof(ms[0])) {  // Avoid overflowing fixed buffer.
                ms[ms_count++] = value;  // Store validated m.
            }  // Close capacity check.
        }  // Close parse loop.
    }  // Close arg parsing branch.

    printf("%6s | %8s | %6s | %6s | %6s | %7s | %6s\\n", "m", "total", "avg", "max", "cap", "copies", "bank");  // Print header.
    printf("------------------------------------------------------------\\n");  // Print a separator line.

    for (size_t i = 0; i < ms_count; i++) {  // Print one row per m value.
        int m = ms[i];  // Read current operation count.
        amortized_summary s;  // Summary output structure.
        if (!amortized_simulate_appends(m, &s)) {  // Run simulation.
            fprintf(stderr, "Simulation failed for m=%d\\n", m);  // Report error.
            return 1;  // Exit with failure.
        }  // Close simulation failure check.
        double avg = (m > 0) ? ((double)s.total_actual_cost / (double)m) : 0.0;  // Compute average cost safely for m=0.
        printf("%6d | %8lld | %6.2f | %6lld | %6d | %7lld | %6lld\\n", m, s.total_actual_cost, avg, s.max_actual_cost, s.final_capacity, s.total_copies, s.final_bank);  // Print row.
    }  // Close row loop.

    return 0;  // Indicate success.
}  // End of main.

