// 04 雙端佇列測試（C++）/ Tests for deque (C++).  // Bilingual file header.

#include "Deque.hpp"  // Include the API under test.

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

static void testPushPopAndPeek() {  // Deque should support both-end operations.
    dequeunit::Deque d;  // Start with empty deque.
    d.pushBack(3);  // Push 3 to back.
    d.pushBack(7);  // Push 7 to back.
    d.pushFront(1);  // Push 1 to front => [1,3,7].
    assertEquals(1, d.peekFront(), "peekFront should return 1");  // Validate front.
    assertEquals(7, d.peekBack(), "peekBack should return 7");  // Validate back.
    assertEquals(1, d.popFront().value, "popFront should return 1");  // Validate popFront.
    assertEquals(7, d.popBack().value, "popBack should return 7");  // Validate popBack.
    dequeunit::PopResult last = d.popBack();  // Pop last element.
    assertEquals(3, last.value, "last popBack should return 3");  // Validate last value.
    assertEquals(0, last.cost.moved, "popBack moved should be 0");  // Validate moved=0.
    assertTrue(d.isEmpty(), "deque should be empty after removing all");  // Validate emptiness.
}  // Close testPushPopAndPeek().

static void testPushResizeCopiedCounts() {  // copied should count resize copies exactly.
    dequeunit::Deque d;  // Start with empty deque (capacity=1).
    assertEquals(0, d.pushBack(10).copied, "first push fits without resize");  // No resize.
    assertEquals(1, d.pushBack(20).copied, "resize 1->2 copies 1 element");  // Copy 1.
    assertEquals(2, d.pushBack(30).copied, "resize 2->4 copies 2 elements");  // Copy 2.
    assertEquals(0, d.pushBack(40).copied, "push fits in capacity 4");  // No resize.
    assertEquals(4, d.pushBack(50).copied, "resize 4->8 copies 4 elements");  // Copy 4.
    assertEquals(5, d.size(), "final size should be 5");  // Validate size.
    assertEquals(8, d.capacity(), "final capacity should be 8");  // Validate capacity.
    assertEquals(1 + 2 + 4, d.totalCopies(), "total copies should sum resizes");  // Validate total copies.
    assertVectorEquals(std::vector<int>{10, 20, 30, 40, 50}, d.toVector(), "toVector should preserve front->back order");  // Validate snapshot.
}  // Close testPushResizeCopiedCounts().

static void testWrapAroundWorks() {  // Deque should wrap around when head is not 0.
    dequeunit::Deque d;  // Start with empty deque.
    for (int v : std::vector<int>{0, 1, 2, 3}) {  // Fill to capacity 4 (with resizes).
        d.pushBack(v);  // Push one value.
    }  // Close loop.
    assertEquals(0, d.popFront().value, "popFront should return 0");  // Pop 0.
    assertEquals(1, d.popFront().value, "popFront should return 1");  // Pop 1 (head offset).
    d.pushBack(4);  // Push 4 (wrap-around).
    d.pushBack(5);  // Push 5 (wrap-around).
    assertVectorEquals(std::vector<int>{2, 3, 4, 5}, d.toVector(), "ordering after wrap-around should match");  // Validate ordering.
}  // Close testWrapAroundWorks().

static void testResizeWhenHeadNotZeroPreservesOrder() {  // Resize should preserve order even when head is offset.
    dequeunit::Deque d;  // Start with empty deque.
    for (int v : std::vector<int>{0, 1, 2, 3}) {  // Fill to capacity 4.
        d.pushBack(v);  // Push one value.
    }  // Close loop.
    d.popFront();  // Remove 0 (head moves).
    d.popFront();  // Remove 1 (head moves).
    d.pushBack(4);  // Push 4 (wrap-around).
    d.pushBack(5);  // Push 5 (deque is full again).

    dequeunit::OperationCost cost = d.pushBack(6);  // Trigger resize via pushBack.
    assertEquals(4, cost.copied, "pushBack resize should copy exactly size elements (4)");  // Validate copied count.
    assertEquals(0, cost.moved, "moved should remain 0");  // Validate moved=0.
    assertVectorEquals(std::vector<int>{2, 3, 4, 5, 6}, d.toVector(), "order should be preserved after pushBack resize");  // Validate ordering.

    dequeunit::OperationCost cost2 = d.pushFront(1);  // Push to front without triggering resize.
    assertEquals(0, cost2.copied, "pushFront should not resize after growth");  // Validate no resize.
    assertVectorEquals(std::vector<int>{1, 2, 3, 4, 5, 6}, d.toVector(), "pushFront should add element to the front");  // Validate ordering.
}  // Close testResizeWhenHeadNotZeroPreservesOrder().

static void testPushFrontTriggersResizeAndPreservesOrder() {  // pushFront should also preserve order when it triggers resize.
    dequeunit::Deque d;  // Start with empty deque.
    for (int v : std::vector<int>{0, 1, 2, 3}) {  // Fill to capacity 4.
        d.pushBack(v);  // Push one value.
    }  // Close loop.
    d.popFront();  // Remove 0 (head moves).
    d.popFront();  // Remove 1 (head moves).
    d.pushBack(4);  // Push 4.
    d.pushBack(5);  // Push 5 (deque is full again).

    dequeunit::OperationCost cost = d.pushFront(1);  // Trigger resize via pushFront.
    assertEquals(4, cost.copied, "pushFront resize should copy exactly size elements (4)");  // Validate copied count.
    assertEquals(0, cost.moved, "moved should remain 0");  // Validate moved=0.
    assertVectorEquals(std::vector<int>{1, 2, 3, 4, 5}, d.toVector(), "order should be [1,2,3,4,5] after resize");  // Validate ordering.
}  // Close testPushFrontTriggersResizeAndPreservesOrder().

static void testEmptyOperationsThrow() {  // peek/pop should reject empty deque.
    dequeunit::Deque d;  // Create empty deque.
    assertThrowsOutOfRange([&]() { (void)d.peekFront(); }, "peekFront should throw on empty");  // Invalid peek.
    assertThrowsOutOfRange([&]() { (void)d.peekBack(); }, "peekBack should throw on empty");  // Invalid peek.
    assertThrowsOutOfRange([&]() { (void)d.popFront(); }, "popFront should throw on empty");  // Invalid pop.
    assertThrowsOutOfRange([&]() { (void)d.popBack(); }, "popBack should throw on empty");  // Invalid pop.
}  // Close testEmptyOperationsThrow().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testPushPopAndPeek();  // Run basic operations test.
        testPushResizeCopiedCounts();  // Run resize copy tests.
        testWrapAroundWorks();  // Run wrap-around test.
        testResizeWhenHeadNotZeroPreservesOrder();  // Run resize-with-offset-head test.
        testPushFrontTriggersResizeAndPreservesOrder();  // Run pushFront-triggered resize test.
        testEmptyOperationsThrow();  // Run empty-operation tests.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

