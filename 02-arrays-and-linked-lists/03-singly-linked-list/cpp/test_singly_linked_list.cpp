// 03 單向鏈結串列測試（C++）/ Tests for singly linked list (C++).  // Bilingual file header.

#include "SinglyLinkedList.hpp"  // Include the implementation under test.

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

static void testPushFrontHopsIsZero() {  // pushFront should never traverse next pointers.
    SinglyLinkedList lst;  // Create empty list.
    std::vector<int> values{1, 2, 3, 4};  // Values to insert.
    for (int v : values) {  // Insert each value.
        OperationCost cost = lst.pushFront(v);  // Insert at head.
        assert(cost.hops == 0);  // Validate cost.
    }  // Close loop.
}  // End testPushFrontHopsIsZero.

static void testPushBackHopsEqualsSizeMinusOne() {  // pushBack should traverse to tail without a tail pointer.
    std::vector<int> ns{0, 1, 2, 3, 5, 10};  // Multiple sizes.
    for (int n : ns) {  // Iterate test cases.
        SinglyLinkedList lst = buildOrderedList(n);  // Build [0..n-1].
        OperationCost cost = lst.pushBack(999);  // Append sentinel.
        int expected = (n == 0) ? 0 : (n - 1);  // Expected hops.
        assert(cost.hops == expected);  // Validate traversal cost.
        assert(lst.size() == n + 1);  // Validate size update.
        std::vector<int> arr = lst.toVector();  // Convert to vector for tail check.
        assert(arr.back() == 999);  // Validate tail value.
    }  // Close loop.
}  // End testPushBackHopsEqualsSizeMinusOne.

static void testGetWithCostHopsEqualsIndex() {  // getWithCost(i) should traverse i links.
    SinglyLinkedList lst = buildOrderedList(10);  // Build [0..9].
    std::vector<int> indices{0, 1, 5, 9};  // Sample indices.
    for (int i : indices) {  // Iterate indices.
        GetResult r = lst.getWithCost(i);  // Fetch value + cost.
        assert(r.value == i);  // Value should equal index.
        assert(r.cost.hops == i);  // Hops should equal index.
    }  // Close loop.
}  // End testGetWithCostHopsEqualsIndex.

static void testInsertAtCostAndOrdering() {  // insertAt should splice correctly and report hops.
    SinglyLinkedList a = buildOrderedList(5);  // Build [0..4].
    OperationCost c0 = a.insertAt(0, 99);  // Insert at head.
    assert(c0.hops == 0);  // Head insertion has zero hops.
    assertVectorEquals(std::vector<int>({99, 0, 1, 2, 3, 4}), a.toVector());  // Validate ordering.

    SinglyLinkedList b = buildOrderedList(5);  // Build [0..4] again.
    OperationCost c3 = b.insertAt(3, 77);  // Insert at index 3.
    assert(c3.hops == 2);  // Predecessor is index 2 => 2 hops.
    assertVectorEquals(std::vector<int>({0, 1, 2, 77, 3, 4}), b.toVector());  // Validate ordering.
}  // End testInsertAtCostAndOrdering.

static void testRemoveAtCostAndOrdering() {  // removeAt should bypass correctly and report hops.
    SinglyLinkedList a = buildOrderedList(5);  // Build [0..4].
    RemoveResult r0 = a.removeAt(0);  // Remove head.
    assert(r0.value == 0);  // Removed value should be 0.
    assert(r0.cost.hops == 0);  // Head removal has zero hops.
    assertVectorEquals(std::vector<int>({1, 2, 3, 4}), a.toVector());  // Validate ordering.

    SinglyLinkedList b = buildOrderedList(5);  // Build [0..4] again.
    RemoveResult r3 = b.removeAt(3);  // Remove value 3.
    assert(r3.value == 3);  // Removed value should be 3.
    assert(r3.cost.hops == 2);  // Predecessor is index 2 => 2 hops.
    assertVectorEquals(std::vector<int>({0, 1, 2, 4}), b.toVector());  // Validate ordering.
}  // End testRemoveAtCostAndOrdering.

static void testIndexOfWithCostHops() {  // indexOfWithCost should count next traversals.
    SinglyLinkedList lst = buildOrderedList(5);  // Build [0..4].
    FindResult f0 = lst.indexOfWithCost(0);  // Find head value.
    assert(f0.index == 0);  // Index should be 0.
    assert(f0.cost.hops == 0);  // Finding head needs 0 hops.
    FindResult f3 = lst.indexOfWithCost(3);  // Find value at index 3.
    assert(f3.index == 3);  // Index should be 3.
    assert(f3.cost.hops == 3);  // Reaching index 3 needs 3 hops.
    FindResult fn = lst.indexOfWithCost(999);  // Search for missing value.
    assert(fn.index == -1);  // Not found sentinel.
    assert(fn.cost.hops == 5);  // Not found should traverse n hops.
}  // End testIndexOfWithCostHops.

static void testInvalidIndicesThrow() {  // Methods should validate indices.
    SinglyLinkedList lst = buildOrderedList(3);  // Build [0..2].
    assertThrowsOutOfRange([&]() { lst.getWithCost(-1); });  // Invalid get.
    assertThrowsOutOfRange([&]() { lst.getWithCost(3); });  // Invalid get.
    assertThrowsOutOfRange([&]() { lst.insertAt(4, 0); });  // Invalid insert.
    assertThrowsOutOfRange([&]() { lst.removeAt(3); });  // Invalid remove.
}  // End testInvalidIndicesThrow.

int main() {  // Run all tests and return non-zero on failure via assert.
    testPushFrontHopsIsZero();  // Run pushFront cost test.
    testPushBackHopsEqualsSizeMinusOne();  // Run pushBack cost test.
    testGetWithCostHopsEqualsIndex();  // Run get cost test.
    testInsertAtCostAndOrdering();  // Run insert cost + ordering test.
    testRemoveAtCostAndOrdering();  // Run remove cost + ordering test.
    testIndexOfWithCostHops();  // Run find cost test.
    testInvalidIndicesThrow();  // Run invalid index test.
    return 0;  // Indicate success.
}  // End main.

