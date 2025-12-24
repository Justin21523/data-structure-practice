// 02 佇列測試（Java）/ Tests for queue (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class QueueDemoTest {  // Minimal test runner without JUnit/Maven.  
    private QueueDemoTest() {  // Prevent instantiation.  
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

    private static void testFifoEnqueueDequeueAndPeek() {  // Queue should obey FIFO semantics.  
        QueueDemo.ArrayQueue q = new QueueDemo.ArrayQueue();  // Start with empty queue.  
        q.enqueue(10);  // Enqueue 10.  
        q.enqueue(20);  // Enqueue 20.  
        q.enqueue(30);  // Enqueue 30.  
        assertEquals(10, q.peek(), "peek should return first enqueued value");  // Validate peek.  
        assertEquals(10, q.dequeue().value, "first dequeue should return 10");  // Validate first dequeue.  
        assertEquals(20, q.dequeue().value, "second dequeue should return 20");  // Validate second dequeue.  
        assertEquals(30, q.dequeue().value, "third dequeue should return 30");  // Validate third dequeue.  
        assertTrue(q.isEmpty(), "queue should be empty after removing all");  // Validate emptiness.  
    }  // End testFifoEnqueueDequeueAndPeek.  

    private static void testEnqueueResizeCopiedCounts() {  // copied should count resize copies exactly.  
        QueueDemo.ArrayQueue q = new QueueDemo.ArrayQueue();  // Start with empty queue (capacity=1).  
        assertEquals(0, q.enqueue(10).copied, "first enqueue fits without resize");  // No resize.  
        assertEquals(1, q.enqueue(20).copied, "resize 1->2 copies 1 element");  // Copy 1.  
        assertEquals(2, q.enqueue(30).copied, "resize 2->4 copies 2 elements");  // Copy 2.  
        assertEquals(0, q.enqueue(40).copied, "enqueue fits in capacity 4");  // No resize.  
        assertEquals(4, q.enqueue(50).copied, "resize 4->8 copies 4 elements");  // Copy 4.  
        assertEquals(5, q.size(), "final size should be 5");  // Validate size.  
        assertEquals(8, q.capacity(), "final capacity should be 8");  // Validate capacity.  
        assertEquals(1 + 2 + 4, q.totalCopies(), "total copies should sum resizes");  // Validate total copies.  
        assertArrayEquals(new int[] {10, 20, 30, 40, 50}, q.toArray(), "toArray should preserve FIFO order");  // Validate order.  
    }  // End testEnqueueResizeCopiedCounts.  

    private static void testDequeueMovedCostIsNMinus1() {  // Dequeue should shift left size-1 elements in naive array queue.  
        int[] ns = new int[] {1, 2, 3, 5, 10};  // Multiple sizes.  
        for (int n : ns) {  // Iterate cases.  
            QueueDemo.ArrayQueue q = QueueDemo.buildOrderedQueue(n);  // Build [0..n-1].  
            QueueDemo.DequeueResult r = q.dequeue();  // Dequeue once.  
            assertEquals(0, r.value, "dequeued value should be 0");  // Validate removed value.  
            int expectedMoved = (n <= 1) ? 0 : (n - 1);  // Shifting left moves n-1 elements.  
            assertEquals(expectedMoved, r.cost.moved, "dequeue moved should equal max(0,n-1)");  // Validate moved count.  
            int[] expected = new int[Math.max(0, n - 1)];  // Expected remaining queue.  
            for (int i = 0; i < expected.length; i++) {  // Fill expected array.  
                expected[i] = i + 1;  // Remaining values should be 1..n-1.  
            }  // Close loop.  
            assertArrayEquals(expected, q.toArray(), "after dequeue, queue should be 1..n-1");  // Validate remaining order.  
        }  // Close loop.  
    }  // End testDequeueMovedCostIsNMinus1.  

    private static void testEmptyOperationsThrow() {  // dequeue/peek should reject empty queue.  
        QueueDemo.ArrayQueue q = new QueueDemo.ArrayQueue();  // Create empty queue.  
        assertThrows(IndexOutOfBoundsException.class, q::peek, "peek should throw on empty");  // Invalid peek.  
        assertThrows(IndexOutOfBoundsException.class, q::dequeue, "dequeue should throw on empty");  // Invalid dequeue.  
    }  // End testEmptyOperationsThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testFifoEnqueueDequeueAndPeek();  // Run FIFO tests.  
        testEnqueueResizeCopiedCounts();  // Run resize cost tests.  
        testDequeueMovedCostIsNMinus1();  // Run dequeue shift cost tests.  
        testEmptyOperationsThrow();  // Run empty-operation tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End QueueDemoTest.  

