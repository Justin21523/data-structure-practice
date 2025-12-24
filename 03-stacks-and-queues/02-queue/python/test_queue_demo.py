"""佇列（Queue）示範測試 / Tests for queue demo.  # Bilingual module header.

We validate correctness (FIFO ordering) and deterministic costs (copied/moved).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from queue_demo import ArrayQueue, build_ordered_queue  # Import code under test.  


class QueueDemoTests(unittest.TestCase):  # Group queue tests in one test case.  
    def test_fifo_enqueue_dequeue_and_peek(self) -> None:  # Queue should obey FIFO semantics.  
        q = ArrayQueue()  # Start with empty queue.  
        q.enqueue(10)  # Enqueue 10.  
        q.enqueue(20)  # Enqueue 20.  
        q.enqueue(30)  # Enqueue 30.  
        self.assertEqual(10, q.peek())  # Peek should return first enqueued value.  
        self.assertEqual(10, q.dequeue().value)  # First dequeue returns 10.  
        self.assertEqual(20, q.dequeue().value)  # Second dequeue returns 20.  
        self.assertEqual(30, q.dequeue().value)  # Third dequeue returns 30.  
        self.assertTrue(q.is_empty())  # Queue should be empty after removing all.  

    def test_enqueue_resize_copied_counts(self) -> None:  # copied should count resize copies exactly.  
        q = ArrayQueue()  # Start with empty queue (capacity=1).  
        self.assertEqual(0, q.enqueue(10).copied)  # First enqueue fits without resize.  
        self.assertEqual(1, q.enqueue(20).copied)  # Resize 1->2 copies 1 element.  
        self.assertEqual(2, q.enqueue(30).copied)  # Resize 2->4 copies 2 elements.  
        self.assertEqual(0, q.enqueue(40).copied)  # Fits in capacity 4.  
        self.assertEqual(4, q.enqueue(50).copied)  # Resize 4->8 copies 4 elements.  
        self.assertEqual(5, q.size)  # Validate final size.  
        self.assertEqual(8, q.capacity)  # Validate final capacity.  
        self.assertEqual(1 + 2 + 4, q.total_copies)  # Total copies should sum resizes.  
        self.assertEqual([10, 20, 30, 40, 50], q.to_list())  # Ordering should remain FIFO.  

    def test_dequeue_moved_cost_is_n_minus_1(self) -> None:  # Dequeue should shift left size-1 elements in naive array queue.  
        for n in [1, 2, 3, 5, 10]:  # Multiple sizes including boundary cases.  
            q = build_ordered_queue(n)  # Build [0..n-1].  
            r = q.dequeue()  # Dequeue once.  
            self.assertEqual(0, r.value)  # Dequeued value should be 0.  
            expected_moved = 0 if n <= 1 else n - 1  # Shifting left moves n-1 elements.  
            self.assertEqual(expected_moved, r.cost.moved)  # Validate moved count.  
            self.assertEqual(list(range(1, n)), q.to_list())  # Remaining queue should be [1..n-1].  

    def test_empty_operations_raise(self) -> None:  # dequeue/peek should reject empty queue.  
        q = ArrayQueue()  # Create empty queue.  
        with self.assertRaises(IndexError):  # peek should raise on empty.  
            q.peek()  # Invalid peek.  
        with self.assertRaises(IndexError):  # dequeue should raise on empty.  
            q.dequeue()  # Invalid dequeue.  


if __name__ == "__main__":  # Allow running tests via `python3 test_queue_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

