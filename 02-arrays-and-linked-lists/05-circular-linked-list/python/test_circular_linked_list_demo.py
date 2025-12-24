"""環狀鏈結串列示範測試 / Tests for circular linked list demo.  # Bilingual module header.

We validate correctness (ordering/values) and deterministic traversal costs (hops).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from circular_linked_list_demo import (  # Import code under test.  
    CircularLinkedList,  # Circular linked list implementation.  
    build_ordered_list,  # Helper for building [0..n-1] lists.  
)  


class CircularLinkedListDemoTests(unittest.TestCase):  # Group circular linked list tests in one test case.  
    def test_push_front_and_push_back_cost_zero(self) -> None:  # push operations should be O(1) with tail pointer.  
        lst = CircularLinkedList()  # Start with empty list.  
        self.assertEqual(0, lst.push_back(1).hops)  # push_back should cost 0 hops.  
        self.assertEqual([1], lst.to_list())  # Validate ordering.  
        self.assertEqual(0, lst.push_front(0).hops)  # push_front should cost 0 hops.  
        self.assertEqual([0, 1], lst.to_list())  # Validate ordering.  

    def test_pop_front_cost_zero(self) -> None:  # pop_front should be O(1).  
        lst = build_ordered_list(3)  # Build [0,1,2].  
        r = lst.pop_front()  # Pop head.  
        self.assertEqual(0, r.value)  # Removed value should be 0.  
        self.assertEqual(0, r.cost.hops)  # pop_front should cost 0 hops.  
        self.assertEqual([1, 2], lst.to_list())  # Validate ordering after removal.  

    def test_pop_back_cost_is_n_minus_2(self) -> None:  # pop_back requires finding predecessor in singly circular list.  
        for n in [1, 2, 3, 5, 10]:  # Multiple sizes including boundary cases.  
            lst = build_ordered_list(n)  # Build [0..n-1].  
            r = lst.pop_back()  # Pop tail.  
            self.assertEqual(n - 1, r.value)  # Removed value should be last element.  
            expected_hops = 0 if n <= 1 else n - 2  # Predecessor search traverses n-2 links.  
            self.assertEqual(expected_hops, r.cost.hops)  # Validate traversal cost.  
            self.assertEqual(list(range(n - 1)), lst.to_list())  # Validate ordering after removal.  

    def test_get_with_cost_hops_equals_index(self) -> None:  # get_with_cost(i) should traverse i links from head.  
        lst = build_ordered_list(10)  # Build [0..9].  
        for i in [0, 1, 5, 9]:  # Sample indices.  
            r = lst.get_with_cost(i)  # Fetch value + cost.  
            self.assertEqual(i, r.value)  # Value should equal index in ordered list.  
            self.assertEqual(i, r.cost.hops)  # Hops should equal index.  

    def test_rotate_changes_order_and_cost(self) -> None:  # rotate(k) should rotate left by k steps with hops=k%size.  
        base = [0, 1, 2, 3]  # Base list for expected rotations.  
        for steps, expected in [  # Provide (steps, expected_list) pairs.  
            (0, [0, 1, 2, 3]),  # No rotation.  
            (1, [1, 2, 3, 0]),  # Left rotate by 1.  
            (3, [3, 0, 1, 2]),  # Left rotate by 3.  
            (4, [0, 1, 2, 3]),  # Full cycle rotation.  
            (7, [3, 0, 1, 2]),  # 7 % 4 == 3.  
        ]:  # Close list literal.  
            lst = build_ordered_list(len(base))  # Build base list.  
            cost = lst.rotate(steps)  # Rotate by steps.  
            self.assertEqual(steps % len(base), cost.hops)  # Validate hop count (mod size).  
            self.assertEqual(expected, lst.to_list())  # Validate rotated ordering.  

    def test_index_of_with_cost_hops(self) -> None:  # index_of_with_cost should count next traversals.  
        lst = build_ordered_list(5)  # Build [0..4].  
        f0 = lst.index_of_with_cost(0)  # Find head value.  
        self.assertEqual(0, f0.index)  # Index should be 0.  
        self.assertEqual(0, f0.cost.hops)  # Finding head needs 0 hops.  
        f3 = lst.index_of_with_cost(3)  # Find value at index 3.  
        self.assertEqual(3, f3.index)  # Index should be 3.  
        self.assertEqual(3, f3.cost.hops)  # Finding index 3 needs 3 hops.  
        fn = lst.index_of_with_cost(999)  # Search for missing value.  
        self.assertEqual(-1, fn.index)  # Not found sentinel.  
        self.assertEqual(5, fn.cost.hops)  # Not found traverses exactly size hops.  

    def test_invalid_indices_and_inputs(self) -> None:  # Methods should validate indices and inputs.  
        lst = build_ordered_list(3)  # Build [0..2].  
        with self.assertRaises(IndexError):  # get should reject index==size.  
            lst.get_with_cost(3)  # Out of range.  
        with self.assertRaises(ValueError):  # rotate should reject negative steps.  
            lst.rotate(-1)  # Invalid steps.  
        empty = CircularLinkedList()  # Create empty list.  
        with self.assertRaises(IndexError):  # pop_front should reject empty list.  
            empty.pop_front()  # Pop on empty.  
        with self.assertRaises(IndexError):  # pop_back should reject empty list.  
            empty.pop_back()  # Pop on empty.  


if __name__ == "__main__":  # Allow running tests via `python3 test_circular_linked_list_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

