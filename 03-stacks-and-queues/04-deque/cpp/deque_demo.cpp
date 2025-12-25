// 04 雙端佇列示範程式（C++）/ Deque demo program (C++).  // Bilingual file header.

#include "Deque.hpp"  // Import the deque implementation and helpers.

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
        dequeunit::PushSummary s = dequeunit::simulatePushBacks(m);  // Simulate and summarize.
        double avg = (s.m > 0) ? (static_cast<double>(s.totalActualCost) / static_cast<double>(s.m)) : 0.0;  // Average actual cost per op.
        std::cout << std::setw(6) << s.m << " | " << std::setw(6) << s.finalCapacity << " | " << std::setw(7) << s.totalCopies << " | " << std::setw(6) << std::fixed << std::setprecision(2) << avg << " | " << std::setw(6) << s.maxCopiedInOneOp << "\n";  // Row.
    }  // Close loop.
}  // End printPushGrowthTable().

static void printDequeDemo() {  // Print a small deque example using both ends.
    dequeunit::Deque d;  // Create an empty deque.
    d.pushBack(3);  // Push 3 to back.
    d.pushBack(7);  // Push 7 to back.
    d.pushFront(1);  // Push 1 to front (deque becomes [1,3,7]).
    int front = d.peekFront();  // Peek front value.
    int back = d.peekBack();  // Peek back value.
    int p1 = d.popFront().value;  // Pop front (should be 1).
    int p2 = d.popBack().value;  // Pop back (should be 7).
    int p3 = d.popBack().value;  // Pop back (should be 3).
    std::cout << "pushBack [3,7], pushFront [1] => peekFront=" << front << ", peekBack=" << back << ", pops=[" << p1 << "," << p2 << "," << p3 << "]\n";  // Print demo line.
}  // End printDequeDemo().

int main(int argc, char** argv) {  // Entry point for demo CLI.
    std::vector<int> ms = parseMsOrDefault(argc, argv);  // Parse m values or use defaults.
    std::cout << "=== pushBack growth (m pushes) ===\n";  // Print section title.
    printPushGrowthTable(ms);  // Print growth table.
    std::cout << "\n";  // Blank line.
    std::cout << "=== deque demo (both ends) ===\n";  // Print section title.
    printDequeDemo();  // Print deque demo line.
    return 0;  // Exit success.
}  // Close main().

