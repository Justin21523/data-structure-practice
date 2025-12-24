"""雙向鏈結串列示範測試 / Tests for doubly linked list demo.  # Bilingual module header.

We validate correctness (ordering/values) and deterministic traversal costs (hops).  # English overview.
"""  # End of module docstring.

import unittest  # Use the built-in unittest framework.  

from doubly_linked_list_demo import (  # Import code under test.  
    DoublyLinkedList,  # Doubly linked list implementation.  
    build_ordered_list,  # Helper for building [0..n-1] lists.  
)  


def _expected_get_hops(size: int, index: int) -> int:  # Compute expected hops when traversing from the nearest end.  
    return min(index, size - 1 - index)  # Nearest-end traversal cost formula.  


class DoublyLinkedListDemoTests(unittest.TestCase):  # Group doubly linked list tests in one test case.  
    def test_push_and_pop_ends_are_o1(self) -> None:  # push/pop at both ends should have 0 hops with head+tail.  
        for n in [0, 1, 2, 5]:  # Multiple sizes including boundary cases.  
            lst = build_ordered_list(n)  # Build [0..n-1].  
            self.assertEqual(0, lst.push_back(999).hops)  # pushBack should be O(1).  
            self.assertEqual(0, lst.push_front(888).hops)  # pushFront should be O(1).  
            self.assertEqual(0, lst.pop_back().cost.hops)  # popBack should be O(1).  
            self.assertEqual(0, lst.pop_front().cost.hops)  # popFront should be O(1).  

    def test_get_with_cost_uses_nearest_end(self) -> None:  # get(i) hops should be min(i, size-1-i).  
        size = 16  # Use a fixed size to cover both head-side and tail-side indices.  
        lst = build_ordered_list(size)  # Build [0..15].  
        for i in [0, 1, 2, 4, 8, 15]:  # Sample indices including near tail.  
            r = lst.get_with_cost(i)  # Fetch value + cost.  
            self.assertEqual(i, r.value)  # Ordered list value should equal index.  
            self.assertEqual(_expected_get_hops(size, i), r.cost.hops)  # Validate hops formula.  

    def test_insert_at_cost_and_ordering(self) -> None:  # insert_at should splice correctly and report traversal hops.  
        lst = build_ordered_list(6)  # Build [0..5].  
        cost0 = lst.insert_at(0, 99)  # Insert at head.  
        self.assertEqual(0, cost0.hops)  # Head insertion needs no traversal.  
        self.assertEqual([99, 0, 1, 2, 3, 4, 5], lst.to_list())  # Validate ordering after insertion.  

        lst2 = build_ordered_list(6)  # Build [0..5] again.  
        cost3 = lst2.insert_at(3, 77)  # Insert before value 3.  
        self.assertEqual(min(3, 6 - 1 - 3), cost3.hops)  # Hops should match nearest-end traversal on size=6.  
        self.assertEqual([0, 1, 2, 77, 3, 4, 5], lst2.to_list())  # Validate ordering after insertion.  

        lst3 = build_ordered_list(6)  # Build [0..5] again.  
        cost_end = lst3.insert_at(6, 55)  # Insert at tail (index==size).  
        self.assertEqual(0, cost_end.hops)  # Tail insertion needs no traversal.  
        self.assertEqual([0, 1, 2, 3, 4, 5, 55], lst3.to_list())  # Validate ordering after insertion.  

    def test_remove_at_cost_and_ordering(self) -> None:  # remove_at should bypass correctly and report traversal hops.  
        lst = build_ordered_list(6)  # Build [0..5].  
        r0 = lst.remove_at(0)  # Remove head.  
        self.assertEqual(0, r0.value)  # Removed value should be 0.  
        self.assertEqual(0, r0.cost.hops)  # Head removal needs no traversal.  
        self.assertEqual([1, 2, 3, 4, 5], lst.to_list())  # Validate ordering after removal.  

        lst2 = build_ordered_list(6)  # Build [0..5] again.  
        r5 = lst2.remove_at(5)  # Remove tail.  
        self.assertEqual(5, r5.value)  # Removed value should be 5.  
        self.assertEqual(0, r5.cost.hops)  # Tail removal needs no traversal.  
        self.assertEqual([0, 1, 2, 3, 4], lst2.to_list())  # Validate ordering after removal.  

        lst3 = build_ordered_list(6)  # Build [0..5] again.  
        r3 = lst3.remove_at(3)  # Remove value 3.  
        self.assertEqual(3, r3.value)  # Removed value should be 3.  
        self.assertEqual(min(3, 6 - 1 - 3), r3.cost.hops)  # Hops should match nearest-end traversal.  
        self.assertEqual([0, 1, 2, 4, 5], lst3.to_list())  # Validate ordering after removal.  

    def test_index_of_with_cost_hops(self) -> None:  # index_of_with_cost should count forward traversals.  
        lst = build_ordered_list(5)  # Build [0..4].  
        f0 = lst.index_of_with_cost(0)  # Find head value.  
        self.assertEqual(0, f0.index)  # Index should be 0.  
        self.assertEqual(0, f0.cost.hops)  # Finding head needs 0 hops.  
        f3 = lst.index_of_with_cost(3)  # Find value at index 3.  
        self.assertEqual(3, f3.index)  # Index should be 3.  
        self.assertEqual(3, f3.cost.hops)  # Finding index 3 needs 3 hops.  
        fn = lst.index_of_with_cost(999)  # Search for missing value.  
        self.assertEqual(-1, fn.index)  # Not found sentinel.  
        self.assertEqual(5, fn.cost.hops)  # Not found traverses n hops.  

    def test_invalid_indices_raise(self) -> None:  # Operations should validate indices.  
        lst = build_ordered_list(3)  # Build [0..2].  
        with self.assertRaises(IndexError):  # get should reject invalid index.  
            lst.get_with_cost(3)  # Out of range.  
        with self.assertRaises(IndexError):  # insert should reject index>size.  
            lst.insert_at(4, 0)  # Out of range for insert.  
        with self.assertRaises(IndexError):  # remove should reject index==size.  
            lst.remove_at(3)  # Out of range for remove.  
        empty = DoublyLinkedList()  # Create empty list.  
        with self.assertRaises(IndexError):  # pop_back should reject empty list.  
            empty.pop_back()  # Pop on empty.  
        with self.assertRaises(IndexError):  # pop_front should reject empty list.  
            empty.pop_front()  # Pop on empty.  


if __name__ == "__main__":  # Allow running tests via `python3 test_doubly_linked_list_demo.py`.  
    unittest.main(verbosity=2)  # Run with higher verbosity for easier debugging.  

