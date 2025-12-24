// 01 靜態陣列測試（C++）/ Tests for static array (C++).  // Bilingual test file header.
#include "FixedArray.hpp"  // Include the API under test.

#include <iostream>  // Use std::cout for test output.
#include <stdexcept>  // Use std::runtime_error for assertion failures.
#include <vector>  // Use std::vector for expected sequences.

static void assertTrue(bool condition, const std::string& message) {  // Minimal boolean assertion helper.
    if (!condition) {  // Fail when condition is false.
        throw std::runtime_error("FAIL: " + message);  // Throw to signal test failure.
    }  // Close failure branch.
}  // End assertTrue.

static void assertEqLL(long long expected, long long actual, const std::string& message) {  // Minimal long long equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error("FAIL: " + message + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Throw mismatch.
    }  // Close failure branch.
}  // End assertEqLL.

static void assertVecEq(const std::vector<int>& expected, const std::vector<int>& actual, const std::string& message) {  // Vector equality assertion.
    if (expected != actual) {  // Fail when vectors differ.
        throw std::runtime_error("FAIL: " + message);  // Throw with message (values are easy to inspect in a debugger).
    }  // Close failure branch.
}  // End assertVecEq.

static void testAppendGetToVector() {  // Append should place values in order and return moved=0.
    fixedarray::FixedArray a(3);  // Create small array.
    assertEqLL(0, a.append(10), "append moved should be 0");  // Append at end moves 0.
    assertEqLL(0, a.append(20), "append moved should be 0");  // Append at end moves 0.
    assertEqLL(10, a.get(0), "get(0) should be 10");  // Validate first element.
    assertEqLL(20, a.get(1), "get(1) should be 20");  // Validate second element.
    assertVecEq(std::vector<int>({10, 20}), a.toVector(), "toVector should match values");  // Validate ordering.
}  // End testAppendGetToVector.

static void testSetUpdatesValue() {  // set should update in-place.
    fixedarray::FixedArray a(2);  // Create array.
    a.append(1);  // Fill.
    a.append(2);  // Fill.
    a.set(1, 99);  // Update.
    assertVecEq(std::vector<int>({1, 99}), a.toVector(), "set should update element");  // Validate.
}  // End testSetUpdatesValue.

static void testInsertAtShiftsRightAndCountsMoves() {  // insertAt should shift right and count moves.
    fixedarray::FixedArray a(5);  // Capacity for insertion.
    a.append(1);  // [1]
    a.append(2);  // [1,2]
    a.append(3);  // [1,2,3]
    int moved = a.insertAt(1, 99);  // Insert at index 1; should move 2 elements.
    assertEqLL(2, moved, "insertAt moved should equal size-index");  // Validate move count.
    assertVecEq(std::vector<int>({1, 99, 2, 3}), a.toVector(), "insertAt should shift right");  // Validate ordering.
}  // End testInsertAtShiftsRightAndCountsMoves.

static void testRemoveAtShiftsLeftAndCountsMoves() {  // removeAt should shift left and count moves.
    fixedarray::FixedArray a(4);  // Capacity 4.
    a.append(10);  // Fill.
    a.append(20);  // Fill.
    a.append(30);  // Fill.
    a.append(40);  // Fill.
    fixedarray::RemoveResult r = a.removeAt(1);  // Remove 20; should move 2 elements.
    assertEqLL(20, r.value, "removed value should be returned");  // Validate removed value.
    assertEqLL(2, r.moved, "removeAt moved should equal size-index-1");  // Validate moved count.
    assertVecEq(std::vector<int>({10, 30, 40}), a.toVector(), "removeAt should shift left");  // Validate ordering.
}  // End testRemoveAtShiftsLeftAndCountsMoves.

static void testIndexOf() {  // indexOf should find first match or -1.
    fixedarray::FixedArray a(5);  // Create array.
    a.append(7);  // [7]
    a.append(8);  // [7,8]
    a.append(7);  // [7,8,7]
    assertEqLL(0, a.indexOf(7), "indexOf should return first occurrence");  // Validate first match.
    assertEqLL(1, a.indexOf(8), "indexOf should return correct index");  // Validate.
    assertEqLL(-1, a.indexOf(999), "indexOf should return -1 when not found");  // Validate not found.
}  // End testIndexOf.

static void testOverflowAndInvalidIndicesThrow() {  // Validate exception behavior.
    fixedarray::FixedArray a(1);  // Capacity 1.
    a.append(1);  // Fill.
    bool threw = false;  // Track whether append throws.
    try {  // Try overflow append.
        a.append(2);  // Should throw when full.
    } catch (const std::exception&) {  // Catch expected exception.
        threw = true;  // Mark as thrown.
    }  // Close catch.
    assertTrue(threw, "append should throw when full");  // Validate.
}  // End testOverflowAndInvalidIndicesThrow.

static void testSimulationFormulas() {  // Simulation helpers should match known formulas.
    assertEqLL(0, fixedarray::simulateInsertMoves(0, 0), "insert into empty moves 0");  // n=0 boundary.
    assertEqLL(4, fixedarray::simulateInsertMoves(4, 0), "insert at head moves n");  // head insert.
    assertEqLL(0, fixedarray::simulateInsertMoves(4, 4), "insert at tail moves 0");  // tail insert.
    assertEqLL(3, fixedarray::simulateRemoveMoves(4, 0), "remove at head moves n-1");  // head remove.
    assertEqLL(0, fixedarray::simulateRemoveMoves(4, 3), "remove at tail moves 0");  // tail remove.
}  // End testSimulationFormulas.

int main() {  // Run all tests and return non-zero on failure.
    try {  // Catch exceptions to produce a clean test runner.
        std::cout << "=== FixedArray Tests (C++) ===\n";  // Print header.
        testAppendGetToVector();  // Run append/get test.
        testSetUpdatesValue();  // Run set test.
        testInsertAtShiftsRightAndCountsMoves();  // Run insert test.
        testRemoveAtShiftsLeftAndCountsMoves();  // Run remove test.
        testIndexOf();  // Run indexOf test.
        testOverflowAndInvalidIndicesThrow();  // Run overflow test.
        testSimulationFormulas();  // Run formula checks.
        std::cout << "All tests PASSED.\n";  // Report success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Report failure message.
        std::cerr << ex.what() << "\n";  // Print error.
        return 1;  // Exit failure.
    }  // Close catch.
}  // End main.

