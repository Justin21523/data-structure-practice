// 04 雙向鏈結串列示範（C++）/ Doubly linked list demo (C++).  // Bilingual file header.

#include "DoublyLinkedList.hpp"  // Import the list implementation for the demo.

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

static void printPushPopTable(const std::vector<int>& ns) {  // Print push/pop costs table.
    std::cout << "=== push/pop hops at both ends ===\n";  // Print section title.
    std::cout << "     n |  pushB |   popB |  pushF |   popF\n";  // Print header.
    std::cout << "-------------------------------------------------\n";  // Print separator.
    for (int n : ns) {  // Render one row per n.
        DoublyLinkedList a = buildOrderedList(n);  // Build list of size n.
        int pushB = a.pushBack(999).hops;  // Measure pushBack hops.
        int popB = (a.size() > 0) ? a.popBack().cost.hops : 0;  // Measure popBack hops (handle n=0).

        DoublyLinkedList b = buildOrderedList(n);  // Build list of size n.
        int pushF = b.pushFront(999).hops;  // Measure pushFront hops.
        int popF = (b.size() > 0) ? b.popFront().cost.hops : 0;  // Measure popFront hops (handle n=0).

        std::cout << std::string(6 - std::to_string(n).size(), ' ') << n  // Print n with right alignment.
                  << " | " << std::string(6 - std::to_string(pushB).size(), ' ') << pushB  // Print pushB.
                  << " | " << std::string(6 - std::to_string(popB).size(), ' ') << popB  // Print popB.
                  << " | " << std::string(6 - std::to_string(pushF).size(), ' ') << pushF  // Print pushF.
                  << " | " << std::string(6 - std::to_string(popF).size(), ' ') << popF  // Print popF.
                  << "\n";  // End row.
    }  // Close loop.
}  // End printPushPopTable.

static void printGetHopsTable() {  // Print get(i) hops for a fixed list size.
    DoublyLinkedList lst = buildOrderedList(16);  // Build ordered list [0..15].
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
    printPushPopTable(ns);  // Print push/pop table.
    printGetHopsTable();  // Print get-cost table.
    return 0;  // Indicate success.
}  // End main.

