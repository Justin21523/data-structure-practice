// 04 雙端佇列測試（Java）/ Tests for deque (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class DequeDemoTest {  // Minimal test runner without JUnit/Maven.  
    private DequeDemoTest() {  // Prevent instantiation.  
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

    private static void testBasicBothEndsOperations() {  // Deque should support operations at both ends.  
        DequeDemo.Deque d = new DequeDemo.Deque();  // Start with empty deque.  
        d.pushBack(3);  // Push 3 to back.  
        d.pushBack(7);  // Push 7 to back.  
        d.pushFront(1);  // Push 1 to front.  
        assertArrayEquals(new int[] {1, 3, 7}, d.toArray(), "ordering should match");  // Validate ordering.  
        assertEquals(1, d.peekFront(), "peekFront should be 1");  // Validate peekFront.  
        assertEquals(7, d.peekBack(), "peekBack should be 7");  // Validate peekBack.  
        assertEquals(1, d.popFront().value, "popFront should return 1");  // Validate popFront.  
        assertEquals(7, d.popBack().value, "popBack should return 7");  // Validate popBack.  
        assertEquals(3, d.popBack().value, "popBack should return 3");  // Validate popBack.  
        assertTrue(d.isEmpty(), "deque should be empty at end");  // Validate emptiness.  
    }  // End testBasicBothEndsOperations.  

    private static void testMovedIsAlwaysZero() {  // With circular buffer, operations should not shift elements.  
        DequeDemo.Deque d = new DequeDemo.Deque();  // Start with empty deque.  
        assertEquals(0, d.pushBack(10).moved, "pushBack moved should be 0");  // Validate moved=0.  
        assertEquals(0, d.pushFront(20).moved, "pushFront moved should be 0");  // Validate moved=0.  
        assertEquals(0, d.popFront().cost.moved, "popFront moved should be 0");  // Validate moved=0.  
        d.pushBack(30);  // Add one more element.  
        assertEquals(0, d.popBack().cost.moved, "popBack moved should be 0");  // Validate moved=0.  
    }  // End testMovedIsAlwaysZero.  

    private static void testResizeCopiedCountsWithPushBack() {  // copied should count resize copies exactly.  
        DequeDemo.Deque d = new DequeDemo.Deque();  // Start with empty deque (capacity=1).  
        assertEquals(0, d.pushBack(10).copied, "first push fits without resize");  // No resize.  
        assertEquals(1, d.pushBack(20).copied, "resize 1->2 copies 1 element");  // Copy 1.  
        assertEquals(2, d.pushBack(30).copied, "resize 2->4 copies 2 elements");  // Copy 2.  
        assertEquals(0, d.pushBack(40).copied, "push fits in capacity 4");  // No resize.  
        assertEquals(4, d.pushBack(50).copied, "resize 4->8 copies 4 elements");  // Copy 4.  
        assertEquals(5, d.size(), "final size should be 5");  // Validate size.  
        assertEquals(8, d.capacity(), "final capacity should be 8");  // Validate capacity.  
        assertEquals(1 + 2 + 4, d.totalCopies(), "total copies should sum resizes");  // Validate total copies.  
        assertArrayEquals(new int[] {10, 20, 30, 40, 50}, d.toArray(), "ordering should be preserved");  // Validate ordering.  
    }  // End testResizeCopiedCountsWithPushBack.  

    private static void testWrapAroundWithBothEnds() {  // pushFront/popBack should work when head wraps.  
        DequeDemo.Deque d = new DequeDemo.Deque();  // Start with empty deque.  
        for (int v : new int[] {0, 1, 2, 3}) {  // Fill to capacity 4 (with resizes).  
            d.pushBack(v);  // Push to back.  
        }  // Close loop.  
        assertEquals(0, d.popFront().value, "popFront should return 0");  // Pop front => 0.  
        assertEquals(1, d.popFront().value, "popFront should return 1");  // Pop front => 1.  
        d.pushBack(4);  // Push back into wrapped slots if needed.  
        d.pushBack(5);  // Push back again.  
        assertArrayEquals(new int[] {2, 3, 4, 5}, d.toArray(), "ordering after wrap-around should match");  // Validate ordering.  
        d.pushFront(1);  // Push front (head moves left with wrap-around).  
        assertArrayEquals(new int[] {1, 2, 3, 4, 5}, d.toArray(), "ordering after pushFront should match");  // Validate ordering.  
    }  // End testWrapAroundWithBothEnds.  

    private static void testResizeWhenHeadNotZeroPreservesOrder() {  // Resize should preserve order even when head is offset.  
        DequeDemo.Deque d = new DequeDemo.Deque();  // Start with empty deque.  
        for (int v : new int[] {0, 1, 2, 3}) {  // Fill to capacity 4.  
            d.pushBack(v);  // Push to back.  
        }  // Close loop.  
        d.popFront();  // Remove 0 (head moves).  
        d.popFront();  // Remove 1 (head moves).  
        d.pushBack(4);  // Push back (wrap-around).  
        d.pushBack(5);  // Push back (deque full again).  
        DequeDemo.OperationCost cost = d.pushBack(6);  // Trigger resize and push 6.  
        assertEquals(4, cost.copied, "resize should copy exactly size elements (4)");  // Validate copied count.  
        assertEquals(0, cost.moved, "moved should remain 0");  // Validate moved=0.  
        assertArrayEquals(new int[] {2, 3, 4, 5, 6}, d.toArray(), "order should be preserved after resize");  // Validate ordering.  
    }  // End testResizeWhenHeadNotZeroPreservesOrder.  

    private static void testEmptyOperationsThrow() {  // pops/peeks should reject empty deque.  
        DequeDemo.Deque d = new DequeDemo.Deque();  // Create empty deque.  
        assertThrows(IndexOutOfBoundsException.class, d::peekFront, "peekFront should throw on empty");  // Invalid peek.  
        assertThrows(IndexOutOfBoundsException.class, d::peekBack, "peekBack should throw on empty");  // Invalid peek.  
        assertThrows(IndexOutOfBoundsException.class, d::popFront, "popFront should throw on empty");  // Invalid pop.  
        assertThrows(IndexOutOfBoundsException.class, d::popBack, "popBack should throw on empty");  // Invalid pop.  
    }  // End testEmptyOperationsThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testBasicBothEndsOperations();  // Run basic both-ends tests.  
        testMovedIsAlwaysZero();  // Run moved=0 tests.  
        testResizeCopiedCountsWithPushBack();  // Run resize copy tests.  
        testWrapAroundWithBothEnds();  // Run wrap-around tests.  
        testResizeWhenHeadNotZeroPreservesOrder();  // Run resize-with-offset-head test.  
        testEmptyOperationsThrow();  // Run empty-operation tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End DequeDemoTest.  

