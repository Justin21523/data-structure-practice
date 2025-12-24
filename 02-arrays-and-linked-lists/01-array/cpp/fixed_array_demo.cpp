// 01 靜態陣列示範（C++）/ Static array demo (C++).  // Bilingual file header.
#include "FixedArray.hpp"  // Import FixedArray helpers for simulation.

#include <cstdlib>  // Provide std::strtol for CLI parsing.
#include <iomanip>  // Provide std::setw for aligned table printing.
#include <iostream>  // Provide std::cout/std::cerr for CLI output.
#include <limits>  // Provide std::numeric_limits for safe int range checks.
#include <vector>  // Provide std::vector for storing n values.

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
}  // End parseInt.

static void printUsage(const char* programName) {  // Print a short usage message for invalid input.
    std::cerr << "Usage: " << programName << " [n1 n2 ...] (each n must be >= 0)\n";  // Explain expected arguments.
}  // End printUsage.

int main(int argc, char** argv) {  // CLI entry point that prints move-count table.
    std::vector<int> ns;  // Store n values requested by the user (or defaults).
    if (argc <= 1) {  // Use defaults when no args are provided.
        ns = {0, 1, 2, 4, 8, 16};  // Include boundary case n=0.
    } else {  // Parse user-provided n values.
        for (int i = 1; i < argc; i++) {  // Parse each CLI token.
            int value = 0;  // Temporary storage for parsed integer.
            if (!parseInt(argv[i], value) || value < 0) {  // Validate and require n >= 0.
                printUsage(argv[0]);  // Print help.
                return 2;  // Return non-zero for invalid usage.
            }  // Close validation.
            ns.push_back(value);  // Store validated value.
        }  // Close parse loop.
    }  // Close parsing branch.

    std::cout << std::setw(6) << "n"  // Column: n.
              << " | " << std::setw(6) << "ins@0"  // Column: insert at head moves.
              << " | " << std::setw(7) << "ins@mid"  // Column: insert at mid moves.
              << " | " << std::setw(7) << "ins@end"  // Column: insert at end moves.
              << " | " << std::setw(6) << "rm@0"  // Column: remove at head moves.
              << " | " << std::setw(7) << "rm@mid"  // Column: remove at mid moves.
              << " | " << std::setw(7) << "rm@end"  // Column: remove at end moves.
              << '\n';  // End header.
    std::cout << "---------------------------------------------------------------\n";  // Print separator line.

    for (int n : ns) {  // Render one row per n.
        int mid = n / 2;  // Choose deterministic middle index.

        int ins0 = fixedarray::simulateInsertMoves(n, 0);  // Moves for head insert.
        int insm = fixedarray::simulateInsertMoves(n, mid);  // Moves for middle insert.
        int inse = fixedarray::simulateInsertMoves(n, n);  // Moves for tail insert.

        if (n == 0) {  // Removal is not defined for empty arrays.
            std::cout << std::setw(6) << n  // Print n.
                      << " | " << std::setw(6) << ins0  // Print insert@0.
                      << " | " << std::setw(7) << insm  // Print insert@mid.
                      << " | " << std::setw(7) << inse  // Print insert@end.
                      << " | " << std::setw(6) << "n/a"  // Print remove@0 as n/a.
                      << " | " << std::setw(7) << "n/a"  // Print remove@mid as n/a.
                      << " | " << std::setw(7) << "n/a"  // Print remove@end as n/a.
                      << '\n';  // End row.
        } else {  // For n>0 we can compute removes.
            int rm0 = fixedarray::simulateRemoveMoves(n, 0);  // Moves for head remove.
            int rmm = fixedarray::simulateRemoveMoves(n, mid);  // Moves for middle remove.
            int rme = fixedarray::simulateRemoveMoves(n, n - 1);  // Moves for tail remove.
            std::cout << std::setw(6) << n  // Print n.
                      << " | " << std::setw(6) << ins0  // Print insert@0.
                      << " | " << std::setw(7) << insm  // Print insert@mid.
                      << " | " << std::setw(7) << inse  // Print insert@end.
                      << " | " << std::setw(6) << rm0  // Print remove@0.
                      << " | " << std::setw(7) << rmm  // Print remove@mid.
                      << " | " << std::setw(7) << rme  // Print remove@end.
                      << '\n';  // End row.
        }  // Close branch.
    }  // Close loop.

    return 0;  // Indicate success.
}  // End main.

