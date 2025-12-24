// 02 佇列測試（C++）/ Tests for queue (C++).  // Bilingual file header.

#include "Queue.hpp"  // Include the API under test.

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

static void testFifoEnqueueDequeueAndPeek() {  // Queue should obey FIFO semantics.
    queueunit::ArrayQueue q;  // Start with empty queue.
    q.enqueue(10);  // Enqueue 10.
    q.enqueue(20);  // Enqueue 20.
    q.enqueue(30);  // Enqueue 30.
    assertEquals(10, q.peek(), "peek should return first enqueued value");  // Validate peek.
    assertEquals(10, q.dequeue().value, "first dequeue should return 10");  // Validate first dequeue.
    assertEquals(20, q.dequeue().value, "second dequeue should return 20");  // Validate second dequeue.
    assertEquals(30, q.dequeue().value, "third dequeue should return 30");  // Validate third dequeue.
    assertTrue(q.isEmpty(), "queue should be empty after removing all");  // Validate emptiness.
}  // Close testFifoEnqueueDequeueAndPeek().

static void testEnqueueResizeCopiedCounts() {  // copied should count resize copies exactly.
    queueunit::ArrayQueue q;  // Start with empty queue (capacity=1).
    assertEquals(0, q.enqueue(10).copied, "first enqueue fits without resize");  // No resize.
    assertEquals(1, q.enqueue(20).copied, "resize 1->2 copies 1 element");  // Copy 1.
    assertEquals(2, q.enqueue(30).copied, "resize 2->4 copies 2 elements");  // Copy 2.
    assertEquals(0, q.enqueue(40).copied, "enqueue fits in capacity 4");  // No resize.
    assertEquals(4, q.enqueue(50).copied, "resize 4->8 copies 4 elements");  // Copy 4.
    assertEquals(5, q.size(), "final size should be 5");  // Validate size.
    assertEquals(8, q.capacity(), "final capacity should be 8");  // Validate capacity.
    assertEquals(1 + 2 + 4, q.totalCopies(), "total copies should sum resizes");  // Validate total copies.
    assertVectorEquals(std::vector<int>{10, 20, 30, 40, 50}, q.toVector(), "toVector should preserve FIFO order");  // Validate snapshot.
}  // Close testEnqueueResizeCopiedCounts().

static void testDequeueMovedCostIsNMinus1() {  // Dequeue should shift left size-1 elements in naive array queue.
    for (int n : std::vector<int>{1, 2, 3, 5, 10}) {  // Multiple sizes.
        queueunit::ArrayQueue q = queueunit::buildOrderedQueue(n);  // Build [0..n-1].
        queueunit::DequeueResult r = q.dequeue();  // Dequeue once.
        assertEquals(0, r.value, "dequeued value should be 0");  // Validate removed value.
        int expectedMoved = (n <= 1) ? 0 : (n - 1);  // Shifting left moves n-1 elements.
        assertEquals(expectedMoved, r.cost.moved, "dequeue moved should equal max(0,n-1)");  // Validate moved count.
        std::vector<int> expected;  // Build expected remaining queue.
        for (int i = 1; i < n; i++) {  // Fill expected values 1..n-1.
            expected.push_back(i);  // Append one value.
        }  // Close loop.
        assertVectorEquals(expected, q.toVector(), "after dequeue, queue should be 1..n-1");  // Validate remaining order.
    }  // Close loop.
}  // Close testDequeueMovedCostIsNMinus1().

static void testEmptyOperationsThrow() {  // dequeue/peek should reject empty queue.
    queueunit::ArrayQueue q;  // Create empty queue.
    assertThrowsOutOfRange([&]() { (void)q.peek(); }, "peek should throw on empty");  // Invalid peek.
    assertThrowsOutOfRange([&]() { (void)q.dequeue(); }, "dequeue should throw on empty");  // Invalid dequeue.
}  // Close testEmptyOperationsThrow().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testFifoEnqueueDequeueAndPeek();  // Run FIFO tests.
        testEnqueueResizeCopiedCounts();  // Run resize cost tests.
        testDequeueMovedCostIsNMinus1();  // Run dequeue shift cost tests.
        testEmptyOperationsThrow();  // Run empty-operation tests.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

