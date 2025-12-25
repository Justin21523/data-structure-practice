// 02 二元搜尋樹測試（C++）/ Tests for BST (C++).  // Bilingual file header.

#include "BinarySearchTree.hpp"  // Include the API under test.

#include <iostream>  // Provide std::cout/std::cerr for status output.
#include <optional>  // Provide std::optional for min/max expectations.
#include <stdexcept>  // Provide std::runtime_error for test failures.
#include <vector>  // Provide std::vector for expected inorder sequences.

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

static void assertOptionalEquals(const std::optional<int>& expected, const std::optional<int>& actual, const char* message) {  // Minimal optional equality helper.
    if (expected.has_value() != actual.has_value()) {  // Fail on presence mismatch.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close presence check.
    if (expected.has_value() && *expected != *actual) {  // Fail on value mismatch.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close value check.
}  // Close assertOptionalEquals().

static void assertVectorEquals(const std::vector<int>& expected, const std::vector<int>& actual, const char* message) {  // Minimal vector equality helper.
    if (expected != actual) {  // Fail when vectors differ.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertVectorEquals().

static void testEmptyTreeProperties() {  // Empty tree should follow our edge-based height definition.
    bstunit::BinarySearchTree t = bstunit::BinarySearchTree::fromValues({});  // Build an empty BST.
    assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.
    assertEquals(0, t.size(), "size should be 0");  // Validate size.
    assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.
    assertOptionalEquals(std::nullopt, t.min(), "min should be nullopt for empty");  // Validate min.
    assertOptionalEquals(std::nullopt, t.max(), "max should be nullopt for empty");  // Validate max.
    assertVectorEquals({}, t.inorder(), "inorder should be empty");  // Validate inorder.
    assertTrue(!t.contains(1), "contains should be false on empty");  // Validate contains.
    assertTrue(!t.remove(1), "remove should be false when key is missing");  // Validate delete missing.
}  // Close testEmptyTreeProperties().

static void testInsertContainsAndDuplicates() {  // Insert should reject duplicates and contains should reflect membership.
    bstunit::BinarySearchTree t;  // Start from an empty tree.
    assertTrue(t.insert(5), "first insert should succeed");  // Insert 5.
    assertTrue(t.contains(5), "contains should find inserted key");  // Validate contains.
    assertTrue(!t.insert(5), "duplicate insert should be rejected");  // Duplicate insert.
    assertEquals(1, t.size(), "size should remain 1 after duplicate");  // Validate size.
    assertTrue(!t.contains(99), "contains should be false for missing key");  // Validate missing.
}  // Close testInsertContainsAndDuplicates().

static void testInorderSortedAndMinMax() {  // Inorder traversal should be sorted for a valid BST.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic BST insert order.
    bstunit::BinarySearchTree t = bstunit::BinarySearchTree::fromValues(values);  // Build BST by inserting in order.
    assertEquals(9, t.size(), "size should be 9");  // Validate size.
    assertEquals(3, t.height(), "height should be 3");  // Validate height.
    assertOptionalEquals(1, t.min(), "min should be 1");  // Validate min.
    assertOptionalEquals(14, t.max(), "max should be 14");  // Validate max.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 7, 8, 10, 13, 14}, t.inorder(), "inorder should be sorted");  // Validate inorder.
}  // Close testInorderSortedAndMinMax().

static void testDeleteLeafOneChildTwoChildren() {  // Delete should handle all three structural cases.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Build a tree with multiple delete cases.
    bstunit::BinarySearchTree t = bstunit::BinarySearchTree::fromValues(values);  // Build BST by insertion.

    assertTrue(t.remove(7), "remove leaf (7) should succeed");  // Delete leaf node.
    assertTrue(!t.contains(7), "deleted key (7) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 8, 10, 13, 14}, t.inorder(), "inorder after deleting 7 should match");  // Validate inorder.

    assertTrue(t.remove(14), "remove one-child node (14) should succeed");  // Delete one-child node.
    assertTrue(!t.contains(14), "deleted key (14) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 14 should match");  // Validate inorder.

    assertTrue(t.remove(3), "remove two-child node (3) should succeed");  // Delete two-child node.
    assertTrue(!t.contains(3), "deleted key (3) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 3 should match");  // Validate inorder.
    assertEquals(2, t.height(), "height should be 2 after deletions");  // Validate height change.
    assertOptionalEquals(1, t.min(), "min should remain 1");  // Validate min.
    assertOptionalEquals(13, t.max(), "max should become 13");  // Validate max after deleting 14.
}  // Close testDeleteLeafOneChildTwoChildren().

static void testDeleteRootTwoChildren() {  // Deleting the root with two children should work and keep BST order.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Root (8) has two children.
    bstunit::BinarySearchTree t = bstunit::BinarySearchTree::fromValues(values);  // Build BST by insertion.
    assertTrue(t.remove(8), "remove root (8) should succeed");  // Delete root.
    assertTrue(!t.contains(8), "deleted root key (8) should be missing");  // Validate deletion.
    assertEquals(8, t.size(), "size should be 8 after deleting root");  // Validate size.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 7, 10, 13, 14}, t.inorder(), "inorder should remain sorted without 8");  // Validate inorder.
    assertOptionalEquals(1, t.min(), "min should stay 1");  // Validate min.
    assertOptionalEquals(14, t.max(), "max should stay 14");  // Validate max.
}  // Close testDeleteRootTwoChildren().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testEmptyTreeProperties();  // Run empty tree tests.
        testInsertContainsAndDuplicates();  // Run insert/contains/duplicate tests.
        testInorderSortedAndMinMax();  // Run inorder/min/max tests.
        testDeleteLeafOneChildTwoChildren();  // Run delete case tests.
        testDeleteRootTwoChildren();  // Run root deletion test.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

