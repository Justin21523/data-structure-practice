"""單向鏈結串列（Singly Linked List）示範 / Singly linked list demo.  # Bilingual module header.

本單元實作「單向鏈結串列（singly linked list）」並量化走訪成本：  # Traditional Chinese overview.
- 每個節點有 value 與 next 指標（pointer/reference）  # Explain node structure.
- 我們只維護 head（不維護 tail），用來凸顯尾端插入可能是 O(n)  # Explain why we omit tail.
- 我們用 hops 計數「做了幾次 current = current.next」  # Define the cost metric.

We return deterministic per-operation costs so tests can validate both correctness and complexity intuition.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable records.  
import sys  # Read CLI arguments for demo sizes.  
from typing import List, Optional, Sequence  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make cost records immutable and deterministic.  
class OperationCost:  # Record the traversal cost of one operation.  
    hops: int  # Number of `next` pointer traversals performed.  


@dataclass(frozen=True)  # Make get results immutable.  
class GetResult:  # Return fetched value plus its traversal cost.  
    value: int  # The fetched element value.  
    cost: OperationCost  # Traversal cost record.  


@dataclass(frozen=True)  # Make remove results immutable.  
class RemoveResult:  # Return removed value plus its traversal cost.  
    value: int  # The removed element value.  
    cost: OperationCost  # Traversal cost record.  


@dataclass(frozen=True)  # Make find results immutable.  
class FindResult:  # Return found index plus traversal cost.  
    index: int  # Found index, or -1 if not found.  
    cost: OperationCost  # Traversal cost record.  


class _Node:  # A singly linked list node (value + next pointer).  
    __slots__ = ("value", "next")  # Keep the node minimal and explicit.  

    def __init__(self, value: int, next_node: Optional[_Node]) -> None:  # Initialize a node with value and next pointer.  
        self.value = value  # Store node value.  
        self.next: Optional[_Node] = next_node  # Store next pointer (or None).  


class SinglyLinkedList:  # A teaching-oriented singly linked list (head-only, no tail).  
    def __init__(self) -> None:  # Initialize an empty list.  
        self._head: Optional[_Node] = None  # Head pointer (None means empty).  
        self._size = 0  # Track list size for O(1) length queries.  

    @property
    def size(self) -> int:  # Expose size for callers/tests.  
        return self._size  # Return the number of stored nodes.  

    def is_empty(self) -> bool:  # Convenience helper to check emptiness.  
        return self._size == 0  # Empty iff size is zero.  

    def to_list(self) -> List[int]:  # Convert to a Python list for easy validation/printing.  
        out: List[int] = []  # Accumulate values in order.  
        current = self._head  # Start traversal from head.  
        while current is not None:  # Stop at end of list.  
            out.append(current.value)  # Append one value.  
            current = current.next  # Move to next node.  
        return out  # Return the collected values.  

    def _require_index_in_range(self, index: int) -> None:  # Validate index in [0, size-1].  
        if index < 0 or index >= self._size:  # Reject invalid indices.  
            raise IndexError("index out of range")  # Signal invalid index.  

    def _require_insert_index_in_range(self, index: int) -> None:  # Validate insert index in [0, size].  
        if index < 0 or index > self._size:  # Reject invalid insertion indices.  
            raise IndexError("index out of range for insert")  # Signal invalid index.  

    def push_front(self, value: int) -> OperationCost:  # Insert at head (O(1), hops=0).  
        self._head = _Node(value, self._head)  # Create new node and link it before current head.  
        self._size += 1  # Update size.  
        return OperationCost(hops=0)  # Head insertion does not traverse `next`.  

    def push_back(self, value: int) -> OperationCost:  # Insert at tail (O(n) without a tail pointer).  
        if self._head is None:  # Special-case empty list.  
            self._head = _Node(value, None)  # New node becomes head.  
            self._size = 1  # Size becomes 1.  
            return OperationCost(hops=0)  # No traversal needed for empty list.  

        hops = 0  # Count how many times we traverse `next`.  
        current = self._head  # Start from head.  
        while current.next is not None:  # Walk until we reach the last node.  
            current = current.next  # Traverse one `next` pointer.  
            hops += 1  # Count one hop.  
        current.next = _Node(value, None)  # Link new node after the last node.  
        self._size += 1  # Update size.  
        return OperationCost(hops=hops)  # Return traversal cost.  

    def get_with_cost(self, index: int) -> GetResult:  # Get element at index (O(n) due to traversal).  
        self._require_index_in_range(index)  # Validate index.  
        hops = 0  # Count `next` traversals.  
        current = self._head  # Start from head.  
        for _ in range(index):  # Move forward index times.  
            assert current is not None  # Help type-checkers; invariant holds after validation.  
            current = current.next  # Traverse one link.  
            hops += 1  # Count one hop.  
        assert current is not None  # After index moves, current is the node at index.  
        return GetResult(value=current.value, cost=OperationCost(hops=hops))  # Return value + cost.  

    def insert_at(self, index: int, value: int) -> OperationCost:  # Insert at index (O(n) except at head).  
        self._require_insert_index_in_range(index)  # Validate index.  
        if index == 0:  # Head insertion is a special O(1) case.  
            return self.push_front(value)  # Delegate to push_front to keep logic consistent.  

        hops = 0  # Count traversal hops.  
        prev = self._head  # Start at head; we need the (index-1)th node.  
        for _ in range(index - 1):  # Move (index-1) times to reach predecessor.  
            assert prev is not None  # Predecessor must exist for valid index.  
            prev = prev.next  # Traverse one link.  
            hops += 1  # Count one hop.  
        assert prev is not None  # Predecessor node exists.  
        prev.next = _Node(value, prev.next)  # Splice new node after predecessor.  
        self._size += 1  # Update size.  
        return OperationCost(hops=hops)  # Return traversal cost.  

    def remove_at(self, index: int) -> RemoveResult:  # Remove node at index and return removed value + cost.  
        self._require_index_in_range(index)  # Validate index.  
        if index == 0:  # Removing head is O(1).  
            assert self._head is not None  # Head must exist when size>0.  
            removed_value = self._head.value  # Capture removed value.  
            self._head = self._head.next  # Bypass old head.  
            self._size -= 1  # Update size.  
            return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

        hops = 0  # Count traversal hops.  
        prev = self._head  # Start at head; we need predecessor of the removed node.  
        for _ in range(index - 1):  # Move (index-1) times.  
            assert prev is not None  # Predecessor must exist.  
            prev = prev.next  # Traverse one link.  
            hops += 1  # Count one hop.  
        assert prev is not None and prev.next is not None  # Both predecessor and target must exist.  
        removed_value = prev.next.value  # Capture target value.  
        prev.next = prev.next.next  # Bypass the removed node.  
        self._size -= 1  # Update size.  
        return RemoveResult(value=removed_value, cost=OperationCost(hops=hops))  # Return value + cost.  

    def index_of_with_cost(self, value: int) -> FindResult:  # Find first index of value (O(n)).  
        current = self._head  # Start from head.  
        index = 0  # Track current index.  
        hops = 0  # Count `next` traversals (current = current.next).  
        while current is not None:  # Traverse until end.  
            if current.value == value:  # Check for match.  
                return FindResult(index=index, cost=OperationCost(hops=hops))  # Return index + cost so far.  
            current = current.next  # Move forward one node.  
            hops += 1  # Count one hop.  
            index += 1  # Move to next index.  
        return FindResult(index=-1, cost=OperationCost(hops=hops))  # Not found: hops equals number of traversals performed.  


def build_ordered_list(n: int) -> SinglyLinkedList:  # Build a list containing [0, 1, ..., n-1].  
    if n < 0:  # Reject invalid sizes early.  
        raise ValueError("n must be >= 0")  # Signal invalid input.  
    lst = SinglyLinkedList()  # Create empty list.  
    for v in range(n - 1, -1, -1):  # Push front in reverse so final order is ascending.  
        lst.push_front(v)  # O(1) head insertion per node.  
    return lst  # Return the built list.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_push_front_vs_back_table(ns: Sequence[int]) -> str:  # Compare pushFront vs pushBack hops for multiple n values.  
    header = f"{'n':>6} | {'frontH':>6} | {'backH':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for n in ns:  # Render one row per n.  
        a = build_ordered_list(n)  # Build a list of size n.  
        front_cost = a.push_front(999)  # Measure head insertion cost.  
        b = build_ordered_list(n)  # Build another list of size n.  
        back_cost = b.push_back(999)  # Measure tail insertion cost (without tail pointer).  
        lines.append(f"{n:6d} | {front_cost.hops:6d} | {back_cost.hops:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines into a printable table.  


def format_get_hops_table(n: int, indices: Sequence[int]) -> str:  # Show get(i) hops on an ordered list of size n.  
    lst = build_ordered_list(n)  # Build an ordered list.  
    header = f"{'i':>6} | {'value':>6} | {'hops':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for i in indices:  # Render one row per index.  
        r = lst.get_with_cost(i)  # Fetch value and cost.  
        lines.append(f"{i:6d} | {r.value:6d} | {r.cost.hops:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ns = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8, 16])  # Use args as n list, or defaults.  
    print("=== pushFront vs pushBack hops at size n ===")  # Print section title.  
    print(format_push_front_vs_back_table(ns))  # Print comparison table.  
    print()  # Blank line between sections.  
    print("=== get(i) hops on list of size 16 ===")  # Print section title.  
    print(format_get_hops_table(16, indices=[0, 1, 2, 4, 8, 15]))  # Print get-cost table.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  
