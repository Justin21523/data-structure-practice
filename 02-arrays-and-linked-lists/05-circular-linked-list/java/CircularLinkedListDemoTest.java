// 05 環狀鏈結串列測試（Java）/ Tests for circular linked list (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class CircularLinkedListDemoTest {  // Minimal test runner without JUnit/Maven.  
    private CircularLinkedListDemoTest() {  // Prevent instantiation.  
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

    private static void testPushCostsAreZero() {  // pushFront/pushBack should be O(1) with tail pointer.  
        CircularLinkedListDemo.CircularLinkedList lst = new CircularLinkedListDemo.CircularLinkedList();  // Start with empty list.  
        assertEquals(0, lst.pushBack(1).hops, "pushBack hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {1}, lst.toArray(), "pushBack should append");  // Validate ordering.  
        assertEquals(0, lst.pushFront(0).hops, "pushFront hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {0, 1}, lst.toArray(), "pushFront should prepend");  // Validate ordering.  
    }  // End testPushCostsAreZero.  

    private static void testPopFrontCostIsZero() {  // popFront should be O(1).  
        CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.buildOrderedList(3);  // Build [0,1,2].  
        CircularLinkedListDemo.RemoveResult r = lst.popFront();  // Pop head.  
        assertEquals(0, r.value, "popFront should remove 0");  // Validate value.  
        assertEquals(0, r.cost.hops, "popFront hops should be 0");  // Validate cost.  
        assertArrayEquals(new int[] {1, 2}, lst.toArray(), "popFront should remove head");  // Validate ordering.  
    }  // End testPopFrontCostIsZero.  

    private static void testPopBackCostIsNMinus2() {  // popBack requires finding predecessor in singly circular list.  
        int[] ns = new int[] {1, 2, 3, 5, 10};  // Multiple sizes.  
        for (int n : ns) {  // Iterate test cases.  
            CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.buildOrderedList(n);  // Build [0..n-1].  
            CircularLinkedListDemo.RemoveResult r = lst.popBack();  // Pop tail.  
            assertEquals(n - 1, r.value, "popBack should remove last value");  // Validate removed value.  
            int expectedHops = (n <= 1) ? 0 : (n - 2);  // Predecessor search traverses n-2 links.  
            assertEquals(expectedHops, r.cost.hops, "popBack hops should equal max(0,n-2)");  // Validate cost.  
            int[] expected = new int[Math.max(0, n - 1)];  // Expected array after popBack.  
            for (int i = 0; i < expected.length; i++) {  // Fill expected array.  
                expected[i] = i;  // Values remain 0..n-2.  
            }  // Close loop.  
            assertArrayEquals(expected, lst.toArray(), "popBack should remove tail");  // Validate ordering.  
        }  // Close loop.  
    }  // End testPopBackCostIsNMinus2.  

    private static void testGetWithCostHopsEqualsIndex() {  // getWithCost(i) should traverse i links from head.  
        CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.buildOrderedList(10);  // Build [0..9].  
        int[] indices = new int[] {0, 1, 5, 9};  // Sample indices.  
        for (int i : indices) {  // Iterate indices.  
            CircularLinkedListDemo.GetResult r = lst.getWithCost(i);  // Fetch value + cost.  
            assertEquals(i, r.value, "ordered list value should equal index");  // Validate value.  
            assertEquals(i, r.cost.hops, "getWithCost hops should equal index");  // Validate cost.  
        }  // Close loop.  
    }  // End testGetWithCostHopsEqualsIndex.  

    private static void testRotateChangesOrderAndCost() {  // rotate(k) should rotate left by k with hops=k%size.  
        int size = 4;  // Use a small fixed size.  
        int[][] cases = new int[][] {  // Provide {steps, expectedFirst} plus expected arrays in code below.  
            new int[] {0, 0},  // No rotation => head 0.  
            new int[] {1, 1},  // Rotate 1 => head 1.  
            new int[] {3, 3},  // Rotate 3 => head 3.  
            new int[] {4, 0},  // Full cycle => head 0.  
            new int[] {7, 3},  // 7%4==3 => head 3.  
        };  // Close cases.  

        for (int[] c : cases) {  // Iterate cases.  
            int steps = c[0];  // Read steps.  
            CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.buildOrderedList(size);  // Build [0,1,2,3].  
            CircularLinkedListDemo.OperationCost cost = lst.rotate(steps);  // Rotate by steps.  
            assertEquals(steps % size, cost.hops, "rotate hops should equal steps%size");  // Validate cost.  
            int[] arr = lst.toArray();  // Snapshot after rotation.  
            assertEquals(c[1], arr[0], "rotated head value should match");  // Validate head value.  
        }  // Close loop.  
    }  // End testRotateChangesOrderAndCost.  

    private static void testIndexOfWithCostHops() {  // indexOfWithCost should count traversals up to match or size.  
        CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.buildOrderedList(5);  // Build [0..4].  
        CircularLinkedListDemo.FindResult f0 = lst.indexOfWithCost(0);  // Find head value.  
        assertEquals(0, f0.index, "indexOf(0) should return 0");  // Validate index.  
        assertEquals(0, f0.cost.hops, "finding head needs 0 hops");  // Validate cost.  
        CircularLinkedListDemo.FindResult f3 = lst.indexOfWithCost(3);  // Find value at index 3.  
        assertEquals(3, f3.index, "indexOf(3) should return 3");  // Validate index.  
        assertEquals(3, f3.cost.hops, "finding index 3 needs 3 hops");  // Validate cost.  
        CircularLinkedListDemo.FindResult fn = lst.indexOfWithCost(999);  // Search missing.  
        assertEquals(-1, fn.index, "indexOf(missing) should return -1");  // Validate not found.  
        assertEquals(5, fn.cost.hops, "missing value should traverse size hops");  // Validate cost.  
    }  // End testIndexOfWithCostHops.  

    private static void testInvalidIndicesAndInputsThrow() {  // Methods should validate indices and inputs.  
        CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.buildOrderedList(3);  // Build [0..2].  
        assertThrows(IndexOutOfBoundsException.class, () -> lst.getWithCost(3), "getWithCost should throw on index==size");  // Invalid get.  
        assertThrows(IllegalArgumentException.class, () -> lst.rotate(-1), "rotate should throw on negative steps");  // Invalid rotate.  
        CircularLinkedListDemo.CircularLinkedList empty = new CircularLinkedListDemo.CircularLinkedList();  // Create empty list.  
        assertThrows(IndexOutOfBoundsException.class, () -> empty.popFront(), "popFront should throw on empty");  // Invalid pop.  
        assertThrows(IndexOutOfBoundsException.class, () -> empty.popBack(), "popBack should throw on empty");  // Invalid pop.  
    }  // End testInvalidIndicesAndInputsThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testPushCostsAreZero();  // Run push tests.  
        testPopFrontCostIsZero();  // Run popFront test.  
        testPopBackCostIsNMinus2();  // Run popBack test.  
        testGetWithCostHopsEqualsIndex();  // Run get cost test.  
        testRotateChangesOrderAndCost();  // Run rotate test.  
        testIndexOfWithCostHops();  // Run indexOf test.  
        testInvalidIndicesAndInputsThrow();  // Run invalid input tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End CircularLinkedListDemoTest.  

