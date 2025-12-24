// 02 動態陣列測試（C++）/ Tests for dynamic array (C++).  // Bilingual test file header.
#include "DynamicArray.hpp"  // Include API under test.

#include <iostream>  // Use std::cout for test output.
#include <stdexcept>  // Use std::runtime_error for assertion failures.
#include <vector>  // Use std::vector for expected sequences.

static void assertTrue(bool condition, const std::string& message) {  // Minimal boolean assertion helper.
    if (!condition) {  // Fail when condition is false.
        throw std::runtime_error("FAIL: " + message);  // Throw to signal test failure.
    }  // Close failure branch.
}  // End assertTrue.

static void assertEqLL(long long expected, long long actual, const std::string& message) {  // Minimal equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error("FAIL: " + message + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Throw mismatch.
    }  // Close failure branch.
}  // End assertEqLL.

static void assertVecEq(const std::vector<int>& expected, const std::vector<int>& actual, const std::string& message) {  // Vector equality assertion.
    if (expected != actual) {  // Fail when vectors differ.
        throw std::runtime_error("FAIL: " + message);  // Throw with message.
    }  // Close failure branch.
}  // End assertVecEq.

static void testCapacityInvariant() {  // Capacity should be power-of-two and >= size.
    int ms[] = {0, 1, 2, 3, 4, 5, 8, 9, 16, 33, 100};  // Mix sizes.
    for (int m : ms) {  // Iterate test cases.
        dynamicarray::AppendSummary s = dynamicarray::simulateAppends(m);  // Simulate m appends.
        assertEqLL(m, s.finalSize, "final size should equal m");  // Validate size.
        assertTrue(dynamicarray::isPowerOfTwo(s.finalCapacity), "capacity should be power of two");  // Validate capacity form.
        assertTrue(s.finalCapacity >= std::max(1, m), "capacity should be >= max(1,m)");  // Validate bound.
    }  // Close loop.
}  // End testCapacityInvariant.

static void testTotalCopiesEqualsFinalCapacityMinusOne() {  // Copies sum to cap-1 under doubling.
    int ms[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33};  // Cover boundaries.
    for (int m : ms) {  // Iterate test cases.
        dynamicarray::AppendSummary s = dynamicarray::simulateAppends(m);  // Simulate m appends.
        assertEqLL(static_cast<long long>(s.finalCapacity - 1), s.totalCopies, "total copies should equal finalCapacity - 1");  // Validate copy sum.
    }  // Close loop.
}  // End testTotalCopiesEqualsFinalCapacityMinusOne.

static void testAggregateTotalCostIsLinear() {  // Total actual cost should be <= 3m.
    int ms[] = {0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256};  // Multiple sizes.
    for (int m : ms) {  // Iterate test cases.
        dynamicarray::AppendSummary s = dynamicarray::simulateAppends(m);  // Simulate m appends.
        assertTrue(s.totalActualCost <= static_cast<long long>(3) * static_cast<long long>(m), "total actual cost should be <= 3m");  // Validate bound.
    }  // Close loop.
}  // End testAggregateTotalCostIsLinear.

static void testInsertAtShiftsRight() {  // insertAt should shift right and keep ordering.
    dynamicarray::DynamicArray a;  // Create empty array.
    a.append(1);  // [1]
    a.append(2);  // [1,2]
    a.append(3);  // [1,2,3]
    dynamicarray::OperationCost cost = a.insertAt(1, 99);  // Insert at index 1.
    assertEqLL(2, cost.moved, "insertAt moved should equal size-index");  // Validate moved count.
    assertVecEq(std::vector<int>({1, 99, 2, 3}), a.toVector(), "insertAt should shift right");  // Validate ordering.
}  // End testInsertAtShiftsRight.

static void testRemoveAtShiftsLeft() {  // removeAt should shift left and keep ordering.
    dynamicarray::DynamicArray a;  // Create empty array.
    a.append(10);  // Fill.
    a.append(20);  // Fill.
    a.append(30);  // Fill.
    a.append(40);  // Fill.
    dynamicarray::RemoveResult r = a.removeAt(1);  // Remove 20.
    assertEqLL(20, r.value, "removeAt should return removed value");  // Validate removed value.
    assertEqLL(2, r.cost.moved, "removeAt moved should equal size-index-1");  // Validate moved count.
    assertVecEq(std::vector<int>({10, 30, 40}), a.toVector(), "removeAt should shift left");  // Validate ordering.
}  // End testRemoveAtShiftsLeft.

static void testInvalidIndicesThrow() {  // Methods should validate indices.
    dynamicarray::DynamicArray a;  // Create empty array.
    a.append(1);  // Size is 1.
    bool threw = false;  // Track whether get throws.
    try {  // Try invalid get.
        a.get(1);  // Should throw.
    } catch (const std::exception&) {  // Catch expected exception.
        threw = true;  // Mark as thrown.
    }  // Close catch.
    assertTrue(threw, "get should throw on invalid index");  // Validate.
}  // End testInvalidIndicesThrow.

int main() {  // Run all tests and return non-zero on failure.
    try {  // Catch exceptions to produce clean output.
        std::cout << "=== DynamicArray Tests (C++) ===\n";  // Print header.
        testCapacityInvariant();  // Run capacity invariant test.
        testTotalCopiesEqualsFinalCapacityMinusOne();  // Run copy-sum test.
        testAggregateTotalCostIsLinear();  // Run aggregate bound test.
        testInsertAtShiftsRight();  // Run insert shift test.
        testRemoveAtShiftsLeft();  // Run remove shift test.
        testInvalidIndicesThrow();  // Run invalid index test.
        std::cout << "All tests PASSED.\n";  // Report success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Report failure message.
        std::cerr << ex.what() << "\n";  // Print error.
        return 1;  // Exit failure.
    }  // Close catch.
}  // End main.

