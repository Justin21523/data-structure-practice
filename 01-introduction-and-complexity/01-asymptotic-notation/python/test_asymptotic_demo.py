"""漸進符號示範測試 / Tests for asymptotic demo."""  # Module docstring (bilingual).  

import unittest  # Use the standard library test framework (no external deps).  

from asymptotic_demo import (  # Import the functions under test from the demo module.  
    count_constant_ops,  # O(1) example operation counter.  
    count_linear_ops,  # O(n) example operation counter.  
    count_log2_ops,  # O(log n) example operation counter.  
    count_n_log2_n_ops,  # O(n log n) example operation counter.  
    count_quadratic_ops,  # O(n^2) example operation counter.  
)  # End of import list.  


class AsymptoticDemoTests(unittest.TestCase):  # Group all demo tests in one test case class.  
    def test_constant_ops_is_constant(self) -> None:  # Verify the O(1) demo does not depend on n.  
        self.assertEqual(count_constant_ops(0), count_constant_ops(10))  # Same output for different inputs.  
        self.assertEqual(count_constant_ops(1), 3)  # This demo uses exactly 3 simulated operations.  

    def test_log2_ops_matches_halving_count(self) -> None:  # Verify the halving loop count matches floor(log2(n)).  
        self.assertEqual(count_log2_ops(1), 0)  # No halving needed when n == 1.  
        self.assertEqual(count_log2_ops(2), 1)  # 2 -> 1 uses one halving.  
        self.assertEqual(count_log2_ops(8), 3)  # 8 -> 4 -> 2 -> 1 uses three halvings.  

    def test_log2_ops_rejects_non_positive(self) -> None:  # Ensure invalid n is rejected for the log demo.  
        with self.assertRaises(ValueError):  # Expect a ValueError for n == 0.  
            _ = count_log2_ops(0)  # Trigger the validation path.  

    def test_linear_ops_equals_n(self) -> None:  # Verify the linear demo returns exactly n operations.  
        self.assertEqual(count_linear_ops(0), 0)  # Zero-sized input performs zero operations.  
        self.assertEqual(count_linear_ops(5), 5)  # One operation per iteration for n iterations.  

    def test_quadratic_ops_equals_n_squared(self) -> None:  # Verify the quadratic demo returns n^2 operations.  
        self.assertEqual(count_quadratic_ops(0), 0)  # Zero-sized input performs zero operations.  
        self.assertEqual(count_quadratic_ops(4), 16)  # Nested loops produce 4 * 4 = 16 operations.  

    def test_n_log_n_ops_equals_n_times_log2_n(self) -> None:  # Verify the n log n demo matches the intended pattern.  
        self.assertEqual(count_n_log2_n_ops(0), 0)  # Define 0 * log(0) as 0 for this discrete demo.  
        self.assertEqual(count_n_log2_n_ops(8), 24)  # 8 * log2(8) = 8 * 3 = 24 operations here.  


if __name__ == "__main__":  # Allow running this file directly for convenience.  
    unittest.main(verbosity=2)  # Execute the test suite with verbose output.  
