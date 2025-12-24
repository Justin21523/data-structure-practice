// 03 單向鏈結串列測試（Java）/ Tests for singly linked list (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class SinglyLinkedListDemoTest {  // Minimal test runner without JUnit/Maven.  
    private SinglyLinkedListDemoTest() {  // Prevent instantiation.  
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

    private static void testPushFrontHopsIsZero() {  // pushFront should never traverse next pointers.  
        SinglyLinkedListDemo.SinglyLinkedList lst = new SinglyLinkedListDemo.SinglyLinkedList();  // Create empty list.  
        int[] values = new int[] {1, 2, 3, 4};  // Values to insert.  
        for (int v : values) {  // Iterate values.  
            SinglyLinkedListDemo.OperationCost cost = lst.pushFront(v);  // Insert at head.  
            assertEquals(0, cost.hops, "pushFront hops should be 0");  // Validate cost.  
        }  // Close loop.  
    }  // End testPushFrontHopsIsZero.  

    private static void testPushBackHopsEqualsSizeMinusOne() {  // pushBack should traverse to tail without a tail pointer.  
        int[] ns = new int[] {0, 1, 2, 3, 5, 10};  // Multiple sizes.  
        for (int n : ns) {  // Iterate test cases.  
            SinglyLinkedListDemo.SinglyLinkedList lst = SinglyLinkedListDemo.buildOrderedList(n);  // Build [0..n-1].  
            SinglyLinkedListDemo.OperationCost cost = lst.pushBack(999);  // Append sentinel.  
            int expected = (n == 0) ? 0 : (n - 1);  // Expected hops for head-only list.  
            assertEquals(expected, cost.hops, "pushBack hops should equal max(0,n-1)");  // Validate traversal cost.  
            assertEquals(n + 1, lst.size(), "size should increase by one");  // Validate size update.  
            int[] arr = lst.toArray();  // Convert to array for tail check.  
            assertEquals(999, arr[arr.length - 1], "pushBack should append to tail");  // Validate tail value.  
        }  // Close loop.  
    }  // End testPushBackHopsEqualsSizeMinusOne.  

    private static void testGetWithCostHopsEqualsIndex() {  // getWithCost(i) should traverse i links.  
        SinglyLinkedListDemo.SinglyLinkedList lst = SinglyLinkedListDemo.buildOrderedList(10);  // Build [0..9].  
        int[] indices = new int[] {0, 1, 5, 9};  // Sample indices.  
        for (int i : indices) {  // Iterate indices.  
            SinglyLinkedListDemo.GetResult r = lst.getWithCost(i);  // Fetch value + cost.  
            assertEquals(i, r.value, "value should equal index for ordered list");  // Validate value.  
            assertEquals(i, r.cost.hops, "getWithCost hops should equal index");  // Validate cost.  
        }  // Close loop.  
    }  // End testGetWithCostHopsEqualsIndex.  

    private static void testInsertAtHopsEqualsIndexMinusOne() {  // insertAt(i) should traverse (i-1) links for i>0.  
        SinglyLinkedListDemo.SinglyLinkedList a = SinglyLinkedListDemo.buildOrderedList(5);  // Build [0..4].  
        SinglyLinkedListDemo.OperationCost c0 = a.insertAt(0, 99);  // Insert at head.  
        assertEquals(0, c0.hops, "insertAt(0) hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {99, 0, 1, 2, 3, 4}, a.toArray(), "insertAt(0) should insert at head");  // Validate order.  

        SinglyLinkedListDemo.SinglyLinkedList b = SinglyLinkedListDemo.buildOrderedList(5);  // Build [0..4] again.  
        SinglyLinkedListDemo.OperationCost c3 = b.insertAt(3, 77);  // Insert at index 3.  
        assertEquals(2, c3.hops, "insertAt(3) hops should be 2");  // Validate cost.  
        assertArrayEquals(new int[] {0, 1, 2, 77, 3, 4}, b.toArray(), "insertAt should splice correctly");  // Validate order.  
    }  // End testInsertAtHopsEqualsIndexMinusOne.  

    private static void testRemoveAtHopsEqualsIndexMinusOne() {  // removeAt(i) should traverse (i-1) links for i>0.  
        SinglyLinkedListDemo.SinglyLinkedList a = SinglyLinkedListDemo.buildOrderedList(5);  // Build [0..4].  
        SinglyLinkedListDemo.RemoveResult r0 = a.removeAt(0);  // Remove head.  
        assertEquals(0, r0.value, "removeAt(0) should remove head value");  // Validate removed value.  
        assertEquals(0, r0.cost.hops, "removeAt(0) hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {1, 2, 3, 4}, a.toArray(), "removeAt(0) should remove head");  // Validate order.  

        SinglyLinkedListDemo.SinglyLinkedList b = SinglyLinkedListDemo.buildOrderedList(5);  // Build [0..4] again.  
        SinglyLinkedListDemo.RemoveResult r3 = b.removeAt(3);  // Remove value 3.  
        assertEquals(3, r3.value, "removeAt(3) should remove value 3");  // Validate removed value.  
        assertEquals(2, r3.cost.hops, "removeAt(3) hops should be 2");  // Validate cost.  
        assertArrayEquals(new int[] {0, 1, 2, 4}, b.toArray(), "removeAt should bypass removed node");  // Validate order.  
    }  // End testRemoveAtHopsEqualsIndexMinusOne.  

    private static void testIndexOfWithCostHops() {  // indexOfWithCost should count next traversals.  
        SinglyLinkedListDemo.SinglyLinkedList lst = SinglyLinkedListDemo.buildOrderedList(5);  // Build [0..4].  
        SinglyLinkedListDemo.FindResult f0 = lst.indexOfWithCost(0);  // Find head value.  
        assertEquals(0, f0.index, "indexOf(0) should return 0");  // Validate index.  
        assertEquals(0, f0.cost.hops, "finding head needs 0 hops");  // Validate cost.  
        SinglyLinkedListDemo.FindResult f3 = lst.indexOfWithCost(3);  // Find middle value.  
        assertEquals(3, f3.index, "indexOf(3) should return 3");  // Validate index.  
        assertEquals(3, f3.cost.hops, "finding index 3 needs 3 hops");  // Validate cost.  
        SinglyLinkedListDemo.FindResult fn = lst.indexOfWithCost(999);  // Search for missing value.  
        assertEquals(-1, fn.index, "indexOf(missing) should return -1");  // Validate not-found sentinel.  
        assertEquals(5, fn.cost.hops, "missing value should traverse n hops");  // Validate cost.  
    }  // End testIndexOfWithCostHops.  

    private static void testInvalidIndicesThrow() {  // Methods should validate indices.  
        SinglyLinkedListDemo.SinglyLinkedList lst = SinglyLinkedListDemo.buildOrderedList(3);  // Build [0..2].  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.getWithCost(-1), "getWithCost should throw on negative index");  // Invalid get.  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.getWithCost(3), "getWithCost should throw on index==size");  // Invalid get.  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.insertAt(4, 0), "insertAt should throw on index>size");  // Invalid insert.  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.removeAt(3), "removeAt should throw on index==size");  // Invalid remove.  
    }  // End testInvalidIndicesThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testPushFrontHopsIsZero();  // Run pushFront cost test.  
        testPushBackHopsEqualsSizeMinusOne();  // Run pushBack cost test.  
        testGetWithCostHopsEqualsIndex();  // Run get cost test.  
        testInsertAtHopsEqualsIndexMinusOne();  // Run insert cost test.  
        testRemoveAtHopsEqualsIndexMinusOne();  // Run remove cost test.  
        testIndexOfWithCostHops();  // Run find cost test.  
        testInvalidIndicesThrow();  // Run index validation test.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End SinglyLinkedListDemoTest.  

