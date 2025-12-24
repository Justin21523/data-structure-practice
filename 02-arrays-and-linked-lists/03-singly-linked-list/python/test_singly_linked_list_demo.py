"""單向鏈結串列示範測試 / Tests for singly linked list demo.  # Bilingual module header.

We validate both correctness (ordering/values) and deterministic traversal costs (hops).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from singly_linked_list_demo import (  # Import code under test.  
    SinglyLinkedList,  # Singly linked list implementation.  
    build_ordered_list,  # Helper for building [0..n-1] lists.  
)  


class SinglyLinkedListDemoTests(unittest.TestCase):  # Group singly linked list tests in one test case.  
    def test_push_front_hops_is_zero(self) -> None:  # push_front should never traverse next pointers.  
        lst = SinglyLinkedList()  # Start with an empty list.  
        for v in [1, 2, 3, 4]:  # Insert a few values.  
            cost = lst.push_front(v)  # Head insertion.  
            self.assertEqual(0, cost.hops)  # Head insertion cost is always zero hops.  

    def test_push_back_hops_equals_size_minus_one(self) -> None:  # push_back should traverse to tail without a tail pointer.  
        for n in [0, 1, 2, 3, 5, 10]:  # Multiple sizes including boundary cases.  
            lst = build_ordered_list(n)  # Build [0..n-1].  
            cost = lst.push_back(999)  # Append sentinel at tail.  
            expected = 0 if n == 0 else n - 1  # Without tail pointer, hops should be n-1 for non-empty lists.  
            self.assertEqual(expected, cost.hops)  # Validate traversal cost.  
            self.assertEqual(n + 1, lst.size)  # Size should increase by one.  
            self.assertEqual(999, lst.to_list()[-1])  # The appended value should be at the end.  

    def test_get_with_cost_hops_equals_index(self) -> None:  # get_with_cost(i) should traverse i links.  
        lst = build_ordered_list(10)  # Build list [0..9].  
        for i in [0, 1, 5, 9]:  # Sample indices.  
            r = lst.get_with_cost(i)  # Fetch value and cost.  
            self.assertEqual(i, r.value)  # Value should equal index in the ordered list.  
            self.assertEqual(i, r.cost.hops)  # Hops should equal index (number of next traversals).  

    def test_insert_at_hops_equals_index_minus_one(self) -> None:  # insert_at(i) should traverse (i-1) links for i>0.  
        a = build_ordered_list(5)  # Build [0..4].  
        c0 = a.insert_at(0, 99)  # Insert at head.  
        self.assertEqual(0, c0.hops)  # Head insertion has zero hops.  
        self.assertEqual([99, 0, 1, 2, 3, 4], a.to_list())  # Validate ordering after insertion.  

        b = build_ordered_list(5)  # Build [0..4] again.  
        c3 = b.insert_at(3, 77)  # Insert before value 3.  
        self.assertEqual(2, c3.hops)  # For index 3, predecessor is index 2 => 2 hops.  
        self.assertEqual([0, 1, 2, 77, 3, 4], b.to_list())  # Validate ordering after insertion.  

    def test_remove_at_hops_equals_index_minus_one(self) -> None:  # remove_at(i) should traverse (i-1) links for i>0.  
        a = build_ordered_list(5)  # Build [0..4].  
        r0 = a.remove_at(0)  # Remove head.  
        self.assertEqual(0, r0.value)  # Removed value should be 0.  
        self.assertEqual(0, r0.cost.hops)  # Head removal has zero hops.  
        self.assertEqual([1, 2, 3, 4], a.to_list())  # Validate ordering after removal.  

        b = build_ordered_list(5)  # Build [0..4] again.  
        r3 = b.remove_at(3)  # Remove value 3.  
        self.assertEqual(3, r3.value)  # Removed value should be 3.  
        self.assertEqual(2, r3.cost.hops)  # For index 3, predecessor is index 2 => 2 hops.  
        self.assertEqual([0, 1, 2, 4], b.to_list())  # Validate ordering after removal.  

    def test_index_of_with_cost_hops(self) -> None:  # index_of_with_cost should count next traversals.  
        lst = build_ordered_list(5)  # Build [0..4].  
        f0 = lst.index_of_with_cost(0)  # Find head value.  
        self.assertEqual(0, f0.index)  # Index should be 0.  
        self.assertEqual(0, f0.cost.hops)  # Finding head needs zero hops.  
        f3 = lst.index_of_with_cost(3)  # Find a middle value.  
        self.assertEqual(3, f3.index)  # Index should be 3.  
        self.assertEqual(3, f3.cost.hops)  # Reaching index 3 takes 3 hops.  
        fn = lst.index_of_with_cost(999)  # Search for missing value.  
        self.assertEqual(-1, fn.index)  # Not found sentinel.  
        self.assertEqual(5, fn.cost.hops)  # Not found traverses through all nodes => n hops.  

    def test_invalid_indices_raise(self) -> None:  # Operations should validate indices.  
        lst = build_ordered_list(3)  # Build [0..2].  
        with self.assertRaises(IndexError):  # get should reject negative index.  
            lst.get_with_cost(-1)  # Invalid index.  
        with self.assertRaises(IndexError):  # get should reject index==size.  
            lst.get_with_cost(3)  # Out of range.  
        with self.assertRaises(IndexError):  # insert should reject index>size.  
            lst.insert_at(4, 0)  # Out of range for insert.  
        with self.assertRaises(IndexError):  # remove should reject index==size.  
            lst.remove_at(3)  # Out of range for remove.  


if __name__ == "__main__":  # Allow running tests via `python3 test_singly_linked_list_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

