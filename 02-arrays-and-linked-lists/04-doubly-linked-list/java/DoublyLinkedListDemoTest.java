// 04 雙向鏈結串列測試（Java）/ Tests for doubly linked list (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class DoublyLinkedListDemoTest {  // Minimal test runner without JUnit/Maven.  
    private DoublyLinkedListDemoTest() {  // Prevent instantiation.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    private static void assertTrue(boolean condition, String message) {  // Minimal boolean assertion helper.  
        if (!condition) {  // Fail when condition is false.  
            throw new AssertionError("FAIL: " + message);  // Throw to signal test failure.  
        }  // Close failure branch.  
    }  // End assertTrue.  

    private static void assertEquals(long expected, long actual, String message) {  // Minimal long equality assertion helper.  
        if (expected != actual) {  // Fail when values differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + expected + ", actual=" + actual + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertEquals.  

    private static void assertArrayEquals(int[] expected, int[] actual, String message) {  // Minimal array equality helper.  
        if (!Arrays.equals(expected, actual)) {  // Fail when arrays differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + Arrays.toString(expected) + ", actual=" + Arrays.toString(actual) + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertArrayEquals.  

    private static void assertThrows(Class<? extends Throwable> expectedType, Runnable action, String message) {  // Assert that an action throws.  
        try {  // Execute action and expect it to throw.  
            action.run();  // Run action.  
        } catch (Throwable t) {  // Catch any throwable.  
            if (expectedType.isInstance(t)) {  // Accept expected type.  
                return;  // Test passed.  
            }  // Close type check.  
            throw new AssertionError("FAIL: " + message + " (threw " + t.getClass().getName() + ")");  // Wrong type.  
        }  // Close catch.  
        throw new AssertionError("FAIL: " + message + " (no exception thrown)");  // Fail if nothing thrown.  
    }  // End assertThrows.  

    private static int expectedGetHops(int size, int index) {  // Compute expected hops when traversing from the nearest end.  
        return Math.min(index, size - 1 - index);  // Nearest-end traversal cost formula.  
    }  // End expectedGetHops.  

    private static void testPushAndPopEndsAreO1() {  // push/pop at both ends should have 0 hops with head+tail.  
        int[] ns = new int[] {0, 1, 2, 5};  // Multiple sizes.  
        for (int n : ns) {  // Iterate test cases.  
            DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.buildOrderedList(n);  // Build [0..n-1].  
            assertEquals(0, lst.pushBack(999).hops, "pushBack hops should be 0");  // Validate O(1) tail insert.  
            assertEquals(0, lst.pushFront(888).hops, "pushFront hops should be 0");  // Validate O(1) head insert.  
            assertEquals(0, lst.popBack().cost.hops, "popBack hops should be 0");  // Validate O(1) tail remove.  
            assertEquals(0, lst.popFront().cost.hops, "popFront hops should be 0");  // Validate O(1) head remove.  
        }  // Close loop.  
    }  // End testPushAndPopEndsAreO1.  

    private static void testGetWithCostUsesNearestEnd() {  // getWithCost(i) hops should be min(i, size-1-i).  
        int size = 16;  // Fixed size for coverage.  
        DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.buildOrderedList(size);  // Build [0..15].  
        int[] indices = new int[] {0, 1, 2, 4, 8, 15};  // Sample indices.  
        for (int i : indices) {  // Iterate indices.  
            DoublyLinkedListDemo.GetResult r = lst.getWithCost(i);  // Fetch value + cost.  
            assertEquals(i, r.value, "ordered list value should equal index");  // Validate value.  
            assertEquals(expectedGetHops(size, i), r.cost.hops, "getWithCost hops should match nearest-end formula");  // Validate hops.  
        }  // Close loop.  
    }  // End testGetWithCostUsesNearestEnd.  

    private static void testInsertAtCostAndOrdering() {  // insertAt should splice correctly and report traversal hops.  
        DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.buildOrderedList(6);  // Build [0..5].  
        DoublyLinkedListDemo.OperationCost c0 = lst.insertAt(0, 99);  // Insert at head.  
        assertEquals(0, c0.hops, "insertAt(0) hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {99, 0, 1, 2, 3, 4, 5}, lst.toArray(), "insertAt(0) should insert at head");  // Validate ordering.  

        DoublyLinkedListDemo.DoublyLinkedList lst2 = DoublyLinkedListDemo.buildOrderedList(6);  // Build [0..5] again.  
        DoublyLinkedListDemo.OperationCost c3 = lst2.insertAt(3, 77);  // Insert at index 3.  
        assertEquals(Math.min(3, 6 - 1 - 3), c3.hops, "insertAt(3) hops should match nearest-end formula");  // Validate hops.  
        assertArrayEquals(new int[] {0, 1, 2, 77, 3, 4, 5}, lst2.toArray(), "insertAt should splice correctly");  // Validate ordering.  

        DoublyLinkedListDemo.DoublyLinkedList lst3 = DoublyLinkedListDemo.buildOrderedList(6);  // Build [0..5] again.  
        DoublyLinkedListDemo.OperationCost ce = lst3.insertAt(6, 55);  // Insert at tail (index==size).  
        assertEquals(0, ce.hops, "insertAt(size) hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {0, 1, 2, 3, 4, 5, 55}, lst3.toArray(), "insertAt(size) should append");  // Validate ordering.  
    }  // End testInsertAtCostAndOrdering.  

    private static void testRemoveAtCostAndOrdering() {  // removeAt should bypass correctly and report traversal hops.  
        DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.buildOrderedList(6);  // Build [0..5].  
        DoublyLinkedListDemo.RemoveResult r0 = lst.removeAt(0);  // Remove head.  
        assertEquals(0, r0.value, "removeAt(0) should remove value 0");  // Validate value.  
        assertEquals(0, r0.cost.hops, "removeAt(0) hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {1, 2, 3, 4, 5}, lst.toArray(), "removeAt(0) should remove head");  // Validate ordering.  

        DoublyLinkedListDemo.DoublyLinkedList lst2 = DoublyLinkedListDemo.buildOrderedList(6);  // Build [0..5] again.  
        DoublyLinkedListDemo.RemoveResult r5 = lst2.removeAt(5);  // Remove tail.  
        assertEquals(5, r5.value, "removeAt(tail) should remove value 5");  // Validate value.  
        assertEquals(0, r5.cost.hops, "removeAt(tail) hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {0, 1, 2, 3, 4}, lst2.toArray(), "removeAt(tail) should remove tail");  // Validate ordering.  

        DoublyLinkedListDemo.DoublyLinkedList lst3 = DoublyLinkedListDemo.buildOrderedList(6);  // Build [0..5] again.  
        DoublyLinkedListDemo.RemoveResult r3 = lst3.removeAt(3);  // Remove value 3.  
        assertEquals(3, r3.value, "removeAt(3) should remove value 3");  // Validate value.  
        assertEquals(Math.min(3, 6 - 1 - 3), r3.cost.hops, "removeAt(3) hops should match nearest-end formula");  // Validate cost.  
        assertArrayEquals(new int[] {0, 1, 2, 4, 5}, lst3.toArray(), "removeAt should bypass removed node");  // Validate ordering.  
    }  // End testRemoveAtCostAndOrdering.  

    private static void testIndexOfWithCostHops() {  // indexOfWithCost should count forward traversals.  
        DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.buildOrderedList(5);  // Build [0..4].  
        DoublyLinkedListDemo.FindResult f0 = lst.indexOfWithCost(0);  // Find head value.  
        assertEquals(0, f0.index, "indexOf(0) should return 0");  // Validate index.  
        assertEquals(0, f0.cost.hops, "finding head needs 0 hops");  // Validate cost.  
        DoublyLinkedListDemo.FindResult f3 = lst.indexOfWithCost(3);  // Find value at index 3.  
        assertEquals(3, f3.index, "indexOf(3) should return 3");  // Validate index.  
        assertEquals(3, f3.cost.hops, "finding index 3 needs 3 hops");  // Validate cost.  
        DoublyLinkedListDemo.FindResult fn = lst.indexOfWithCost(999);  // Search for missing value.  
        assertEquals(-1, fn.index, "indexOf(missing) should return -1");  // Validate sentinel.  
        assertEquals(5, fn.cost.hops, "missing value should traverse n hops");  // Validate cost.  
    }  // End testIndexOfWithCostHops.  

    private static void testInvalidIndicesThrow() {  // Methods should validate indices and empty pops.  
        DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.buildOrderedList(3);  // Build [0..2].  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.getWithCost(3), "getWithCost should throw on index==size");  // Invalid get.  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.insertAt(4, 0), "insertAt should throw on index>size");  // Invalid insert.  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.removeAt(3), "removeAt should throw on index==size");  // Invalid remove.  
        DoublyLinkedListDemo.DoublyLinkedList empty = new DoublyLinkedListDemo.DoublyLinkedList();  // Create empty list.  
        assertThrows(IndexOutOfBoundsException.class, () -> empty.popBack(), "popBack should throw on empty list");  // Invalid pop.  
        assertThrows(IndexOutOfBoundsException.class, () -> empty.popFront(), "popFront should throw on empty list");  // Invalid pop.  
    }  // End testInvalidIndicesThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testPushAndPopEndsAreO1();  // Run push/pop O(1) test.  
        testGetWithCostUsesNearestEnd();  // Run get cost test.  
        testInsertAtCostAndOrdering();  // Run insert test.  
        testRemoveAtCostAndOrdering();  // Run remove test.  
        testIndexOfWithCostHops();  // Run find cost test.  
        testInvalidIndicesThrow();  // Run invalid index tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End DoublyLinkedListDemoTest.  

