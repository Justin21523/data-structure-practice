// 02 Red-Black Tree 測試（C++，LLRB）/ Tests for red-black tree (C++, LLRB).  // Bilingual file header.

#include "RedBlackTree.hpp"  // Include the API under test.

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
    rbtunit::RedBlackTree t = rbtunit::RedBlackTree::fromValues({});  // Build an empty tree.
    assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.
    assertEquals(0, t.size(), "size should be 0");  // Validate size.
    assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.
    assertOptionalEquals(std::nullopt, t.min(), "min should be nullopt for empty");  // Validate min.
    assertOptionalEquals(std::nullopt, t.max(), "max should be nullopt for empty");  // Validate max.
    assertVectorEquals({}, t.inorder(), "inorder should be empty");  // Validate inorder.
    assertTrue(t.validate(), "empty tree should be valid");  // Empty tree is valid.
    assertTrue(!t.contains(1), "contains should be false on empty");  // Validate contains.
    assertTrue(!t.remove(1), "remove should be false when key is missing");  // Validate delete missing.
}  // Close testEmptyTreeProperties().

static void testInsertContainsAndDuplicates() {  // Insert should reject duplicates and contains should reflect membership.
    rbtunit::RedBlackTree t;  // Start from an empty tree.
    assertTrue(t.insert(5), "first insert should succeed");  // Insert 5.
    assertTrue(t.contains(5), "contains should find inserted key");  // Validate contains.
    assertTrue(!t.insert(5), "duplicate insert should be rejected");  // Duplicate insert.
    assertEquals(1, t.size(), "size should remain 1 after duplicate");  // Validate size.
    assertTrue(t.validate(), "tree should remain valid after duplicate attempt");  // Validate invariants.
    assertTrue(!t.contains(99), "contains should be false for missing key");  // Validate missing.
}  // Close testInsertContainsAndDuplicates().

static void testAscendingInsertsHeightBound() {  // Ascending inserts should still keep height logarithmic.
    std::vector<int> values;  // Build 1..15 input array.
    for (int i = 1; i <= 15; i++) {  // Fill with increasing values.
        values.push_back(i);  // Append i.
    }  // Close fill loop.
    rbtunit::RedBlackTree t = rbtunit::RedBlackTree::fromValues(values);  // Build tree by inserting in order.
    assertVectorEquals(values, t.inorder(), "inorder should be sorted for ascending inserts");  // inorder must stay sorted.
    assertTrue(t.validate(), "tree should satisfy red-black invariants");  // Validate invariants.
    assertLessEqual(7, t.height(), "for n=15, red-black height (edges) must be <= 7");  // Height bound check.
}  // Close testAscendingInsertsHeightBound().

static void testDeleteCasesAndRootDelete() {  // Delete should keep inorder sorted and invariants valid.
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic insert order from many textbooks.
    rbtunit::RedBlackTree t = rbtunit::RedBlackTree::fromValues(values);  // Build tree by inserting in order.
    assertTrue(t.validate(), "initial tree should be valid");  // Sanity-check initial validity.

    assertTrue(!t.remove(999), "remove missing key should return false");  // Deleting missing key should fail.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 7, 8, 10, 13, 14}, t.inorder(), "inorder should be unchanged after missing delete");  // inorder unchanged.
    assertTrue(t.validate(), "tree should remain valid after missing delete");  // Tree should remain valid.

    assertTrue(t.remove(7), "remove leaf (7) should succeed");  // Delete a leaf node.
    assertTrue(!t.contains(7), "deleted key (7) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 8, 10, 13, 14}, t.inorder(), "inorder after deleting 7 should match");  // Validate inorder.
    assertTrue(t.validate(), "tree should remain valid after deleting 7");  // Validate invariants.

    assertTrue(t.remove(14), "remove one-child node (14) should succeed");  // Delete a one-child node in this data set.
    assertTrue(!t.contains(14), "deleted key (14) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 14 should match");  // Validate inorder.
    assertTrue(t.validate(), "tree should remain valid after deleting 14");  // Validate invariants.

    assertTrue(t.remove(3), "remove two-child node (3) should succeed");  // Delete a two-child node.
    assertTrue(!t.contains(3), "deleted key (3) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 3 should match");  // Validate inorder.
    assertTrue(t.validate(), "tree should remain valid after deleting 3");  // Validate invariants.

    rbtunit::RedBlackTree t2 = rbtunit::RedBlackTree::fromValues(values);  // Build a fresh tree for root deletion test.
    assertTrue(t2.remove(8), "remove root (8) should succeed");  // Delete original root.
    assertTrue(!t2.contains(8), "deleted root key (8) should be missing");  // Validate deletion.
    assertVectorEquals(std::vector<int>{1, 3, 4, 6, 7, 10, 13, 14}, t2.inorder(), "inorder should remain sorted without 8");  // Validate inorder.
    assertTrue(t2.validate(), "tree should remain valid after root deletion");  // Validate invariants.
}  // Close testDeleteCasesAndRootDelete().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testEmptyTreeProperties();  // Run empty tree tests.
        testInsertContainsAndDuplicates();  // Run insert/contains/duplicate tests.
        testAscendingInsertsHeightBound();  // Run height bound test.
        testDeleteCasesAndRootDelete();  // Run delete tests.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

