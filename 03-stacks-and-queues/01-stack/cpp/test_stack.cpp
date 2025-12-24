// 01 堆疊測試（C++）/ Tests for stack (C++).  // Bilingual file header.

#include "Stack.hpp"  // Include the API under test.

#include <iostream>  // Provide std::cout for status output.
#include <stdexcept>  // Provide exception base types for assertions.
#include <vector>  // Provide std::vector for expected snapshots.

static void assertTrue(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw to signal test failure.
    }  // Close failure branch.
}  // Close assertTrue().

static void assertEquals(long long expected, long long actual, const char* message) {  // Minimal equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error(std::string("FAIL: ") + message + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertEquals().

static void assertVectorEquals(const std::vector<int>& expected, const std::vector<int>& actual, const char* message) {  // Minimal vector equality helper.
    if (expected != actual) {  // Fail when vectors differ.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw mismatch.
    }  // Close failure branch.
}  // Close assertVectorEquals().

template <typename Func>  // Template for simple exception assertions.
static void assertThrowsOutOfRange(Func f, const char* message) {  // Assert that f throws std::out_of_range.
    try {  // Run action.
        f();  // Execute function.
    } catch (const std::out_of_range&) {  // Accept expected type.
        return;  // Test passed.
    } catch (...) {  // Reject other exceptions.
        throw std::runtime_error(std::string("FAIL: ") + message + " (wrong exception type)");  // Wrong type.
    }  // Close catch.
    throw std::runtime_error(std::string("FAIL: ") + message + " (no exception thrown)");  // Fail if nothing thrown.
}  // Close assertThrowsOutOfRange().

static void testLifoPushPopAndPeek() {  // Stack should obey LIFO semantics.
    stackunit::ArrayStack s;  // Start with empty stack.
    s.push(10);  // Push 10.
    s.push(20);  // Push 20.
    s.push(30);  // Push 30.
    assertEquals(30, s.peek(), "peek should return last pushed value");  // Validate peek.
    assertEquals(30, s.pop().value, "first pop should return 30");  // Validate first pop.
    assertEquals(20, s.pop().value, "second pop should return 20");  // Validate second pop.
    assertEquals(10, s.pop().value, "third pop should return 10");  // Validate third pop.
    assertTrue(s.isEmpty(), "stack should be empty after popping all");  // Validate emptiness.
}  // Close testLifoPushPopAndPeek().

static void testPushResizeCopiedCounts() {  // copied should count resize copies exactly.
    stackunit::ArrayStack s;  // Start with empty stack (capacity=1).
    assertEquals(0, s.push(10).copied, "first push fits without resize");  // No resize.
    assertEquals(1, s.push(20).copied, "resize 1->2 copies 1 element");  // Copy 1.
    assertEquals(2, s.push(30).copied, "resize 2->4 copies 2 elements");  // Copy 2.
    assertEquals(0, s.push(40).copied, "push fits in capacity 4");  // No resize.
    assertEquals(4, s.push(50).copied, "resize 4->8 copies 4 elements");  // Copy 4.
    assertEquals(5, s.size(), "final size should be 5");  // Validate size.
    assertEquals(8, s.capacity(), "final capacity should be 8");  // Validate capacity.
    assertEquals(1 + 2 + 4, s.totalCopies(), "total copies should sum resizes");  // Validate total copies.
    assertVectorEquals(std::vector<int>{10, 20, 30, 40, 50}, s.toVector(), "toVector should preserve bottom->top order");  // Validate snapshot.
}  // Close testPushResizeCopiedCounts().

static void testEmptyOperationsThrow() {  // pop/peek should reject empty stack.
    stackunit::ArrayStack s;  // Create empty stack.
    assertThrowsOutOfRange([&]() { (void)s.peek(); }, "peek should throw on empty");  // Invalid peek.
    assertThrowsOutOfRange([&]() { (void)s.pop(); }, "pop should throw on empty");  // Invalid pop.
}  // Close testEmptyOperationsThrow().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testLifoPushPopAndPeek();  // Run LIFO tests.
        testPushResizeCopiedCounts();  // Run resize cost tests.
        testEmptyOperationsThrow();  // Run empty-operation tests.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

