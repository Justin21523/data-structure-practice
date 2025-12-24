// 01 堆疊示範程式（C++）/ Stack demo program (C++).  // Bilingual file header.

#include "Stack.hpp"  // Import the stack implementation and helpers.

#include <iomanip>  // Provide std::setw for table formatting.
#include <iostream>  // Provide std::cout for console output.
#include <string>  // Provide std::string for argument parsing.
#include <vector>  // Provide std::vector for handling lists of m values.

static std::vector<int> parseMsOrDefault(int argc, char** argv) {  // Parse CLI args into m values or use defaults.
    if (argc <= 1) {  // Use defaults when no args are provided.
        return std::vector<int>{0, 1, 2, 4, 8, 16, 32};  // Default m values.
    }  // Close default branch.
    std::vector<int> ms;  // Accumulate parsed m values.
    for (int i = 1; i < argc; i++) {  // Parse each token.
        ms.push_back(std::stoi(std::string(argv[i])));  // Convert token to int.
    }  // Close loop.
    return ms;  // Return parsed list.
}  // End parseMsOrDefault().

static void printPushGrowthTable(const std::vector<int>& ms) {  // Print push-growth summary table.
    std::cout << std::setw(6) << "m" << " | " << std::setw(6) << "cap" << " | " << std::setw(7) << "copies" << " | " << std::setw(6) << "avg" << " | " << std::setw(6) << "maxCp" << "\n";  // Header.
    std::cout << "----------------------------------------\n";  // Separator line.
    for (int m : ms) {  // Render one row per m.
        stackunit::PushSummary s = stackunit::simulatePushes(m);  // Simulate and summarize.
        double avg = (s.m > 0) ? (static_cast<double>(s.totalActualCost) / static_cast<double>(s.m)) : 0.0;  // Average actual cost per op.
        std::cout << std::setw(6) << s.m << " | " << std::setw(6) << s.finalCapacity << " | " << std::setw(7) << s.totalCopies << " | " << std::setw(6) << std::fixed << std::setprecision(2) << avg << " | " << std::setw(6) << s.maxCopiedInOneOp << "\n";  // Row.
    }  // Close loop.
}  // End printPushGrowthTable().

static void printLifoDemo() {  // Print a small LIFO example with peek/pop.
    stackunit::ArrayStack s;  // Create an empty stack.
    s.push(3);  // Push 3.
    s.push(7);  // Push 7.
    s.push(1);  // Push 1 (becomes top).
    int top = s.peek();  // Peek current top.
    int p1 = s.pop().value;  // Pop first (should be 1).
    int p2 = s.pop().value;  // Pop second (should be 7).
    int p3 = s.pop().value;  // Pop third (should be 3).
    std::cout << "push [3,7,1] => peek=" << top << ", pops=[" << p1 << "," << p2 << "," << p3 << "]\n";  // Print demo line.
}  // End printLifoDemo().

int main(int argc, char** argv) {  // Entry point for demo CLI.
    std::vector<int> ms = parseMsOrDefault(argc, argv);  // Parse m values or use defaults.
    std::cout << "=== Push Growth (m pushes) ===\n";  // Print section title.
    printPushGrowthTable(ms);  // Print growth table.
    std::cout << "\n";  // Blank line.
    std::cout << "=== LIFO demo ===\n";  // Print section title.
    printLifoDemo();  // Print LIFO demo line.
    return 0;  // Exit success.
}  // Close main().

