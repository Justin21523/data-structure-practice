// 03 樹走訪示範（C++）/ Tree traversal demo (C++).  // Bilingual file header.

#include "TreeTraversal.hpp"  // Include the implementation for this unit.

#include <iostream>  // Provide std::cout for demo output.
#include <optional>  // Provide std::optional for building level-order inputs.
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
    std::vector<Opt> values = {1, 2, 3, 4, 5};  // Sample tree used across this repo for traversal examples.
    traversalunit::BinaryTree tree = traversalunit::BinaryTree::fromLevelOrder(values);  // Build tree from level-order list.
    traversalunit::TraversalSummary s = tree.summarize();  // Summarize all traversal variants.

    std::cout << "=== Tree Traversal Demo (C++) ===\n";  // Print header.
    std::cout << "level_order input: [1, 2, 3, 4, 5]\n";  // Print input (fixed for this demo).
    std::cout << "preorder_recursive=" << formatVector(s.preorderRecursive) << "\n";  // Print recursive preorder.
    std::cout << "preorder_iterative=" << formatVector(s.preorderIterative) << "\n";  // Print iterative preorder.
    std::cout << "inorder_recursive=" << formatVector(s.inorderRecursive) << "\n";  // Print recursive inorder.
    std::cout << "inorder_iterative=" << formatVector(s.inorderIterative) << "\n";  // Print iterative inorder.
    std::cout << "postorder_recursive=" << formatVector(s.postorderRecursive) << "\n";  // Print recursive postorder.
    std::cout << "postorder_iterative=" << formatVector(s.postorderIterative) << "\n";  // Print iterative postorder.
    std::cout << "level_order=" << formatVector(s.levelOrder) << "\n";  // Print level-order BFS.
    return 0;  // Exit success.
}  // Close main().

