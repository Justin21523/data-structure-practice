// 03 環狀佇列示範程式（C++）/ Circular queue demo program (C++).  // Bilingual file header.

#include "CircularQueue.hpp"  // Import the circular queue implementation and helpers.

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

static void printEnqueueGrowthTable(const std::vector<int>& ms) {  // Print enqueue-growth summary table.
    std::cout << std::setw(6) << "m" << " | " << std::setw(6) << "cap" << " | " << std::setw(7) << "copies" << " | " << std::setw(6) << "avg" << " | " << std::setw(6) << "maxCp" << "\n";  // Header.
    std::cout << "----------------------------------------\n";  // Separator line.
    for (int m : ms) {  // Render one row per m.
        circularqueueunit::EnqueueSummary s = circularqueueunit::simulateEnqueues(m);  // Simulate and summarize.
        double avg = (s.m > 0) ? (static_cast<double>(s.totalActualCost) / static_cast<double>(s.m)) : 0.0;  // Average actual cost per op.
        std::cout << std::setw(6) << s.m << " | " << std::setw(6) << s.finalCapacity << " | " << std::setw(7) << s.totalCopies << " | " << std::setw(6) << std::fixed << std::setprecision(2) << avg << " | " << std::setw(6) << s.maxCopiedInOneOp << "\n";  // Row.
    }  // Close loop.
}  // End printEnqueueGrowthTable().

static void printDequeueCostTable(const std::vector<int>& ns) {  // Print dequeue moved-cost table (should be 0).
    std::cout << std::setw(6) << "n" << " | " << std::setw(6) << "moved" << "\n";  // Header.
    std::cout << "-----------------\n";  // Separator line.
    for (int n : ns) {  // Render one row per n.
        int moved = (n == 0) ? 0 : circularqueueunit::simulateDequeueCostAtSize(n).moved;  // Skip invalid dequeue on empty queue.
        std::cout << std::setw(6) << n << " | " << std::setw(6) << moved << "\n";  // Row.
    }  // Close loop.
}  // End printDequeueCostTable().

static void printFifoDemo() {  // Print a small FIFO example with peek/dequeue.
    circularqueueunit::CircularQueue q;  // Create an empty queue.
    q.enqueue(3);  // Enqueue 3.
    q.enqueue(7);  // Enqueue 7.
    q.enqueue(1);  // Enqueue 1.
    int front = q.peek();  // Peek current front.
    int d1 = q.dequeue().value;  // Dequeue first (should be 3).
    int d2 = q.dequeue().value;  // Dequeue second (should be 7).
    int d3 = q.dequeue().value;  // Dequeue third (should be 1).
    std::cout << "enqueue [3,7,1] => peek=" << front << ", dequeues=[" << d1 << "," << d2 << "," << d3 << "]\n";  // Print demo line.
}  // End printFifoDemo().

int main(int argc, char** argv) {  // Entry point for demo CLI.
    std::vector<int> ms = parseMsOrDefault(argc, argv);  // Parse m values or use defaults.
    std::vector<int> ns = std::vector<int>{0, 1, 2, 4, 8, 16};  // Fixed n list for dequeue costs.
    std::cout << "=== Enqueue Growth (m enqueues) ===\n";  // Print section title.
    printEnqueueGrowthTable(ms);  // Print enqueue growth table.
    std::cout << "\n";  // Blank line.
    std::cout << "=== Dequeue Shift Cost (should be 0) ===\n";  // Print section title.
    printDequeueCostTable(ns);  // Print dequeue moved table.
    std::cout << "\n";  // Blank line.
    std::cout << "=== FIFO demo ===\n";  // Print section title.
    printFifoDemo();  // Print FIFO demo line.
    return 0;  // Exit success.
}  // Close main().

