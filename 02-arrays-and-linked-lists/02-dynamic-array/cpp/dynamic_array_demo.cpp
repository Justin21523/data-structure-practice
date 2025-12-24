// 02 動態陣列示範（C++）/ Dynamic array demo (C++).  // Bilingual file header.
#include "DynamicArray.hpp"  // Import dynamic array helpers for simulation and printing.

#include <cstdlib>  // Provide std::strtol for CLI parsing.
#include <iomanip>  // Provide std::setw for aligned table printing.
#include <iostream>  // Provide std::cout/std::cerr for CLI output.
#include <limits>  // Provide std::numeric_limits for safe int range checks.
#include <vector>  // Provide std::vector for storing m values.

static bool parseInt(const char* s, int& outValue) {  // Parse an int from a C-string with basic validation.
    if (!s) {  // Reject null pointers.
        return false;  // Signal failure.
    }  // Close null check.
    char* end = nullptr;  // strtol will set this to the first non-parsed character.
    long value = std::strtol(s, &end, 10);  // Parse integer in base 10.
    if (end == s || *end != '\0') {  // Reject empty strings and trailing garbage.
        return false;  // Signal failure.
    }  // Close format check.
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {  // Ensure value fits in int.
        return false;  // Signal failure.
    }  // Close range check.
    outValue = static_cast<int>(value);  // Store parsed value.
    return true;  // Signal success.
}  // End parseInt.

static void printUsage(const char* programName) {  // Print a short usage message for invalid input.
    std::cerr << "Usage: " << programName << " [m1 m2 ...] (each m must be >= 0)\n";  // Explain expected arguments.
}  // End printUsage.

int main(int argc, char** argv) {  // CLI entry point that prints demo tables.
    std::vector<int> ms;  // Store m values requested by the user (or defaults).
    if (argc <= 1) {  // Use defaults when no args are provided.
        ms = {0, 1, 2, 4, 8, 16, 32};  // Default m values.
    } else {  // Parse user-provided m values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token.
            int value = 0;  // Parsed integer.
            if (!parseInt(argv[i], value) || value < 0) {  // Validate and require m >= 0.
                printUsage(argv[0]);  // Print help.
                return 2;  // Invalid usage.
            }  // Close validation.
            ms.push_back(value);  // Store validated value.
        }  // Close loop.
    }  // Close branch.

    std::cout << "=== Append Growth (m appends) ===\n";  // Print section title.
    std::cout << std::setw(6) << "m"  // Column: m.
              << " | " << std::setw(6) << "cap"  // Column: final capacity.
              << " | " << std::setw(7) << "copies"  // Column: total copies.
              << " | " << std::setw(6) << "avg"  // Column: avg actual cost.
              << " | " << std::setw(6) << "maxCp"  // Column: max copied in one op.
              << '\n';  // End header.
    std::cout << "-------------------------------------------\n";  // Print separator.
    for (int m : ms) {  // Render one row per m.
        dynamicarray::AppendSummary s = dynamicarray::simulateAppends(m);  // Simulate and summarize.
        double avg = (m > 0) ? (static_cast<double>(s.totalActualCost) / static_cast<double>(m)) : 0.0;  // Average cost.
        std::cout << std::setw(6) << s.m  // Print m.
                  << " | " << std::setw(6) << s.finalCapacity  // Print capacity.
                  << " | " << std::setw(7) << s.totalCopies  // Print copies.
                  << " | " << std::setw(6) << std::fixed << std::setprecision(2) << avg  // Print avg.
                  << " | " << std::setw(6) << s.maxCopiedInOneOp  // Print max copies.
                  << '\n';  // End row.
    }  // Close loop.

    std::cout << "\n";  // Print blank line.
    std::cout << "=== Append vs insertAt(0) at size n ===\n";  // Print section title.
    std::cout << std::setw(6) << "n"  // Column: n.
              << " | " << std::setw(6) << "appCp"  // Column: append copied.
              << " | " << std::setw(6) << "appMv"  // Column: append moved.
              << " | " << std::setw(7) << "ins0Cp"  // Column: insert@0 copied.
              << " | " << std::setw(7) << "ins0Mv"  // Column: insert@0 moved.
              << '\n';  // End header.
    std::cout << "---------------------------------------------\n";  // Print separator.
    int ns[] = {0, 1, 2, 4, 8, 16};  // Fixed n list for readability.
    for (int n : ns) {  // Render one row per n.
        dynamicarray::OperationCost app = dynamicarray::simulateAppendCostAtSize(n);  // Cost of append at size n.
        dynamicarray::OperationCost ins0 = dynamicarray::simulateInsert0CostAtSize(n);  // Cost of insert@0 at size n.
        std::cout << std::setw(6) << n  // Print n.
                  << " | " << std::setw(6) << app.copied  // Print append copied.
                  << " | " << std::setw(6) << app.moved  // Print append moved.
                  << " | " << std::setw(7) << ins0.copied  // Print insert@0 copied.
                  << " | " << std::setw(7) << ins0.moved  // Print insert@0 moved.
                  << '\n';  // End row.
    }  // Close loop.

    return 0;  // Indicate success.
}  // End main.

