"""環狀鏈結串列（Circular Linked List）示範 / Circular linked list demo.  # Bilingual module header.

本單元實作「單向環狀鏈結串列（singly circular linked list）」並量化走訪成本：  # Traditional Chinese overview.
- 我們只維護 tail，並以 tail.next 當作 head  # Explain tail-only representation.
- 由於沒有 null 結尾，走訪時要用 size 控制次數避免無限迴圈  # Explain iteration safety.
- 我們用 hops 計數「走訪 next」的次數（current = current.next）  # Define the cost metric.

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
class GetResult:  # Return fetched value plus traversal cost.  
    value: int  # The fetched element value.  
    cost: OperationCost  # Traversal cost record.  


@dataclass(frozen=True)  # Make remove results immutable.  
class RemoveResult:  # Return removed value plus traversal cost.  
    value: int  # The removed element value.  
    cost: OperationCost  # Traversal cost record.  


@dataclass(frozen=True)  # Make find results immutable.  
class FindResult:  # Return found index plus traversal cost.  
    index: int  # Found index, or -1 if not found.  
    cost: OperationCost  # Traversal cost record.  


class _Node:  # A singly circular linked list node (value + next).  
    __slots__ = ("value", "next")  # Keep the node minimal and explicit.  

    def __init__(self, value: int, next_node: Optional[_Node]) -> None:  # Initialize node fields.  
        self.value = value  # Store node value.  
        self.next: Optional[_Node] = next_node  # Store next pointer (circular in non-empty list).  


class CircularLinkedList:  # A singly circular linked list implemented with a tail pointer only.  
    def __init__(self) -> None:  # Initialize an empty list.  
        self._tail: Optional[_Node] = None  # Tail pointer (None means empty).  
        self._size = 0  # Track size so we can traverse safely without null terminators.  

    @property
    def size(self) -> int:  # Expose size for callers/tests.  
        return self._size  # Return number of stored nodes.  

    def is_empty(self) -> bool:  # Convenience helper to check emptiness.  
        return self._size == 0  # Empty iff size is zero.  

    def _head(self) -> Optional[_Node]:  # Return head node (tail.next) or None for empty list.  
        if self._tail is None:  # Empty list has no head.  
            return None  # Signal no head.  
        return self._tail.next  # Head is tail.next in a circular list.  

    def to_list(self) -> List[int]:  # Convert to Python list (bounded by size to avoid infinite loops).  
        out: List[int] = []  # Accumulate values.  
        head = self._head()  # Read head node.  
        if head is None:  # Empty list converts to empty array.  
            return out  # Return empty list.  
        current = head  # Start at head.  
        for _ in range(self._size):  # Traverse exactly size nodes.  
            out.append(current.value)  # Append one value.  
            assert current.next is not None  # In non-empty circular list, next should never be None.  
            current = current.next  # Move to next node.  
        return out  # Return collected values.  

    def _require_index_in_range(self, index: int) -> None:  # Validate index in [0, size-1].  
        if index < 0 or index >= self._size:  # Reject invalid indices.  
            raise IndexError("index out of range")  # Signal invalid index.  

    def push_back(self, value: int) -> OperationCost:  # Insert at tail (O(1), hops=0).  
        if self._tail is None:  # Empty list case.  
            node = _Node(value, None)  # Create node with temporary next.  
            node.next = node  # Point to itself to form a 1-node circle.  
            self._tail = node  # Tail becomes node.  
            self._size = 1  # Size becomes 1.  
            return OperationCost(hops=0)  # No traversal.  

        head = self._tail.next  # Read current head (non-null in non-empty list).  
        new_node = _Node(value, head)  # New node points to current head.  
        self._tail.next = new_node  # Old tail points to new node.  
        self._tail = new_node  # New node becomes tail.  
        self._size += 1  # Update size.  
        return OperationCost(hops=0)  # No traversal.  

    def push_front(self, value: int) -> OperationCost:  # Insert at head (O(1), hops=0).  
        if self._tail is None:  # Empty list case.  
            node = _Node(value, None)  # Create node with temporary next.  
            node.next = node  # Point to itself to form a 1-node circle.  
            self._tail = node  # Tail becomes node.  
            self._size = 1  # Size becomes 1.  
            return OperationCost(hops=0)  # No traversal.  

        head = self._tail.next  # Read current head.  
        new_node = _Node(value, head)  # New node points to current head.  
        self._tail.next = new_node  # Tail now points to new head.  
        self._size += 1  # Update size.  
        return OperationCost(hops=0)  # No traversal.  

    def pop_front(self) -> RemoveResult:  # Remove head (O(1), hops=0).  
        if self._tail is None:  # Reject popping from empty list.  
            raise IndexError("pop from empty list")  # Signal invalid operation.  
        head = self._tail.next  # Head is tail.next.  
        assert head is not None  # Head exists in non-empty list.  
        removed_value = head.value  # Capture removed value.  
        if self._size == 1:  # Removing the only node empties the list.  
            self._tail = None  # Clear tail.  
            self._size = 0  # Reset size.  
            return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

        assert head.next is not None  # Next exists in non-empty circular list.  
        self._tail.next = head.next  # Bypass head by linking tail to new head.  
        self._size -= 1  # Update size.  
        return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

    def pop_back(self) -> RemoveResult:  # Remove tail (O(n) in singly circular list), return removed value + hops.  
        if self._tail is None:  # Reject popping from empty list.  
            raise IndexError("pop from empty list")  # Signal invalid operation.  
        removed_value = self._tail.value  # Capture removed value.  
        if self._size == 1:  # Removing the only node empties the list.  
            self._tail = None  # Clear tail.  
            self._size = 0  # Reset size.  
            return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

        head = self._tail.next  # Read head (non-null because size>0).  
        assert head is not None  # Head exists.  
        hops = 0  # Count next traversals to find predecessor of tail.  
        current = head  # Start from head.  
        while current.next is not self._tail:  # Stop when current is predecessor of tail.  
            assert current.next is not None  # Next exists in circular list.  
            current = current.next  # Traverse one next pointer.  
            hops += 1  # Count one hop.  
        current.next = self._tail.next  # Predecessor now points to head (keeps circle).  
        self._tail = current  # Predecessor becomes new tail.  
        self._size -= 1  # Update size.  
        return RemoveResult(value=removed_value, cost=OperationCost(hops=hops))  # Return removed value + traversal cost.  

    def rotate(self, steps: int) -> OperationCost:  # Rotate by moving tail forward `steps` times (left rotation).  
        if steps < 0:  # Reject negative rotations for simplicity.  
            raise ValueError("steps must be >= 0")  # Signal invalid input.  
        if self._tail is None or self._size <= 1:  # Rotating empty/1-node list changes nothing.  
            return OperationCost(hops=0)  # No traversal.  
        steps_mod = steps % self._size  # Reduce steps to avoid unnecessary cycles.  
        hops = 0  # Count traversals.  
        for _ in range(steps_mod):  # Move tail forward by steps_mod.  
            assert self._tail.next is not None  # next exists in circular list.  
            self._tail = self._tail.next  # Advance tail one step.  
            hops += 1  # Count one hop.  
        return OperationCost(hops=hops)  # Return traversal cost.  

    def get_with_cost(self, index: int) -> GetResult:  # Get element at index (O(index) traversal).  
        self._require_index_in_range(index)  # Validate index.  
        head = self._head()  # Read head.  
        assert head is not None  # Head exists when size>0.  
        current = head  # Start from head.  
        hops = 0  # Count next traversals.  
        for _ in range(index):  # Move forward index times.  
            assert current.next is not None  # Next exists in circular list.  
            current = current.next  # Traverse one next pointer.  
            hops += 1  # Count one hop.  
        return GetResult(value=current.value, cost=OperationCost(hops=hops))  # Return value + cost.  

    def index_of_with_cost(self, value: int) -> FindResult:  # Find first index of value and return traversal cost.  
        if self._tail is None:  # Empty list cannot contain the value.  
            return FindResult(index=-1, cost=OperationCost(hops=0))  # Return not found with zero cost.  
        head = self._tail.next  # Read head.  
        assert head is not None  # Head exists.  
        current = head  # Start at head.  
        hops = 0  # Count next traversals.  
        for index in range(self._size):  # Traverse exactly size nodes to avoid infinite loops.  
            if current.value == value:  # Check for match.  
                return FindResult(index=index, cost=OperationCost(hops=hops))  # Return index + cost.  
            assert current.next is not None  # Next exists in circular list.  
            current = current.next  # Move to next node.  
            hops += 1  # Count one hop.  
        return FindResult(index=-1, cost=OperationCost(hops=hops))  # Not found: hops equals size.  


def build_ordered_list(n: int) -> CircularLinkedList:  # Build a list containing [0, 1, ..., n-1].  
    if n < 0:  # Reject invalid sizes early.  
        raise ValueError("n must be >= 0")  # Signal invalid input.  
    lst = CircularLinkedList()  # Create empty list.  
    for v in range(n):  # Append in increasing order.  
        lst.push_back(v)  # push_back is O(1) with tail pointer.  
    return lst  # Return built list.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_basic_cost_table(ns: Sequence[int]) -> str:  # Show push/pop costs at multiple sizes.  
    header = f"{'n':>6} | {'pushB':>6} | {'pushF':>6} | {'popF':>6} | {'popB':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for n in ns:  # Render one row per n.  
        a = build_ordered_list(n)  # Build list of size n.  
        push_b = a.push_back(999).hops  # push_back cost (0).  
        b = build_ordered_list(n)  # Build list of size n.  
        push_f = b.push_front(999).hops  # push_front cost (0).  
        c = build_ordered_list(n)  # Build list of size n.  
        pop_f = c.pop_front().cost.hops if c.size > 0 else 0  # pop_front cost (0; handle n=0).  
        d = build_ordered_list(n)  # Build list of size n.  
        pop_b = d.pop_back().cost.hops if d.size > 0 else 0  # pop_back cost (n-2; handle n=0).  
        lines.append(f"{n:6d} | {push_b:6d} | {push_f:6d} | {pop_f:6d} | {pop_b:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines.  


def format_rotate_table(n: int, steps_list: Sequence[int]) -> str:  # Show rotate(steps) cost and resulting head value.  
    lst = build_ordered_list(n)  # Build base list.  
    header = f"{'steps':>6} | {'hops':>6} | {'head':>6} | {'list':>20}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for steps in steps_list:  # Render one row per rotation.  
        cost = lst.rotate(steps)  # Rotate list.  
        head_value = lst.to_list()[0] if lst.size > 0 else -1  # Read head value (or -1 for empty).  
        snapshot = str(lst.to_list())  # Snapshot list for demo readability.  
        lines.append(f"{steps:6d} | {cost.hops:6d} | {head_value:6d} | {snapshot:>20}")  # Append row.  
    return "\n".join(lines)  # Join lines.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ns = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8])  # Use args as n list, or defaults.  
    print("=== basic costs (push/pop) ===")  # Print section title.  
    print(format_basic_cost_table(ns))  # Print cost table.  
    print()  # Blank line.  
    print("=== rotate demo on n=5 ===")  # Print section title.  
    print(format_rotate_table(5, steps_list=[0, 1, 2, 5, 7]))  # Print rotation table.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  

