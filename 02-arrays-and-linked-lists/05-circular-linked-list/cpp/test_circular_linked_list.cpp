// 05 環狀鏈結串列測試（C++）/ Tests for circular linked list (C++).  // Bilingual file header.

#include "CircularLinkedList.hpp"  // Include the implementation under test.

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

template <typename Fn>  // Template for accepting any callable.
static void assertThrowsInvalidArgument(Fn fn) {  // Assert that a callable throws std::invalid_argument.
    try {  // Run and expect exception.
        fn();  // Execute.
    } catch (const std::invalid_argument&) {  // Accept invalid_argument.
        return;  // Test passed.
    } catch (...) {  // Any other exception type is a failure.
        assert(false);  // Fail fast.
    }  // Close catch chain.
    assert(false);  // Fail if nothing thrown.
}  // End assertThrowsInvalidArgument.

static void testPushCostsAreZero() {  // pushFront/pushBack should be O(1) with tail pointer.
    CircularLinkedList lst;  // Start with empty list.
    assert(lst.pushBack(1).hops == 0);  // pushBack cost should be 0.
    assertVectorEquals(std::vector<int>({1}), lst.toVector());  // Validate ordering.
    assert(lst.pushFront(0).hops == 0);  // pushFront cost should be 0.
    assertVectorEquals(std::vector<int>({0, 1}), lst.toVector());  // Validate ordering.
}  // End testPushCostsAreZero.

static void testPopFrontCostIsZero() {  // popFront should be O(1).
    CircularLinkedList lst = buildOrderedList(3);  // Build [0,1,2].
    RemoveResult r = lst.popFront();  // Pop head.
    assert(r.value == 0);  // Removed value should be 0.
    assert(r.cost.hops == 0);  // popFront cost should be 0.
    assertVectorEquals(std::vector<int>({1, 2}), lst.toVector());  // Validate ordering.
}  // End testPopFrontCostIsZero.

static void testPopBackCostIsNMinus2() {  // popBack requires finding predecessor in singly circular list.
    std::vector<int> ns{1, 2, 3, 5, 10};  // Multiple sizes.
    for (int n : ns) {  // Iterate sizes.
        CircularLinkedList lst = buildOrderedList(n);  // Build [0..n-1].
        RemoveResult r = lst.popBack();  // Pop tail.
        assert(r.value == n - 1);  // Removed value should be last.
        int expected = (n <= 1) ? 0 : (n - 2);  // Predecessor search traverses n-2 links.
        assert(r.cost.hops == expected);  // Validate cost.
        std::vector<int> expectedVec;  // Expected vector after popBack.
        for (int i = 0; i < n - 1; i += 1) {  // Fill expected values.
            expectedVec.push_back(i);  // Values remain 0..n-2.
        }  // Close loop.
        assertVectorEquals(expectedVec, lst.toVector());  // Validate ordering.
    }  // Close loop.
}  // End testPopBackCostIsNMinus2.

static void testGetWithCostHopsEqualsIndex() {  // getWithCost(i) should traverse i links from head.
    CircularLinkedList lst = buildOrderedList(10);  // Build [0..9].
    std::vector<int> indices{0, 1, 5, 9};  // Sample indices.
    for (int i : indices) {  // Iterate indices.
        GetResult r = lst.getWithCost(i);  // Fetch value + cost.
        assert(r.value == i);  // Ordered list value should equal index.
        assert(r.cost.hops == i);  // Hops should equal index.
    }  // Close loop.
}  // End testGetWithCostHopsEqualsIndex.

static void testRotateChangesOrderAndCost() {  // rotate(k) should rotate left by k with hops=k%size.
    int size = 4;  // Fixed size.
    struct Case { int steps; std::vector<int> expected; };  // Define a small case struct.
    std::vector<Case> cases{  // Provide test cases.
        Case{0, std::vector<int>({0, 1, 2, 3})},  // No rotation.
        Case{1, std::vector<int>({1, 2, 3, 0})},  // Rotate by 1.
        Case{3, std::vector<int>({3, 0, 1, 2})},  // Rotate by 3.
        Case{4, std::vector<int>({0, 1, 2, 3})},  // Full cycle.
        Case{7, std::vector<int>({3, 0, 1, 2})},  // 7%4==3.
    };  // Close cases.

    for (const Case& c : cases) {  // Iterate cases.
        CircularLinkedList lst = buildOrderedList(size);  // Build base list.
        OperationCost cost = lst.rotate(c.steps);  // Rotate by steps.
        assert(cost.hops == c.steps % size);  // Validate hop count.
        assertVectorEquals(c.expected, lst.toVector());  // Validate ordering.
    }  // Close loop.
}  // End testRotateChangesOrderAndCost.

static void testIndexOfWithCostHops() {  // indexOfWithCost should count traversals up to match or size.
    CircularLinkedList lst = buildOrderedList(5);  // Build [0..4].
    FindResult f0 = lst.indexOfWithCost(0);  // Find head value.
    assert(f0.index == 0);  // Index should be 0.
    assert(f0.cost.hops == 0);  // Finding head needs 0 hops.
    FindResult f3 = lst.indexOfWithCost(3);  // Find value at index 3.
    assert(f3.index == 3);  // Index should be 3.
    assert(f3.cost.hops == 3);  // Finding index 3 needs 3 hops.
    FindResult fn = lst.indexOfWithCost(999);  // Search missing.
    assert(fn.index == -1);  // Not found sentinel.
    assert(fn.cost.hops == 5);  // Not found traverses size hops.
}  // End testIndexOfWithCostHops.

static void testInvalidIndicesAndInputsThrow() {  // Methods should validate indices and inputs.
    CircularLinkedList lst = buildOrderedList(3);  // Build [0..2].
    assertThrowsOutOfRange([&]() { lst.getWithCost(3); });  // Invalid get.
    assertThrowsInvalidArgument([&]() { lst.rotate(-1); });  // Invalid rotate.
    CircularLinkedList empty;  // Create empty list.
    assertThrowsOutOfRange([&]() { empty.popFront(); });  // Invalid pop.
    assertThrowsOutOfRange([&]() { empty.popBack(); });  // Invalid pop.
}  // End testInvalidIndicesAndInputsThrow.

int main() {  // Run all tests and return non-zero on failure via assert.
    testPushCostsAreZero();  // Run push tests.
    testPopFrontCostIsZero();  // Run popFront test.
    testPopBackCostIsNMinus2();  // Run popBack test.
    testGetWithCostHopsEqualsIndex();  // Run get cost test.
    testRotateChangesOrderAndCost();  // Run rotate test.
    testIndexOfWithCostHops();  // Run indexOf test.
    testInvalidIndicesAndInputsThrow();  // Run invalid input tests.
    return 0;  // Indicate success.
}  // End main.

