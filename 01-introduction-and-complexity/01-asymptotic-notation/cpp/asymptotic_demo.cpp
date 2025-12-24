// 01 漸進符號示範（C++）/ Asymptotic notation demo (C++).  // Bilingual file header.
#include "AsymptoticDemo.hpp"  // Import operation-counting helpers for table printing.

#include <cstdlib>  // Provide std::strtol for CLI parsing.
#include <iomanip>  // Provide std::setw for aligned table printing.
#include <iostream>  // Provide std::cout/std::cerr for CLI output.
#include <limits>  // Provide std::numeric_limits for safe int range checks.
#include <sstream>  // Provide std::ostringstream for building formatted output.

static bool parseInt(const char* s, int& outValue) {  // Parse an int from a C-string with basic validation.
    if (!s) {  // Reject null pointers to avoid undefined behavior.
        return false;  // Signal failure to the caller.
    }  // Close null-check block.
    char* end = nullptr;  // strtol will set this to the first non-parsed character.
    long value = std::strtol(s, &end, 10);  // Parse the integer value in base 10.
    if (end == s || *end != '\0') {  // Reject empty strings and trailing garbage.
        return false;  // Signal failure for malformed numeric input.
    }  // Close format-check block.
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {  // Ensure value fits into int.
        return false;  // Signal failure for out-of-range values.
    }  // Close range-check block.
    outValue = static_cast<int>(value);  // Store the parsed int into the output parameter.
    return true;  // Signal success.
}  // End of parseInt.

static void printUsage(const char* programName) {  // Print a short usage message for invalid input.
    std::cerr << "Usage: " << programName << " [n1 n2 ...] (each n must be >= 1)\n";  // Explain expected arguments.
}  // End of printUsage.

int main(int argc, char** argv) {  // CLI entry point that prints a growth table.
    std::vector<int> ns;  // Store n values requested by the user (or defaults).
    if (argc <= 1) {  // Use defaults when no CLI args are provided.
        ns = {1, 2, 4, 8, 16, 32};  // Default sizes show clear growth differences.
    } else {  // Parse user-provided n values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token starting from argv[1].
            int value = 0;  // Temporary storage for parsed integer.
            if (!parseInt(argv[i], value) || value < 1) {  // Validate parsing and require n >= 1 for log-based counters.
                printUsage(argv[0]);  // Print help so the user can correct input.
                return 2;  // Return a non-zero exit code for invalid usage.
            }  // Close validation block.
            ns.push_back(value);  // Add the validated n value to the list for table printing.
        }  // End parse loop.
    }  // Close arg parsing branch.

    std::cout << std::setw(8) << "n"  // Print the header column for n.
              << " | " << std::setw(8) << "O(1)"  // Print the header column for O(1).
              << " | " << std::setw(8) << "O(log n)"  // Print the header column for O(log n).
              << " | " << std::setw(8) << "O(n)"  // Print the header column for O(n).
              << " | " << std::setw(10) << "O(n log n)"  // Print the header column for O(n log n).
              << " | " << std::setw(10) << "O(n^2)"  // Print the header column for O(n^2).
              << '\n';  // End the header line.
    std::cout << "-------------------------------------------------------------------\n";  // Print a separator line.

    for (int n : ns) {  // Print one row for each input size.
        long long c1 = asymptotic::countConstantOps(n);  // Compute the O(1) example count.
        long long clog = asymptotic::countLog2Ops(n);  // Compute the O(log n) example count.
        long long cn = asymptotic::countLinearOps(n);  // Compute the O(n) example count.
        long long cnlog = asymptotic::countNLog2NOps(n);  // Compute the O(n log n) example count.
        long long cn2 = asymptotic::countQuadraticOps(n);  // Compute the O(n^2) example count.

        std::cout << std::setw(8) << n  // Print the n value aligned to the header width.
                  << " | " << std::setw(8) << c1  // Print the O(1) count.
                  << " | " << std::setw(8) << clog  // Print the O(log n) count.
                  << " | " << std::setw(8) << cn  // Print the O(n) count.
                  << " | " << std::setw(10) << cnlog  // Print the O(n log n) count.
                  << " | " << std::setw(10) << cn2  // Print the O(n^2) count.
                  << '\n';  // End the row.
    }  // End row loop.

    return 0;  // Indicate success to the OS.
}  // End of main.
