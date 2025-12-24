"""攤銷分析示範測試 / Tests for amortized analysis demo.  # Bilingual module header.

We validate three amortized-analysis viewpoints on dynamic array append:  # English overview for readers.
- Aggregate bound: total actual cost grows linearly with m  # Show the aggregate method result.
- Accounting method: bank (credit) never becomes negative  # Show the accounting invariant.
- Potential method: amortized cost per operation is constant (3)  # Show the potential-method result.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework (no external deps).  

from amortized_demo import AmortizedDynamicArray, potential, simulate_appends  # Import the demo code under test.  


def _is_power_of_two(x: int) -> bool:  # Small helper for capacity validation in tests.  
    return x > 0 and (x & (x - 1) == 0)  # Standard bit trick for power-of-two detection.  


class AmortizedDemoTests(unittest.TestCase):  # Group amortized-analysis tests in a single test case.  
    def test_capacity_is_power_of_two_and_ge_size(self) -> None:  # Capacity should follow the doubling rule.  
        for m in [0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 33, 100]:  # Use a mix of powers-of-two and non-powers-of-two.  
            summary = simulate_appends(m)  # Run the simulation and capture its summary.  
            self.assertEqual(m, summary.final_size)  # Size must match the number of appends performed.  
            self.assertTrue(_is_power_of_two(summary.final_capacity))  # Capacity should be a power of two.  
            self.assertGreaterEqual(summary.final_capacity, max(1, m))  # Capacity must be sufficient to hold m items.  

    def test_total_copies_equals_final_capacity_minus_one(self) -> None:  # Doubling copies sum to cap-1.  
        for m in [0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33]:  # Cover boundaries around resizes.  
            summary = simulate_appends(m)  # Run the simulation and capture its summary.  
            self.assertEqual(summary.final_capacity - 1, summary.total_copies)  # Copies should equal 1+2+4+... = cap-1.  

    def test_aggregate_total_cost_is_linear(self) -> None:  # Total actual cost should be <= 3m for this model.  
        for m in [0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256]:  # Test several sizes to reduce off-by-one mistakes.  
            summary = simulate_appends(m)  # Run the simulation and capture its summary.  
            self.assertLessEqual(summary.total_actual_cost, 3 * m)  # Verify the classic O(1) amortized bound.  

    def test_accounting_bank_never_negative(self) -> None:  # bank should never drop below 0 when charging 3.  
        table = AmortizedDynamicArray()  # Start from an empty table with 0 credit.  
        for i in range(256):  # Run a reasonably large sequence to hit multiple resizes.  
            step = table.append(i)  # Append one element and obtain its step record.  
            self.assertGreaterEqual(step.bank_after, 0)  # Ensure credit never becomes negative.  

    def test_potential_method_amortized_cost_is_three(self) -> None:  # amortized cost should equal 3 for each append.  
        table = AmortizedDynamicArray()  # Start from an empty table so Φ starts at 0.  
        for i in range(256):  # Run enough operations to include many cheap and expensive steps.  
            step = table.append(i)  # Append one element and capture per-step cost info.  
            self.assertEqual(3, step.amortized_cost)  # Potential method should make each step's amortized cost constant.  

    def test_potential_function_starts_at_zero(self) -> None:  # Φ(0,1) should be 0 by our chosen offset.  
        self.assertEqual(0, potential(0, 1))  # Validate the chosen +1 offset in the Φ definition.  


if __name__ == "__main__":  # Allow running tests via `python3 test_amortized_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity so failures are easier to diagnose.  

