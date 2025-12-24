// 01 漸進符號示範（C++）/ Asymptotic notation demo (C++).  // Bilingual header line for this module.
#ifndef ASYMPTOTIC_DEMO_HPP  // Header guard to prevent multiple inclusion.
#define ASYMPTOTIC_DEMO_HPP  // Header guard definition.

#include <stdexcept>  // Provide std::invalid_argument for input validation errors.
#include <string>  // Provide std::string for error messages and formatting helpers.
#include <vector>  // Provide std::vector for storing n values and rows.

namespace asymptotic {  // Use a small namespace to avoid polluting the global namespace.

inline void requireNonNegative(int n) {  // Validate that n is non-negative for size-based loops.
    if (n < 0) {  // Reject negative sizes because they do not make sense for complexity analysis.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input to the caller.
    }  // Close validation block.
}  // End of requireNonNegative.

inline void requireAtLeastOne(int n) {  // Validate that n is at least 1 for log-based counters.
    if (n < 1) {  // Reject non-positive sizes because log2(n) is not defined here for n < 1.
        throw std::invalid_argument("n must be >= 1");  // Signal invalid input to the caller.
    }  // Close validation block.
}  // End of requireAtLeastOne.

inline long long countConstantOps(int /*n*/) {  // Simulate a constant-time algorithm independent of n.
    long long operations = 0;  // Initialize the simulated operation counter.
    operations += 1;  // Count a basic operation #1.
    operations += 1;  // Count a basic operation #2.
    operations += 1;  // Count a basic operation #3.
    return operations;  // Return a constant value to illustrate O(1).
}  // End of countConstantOps.

inline long long countLog2Ops(int n) {  // Simulate logarithmic growth via repeated halving.
    requireAtLeastOne(n);  // Validate input before running the halving loop.
    int current = n;  // Copy n so we can shrink it while counting iterations.
    long long operations = 0;  // Initialize the operation counter for the halving loop.
    while (current > 1) {  // Each iteration halves current, giving ~log2(n) iterations.
        current /= 2;  // Reduce the problem size by a factor of 2.
        operations += 1;  // Count one operation per halving step.
    }  // End of halving loop.
    return operations;  // Return floor(log2(n)) for this discrete process.
}  // End of countLog2Ops.

inline long long countLinearOps(int n) {  // Simulate linear growth via one pass of n iterations.
    requireNonNegative(n);  // Validate n so the loop bound is meaningful.
    long long operations = 0;  // Initialize the operation counter.
    for (int i = 0; i < n; i++) {  // Run exactly n iterations to illustrate O(n).
        operations += 1;  // Count one operation per iteration.
    }  // End of linear loop.
    return operations;  // Return n as the total count.
}  // End of countLinearOps.

inline long long countNLog2NOps(int n) {  // Simulate n log n growth via outer n loop + inner halving loop.
    requireNonNegative(n);  // Validate n to keep loops well-defined.
    if (n == 0) {  // Handle the boundary case explicitly for a clean demo definition.
        return 0;  // Define 0 * log(0) as 0 operations in this discrete demonstration.
    }  // Close boundary-case block.
    long long operations = 0;  // Initialize the counter for nested loops.
    for (int i = 0; i < n; i++) {  // Outer loop contributes the linear factor n.
        int current = n;  // Inner loop starts at n each time to produce the logarithmic factor.
        while (current > 1) {  // Inner loop halves current, producing ~log2(n) iterations.
            current /= 2;  // Halve the inner size.
            operations += 1;  // Count one operation per inner iteration.
        }  // End of inner loop.
    }  // End of outer loop.
    return operations;  // Return approximately n * floor(log2(n)).
}  // End of countNLog2NOps.

inline long long countQuadraticOps(int n) {  // Simulate quadratic growth via two nested loops.
    requireNonNegative(n);  // Validate n so nested loops are well-defined.
    long long operations = 0;  // Initialize the counter for the double loop.
    for (int i = 0; i < n; i++) {  // Outer loop runs n times.
        for (int j = 0; j < n; j++) {  // Inner loop runs n times, giving n * n iterations.
            operations += 1;  // Count one operation per inner iteration.
        }  // End of inner loop.
    }  // End of outer loop.
    return operations;  // Return n^2 as the total count.
}  // End of countQuadraticOps.

}  // namespace asymptotic  // Close the namespace block.

#endif  // ASYMPTOTIC_DEMO_HPP  // End of header guard.

