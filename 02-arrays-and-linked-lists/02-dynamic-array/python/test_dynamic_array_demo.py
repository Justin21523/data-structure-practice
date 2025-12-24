"""動態陣列示範測試 / Tests for dynamic array demo.  # Bilingual module header.

We validate both correctness (values/ordering) and deterministic resize/shift cost counts.  # English intent.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from dynamic_array_demo import DynamicArray, simulate_appends  # Import code under test.  


def _is_power_of_two(x: int) -> bool:  # Helper for verifying doubling-capacity behavior.  
    return x > 0 and (x & (x - 1) == 0)  # Standard bit trick for powers of two.  


class DynamicArrayDemoTests(unittest.TestCase):  # Group dynamic array tests in one test case.  
    def test_append_grows_capacity_by_doubling(self) -> None:  # Capacity should be power-of-two and >= size.  
        for m in [0, 1, 2, 3, 4, 5, 8, 9, 16, 33, 100]:  # Mix powers-of-two and non-powers-of-two.  
            s = simulate_appends(m)  # Simulate m appends and read summary.  
            self.assertEqual(m, s.final_size)  # Size should equal number of appends.  
            self.assertTrue(_is_power_of_two(s.final_capacity))  # Capacity should be power of two.  
            self.assertGreaterEqual(s.final_capacity, max(1, m))  # Capacity should be sufficient.  

    def test_total_copies_equals_final_capacity_minus_one(self) -> None:  # Doubling copies sum to cap-1.  
        for m in [0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33]:  # Cover boundaries around resizes.  
            s = simulate_appends(m)  # Simulate m appends.  
            self.assertEqual(s.final_capacity - 1, s.total_copies)  # Copies should equal 1+2+4+... = cap-1.  

    def test_aggregate_total_cost_is_linear(self) -> None:  # Total actual cost (writes+copies) should be <= 3m.  
        for m in [0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256]:  # Multiple sizes for robustness.  
            s = simulate_appends(m)  # Simulate m appends.  
            self.assertLessEqual(s.total_actual_cost, 3 * m)  # Verify amortized O(1) bound.  

    def test_insert_at_shifts_right(self) -> None:  # insert_at should shift right and keep ordering.  
        a = DynamicArray()  # Create empty array.  
        a.append(1)  # [1]  
        a.append(2)  # [1,2]  
        a.append(3)  # [1,2,3]  
        cost = a.insert_at(1, 99)  # Insert at index 1.  
        self.assertEqual(2, cost.moved)  # Moving right should shift 2 elements.  
        self.assertEqual([1, 99, 2, 3], a.to_list())  # Validate ordering after insert.  

    def test_remove_at_shifts_left(self) -> None:  # remove_at should shift left and keep ordering.  
        a = DynamicArray()  # Create empty array.  
        for v in [10, 20, 30, 40]:  # Fill with deterministic values.  
            a.append(v)  # Append one value.  
        result = a.remove_at(1)  # Remove element 20.  
        self.assertEqual(20, result.value)  # Validate removed value.  
        self.assertEqual(2, result.cost.moved)  # Removing at index 1 should move 2 elements left.  
        self.assertEqual([10, 30, 40], a.to_list())  # Validate ordering after removal.  

    def test_invalid_indices_raise(self) -> None:  # get/set/insert/remove should validate indices.  
        a = DynamicArray()  # Create empty array.  
        a.append(1)  # Size becomes 1.  
        with self.assertRaises(IndexError):  # get should reject invalid index.  
            a.get(1)  # Index 1 is out of range.  
        with self.assertRaises(IndexError):  # set should reject invalid index.  
            a.set(-1, 0)  # Negative index invalid.  
        with self.assertRaises(IndexError):  # insert_at should reject index > size.  
            a.insert_at(2, 0)  # When size=1, index 2 invalid for insert.  
        with self.assertRaises(IndexError):  # remove_at should reject index >= size.  
            a.remove_at(1)  # Index 1 invalid for remove.  


if __name__ == "__main__":  # Allow running tests via `python3 test_dynamic_array_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

