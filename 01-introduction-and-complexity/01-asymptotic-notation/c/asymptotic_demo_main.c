/* 01 漸進符號示範 CLI（C）/ Asymptotic notation CLI (C). */  // Bilingual file header for the executable entry point.
#include "asymptotic_demo.h"  // Import the operation-counting functions used to print the table.

#include <errno.h>  // Use errno to validate numeric parsing via strtol.
#include <limits.h>  // Use INT_MIN/INT_MAX to validate parsed values fit into int.
#include <stdio.h>  // Use printf/fprintf for CLI output.
#include <stdlib.h>  // Use strtol for CLI parsing and exit codes.

static bool parse_int(const char* s, int* out_value) {  // Parse an int from a string with basic validation.
    if (!s || !out_value) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal parsing failure to the caller.
    }  // Close null-check block.

    errno = 0;  // Reset errno so we can detect conversion errors reliably.
    char* end = NULL;  // strtol will set this to the first non-parsed character.
    long value = strtol(s, &end, 10);  // Convert the string to a long in base 10.
    if (errno != 0) {  // Detect overflow/underflow or other conversion errors.
        return false;  // Signal parsing failure to the caller.
    }  // Close errno-check block.
    if (end == s || *end != '\0') {  // Reject empty strings and trailing garbage like "12abc".
        return false;  // Signal parsing failure for malformed numeric input.
    }  // Close format-check block.
    if (value < INT_MIN || value > INT_MAX) {  // Ensure the parsed number fits into an int.
        return false;  // Signal failure when the number is out of range.
    }  // Close range-check block.

    *out_value = (int)value;  // Store the parsed integer into the output parameter.
    return true;  // Signal success to the caller.
}  // End of parse_int.

static void print_usage(const char* program_name) {  // Print a short usage message for invalid CLI input.
    fprintf(stderr, "Usage: %s [n1 n2 ...] (each n must be >= 1)\\n", program_name);  // Explain expected arguments.
}  // End of print_usage.

int main(int argc, char** argv) {  // CLI entry point that prints a growth table.
    const int default_ns[] = {1, 2, 4, 8, 16, 32};  // Default sizes show clear growth differences.
    int ns[64];  // Use a fixed buffer for simplicity in this teaching-oriented demo.
    size_t ns_count = 0;  // Track how many n values we will print.

    if (argc <= 1) {  // If no args are provided, use the default sequence.
        for (size_t i = 0; i < sizeof(default_ns) / sizeof(default_ns[0]); i++) {  // Copy each default value into ns.
            ns[ns_count++] = default_ns[i];  // Store the default n value and advance the count.
        }  // End of copy loop.
    } else {  // Otherwise parse user-provided n values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token starting from argv[1].
            int value = 0;  // Temporary value for parsed integer.
            if (!parse_int(argv[i], &value) || value < 1) {  // Validate parsing and require n >= 1 for log-based counters.
                print_usage(argv[0]);  // Print help so the user can correct input.
                return 2;  // Return a non-zero exit code for invalid usage.
            }  // Close validation block.
            if (ns_count < sizeof(ns) / sizeof(ns[0])) {  // Avoid overflowing the fixed-size ns buffer.
                ns[ns_count++] = value;  // Store the parsed value for table printing.
            }  // Close capacity check block.
        }  // End of parse loop.
    }  // Close arg parsing branch.

    printf("%8s | %8s | %8s | %8s | %10s | %10s\\n", "n", "O(1)", "O(log n)", "O(n)", "O(n log n)", "O(n^2)");  // Print the table header.
    printf("-------------------------------------------------------------------\\n");  // Print a simple separator line.

    for (size_t i = 0; i < ns_count; i++) {  // Print one row for each n value.
        int n = ns[i];  // Read the current input size for this row.
        long long c1 = count_constant_ops(n);  // Compute the O(1) example count.
        long long clog = count_log2_ops(n);  // Compute the O(log n) example count.
        long long cn = count_linear_ops(n);  // Compute the O(n) example count.
        long long cnlog = count_n_log2_n_ops(n);  // Compute the O(n log n) example count.
        long long cn2 = count_quadratic_ops(n);  // Compute the O(n^2) example count.
        printf("%8d | %8lld | %8lld | %8lld | %10lld | %10lld\\n", n, c1, clog, cn, cnlog, cn2);  // Print the aligned numeric row.
    }  // End of row loop.

    return 0;  // Indicate success to the OS.
}  // End of main.
