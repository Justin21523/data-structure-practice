"""堆疊（Stack）示範測試 / Tests for stack demo.  # Bilingual module header.

We validate correctness (LIFO ordering) and deterministic resize costs (copied).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from stack_demo import ArrayStack  # Import code under test.  


class StackDemoTests(unittest.TestCase):  # Group stack tests in one test case.  
    def test_lifo_push_pop_and_peek(self) -> None:  # Stack should obey LIFO semantics.  
        s = ArrayStack()  # Start with empty stack.  
        s.push(10)  # Push 10.  
        s.push(20)  # Push 20.  
        s.push(30)  # Push 30.  
        self.assertEqual(30, s.peek())  # Peek should return last pushed value.  
        self.assertEqual(30, s.pop().value)  # First pop returns 30.  
        self.assertEqual(20, s.pop().value)  # Second pop returns 20.  
        self.assertEqual(10, s.pop().value)  # Third pop returns 10.  
        self.assertTrue(s.is_empty())  # Stack should be empty after popping all.  

    def test_push_resize_copied_counts(self) -> None:  # copied should count resize copies exactly.  
        s = ArrayStack()  # Start with empty stack (capacity=1).  
        self.assertEqual(0, s.push(10).copied)  # First push fits without resize.  
        self.assertEqual(1, s.push(20).copied)  # Resize 1->2 copies 1 element.  
        self.assertEqual(2, s.push(30).copied)  # Resize 2->4 copies 2 elements.  
        self.assertEqual(0, s.push(40).copied)  # Fits in capacity 4.  
        self.assertEqual(4, s.push(50).copied)  # Resize 4->8 copies 4 elements.  
        self.assertEqual(5, s.size)  # Validate final size.  
        self.assertEqual(8, s.capacity)  # Validate final capacity.  
        self.assertEqual(1 + 2 + 4, s.total_copies)  # Total copies should sum resizes.  

    def test_empty_operations_raise(self) -> None:  # pop/peek should reject empty stack.  
        s = ArrayStack()  # Create empty stack.  
        with self.assertRaises(IndexError):  # peek should raise on empty.  
            s.peek()  # Invalid peek.  
        with self.assertRaises(IndexError):  # pop should raise on empty.  
            s.pop()  # Invalid pop.  


if __name__ == "__main__":  # Allow running tests via `python3 test_stack_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

