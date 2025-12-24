// 05 環狀鏈結串列示範（C++）/ Circular linked list demo (C++).  // Bilingual file header.

#include "CircularLinkedList.hpp"  // Import the list implementation for the demo.

#include <iostream>  // Provide std::cout for CLI output.
#include <string>  // Provide std::string for argument parsing.
#include <vector>  // Provide std::vector for holding demo sizes.

static std::vector<int> parseNs(int argc, char** argv) {  // Parse CLI args into n values with defaults.
    if (argc <= 1) {  // Use defaults when no args provided.
        return std::vector<int>{0, 1, 2, 4, 8};  // Default sizes.
    }  // Close default branch.
    std::vector<int> ns;  // Accumulate parsed sizes.
    for (int i = 1; i < argc; i += 1) {  // Parse each argument token.
        ns.push_back(std::stoi(std::string(argv[i])));  // Convert token to int (throws on invalid input).
    }  // Close loop.
    return ns;  // Return parsed list.
}  // End parseNs.

static void printBasicCostTable(const std::vector<int>& ns) {  // Print basic push/pop cost table.
    std::cout << "=== basic costs (push/pop) ===\n";  // Print section title.
    std::cout << "     n |  pushB |  pushF |   popF |   popB\n";  // Print header.
    std::cout << "-------------------------------------------\n";  // Print separator.
    for (int n : ns) {  // Render one row per n.
        CircularLinkedList a = buildOrderedList(n);  // Build list of size n.
        int pushB = a.pushBack(999).hops;  // pushBack cost (0).
        CircularLinkedList b = buildOrderedList(n);  // Build list of size n.
        int pushF = b.pushFront(999).hops;  // pushFront cost (0).
        CircularLinkedList c = buildOrderedList(n);  // Build list of size n.
        int popF = (c.size() > 0) ? c.popFront().cost.hops : 0;  // popFront cost (0; handle n=0).
        CircularLinkedList d = buildOrderedList(n);  // Build list of size n.
        int popB = (d.size() > 0) ? d.popBack().cost.hops : 0;  // popBack cost (n-2; handle n=0).

        std::cout << std::string(6 - std::to_string(n).size(), ' ') << n  // Print n.
                  << " | " << std::string(6 - std::to_string(pushB).size(), ' ') << pushB  // Print pushB.
                  << " | " << std::string(6 - std::to_string(pushF).size(), ' ') << pushF  // Print pushF.
                  << " | " << std::string(6 - std::to_string(popF).size(), ' ') << popF  // Print popF.
                  << " | " << std::string(6 - std::to_string(popB).size(), ' ') << popB  // Print popB.
                  << "\n";  // End row.
    }  // Close loop.
}  // End printBasicCostTable.

static void printRotateDemo() {  // Print a small rotate demo on n=5.
    std::cout << "\n";  // Blank line.
    std::cout << "=== rotate demo on n=5 ===\n";  // Print section title.
    std::cout << " steps |   hops |   head |                 list\n";  // Print header.
    std::cout << "-------------------------------------------\n";  // Print separator.
    CircularLinkedList lst = buildOrderedList(5);  // Build [0..4].
    std::vector<int> stepsList{0, 1, 2, 5, 7};  // Steps list (cumulative).
    for (int steps : stepsList) {  // Iterate steps.
        OperationCost cost = lst.rotate(steps);  // Rotate list.
        std::vector<int> snapshot = lst.toVector();  // Snapshot list.
        int headValue = snapshot.empty() ? -1 : snapshot[0];  // Read head value.
        std::cout << std::string(6 - std::to_string(steps).size(), ' ') << steps  // Print steps.
                  << " | " << std::string(6 - std::to_string(cost.hops).size(), ' ') << cost.hops  // Print hops.
                  << " | " << std::string(6 - std::to_string(headValue).size(), ' ') << headValue  // Print head.
                  << " | [";  // Start list snapshot.
        for (std::size_t i = 0; i < snapshot.size(); i += 1) {  // Print values.
            std::cout << snapshot[i] << (i + 1 == snapshot.size() ? "" : ", ");  // Print one value.
        }  // Close loop.
        std::cout << "]\n";  // Close list snapshot.
    }  // Close loop.
}  // End printRotateDemo.

int main(int argc, char** argv) {  // CLI entry point.
    std::vector<int> ns = parseNs(argc, argv);  // Determine list sizes.
    printBasicCostTable(ns);  // Print basic cost table.
    printRotateDemo();  // Print rotate demo.
    return 0;  // Indicate success.
}  // End main.

