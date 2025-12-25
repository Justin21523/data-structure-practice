// 01 二元樹示範（C++）/ Binary tree demo (C++).  // Bilingual file header.

#include "BinaryTree.hpp"  // Include the implementation for this unit.

#include <iostream>  // Provide std::cout for demo output.
#include <optional>  // Provide std::optional for null entries.
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

int main() {  // CLI entry point for the demo.
    using Opt = std::optional<int>;  // Short alias for optional int.
    std::vector<Opt> values = {1, 2, 3, 4, 5};  // Sample tree: 1 with children 2 and 3; 2 has 4 and 5.

    binarytreeunit::BinaryTree tree = binarytreeunit::BinaryTree::fromLevelOrder(values);  // Build tree from level-order array.
    binarytreeunit::TreeSummary s = tree.summarize();  // Summarize properties and traversals.

    std::cout << "=== Binary Tree Demo (C++) ===\n";  // Print header.
    std::cout << "level_order input: [1, 2, 3, 4, 5]\n";  // Print input (fixed for this demo).
    std::cout << "size=" << s.size << ", height=" << s.height << ", leaves=" << s.leaves << "\n";  // Print key properties.
    std::cout << "preorder=" << formatVector(s.preorder) << "\n";  // Print preorder.
    std::cout << "inorder=" << formatVector(s.inorder) << "\n";  // Print inorder.
    std::cout << "postorder=" << formatVector(s.postorder) << "\n";  // Print postorder.
    std::cout << "level_order=" << formatVector(s.levelOrder) << "\n";  // Print level-order.
    return 0;  // Exit success.
}  // Close main().

