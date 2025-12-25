// 01 二元樹測試（C++）/ Tests for binary tree (C++).  // Bilingual file header.

#include "BinaryTree.hpp"  // Include the API under test.

#include <iostream>  // Provide std::cout/std::cerr for status output.
#include <optional>  // Provide std::optional for building level-order inputs.
#include <stdexcept>  // Provide std::runtime_error for test failures.
#include <vector>  // Provide std::vector for expected traversal sequences.

static void assertTrue(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw to signal test failure.
    }  // Close failure branch.
}  // Close assertTrue().

static void assertEquals(int expected, int actual, const char* message) {  // Minimal int equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error(std::string("FAIL: ") + message + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertEquals().

static void assertVectorEquals(const std::vector<int>& expected, const std::vector<int>& actual, const char* message) {  // Minimal vector equality helper.
    if (expected != actual) {  // Fail when vectors differ.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertVectorEquals().

static void testEmptyTreeFromEmptyArray() {  // Empty input should create an empty tree.
    std::vector<std::optional<int>> values = {};  // Empty level-order array.
    binarytreeunit::BinaryTree t = binarytreeunit::BinaryTree::fromLevelOrder(values);  // Build tree.
    assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.
    assertEquals(0, t.size(), "size should be 0");  // Validate size.
    assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.
    assertEquals(0, t.countLeaves(), "leaves should be 0 for empty");  // Validate leaves.
    assertVectorEquals({}, t.preorder(), "preorder should be empty");  // Validate preorder.
    assertVectorEquals({}, t.inorder(), "inorder should be empty");  // Validate inorder.
    assertVectorEquals({}, t.postorder(), "postorder should be empty");  // Validate postorder.
    assertVectorEquals({}, t.levelOrder(), "levelOrder should be empty");  // Validate BFS.
}  // Close testEmptyTreeFromEmptyArray().

static void testSingleNodeTree() {  // Single value should create a one-node tree.
    std::vector<std::optional<int>> values = {1};  // One-node level-order array.
    binarytreeunit::BinaryTree t = binarytreeunit::BinaryTree::fromLevelOrder(values);  // Build tree.
    assertTrue(!t.isEmpty(), "tree should not be empty");  // Validate non-empty.
    assertEquals(1, t.size(), "size should be 1");  // Validate size.
    assertEquals(0, t.height(), "height should be 0 for leaf");  // Validate height.
    assertEquals(1, t.countLeaves(), "leaves should be 1");  // Validate leaves.
    assertVectorEquals(std::vector<int>{1}, t.preorder(), "preorder should be [1]");  // Validate preorder.
    assertVectorEquals(std::vector<int>{1}, t.inorder(), "inorder should be [1]");  // Validate inorder.
    assertVectorEquals(std::vector<int>{1}, t.postorder(), "postorder should be [1]");  // Validate postorder.
    assertVectorEquals(std::vector<int>{1}, t.levelOrder(), "levelOrder should be [1]");  // Validate BFS.
}  // Close testSingleNodeTree().

static void testSampleTreeWithoutHoles() {  // Sample tree should produce expected traversals.
    std::vector<std::optional<int>> values = {1, 2, 3, 4, 5};  // Sample level-order array.
    binarytreeunit::BinaryTree t = binarytreeunit::BinaryTree::fromLevelOrder(values);  // Build tree.
    assertEquals(5, t.size(), "size should be 5");  // Validate size.
    assertEquals(2, t.height(), "height should be 2 (edges)");  // Validate height.
    assertEquals(3, t.countLeaves(), "leaves should be 3");  // Validate leaves.
    assertVectorEquals(std::vector<int>{1, 2, 4, 5, 3}, t.preorder(), "preorder should match");  // Validate preorder.
    assertVectorEquals(std::vector<int>{4, 2, 5, 1, 3}, t.inorder(), "inorder should match");  // Validate inorder.
    assertVectorEquals(std::vector<int>{4, 5, 2, 3, 1}, t.postorder(), "postorder should match");  // Validate postorder.
    assertVectorEquals(std::vector<int>{1, 2, 3, 4, 5}, t.levelOrder(), "levelOrder should match");  // Validate BFS.
}  // Close testSampleTreeWithoutHoles().

static void testTreeWithHoles() {  // Missing nodes (nulls) should be handled correctly.
    using Opt = std::optional<int>;  // Short alias for optional int.
    std::vector<Opt> values = {1, 2, 3, Opt{}, 5, Opt{}, 7};  // Sample with holes.
    binarytreeunit::BinaryTree t = binarytreeunit::BinaryTree::fromLevelOrder(values);  // Build tree with holes.
    assertEquals(5, t.size(), "size should be 5");  // Validate size.
    assertEquals(2, t.height(), "height should be 2 (edges)");  // Validate height.
    assertEquals(2, t.countLeaves(), "leaves should be 2");  // Validate leaves.
    assertVectorEquals(std::vector<int>{1, 2, 5, 3, 7}, t.preorder(), "preorder should match");  // Validate preorder.
    assertVectorEquals(std::vector<int>{2, 5, 1, 3, 7}, t.inorder(), "inorder should match");  // Validate inorder.
    assertVectorEquals(std::vector<int>{5, 2, 7, 3, 1}, t.postorder(), "postorder should match");  // Validate postorder.
    assertVectorEquals(std::vector<int>{1, 2, 3, 5, 7}, t.levelOrder(), "levelOrder should match");  // Validate BFS.
}  // Close testTreeWithHoles().

static void testRootNullBuildsEmptyTree() {  // Null root should produce an empty tree even if later values exist.
    using Opt = std::optional<int>;  // Short alias for optional int.
    std::vector<Opt> values = {Opt{}, 1, 2};  // Root is missing.
    binarytreeunit::BinaryTree t = binarytreeunit::BinaryTree::fromLevelOrder(values);  // Build tree.
    assertTrue(t.isEmpty(), "tree should be empty when root is null");  // Validate emptiness.
    assertEquals(0, t.size(), "size should be 0");  // Validate size.
    assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height.
    assertEquals(0, t.countLeaves(), "leaves should be 0");  // Validate leaves.
    assertVectorEquals({}, t.preorder(), "preorder should be empty");  // Validate preorder.
    assertVectorEquals({}, t.inorder(), "inorder should be empty");  // Validate inorder.
    assertVectorEquals({}, t.postorder(), "postorder should be empty");  // Validate postorder.
    assertVectorEquals({}, t.levelOrder(), "levelOrder should be empty");  // Validate BFS.
}  // Close testRootNullBuildsEmptyTree().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testEmptyTreeFromEmptyArray();  // Run empty input test.
        testSingleNodeTree();  // Run single node test.
        testSampleTreeWithoutHoles();  // Run sample tree test.
        testTreeWithHoles();  // Run hole-handling test.
        testRootNullBuildsEmptyTree();  // Run null-root test.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

