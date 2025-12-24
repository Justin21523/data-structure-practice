/* 01 漸進符號示範（C）/ Asymptotic notation demo (C). */  // Bilingual file header.
#include "asymptotic_demo.h"  // Include our public API so definitions match declarations.

long long count_constant_ops(int n) {  // Simulate a constant-time algorithm independent of n.
    (void)n;  // Explicitly ignore n to show that this counter does not depend on input size.
    long long operations = 0;  // Initialize the simulated operation counter.
    operations += 1;  // Count a basic operation #1.
    operations += 1;  // Count a basic operation #2.
    operations += 1;  // Count a basic operation #3.
    return operations;  // Return a constant value to illustrate O(1).
}  // End of count_constant_ops.

long long count_log2_ops(int n) {  // Simulate logarithmic growth via repeated halving.
    if (n < 1) {  // Reject invalid inputs because log2(n) is not defined here for n < 1.
        return -1;  // Use -1 as a simple error signal in this no-exceptions language.
    }  // Close validation block.

    int current = n;  // Copy n so we can shrink it while counting iterations.
    long long operations = 0;  // Initialize the counter for the halving loop.
    while (current > 1) {  // Each iteration halves current, producing ~log2(n) iterations.
        current /= 2;  // Halve the current size using integer division.
        operations += 1;  // Count one operation per halving step.
    }  // End of halving loop.
    return operations;  // Return floor(log2(n)) for this discrete loop.
}  // End of count_log2_ops.

long long count_linear_ops(int n) {  // Simulate linear growth via a single loop of n iterations.
    if (n < 0) {  // Reject invalid negative sizes.
        return -1;  // Signal invalid input.
    }  // Close validation block.

    long long operations = 0;  // Initialize the counter for the linear loop.
    for (int i = 0; i < n; i++) {  // Run exactly n iterations to illustrate O(n).
        operations += 1;  // Count one operation per iteration.
    }  // End of linear loop.
    return operations;  // Return n as the total count.
}  // End of count_linear_ops.

long long count_n_log2_n_ops(int n) {  // Simulate n log n growth via outer n loop + inner halving loop.
    if (n < 0) {  // Reject invalid negative sizes.
        return -1;  // Signal invalid input.
    }  // Close validation block.
    if (n == 0) {  // Handle boundary case explicitly for a clean demo definition.
        return 0;  // Define 0 * log(0) as 0 operations for this discrete demo.
    }  // Close boundary-case block.

    long long operations = 0;  // Initialize the counter for nested loops.
    for (int i = 0; i < n; i++) {  // Outer loop contributes a linear factor of n.
        int current = n;  // Inner loop starts at n each time to produce the logarithmic factor.
        while (current > 1) {  // Inner loop halves current, producing ~log2(n) iterations.
            current /= 2;  // Halve the inner size.
            operations += 1;  // Count one operation per inner iteration.
        }  // End of inner loop.
    }  // End of outer loop.
    return operations;  // Return approximately n * floor(log2(n)).
}  // End of count_n_log2_n_ops.

long long count_quadratic_ops(int n) {  // Simulate quadratic growth via two nested loops.
    if (n < 0) {  // Reject invalid negative sizes.
        return -1;  // Signal invalid input.
    }  // Close validation block.

    long long operations = 0;  // Initialize the counter for the double loop.
    for (int i = 0; i < n; i++) {  // Outer loop runs n times.
        for (int j = 0; j < n; j++) {  // Inner loop runs n times, giving n * n iterations.
            operations += 1;  // Count one operation per inner iteration.
        }  // End of inner loop.
    }  // End of outer loop.
    return operations;  // Return n^2 as the total operation count.
}  // End of count_quadratic_ops.
