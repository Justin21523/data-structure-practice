// 03 單向鏈結串列示範（C++）/ Singly linked list demo (C++).  // Bilingual file header.

#include "SinglyLinkedList.hpp"  // Import the list implementation for the demo.

#include <iostream>  // Provide std::cout for CLI output.
#include <string>  // Provide std::string for argument parsing.
#include <vector>  // Provide std::vector for holding demo sizes.

static std::vector<int> parseNs(int argc, char** argv) {  // Parse CLI args into n values with defaults.
    if (argc <= 1) {  // Use defaults when no args provided.
        return std::vector<int>{0, 1, 2, 4, 8, 16};  // Default sizes.
    }  // Close default branch.
    std::vector<int> ns;  // Accumulate parsed sizes.
    for (int i = 1; i < argc; i += 1) {  // Parse each argument token.
        ns.push_back(std::stoi(std::string(argv[i])));  // Convert token to int (throws on invalid input).
    }  // Close loop.
    return ns;  // Return parsed list.
}  // End parseNs.

static void printPushFrontVsBackTable(const std::vector<int>& ns) {  // Print pushFront vs pushBack hops table.
    std::cout << "=== pushFront vs pushBack hops at size n ===\n";  // Print section title.
    std::cout << "     n | frontH |  backH\n";  // Print header.
    std::cout << "------------------------------\n";  // Print separator.
    for (int n : ns) {  // Render one row per n.
        SinglyLinkedList a = buildOrderedList(n);  // Build list of size n.
        OperationCost front = a.pushFront(999);  // Measure head insertion.
        SinglyLinkedList b = buildOrderedList(n);  // Build another list of size n.
        OperationCost back = b.pushBack(999);  // Measure tail insertion.
        std::cout << std::string(6 - std::to_string(n).size(), ' ') << n  // Print n with right alignment.
                  << " | " << std::string(6 - std::to_string(front.hops).size(), ' ') << front.hops  // Print front hops.
                  << " | " << std::string(6 - std::to_string(back.hops).size(), ' ') << back.hops  // Print back hops.
                  << "\n";  // End row.
    }  // Close loop.
}  // End printPushFrontVsBackTable.

static void printGetHopsTable() {  // Print get(i) hops for a fixed list size.
    SinglyLinkedList lst = buildOrderedList(16);  // Build ordered list [0..15].
    std::cout << "\n";  // Blank line between sections.
    std::cout << "=== get(i) hops on list of size 16 ===\n";  // Print section title.
    std::cout << "     i |  value |   hops\n";  // Print header.
    std::cout << "------------------------------\n";  // Print separator.
    std::vector<int> indices{0, 1, 2, 4, 8, 15};  // Fixed indices for readability.
    for (int i : indices) {  // Render one row per index.
        GetResult r = lst.getWithCost(i);  // Fetch value + cost.
        std::cout << std::string(6 - std::to_string(i).size(), ' ') << i  // Print i.
                  << " | " << std::string(6 - std::to_string(r.value).size(), ' ') << r.value  // Print value.
                  << " | " << std::string(6 - std::to_string(r.cost.hops).size(), ' ') << r.cost.hops  // Print hops.
                  << "\n";  // End row.
    }  // Close loop.
}  // End printGetHopsTable.

int main(int argc, char** argv) {  // CLI entry point.
    std::vector<int> ns = parseNs(argc, argv);  // Determine list sizes.
    printPushFrontVsBackTable(ns);  // Print pushFront vs pushBack table.
    printGetHopsTable();  // Print get-cost table.
    return 0;  // Indicate success.
}  // End main.

