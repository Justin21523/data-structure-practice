// 03 樹走訪測試（C++）/ Tests for tree traversal (C++).  // Bilingual file header.

#include "TreeTraversal.hpp"  // Include the API under test.

#include <iostream>  // Provide std::cout/std::cerr for status output.
#include <optional>  // Provide std::optional for building level-order inputs.
#include <stdexcept>  // Provide std::runtime_error for test failures.
#include <vector>  // Provide std::vector for expected traversal sequences.

static void assertVectorEquals(const std::vector<int>& expected, const std::vector<int>& actual, const char* message) {  // Minimal vector equality helper.
    if (expected != actual) {  // Fail when vectors differ.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertVectorEquals().

static void testEmptyTree() {  // Empty tree should produce empty traversals for all variants.
    std::vector<std::optional<int>> values = {};  // Empty level-order input.
    traversalunit::BinaryTree t = traversalunit::BinaryTree::fromLevelOrder(values);  // Build empty tree.
    assertVectorEquals({}, t.preorderRecursive(), "preorderRecursive should be empty");  // Validate recursive preorder.
    assertVectorEquals({}, t.preorderIterative(), "preorderIterative should be empty");  // Validate iterative preorder.
    assertVectorEquals({}, t.inorderRecursive(), "inorderRecursive should be empty");  // Validate recursive inorder.
    assertVectorEquals({}, t.inorderIterative(), "inorderIterative should be empty");  // Validate iterative inorder.
    assertVectorEquals({}, t.postorderRecursive(), "postorderRecursive should be empty");  // Validate recursive postorder.
    assertVectorEquals({}, t.postorderIterative(), "postorderIterative should be empty");  // Validate iterative postorder.
    assertVectorEquals({}, t.levelOrder(), "levelOrder should be empty");  // Validate level order.
}  // Close testEmptyTree().

static void testSampleTreeMatchesExpected() {  // Sample tree should match known traversal sequences.
    using Opt = std::optional<int>;  // Short alias for optional int.
    std::vector<Opt> values = {1, 2, 3, 4, 5};  // Sample tree used across this repo.
    traversalunit::BinaryTree t = traversalunit::BinaryTree::fromLevelOrder(values);  // Build sample tree.
    std::vector<int> expectedPre = {1, 2, 4, 5, 3};  // Expected preorder.
    std::vector<int> expectedIn = {4, 2, 5, 1, 3};  // Expected inorder.
    std::vector<int> expectedPost = {4, 5, 2, 3, 1};  // Expected postorder.
    std::vector<int> expectedLevel = {1, 2, 3, 4, 5};  // Expected level-order.
    assertVectorEquals(expectedPre, t.preorderRecursive(), "preorderRecursive should match");  // Validate recursive preorder.
    assertVectorEquals(expectedPre, t.preorderIterative(), "preorderIterative should match");  // Validate iterative preorder.
    assertVectorEquals(expectedIn, t.inorderRecursive(), "inorderRecursive should match");  // Validate recursive inorder.
    assertVectorEquals(expectedIn, t.inorderIterative(), "inorderIterative should match");  // Validate iterative inorder.
    assertVectorEquals(expectedPost, t.postorderRecursive(), "postorderRecursive should match");  // Validate recursive postorder.
    assertVectorEquals(expectedPost, t.postorderIterative(), "postorderIterative should match");  // Validate iterative postorder.
    assertVectorEquals(expectedLevel, t.levelOrder(), "levelOrder should match");  // Validate level order.
}  // Close testSampleTreeMatchesExpected().

static void testTreeWithHolesMatchesExpected() {  // Trees with null holes should still traverse correctly.
    using Opt = std::optional<int>;  // Short alias for optional int.
    std::vector<Opt> values = {1, 2, 3, Opt{}, 5, Opt{}, 7};  // Tree with missing children.
    traversalunit::BinaryTree t = traversalunit::BinaryTree::fromLevelOrder(values);  // Build tree with holes.
    std::vector<int> expectedPre = {1, 2, 5, 3, 7};  // Expected preorder.
    std::vector<int> expectedIn = {2, 5, 1, 3, 7};  // Expected inorder.
    std::vector<int> expectedPost = {5, 2, 7, 3, 1};  // Expected postorder.
    std::vector<int> expectedLevel = {1, 2, 3, 5, 7};  // Expected level-order.
    assertVectorEquals(expectedPre, t.preorderRecursive(), "preorderRecursive (holes) should match");  // Validate preorder.
    assertVectorEquals(expectedPre, t.preorderIterative(), "preorderIterative (holes) should match");  // Validate preorder.
    assertVectorEquals(expectedIn, t.inorderRecursive(), "inorderRecursive (holes) should match");  // Validate inorder.
    assertVectorEquals(expectedIn, t.inorderIterative(), "inorderIterative (holes) should match");  // Validate inorder.
    assertVectorEquals(expectedPost, t.postorderRecursive(), "postorderRecursive (holes) should match");  // Validate postorder.
    assertVectorEquals(expectedPost, t.postorderIterative(), "postorderIterative (holes) should match");  // Validate postorder.
    assertVectorEquals(expectedLevel, t.levelOrder(), "levelOrder (holes) should match");  // Validate level order.
}  // Close testTreeWithHolesMatchesExpected().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testEmptyTree();  // Run empty tree tests.
        testSampleTreeMatchesExpected();  // Run sample tree tests.
        testTreeWithHolesMatchesExpected();  // Run hole-handling tests.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

