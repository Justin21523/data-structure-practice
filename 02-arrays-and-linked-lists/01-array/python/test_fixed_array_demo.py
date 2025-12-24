"""靜態陣列示範測試 / Tests for static array demo.  # Bilingual module header.

We validate both correctness (values/ordering) and the deterministic "moved elements" counts.  # English test intent.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework (no external deps).  

from fixed_array_demo import FixedArray, RemoveResult, simulate_insert_moves, simulate_remove_moves  # Import code under test.  


class FixedArrayDemoTests(unittest.TestCase):  # Group fixed-array tests in one test case.  
    def test_append_and_get(self) -> None:  # Appending should place values at increasing indices.  
        a = FixedArray(capacity=3)  # Create a small array for deterministic testing.  
        self.assertEqual(0, a.append(10))  # Append returns moved=0 for tail insert.  
        self.assertEqual(0, a.append(20))  # Append returns moved=0 for tail insert.  
        self.assertEqual(10, a.get(0))  # Validate first element.  
        self.assertEqual(20, a.get(1))  # Validate second element.  
        self.assertEqual([10, 20], a.to_list())  # Validate ordering.  

    def test_set_updates_value(self) -> None:  # set() should update in-place without shifting.  
        a = FixedArray(capacity=2)  # Create a small array.  
        a.append(1)  # Fill index 0.  
        a.append(2)  # Fill index 1.  
        a.set(1, 99)  # Update index 1.  
        self.assertEqual([1, 99], a.to_list())  # Validate updated list.  

    def test_insert_at_shifts_right_and_counts_moves(self) -> None:  # insert_at should shift right correctly.  
        a = FixedArray(capacity=5)  # Create capacity for insertion.  
        a.append(1)  # Array: [1]  
        a.append(2)  # Array: [1,2]  
        a.append(3)  # Array: [1,2,3]  
        moved = a.insert_at(1, 99)  # Insert in middle; should shift 2 elements (2,3).  
        self.assertEqual(2, moved)  # Validate moved count = size_before - index.  
        self.assertEqual([1, 99, 2, 3], a.to_list())  # Validate content after insertion.  

    def test_remove_at_shifts_left_and_counts_moves(self) -> None:  # remove_at should shift left correctly.  
        a = FixedArray(capacity=4)  # Create a small array.  
        for v in [10, 20, 30, 40]:  # Fill array fully.  
            a.append(v)  # Append each value.  
        result = a.remove_at(1)  # Remove the element 20; should shift 30 and 40 left (2 moves).  
        self.assertIsInstance(result, RemoveResult)  # Ensure result is structured.  
        self.assertEqual(20, result.value)  # Validate removed value.  
        self.assertEqual(2, result.moved)  # Validate moved count = size_before - index - 1.  
        self.assertEqual([10, 30, 40], a.to_list())  # Validate content after removal.  

    def test_index_of(self) -> None:  # index_of should find first occurrence or return -1.  
        a = FixedArray(capacity=5)  # Create a small array.  
        for v in [7, 8, 7]:  # Insert duplicates to test first-match behavior.  
            a.append(v)  # Append values.  
        self.assertEqual(0, a.index_of(7))  # First 7 is at index 0.  
        self.assertEqual(1, a.index_of(8))  # 8 is at index 1.  
        self.assertEqual(-1, a.index_of(999))  # Missing value returns -1.  

    def test_overflow_raises(self) -> None:  # Append/insert should fail when the array is full.  
        a = FixedArray(capacity=1)  # Capacity 1 means we can store only one element.  
        a.append(1)  # Fill the array.  
        with self.assertRaises(OverflowError):  # Expect overflow on second append.  
            a.append(2)  # This should raise because there is no space.  

    def test_invalid_indices_raise(self) -> None:  # get/set/insert/remove should validate indices.  
        a = FixedArray(capacity=2)  # Create a small array.  
        a.append(1)  # Size is now 1.  
        with self.assertRaises(IndexError):  # get() should reject out-of-range index.  
            a.get(1)  # Index 1 is invalid when size is 1.  
        with self.assertRaises(IndexError):  # set() should reject out-of-range index.  
            a.set(-1, 0)  # Negative index is invalid.  
        with self.assertRaises(IndexError):  # insert_at should reject index > size.  
            a.insert_at(2, 0)  # When size=1, index 2 is invalid for insert.  
        with self.assertRaises(IndexError):  # remove_at should reject index >= size.  
            a.remove_at(1)  # Index 1 is invalid when size is 1.  

    def test_simulation_move_formulas(self) -> None:  # Simulations should match the known formulas.  
        self.assertEqual(0, simulate_insert_moves(0, 0))  # Inserting into empty array moves 0 elements.  
        self.assertEqual(4, simulate_insert_moves(4, 0))  # Insert at head moves n elements.  
        self.assertEqual(0, simulate_insert_moves(4, 4))  # Insert at tail moves 0 elements.  
        self.assertEqual(3, simulate_remove_moves(4, 0))  # Remove at head moves n-1 elements.  
        self.assertEqual(0, simulate_remove_moves(4, 3))  # Remove at tail moves 0 elements.  


if __name__ == "__main__":  # Allow running tests via `python3 test_fixed_array_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

