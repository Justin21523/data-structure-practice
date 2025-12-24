// 02 攤銷分析示範（C++）/ Amortized analysis demo (C++).  // Bilingual file header.
#include "AmortizedDemo.hpp"  // Import simulation helpers for printing summaries.

#include <cstdlib>  // Provide std::strtol for CLI parsing.
#include <iomanip>  // Provide std::setw for aligned table printing.
#include <iostream>  // Provide std::cout/std::cerr for CLI output.
#include <limits>  // Provide std::numeric_limits for safe int range checks.
#include <vector>  // Provide std::vector for storing m values.

static bool parseInt(const char* s, int& outValue) {  // Parse an int from a C-string with basic validation.
    if (!s) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal failure.
    }  // Close null check.
    char* end = nullptr;  // strtol will set this to the first non-parsed character.
    long value = std::strtol(s, &end, 10);  // Parse integer in base 10.
    if (end == s || *end != '\0') {  // Reject empty strings and trailing garbage.
        return false;  // Signal failure.
    }  // Close format check.
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {  // Ensure value fits into int.
        return false;  // Signal failure.
    }  // Close range check.
    outValue = static_cast<int>(value);  // Store parsed value.
    return true;  // Signal success.
}  // End of parseInt.

static void printUsage(const char* programName) {  // Print a short usage message for invalid input.
    std::cerr << "Usage: " << programName << " [m1 m2 ...] (each m must be >= 0)\n";  // Explain expected arguments.
}  // End of printUsage.

int main(int argc, char** argv) {  // CLI entry point that prints a summary table for multiple m values.
    std::vector<int> ms;  // Store m values requested by the user (or defaults).
    if (argc <= 1) {  // Use defaults when no args are provided.
        ms = {1, 2, 4, 8, 16, 32};  // Default sizes match other units for consistency.
    } else {  // Parse user-provided m values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token starting from argv[1].
            int value = 0;  // Temporary storage for parsed integer.
            if (!parseInt(argv[i], value) || value < 0) {  // Validate parsing and require m >= 0.
                printUsage(argv[0]);  // Print help.
                return 2;  // Return non-zero for invalid usage.
            }  // Close validation block.
            ms.push_back(value);  // Store validated value.
        }  // Close parse loop.
    }  // Close arg parsing branch.

    std::cout << std::setw(6) << "m"  // Column: m.
              << " | " << std::setw(8) << "total"  // Column: total actual cost.
              << " | " << std::setw(6) << "avg"  // Column: average actual cost.
              << " | " << std::setw(6) << "max"  // Column: max single actual cost.
              << " | " << std::setw(6) << "cap"  // Column: final capacity.
              << " | " << std::setw(7) << "copies"  // Column: total copies.
              << " | " << std::setw(6) << "bank"  // Column: final bank.
              << '\n';  // End header line.
    std::cout << "------------------------------------------------------------\n";  // Print separator.

    for (int m : ms) {  // Print one row per m.
        amortized::SimulationSummary s = amortized::simulateAppends(m);  // Run simulation and collect summary.
        if (s.finalSize != m) {  // Sanity check size.
            throw std::runtime_error("size mismatch (internal bug)");  // Fail fast.
        }  // Close check.
        if (!amortized::isPowerOfTwo(s.finalCapacity)) {  // Validate capacity form.
            throw std::runtime_error("capacity not power-of-two (internal bug)");  // Fail fast.
        }  // Close check.
        if (s.finalCapacity < std::max(1, m)) {  // Validate capacity bound.
            throw std::runtime_error("capacity too small (internal bug)");  // Fail fast.
        }  // Close check.

        double avg = (m > 0) ? (static_cast<double>(s.totalActualCost) / static_cast<double>(m)) : 0.0;  // Compute average safely for m=0.
        std::cout << std::setw(6) << s.m  // Print m.
                  << " | " << std::setw(8) << s.totalActualCost  // Print total.
                  << " | " << std::setw(6) << std::fixed << std::setprecision(2) << avg  // Print avg.
                  << " | " << std::setw(6) << s.maxActualCost  // Print max.
                  << " | " << std::setw(6) << s.finalCapacity  // Print cap.
                  << " | " << std::setw(7) << s.totalCopies  // Print copies.
                  << " | " << std::setw(6) << s.finalBank  // Print bank.
                  << '\n';  // End row.
    }  // Close row loop.

    return 0;  // Indicate success.
}  // End of main.

