// 01 AVL Tree 測試（C++）/ Tests for AVL tree (C++).  // Bilingual file header.

#include "AvlTree.hpp"  // Include the API under test.

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

static void assertLessEqual(int upperBound, int actual, const char* message) {  // Minimal <= assertion helper.
    if (actual > upperBound) {  // Fail when value is larger than bound.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertLessEqual().

static void testEmptyTreeProperties() {  // Empty tree should follow our edge-based height definition.
    avlunit::AvlTree t = avlunit::AvlTree::fromValues({});  // Build an empty AVL tree.
    assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.
    assertEquals(0, t.size(), "size should be 0");  // Validate size.
    assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.
    assertOptionalEquals(std::nullopt, t.min(), "min should be nullopt for empty");  // Validate min.
    assertOptionalEquals(std::nullopt, t.max(), "max should be nullopt for empty");  // Validate max.
    assertVectorEquals({}, t.inorder(), "inorder should be empty");  // Validate inorder.
    assertTrue(t.validate(), "empty tree should be valid");  // Validate invariants.
    assertTrue(!t.contains(1), "contains should be false on empty");  // Validate contains.
    assertTrue(!t.remove(1), "remove should be false when key is missing");  // Validate delete missing.
}  // Close testEmptyTreeProperties().

static void testInsertContainsAndDuplicates() {  // Insert should reject duplicates and contains should reflect membership.
    avlunit::AvlTree t;  // Start from an empty tree.
    assertTrue(t.insert(5), "first insert should succeed");  // Insert 5.
    assertTrue(t.contains(5), "contains should find inserted key");  // Validate contains.
    assertTrue(!t.insert(5), "duplicate insert should be rejected");  // Duplicate insert.
    assertEquals(1, t.size(), "size should remain 1 after duplicate");  // Validate size.
    assertTrue(t.validate(), "tree should remain valid after duplicate attempt");  // Validate invariants.
    assertTrue(!t.contains(99), "contains should be false for missing key");  // Validate missing.
}  // Close testInsertContainsAndDuplicates().

static void testRotationPatterns() {  // Classic 3-key patterns should produce a perfectly balanced tree.
    std::vector<std::vector<int>> sequences = {  // Define insert sequences for each rotation pattern.
        {3, 2, 1},  // LL case.
        {1, 2, 3},  // RR case.
        {3, 1, 2},  // LR case.
        {1, 3, 2},  // RL case.
    };  // Close sequences list.

    for (const auto& seq : sequences) {  // Check each scenario.
        avlunit::AvlTree t = avlunit::AvlTree::fromValues(seq);  // Build AVL tree from sequence.
        assertTrue(t.validate(), "tree should be valid after inserts");  // Validate invariants.
        assertVectorEquals(std::vector<int>{1, 2, 3}, t.inorder(), "inorder should be sorted");  // inorder must stay sorted.
        assertEquals(1, t.height(), "height should be 1 for 3 nodes");  // Height should be 1 in edges.
    }  // Close loop.
}  // Close testRotationPatterns().

static void testInorderSortedAndHeightBound() {  // AVL should keep height small while preserving sorted inorder.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic insert order from many textbooks.
    avlunit::AvlTree t = avlunit::AvlTree::fromValues(values);  // Build AVL tree by inserting in order.
    assertEquals(9, t.size(), "size should be 9");  // Validate size.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 7, 8, 10, 13, 14}, t.inorder(), "inorder should be sorted");  // Validate inorder.
    assertOptionalEquals(1, t.min(), "min should be 1");  // Validate min.
    assertOptionalEquals(14, t.max(), "max should be 14");  // Validate max.
    assertTrue(t.validate(), "tree should satisfy AVL invariants");  // Validate invariants.
    assertLessEqual(3, t.height(), "for 9 nodes, AVL height (edges) must be <= 3");  // Bound check.
}  // Close testInorderSortedAndHeightBound().

static void testDeleteCasesAndRootDelete() {  // Delete should rebalance and keep inorder sorted.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Build a tree with multiple delete cases.
    avlunit::AvlTree t = avlunit::AvlTree::fromValues(values);  // Build AVL tree by insertion.
    assertTrue(t.validate(), "initial tree should be valid");  // Sanity-check initial validity.

    assertTrue(t.remove(7), "remove leaf (7) should succeed");  // Delete leaf node.
    assertTrue(!t.contains(7), "deleted key (7) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 8, 10, 13, 14}, t.inorder(), "inorder after deleting 7 should match");  // Validate inorder.
    assertTrue(t.validate(), "tree should remain valid after deleting 7");  // Validate invariants.

    assertTrue(t.remove(14), "remove one-child node (14) should succeed");  // Delete one-child node.
    assertTrue(!t.contains(14), "deleted key (14) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 14 should match");  // Validate inorder.
    assertTrue(t.validate(), "tree should remain valid after deleting 14");  // Validate invariants.

    assertTrue(t.remove(3), "remove two-child node (3) should succeed");  // Delete two-child node.
    assertTrue(!t.contains(3), "deleted key (3) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 3 should match");  // Validate inorder.
    assertTrue(t.validate(), "tree should remain valid after deleting 3");  // Validate invariants.
    assertLessEqual(2, t.height(), "with 6 nodes, AVL height (edges) must be <= 2");  // Bound check.

    avlunit::AvlTree t2 = avlunit::AvlTree::fromValues(values);  // Build a fresh tree for root deletion test.
    assertTrue(t2.remove(8), "remove root (8) should succeed");  // Delete original root.
    assertTrue(!t2.contains(8), "deleted root key (8) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 7, 10, 13, 14}, t2.inorder(), "inorder should remain sorted without 8");  // Validate inorder.
    assertTrue(t2.validate(), "tree should remain valid after root deletion");  // Validate invariants.
}  // Close testDeleteCasesAndRootDelete().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testEmptyTreeProperties();  // Run empty tree tests.
        testInsertContainsAndDuplicates();  // Run insert/contains/duplicate tests.
        testRotationPatterns();  // Run rotation-pattern tests.
        testInorderSortedAndHeightBound();  // Run inorder/min/max/height bound tests.
        testDeleteCasesAndRootDelete();  // Run delete tests.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

