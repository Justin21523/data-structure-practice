// 03 環狀佇列測試（Java）/ Tests for circular queue (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class CircularQueueDemoTest {  // Minimal test runner without JUnit/Maven.  
    private CircularQueueDemoTest() {  // Prevent instantiation.  
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
        CircularQueueDemo.CircularQueue q = new CircularQueueDemo.CircularQueue();  // Start with empty queue.  
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
        CircularQueueDemo.CircularQueue q = new CircularQueueDemo.CircularQueue();  // Start with empty queue (capacity=1).  
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

    private static void testDequeueMovedCostIsZero() {  // Dequeue should not shift elements in circular queue.  
        int[] ns = new int[] {1, 2, 3, 5, 10};  // Multiple sizes.  
        for (int n : ns) {  // Iterate cases.  
            CircularQueueDemo.CircularQueue q = CircularQueueDemo.buildOrderedQueue(n);  // Build [0..n-1].  
            CircularQueueDemo.DequeueResult r = q.dequeue();  // Dequeue once.  
            assertEquals(0, r.value, "dequeued value should be 0");  // Validate removed value.  
            assertEquals(0, r.cost.moved, "dequeue moved should be 0");  // Validate moved=0.  
            int[] expected = new int[Math.max(0, n - 1)];  // Expected remaining queue.  
            for (int i = 0; i < expected.length; i++) {  // Fill expected array.  
                expected[i] = i + 1;  // Remaining values should be 1..n-1.  
            }  // Close loop.  
            assertArrayEquals(expected, q.toArray(), "after dequeue, queue should be 1..n-1");  // Validate remaining order.  
        }  // Close loop.  
    }  // End testDequeueMovedCostIsZero.  

    private static void testWrapAroundWorks() {  // Enqueue should wrap around when head is not 0.  
        CircularQueueDemo.CircularQueue q = new CircularQueueDemo.CircularQueue();  // Start with empty queue.  
        for (int v : new int[] {0, 1, 2, 3}) {  // Fill queue to capacity 4 (with resizes).  
            q.enqueue(v);  // Enqueue one value.  
        }  // Close loop.  
        assertArrayEquals(new int[] {0, 1, 2, 3}, q.toArray(), "initial ordering should match");  // Validate ordering.  
        assertEquals(0, q.dequeue().value, "dequeue should return 0");  // Dequeue 0.  
        assertEquals(1, q.dequeue().value, "dequeue should return 1");  // Dequeue 1 (head offset).  
        q.enqueue(4);  // Enqueue 4 (should wrap if needed).  
        q.enqueue(5);  // Enqueue 5 (should wrap if needed).  
        assertArrayEquals(new int[] {2, 3, 4, 5}, q.toArray(), "ordering after wrap-around should match");  // Validate wrap ordering.  
    }  // End testWrapAroundWorks.  

    private static void testResizeWhenHeadNotZeroPreservesOrder() {  // Resize should preserve order even when head is offset.  
        CircularQueueDemo.CircularQueue q = new CircularQueueDemo.CircularQueue();  // Start with empty queue.  
        for (int v : new int[] {0, 1, 2, 3}) {  // Fill queue to capacity 4.  
            q.enqueue(v);  // Enqueue one value.  
        }  // Close loop.  
        q.dequeue();  // Remove 0 (head moves).  
        q.dequeue();  // Remove 1 (head moves).  
        q.enqueue(4);  // Enqueue 4 (wrap-around into freed slots).  
        q.enqueue(5);  // Enqueue 5 (queue is full again).  
        CircularQueueDemo.OperationCost cost = q.enqueue(6);  // Trigger resize and enqueue 6.  
        assertEquals(4, cost.copied, "resize should copy exactly size elements (4)");  // Validate copied count.  
        assertEquals(0, cost.moved, "moved should remain 0");  // Validate moved=0.  
        assertArrayEquals(new int[] {2, 3, 4, 5, 6}, q.toArray(), "order should be preserved after resize");  // Validate ordering.  
    }  // End testResizeWhenHeadNotZeroPreservesOrder.  

    private static void testEmptyOperationsThrow() {  // dequeue/peek should reject empty queue.  
        CircularQueueDemo.CircularQueue q = new CircularQueueDemo.CircularQueue();  // Create empty queue.  
        assertThrows(IndexOutOfBoundsException.class, q::peek, "peek should throw on empty");  // Invalid peek.  
        assertThrows(IndexOutOfBoundsException.class, q::dequeue, "dequeue should throw on empty");  // Invalid dequeue.  
    }  // End testEmptyOperationsThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testFifoEnqueueDequeueAndPeek();  // Run FIFO tests.  
        testEnqueueResizeCopiedCounts();  // Run resize copy tests.  
        testDequeueMovedCostIsZero();  // Run moved=0 tests.  
        testWrapAroundWorks();  // Run wrap-around test.  
        testResizeWhenHeadNotZeroPreservesOrder();  // Run resize-with-offset-head test.  
        testEmptyOperationsThrow();  // Run empty-operation tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End CircularQueueDemoTest.  

