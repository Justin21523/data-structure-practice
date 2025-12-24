// 01 堆疊測試（Java）/ Tests for stack (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class StackDemoTest {  // Minimal test runner without JUnit/Maven.  
    private StackDemoTest() {  // Prevent instantiation.  
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

    private static void testLifoPushPopAndPeek() {  // Stack should obey LIFO semantics.  
        StackDemo.ArrayStack s = new StackDemo.ArrayStack();  // Start with empty stack.  
        s.push(10);  // Push 10.  
        s.push(20);  // Push 20.  
        s.push(30);  // Push 30.  
        assertEquals(30, s.peek(), "peek should return last pushed value");  // Validate peek.  
        assertEquals(30, s.pop().value, "first pop should return 30");  // Validate first pop.  
        assertEquals(20, s.pop().value, "second pop should return 20");  // Validate second pop.  
        assertEquals(10, s.pop().value, "third pop should return 10");  // Validate third pop.  
        assertTrue(s.isEmpty(), "stack should be empty after popping all");  // Validate emptiness.  
    }  // End testLifoPushPopAndPeek.  

    private static void testPushResizeCopiedCounts() {  // copied should count resize copies exactly.  
        StackDemo.ArrayStack s = new StackDemo.ArrayStack();  // Start with empty stack (capacity=1).  
        assertEquals(0, s.push(10).copied, "first push fits without resize");  // No resize.  
        assertEquals(1, s.push(20).copied, "resize 1->2 copies 1 element");  // Copy 1.  
        assertEquals(2, s.push(30).copied, "resize 2->4 copies 2 elements");  // Copy 2.  
        assertEquals(0, s.push(40).copied, "push fits in capacity 4");  // No resize.  
        assertEquals(4, s.push(50).copied, "resize 4->8 copies 4 elements");  // Copy 4.  
        assertEquals(5, s.size(), "final size should be 5");  // Validate size.  
        assertEquals(8, s.capacity(), "final capacity should be 8");  // Validate capacity.  
        assertEquals(1 + 2 + 4, s.totalCopies(), "total copies should sum resizes");  // Validate total copies.  
        assertArrayEquals(new int[] {10, 20, 30, 40, 50}, s.toArray(), "toArray should preserve bottom->top order");  // Validate snapshot order.  
    }  // End testPushResizeCopiedCounts.  

    private static void testEmptyOperationsThrow() {  // pop/peek should reject empty stack.  
        StackDemo.ArrayStack s = new StackDemo.ArrayStack();  // Create empty stack.  
        assertThrows(IndexOutOfBoundsException.class, s::peek, "peek should throw on empty");  // Invalid peek.  
        assertThrows(IndexOutOfBoundsException.class, s::pop, "pop should throw on empty");  // Invalid pop.  
    }  // End testEmptyOperationsThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testLifoPushPopAndPeek();  // Run LIFO tests.  
        testPushResizeCopiedCounts();  // Run resize cost tests.  
        testEmptyOperationsThrow();  // Run empty-operation tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End StackDemoTest.  

