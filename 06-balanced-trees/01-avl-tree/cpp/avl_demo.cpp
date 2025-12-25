// 01 AVL Tree 示範（C++）/ AVL tree demo (C++).  // Bilingual file header.

#include "AvlTree.hpp"  // Include the implementation for this unit.

#include <iostream>  // Provide std::cout for demo output.
#include <optional>  // Provide std::optional for printing min/max.
#include <sstream>  // Provide std::ostringstream for simple formatting helpers.
#include <vector>  // Provide std::vector for local containers.

static std::string formatVector(const std::vector<int>& values) {  // Format vector as [a, b, c].
    std::ostringstream oss;  // Build output string incrementally.
    oss << "[";  // Print opening bracket.
    for (size_t i = 0; i < values.size(); i++) {  // Print each element.
        if (i > 0) {  // Print separator after first element.
            oss << ", ";  // Print comma+space.
        }  // Close separator branch.
        oss << values[i];  // Print one value.
    }  // Close loop.
    oss << "]";  // Print closing bracket.
    return oss.str();  // Return formatted string.
}  // Close formatVector().

static std::string formatOptionalInt(const std::optional<int>& v) {  // Format optional int as number or "null".
    if (!v.has_value()) {  // Empty optional means missing value.
        return "null";  // Return null marker.
    }  // Close empty case.
    return std::to_string(*v);  // Return numeric string.
}  // Close formatOptionalInt().

int main() {  // CLI entry point for the demo.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic textbook insert order.
    avlunit::AvlTree tree = avlunit::AvlTree::fromValues(values);  // Build AVL tree by inserting in order.
    avlunit::TreeSummary s = tree.summarize();  // Summarize properties and inorder traversal.

    std::cout << "=== AVL Tree Demo (C++) ===\n";  // Print header.
    std::cout << "insert order: " << formatVector(values) << "\n";  // Print insertion order.
    std::cout << "size=" << s.size << ", height=" << s.height << ", min=" << formatOptionalInt(s.minValue) << ", max=" << formatOptionalInt(s.maxValue) << ", valid=" << (s.isValid ? "true" : "false") << "\n";  // Print key properties.
    std::cout << "inorder(sorted)=" << formatVector(s.inorder) << "\n";  // Print inorder traversal.
    std::cout << "contains(6)=" << (tree.contains(6) ? "true" : "false") << ", contains(99)=" << (tree.contains(99) ? "true" : "false") << "\n";  // Print search examples.

    tree.remove(7);  // Delete a leaf (7).
    tree.remove(14);  // Delete a one-child node (14 has left child 13).
    tree.remove(3);  // Delete a two-child node (3) and rebalance.
    std::cout << "after deletes (7,14,3) inorder=" << formatVector(tree.inorder()) << ", valid=" << (tree.validate() ? "true" : "false") << "\n";  // Show inorder after deletions.
    return 0;  // Exit success.
}  // Close main().

