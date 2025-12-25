"""雙端佇列（Deque）示範測試 / Tests for deque demo.  # Bilingual module header.

We validate correctness (both-end operations), wrap-around behavior, and deterministic costs (moved=0).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from deque_demo import Deque  # Import code under test.  


class DequeDemoTests(unittest.TestCase):  # Group deque tests in one test case.  
    def test_basic_both_ends_operations(self) -> None:  # Deque should support operations at both ends.  
        d = Deque()  # Start with empty deque.  
        d.push_back(3)  # Push 3 to back.  
        d.push_back(7)  # Push 7 to back.  
        d.push_front(1)  # Push 1 to front.  
        self.assertEqual([1, 3, 7], d.to_list())  # Validate ordering.  
        self.assertEqual(1, d.peek_front())  # Validate peek_front.  
        self.assertEqual(7, d.peek_back())  # Validate peek_back.  
        self.assertEqual(1, d.pop_front().value)  # Pop front returns 1.  
        self.assertEqual(7, d.pop_back().value)  # Pop back returns 7.  
        self.assertEqual(3, d.pop_back().value)  # Pop back returns 3.  
        self.assertTrue(d.is_empty())  # Deque should be empty at end.  

    def test_moved_is_always_zero(self) -> None:  # With circular buffer, operations should not shift elements.  
        d = Deque()  # Start with empty deque.  
        self.assertEqual(0, d.push_back(10).moved)  # push_back should not shift.  
        self.assertEqual(0, d.push_front(20).moved)  # push_front should not shift.  
        self.assertEqual(0, d.pop_front().cost.moved)  # pop_front should not shift.  
        d.push_back(30)  # Add one more element.  
        self.assertEqual(0, d.pop_back().cost.moved)  # pop_back should not shift.  

    def test_resize_copied_counts_with_push_back(self) -> None:  # copied should count resize copies exactly.  
        d = Deque()  # Start with empty deque (capacity=1).  
        self.assertEqual(0, d.push_back(10).copied)  # First push fits without resize.  
        self.assertEqual(1, d.push_back(20).copied)  # Resize 1->2 copies 1 element.  
        self.assertEqual(2, d.push_back(30).copied)  # Resize 2->4 copies 2 elements.  
        self.assertEqual(0, d.push_back(40).copied)  # Fits in capacity 4.  
        self.assertEqual(4, d.push_back(50).copied)  # Resize 4->8 copies 4 elements.  
        self.assertEqual(5, d.size)  # Validate final size.  
        self.assertEqual(8, d.capacity)  # Validate final capacity.  
        self.assertEqual(1 + 2 + 4, d.total_copies)  # Total copies should sum resizes.  
        self.assertEqual([10, 20, 30, 40, 50], d.to_list())  # Ordering should be preserved.  

    def test_wrap_around_with_both_ends(self) -> None:  # push_front/pop_back should work when head wraps.  
        d = Deque()  # Start with empty deque.  
        for v in [0, 1, 2, 3]:  # Fill to capacity 4 (with resizes).  
            d.push_back(v)  # Push to back.  
        self.assertEqual([0, 1, 2, 3], d.to_list())  # Validate ordering.  
        self.assertEqual(0, d.pop_front().value)  # Pop front => 0 (head moves).  
        self.assertEqual(1, d.pop_front().value)  # Pop front => 1 (head moves).  
        d.push_back(4)  # Push back into wrapped slots if needed.  
        d.push_back(5)  # Push back again.  
        self.assertEqual([2, 3, 4, 5], d.to_list())  # Validate ordering after wrap-around.  
        d.push_front(1)  # Push front (head moves left with wrap-around).  
        self.assertEqual([1, 2, 3, 4, 5], d.to_list())  # Validate ordering after push_front.  

    def test_resize_when_head_not_zero_preserves_order(self) -> None:  # Resize should preserve order even when head is offset.  
        d = Deque()  # Start with empty deque.  
        for v in [0, 1, 2, 3]:  # Fill to capacity 4.  
            d.push_back(v)  # Push to back.  
        d.pop_front()  # Remove 0 (head moves).  
        d.pop_front()  # Remove 1 (head moves).  
        d.push_back(4)  # Push back (wrap-around).  
        d.push_back(5)  # Push back (deque full again).  
        cost = d.push_back(6)  # Trigger resize and push 6.  
        self.assertEqual(4, cost.copied)  # Resize should copy exactly size elements (4).  
        self.assertEqual(0, cost.moved)  # moved stays 0.  
        self.assertEqual([2, 3, 4, 5, 6], d.to_list())  # Ordering should be preserved after resize.  

    def test_empty_operations_raise(self) -> None:  # pops/peeks should reject empty deque.  
        d = Deque()  # Create empty deque.  
        with self.assertRaises(IndexError):  # peek_front should raise on empty.  
            d.peek_front()  # Invalid peek.  
        with self.assertRaises(IndexError):  # peek_back should raise on empty.  
            d.peek_back()  # Invalid peek.  
        with self.assertRaises(IndexError):  # pop_front should raise on empty.  
            d.pop_front()  # Invalid pop.  
        with self.assertRaises(IndexError):  # pop_back should raise on empty.  
            d.pop_back()  # Invalid pop.  


if __name__ == "__main__":  # Allow running tests via `python3 test_deque_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

