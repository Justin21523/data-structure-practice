"""環狀佇列（Circular Queue）示範測試 / Tests for circular queue demo.  # Bilingual module header.

We validate correctness (FIFO ordering), wrap-around behavior, and deterministic costs (moved=0).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from circular_queue_demo import CircularQueue, build_ordered_queue  # Import code under test.  


class CircularQueueDemoTests(unittest.TestCase):  # Group circular queue tests in one test case.  
    def test_fifo_enqueue_dequeue_and_peek(self) -> None:  # Queue should obey FIFO semantics.  
        q = CircularQueue()  # Start with empty queue.  
        q.enqueue(10)  # Enqueue 10.  
        q.enqueue(20)  # Enqueue 20.  
        q.enqueue(30)  # Enqueue 30.  
        self.assertEqual(10, q.peek())  # Peek should return first enqueued value.  
        self.assertEqual(10, q.dequeue().value)  # First dequeue returns 10.  
        self.assertEqual(20, q.dequeue().value)  # Second dequeue returns 20.  
        self.assertEqual(30, q.dequeue().value)  # Third dequeue returns 30.  
        self.assertTrue(q.is_empty())  # Queue should be empty after removing all.  

    def test_enqueue_resize_copied_counts(self) -> None:  # copied should count resize copies exactly.  
        q = CircularQueue()  # Start with empty queue (capacity=1).  
        self.assertEqual(0, q.enqueue(10).copied)  # First enqueue fits without resize.  
        self.assertEqual(1, q.enqueue(20).copied)  # Resize 1->2 copies 1 element.  
        self.assertEqual(2, q.enqueue(30).copied)  # Resize 2->4 copies 2 elements.  
        self.assertEqual(0, q.enqueue(40).copied)  # Fits in capacity 4.  
        self.assertEqual(4, q.enqueue(50).copied)  # Resize 4->8 copies 4 elements.  
        self.assertEqual(5, q.size)  # Validate final size.  
        self.assertEqual(8, q.capacity)  # Validate final capacity.  
        self.assertEqual(1 + 2 + 4, q.total_copies)  # Total copies should sum resizes.  
        self.assertEqual([10, 20, 30, 40, 50], q.to_list())  # Ordering should remain FIFO.  

    def test_dequeue_moved_cost_is_zero(self) -> None:  # Dequeue should not shift elements in circular queue.  
        for n in [1, 2, 3, 5, 10]:  # Multiple sizes including boundary cases.  
            q = build_ordered_queue(n)  # Build [0..n-1].  
            r = q.dequeue()  # Dequeue once.  
            self.assertEqual(0, r.value)  # Dequeued value should be 0.  
            self.assertEqual(0, r.cost.moved)  # moved should be 0 (no shift).  
            self.assertEqual(list(range(1, n)), q.to_list())  # Remaining queue should be [1..n-1].  

    def test_wrap_around_works(self) -> None:  # Enqueue should wrap around when head is not 0.  
        q = CircularQueue()  # Start with empty queue.  
        for v in [0, 1, 2, 3]:  # Fill queue to capacity 4 (with resizes).  
            q.enqueue(v)  # Enqueue one value.  
        self.assertEqual([0, 1, 2, 3], q.to_list())  # Validate ordering.  
        self.assertEqual(0, q.dequeue().value)  # Dequeue 0.  
        self.assertEqual(1, q.dequeue().value)  # Dequeue 1 (head should now be offset).  
        q.enqueue(4)  # Enqueue 4 (should wrap if needed).  
        q.enqueue(5)  # Enqueue 5 (should wrap if needed).  
        self.assertEqual([2, 3, 4, 5], q.to_list())  # Validate ordering after wrap-around.  
        self.assertEqual(2, q.dequeue().value)  # Dequeue 2.  
        self.assertEqual(3, q.dequeue().value)  # Dequeue 3.  
        self.assertEqual(4, q.dequeue().value)  # Dequeue 4.  
        self.assertEqual(5, q.dequeue().value)  # Dequeue 5.  
        self.assertTrue(q.is_empty())  # Queue should be empty at end.  

    def test_resize_when_head_not_zero_preserves_order(self) -> None:  # Resize should copy in queue order even when head is offset.  
        q = CircularQueue()  # Start with empty queue.  
        for v in [0, 1, 2, 3]:  # Fill queue to capacity 4.  
            q.enqueue(v)  # Enqueue one value.  
        q.dequeue()  # Remove 0 (head moves).  
        q.dequeue()  # Remove 1 (head moves).  
        q.enqueue(4)  # Enqueue 4 (wrap-around into freed slots).  
        q.enqueue(5)  # Enqueue 5 (queue is full again).  
        cost = q.enqueue(6)  # Trigger resize and enqueue 6.  
        self.assertEqual(4, cost.copied)  # Resize should copy exactly size elements (4).  
        self.assertEqual(0, cost.moved)  # moved stays 0.  
        self.assertEqual([2, 3, 4, 5, 6], q.to_list())  # Ordering should be preserved after resize.  

    def test_empty_operations_raise(self) -> None:  # dequeue/peek should reject empty queue.  
        q = CircularQueue()  # Create empty queue.  
        with self.assertRaises(IndexError):  # peek should raise on empty.  
            q.peek()  # Invalid peek.  
        with self.assertRaises(IndexError):  # dequeue should raise on empty.  
            q.dequeue()  # Invalid dequeue.  


if __name__ == "__main__":  # Allow running tests via `python3 test_circular_queue_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

