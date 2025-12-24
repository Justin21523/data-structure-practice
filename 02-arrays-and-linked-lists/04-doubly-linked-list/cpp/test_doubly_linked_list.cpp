// 04 雙向鏈結串列測試（C++）/ Tests for doubly linked list (C++).  // Bilingual file header.

#include "DoublyLinkedList.hpp"  // Include the implementation under test.

#include <cassert>  // Provide assert for minimal testing.
#include <stdexcept>  // Provide exception types for assertThrows helper.
#include <vector>  // Provide std::vector for expected arrays.

static void assertVectorEquals(const std::vector<int>& expected, const std::vector<int>& actual) {  // Compare vectors for equality.
    assert(expected == actual);  // Use operator== which checks element-wise equality.
}  // End assertVectorEquals.

template <typename Fn>  // Template for accepting any callable.
static void assertThrowsOutOfRange(Fn fn) {  // Assert that a callable throws std::out_of_range.
    try {  // Run and expect exception.
        fn();  // Execute.
    } catch (const std::out_of_range&) {  // Accept out_of_range.
        return;  // Test passed.
    } catch (...) {  // Any other exception type is a failure.
        assert(false);  // Fail fast.
    }  // Close catch chain.
    assert(false);  // Fail if nothing thrown.
}  // End assertThrowsOutOfRange.

static int expectedGetHops(int size, int index) {  // Compute expected hops when traversing from the nearest end.
    int a = index;  // Hops from head.
    int b = size - 1 - index;  // Hops from tail.
    return (a < b) ? a : b;  // Return min(a,b).
}  // End expectedGetHops.

static void testPushPopEndsAreO1() {  // push/pop at both ends should have 0 hops with head+tail.
    std::vector<int> ns{0, 1, 2, 5};  // Multiple sizes.
    for (int n : ns) {  // Iterate test cases.
        DoublyLinkedList lst = buildOrderedList(n);  // Build [0..n-1].
        assert(lst.pushBack(999).hops == 0);  // pushBack should be O(1).
        assert(lst.pushFront(888).hops == 0);  // pushFront should be O(1).
        assert(lst.popBack().cost.hops == 0);  // popBack should be O(1).
        assert(lst.popFront().cost.hops == 0);  // popFront should be O(1).
    }  // Close loop.
}  // End testPushPopEndsAreO1.

static void testGetWithCostUsesNearestEnd() {  // getWithCost(i) hops should be min(i, size-1-i).
    int size = 16;  // Fixed size for coverage.
    DoublyLinkedList lst = buildOrderedList(size);  // Build [0..15].
    std::vector<int> indices{0, 1, 2, 4, 8, 15};  // Sample indices.
    for (int i : indices) {  // Iterate indices.
        GetResult r = lst.getWithCost(i);  // Fetch value + cost.
        assert(r.value == i);  // Ordered list value should equal index.
        assert(r.cost.hops == expectedGetHops(size, i));  // Validate hops formula.
    }  // Close loop.
}  // End testGetWithCostUsesNearestEnd.

static void testInsertAtCostAndOrdering() {  // insertAt should splice correctly and report traversal hops.
    DoublyLinkedList a = buildOrderedList(6);  // Build [0..5].
    OperationCost c0 = a.insertAt(0, 99);  // Insert at head.
    assert(c0.hops == 0);  // Head insertion needs no traversal.
    assertVectorEquals(std::vector<int>({99, 0, 1, 2, 3, 4, 5}), a.toVector());  // Validate ordering.

    DoublyLinkedList b = buildOrderedList(6);  // Build [0..5] again.
    OperationCost c3 = b.insertAt(3, 77);  // Insert at index 3.
    assert(c3.hops == expectedGetHops(6, 3));  // Validate hops.
    assertVectorEquals(std::vector<int>({0, 1, 2, 77, 3, 4, 5}), b.toVector());  // Validate ordering.

    DoublyLinkedList c = buildOrderedList(6);  // Build [0..5] again.
    OperationCost ce = c.insertAt(6, 55);  // Insert at tail (index==size).
    assert(ce.hops == 0);  // Tail insertion needs no traversal.
    assertVectorEquals(std::vector<int>({0, 1, 2, 3, 4, 5, 55}), c.toVector());  // Validate ordering.
}  // End testInsertAtCostAndOrdering.

static void testRemoveAtCostAndOrdering() {  // removeAt should bypass correctly and report traversal hops.
    DoublyLinkedList a = buildOrderedList(6);  // Build [0..5].
    RemoveResult r0 = a.removeAt(0);  // Remove head.
    assert(r0.value == 0);  // Removed value should be 0.
    assert(r0.cost.hops == 0);  // Head removal needs no traversal.
    assertVectorEquals(std::vector<int>({1, 2, 3, 4, 5}), a.toVector());  // Validate ordering.

    DoublyLinkedList b = buildOrderedList(6);  // Build [0..5] again.
    RemoveResult r5 = b.removeAt(5);  // Remove tail.
    assert(r5.value == 5);  // Removed value should be 5.
    assert(r5.cost.hops == 0);  // Tail removal needs no traversal.
    assertVectorEquals(std::vector<int>({0, 1, 2, 3, 4}), b.toVector());  // Validate ordering.

    DoublyLinkedList c = buildOrderedList(6);  // Build [0..5] again.
    RemoveResult r3 = c.removeAt(3);  // Remove value 3.
    assert(r3.value == 3);  // Removed value should be 3.
    assert(r3.cost.hops == expectedGetHops(6, 3));  // Validate hops.
    assertVectorEquals(std::vector<int>({0, 1, 2, 4, 5}), c.toVector());  // Validate ordering.
}  // End testRemoveAtCostAndOrdering.

static void testIndexOfWithCostHops() {  // indexOfWithCost should count forward traversals.
    DoublyLinkedList lst = buildOrderedList(5);  // Build [0..4].
    FindResult f0 = lst.indexOfWithCost(0);  // Find head value.
    assert(f0.index == 0);  // Index should be 0.
    assert(f0.cost.hops == 0);  // Finding head needs 0 hops.
    FindResult f3 = lst.indexOfWithCost(3);  // Find value at index 3.
    assert(f3.index == 3);  // Index should be 3.
    assert(f3.cost.hops == 3);  // Finding index 3 needs 3 hops.
    FindResult fn = lst.indexOfWithCost(999);  // Search for missing value.
    assert(fn.index == -1);  // Not found sentinel.
    assert(fn.cost.hops == 5);  // Not found should traverse n hops.
}  // End testIndexOfWithCostHops.

static void testInvalidIndicesThrow() {  // Methods should validate indices and empty pops.
    DoublyLinkedList lst = buildOrderedList(3);  // Build [0..2].
    assertThrowsOutOfRange([&]() { lst.getWithCost(3); });  // Invalid get.
    assertThrowsOutOfRange([&]() { lst.insertAt(4, 0); });  // Invalid insert.
    assertThrowsOutOfRange([&]() { lst.removeAt(3); });  // Invalid remove.
    DoublyLinkedList empty;  // Create empty list.
    assertThrowsOutOfRange([&]() { empty.popBack(); });  // Invalid pop.
    assertThrowsOutOfRange([&]() { empty.popFront(); });  // Invalid pop.
}  // End testInvalidIndicesThrow.

int main() {  // Run all tests and return non-zero on failure via assert.
    testPushPopEndsAreO1();  // Run push/pop O(1) test.
    testGetWithCostUsesNearestEnd();  // Run get cost test.
    testInsertAtCostAndOrdering();  // Run insert test.
    testRemoveAtCostAndOrdering();  // Run remove test.
    testIndexOfWithCostHops();  // Run find cost test.
    testInvalidIndicesThrow();  // Run invalid input tests.
    return 0;  // Indicate success.
}  // End main.

