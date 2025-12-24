"""雙向鏈結串列（Doubly Linked List）示範 / Doubly linked list demo.  # Bilingual module header.

本單元實作「雙向鏈結串列（doubly linked list）」並量化走訪成本：  # Traditional Chinese overview.
- 每個節點有 value、prev、next  # Explain node structure.
- 串列維護 head 與 tail，使 push/pop 兩端操作皆為 O(1)  # Explain head/tail benefit.
- 走訪時可從 head 或 tail 選擇較近端開始（雙向串列的常見優化）  # Explain bidirectional traversal.
- 我們用 hops 計數「走訪指標」的次數（next 或 prev 都算一次）  # Define the cost metric.

We return deterministic per-operation costs so tests can validate both correctness and complexity intuition.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable records.  
import sys  # Read CLI arguments for demo sizes.  
from typing import List, Optional, Sequence, Tuple  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make cost records immutable and deterministic.  
class OperationCost:  # Record the traversal cost of one operation.  
    hops: int  # Number of pointer traversals performed (next or prev).  


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


class _Node:  # A doubly linked list node (value + prev/next pointers).  
    __slots__ = ("value", "prev", "next")  # Keep the node minimal and explicit.  

    def __init__(self, value: int, prev_node: Optional[_Node], next_node: Optional[_Node]) -> None:  # Initialize node fields.  
        self.value = value  # Store node value.  
        self.prev: Optional[_Node] = prev_node  # Store previous pointer (or None).  
        self.next: Optional[_Node] = next_node  # Store next pointer (or None).  


class DoublyLinkedList:  # A teaching-oriented doubly linked list (head + tail).  
    def __init__(self) -> None:  # Initialize an empty list.  
        self._head: Optional[_Node] = None  # Head pointer (None means empty).  
        self._tail: Optional[_Node] = None  # Tail pointer (None means empty).  
        self._size = 0  # Track list size for O(1) length queries.  

    @property
    def size(self) -> int:  # Expose size for callers/tests.  
        return self._size  # Return number of stored nodes.  

    def is_empty(self) -> bool:  # Convenience helper to check emptiness.  
        return self._size == 0  # Empty iff size is zero.  

    def to_list(self) -> List[int]:  # Convert to a Python list for easy validation/printing.  
        out: List[int] = []  # Accumulate values in order.  
        current = self._head  # Start traversal from head.  
        while current is not None:  # Stop at end of list.  
            out.append(current.value)  # Append one value.  
            current = current.next  # Move forward via next.  
        return out  # Return collected values.  

    def _require_index_in_range(self, index: int) -> None:  # Validate index in [0, size-1].  
        if index < 0 or index >= self._size:  # Reject invalid indices.  
            raise IndexError("index out of range")  # Signal invalid index.  

    def _require_insert_index_in_range(self, index: int) -> None:  # Validate insert index in [0, size].  
        if index < 0 or index > self._size:  # Reject invalid insertion indices.  
            raise IndexError("index out of range for insert")  # Signal invalid index.  

    def _node_at_with_cost(self, index: int) -> Tuple[_Node, OperationCost]:  # Return node at index plus traversal cost.  
        self._require_index_in_range(index)  # Validate index.  
        assert self._head is not None and self._tail is not None  # Non-empty list must have head and tail.  

        if index < self._size // 2:  # Choose head-side traversal when index is in the first half.  
            hops = 0  # Count pointer traversals.  
            current = self._head  # Start from head.  
            for _ in range(index):  # Move forward index times.  
                assert current.next is not None  # We should not run past the target index.  
                current = current.next  # Traverse one next pointer.  
                hops += 1  # Count one hop.  
            return current, OperationCost(hops=hops)  # Return node + cost.  

        hops = 0  # Count pointer traversals.  
        current = self._tail  # Start from tail.  
        steps = self._size - 1 - index  # How many prev steps needed to reach index from tail.  
        for _ in range(steps):  # Move backward `steps` times.  
            assert current.prev is not None  # We should not run past the target index.  
            current = current.prev  # Traverse one prev pointer.  
            hops += 1  # Count one hop.  
        return current, OperationCost(hops=hops)  # Return node + cost.  

    def push_front(self, value: int) -> OperationCost:  # Insert at head (O(1), hops=0).  
        new_head = _Node(value, prev_node=None, next_node=self._head)  # Create new node that points to old head.  
        if self._head is None:  # Empty list case: head and tail become the new node.  
            self._head = new_head  # Set head.  
            self._tail = new_head  # Set tail.  
            self._size = 1  # Size becomes 1.  
            return OperationCost(hops=0)  # No traversal needed.  

        self._head.prev = new_head  # Old head now points back to new head.  
        self._head = new_head  # Update head pointer.  
        self._size += 1  # Update size.  
        return OperationCost(hops=0)  # Head insertion does not traverse.  

    def push_back(self, value: int) -> OperationCost:  # Insert at tail (O(1) with a tail pointer).  
        new_tail = _Node(value, prev_node=self._tail, next_node=None)  # Create new node that points back to old tail.  
        if self._tail is None:  # Empty list case: head and tail become the new node.  
            self._head = new_tail  # Set head.  
            self._tail = new_tail  # Set tail.  
            self._size = 1  # Size becomes 1.  
            return OperationCost(hops=0)  # No traversal needed.  

        self._tail.next = new_tail  # Old tail now points forward to new tail.  
        self._tail = new_tail  # Update tail pointer.  
        self._size += 1  # Update size.  
        return OperationCost(hops=0)  # Tail insertion does not traverse.  

    def pop_front(self) -> RemoveResult:  # Remove from head (O(1), hops=0).  
        if self._head is None:  # Reject popping from an empty list.  
            raise IndexError("pop from empty list")  # Signal invalid operation.  
        removed_value = self._head.value  # Capture removed value.  
        new_head = self._head.next  # Candidate new head.  
        if new_head is None:  # Removing the only node makes the list empty.  
            self._head = None  # Clear head.  
            self._tail = None  # Clear tail.  
            self._size = 0  # Reset size.  
            return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

        new_head.prev = None  # New head has no previous node.  
        self._head = new_head  # Update head pointer.  
        self._size -= 1  # Update size.  
        return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

    def pop_back(self) -> RemoveResult:  # Remove from tail (O(1), hops=0).  
        if self._tail is None:  # Reject popping from an empty list.  
            raise IndexError("pop from empty list")  # Signal invalid operation.  
        removed_value = self._tail.value  # Capture removed value.  
        new_tail = self._tail.prev  # Candidate new tail.  
        if new_tail is None:  # Removing the only node makes the list empty.  
            self._head = None  # Clear head.  
            self._tail = None  # Clear tail.  
            self._size = 0  # Reset size.  
            return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

        new_tail.next = None  # New tail has no next node.  
        self._tail = new_tail  # Update tail pointer.  
        self._size -= 1  # Update size.  
        return RemoveResult(value=removed_value, cost=OperationCost(hops=0))  # No traversal.  

    def get_with_cost(self, index: int) -> GetResult:  # Get value at index with traversal cost.  
        node, cost = self._node_at_with_cost(index)  # Locate node and compute hops.  
        return GetResult(value=node.value, cost=cost)  # Return value + cost.  

    def insert_at(self, index: int, value: int) -> OperationCost:  # Insert at index and return traversal cost.  
        self._require_insert_index_in_range(index)  # Validate insertion index.  
        if index == 0:  # Head insertion is O(1).  
            return self.push_front(value)  # Delegate to push_front.  
        if index == self._size:  # Tail insertion is O(1).  
            return self.push_back(value)  # Delegate to push_back.  

        target, cost = self._node_at_with_cost(index)  # Find the node currently at index.  
        assert target.prev is not None  # For index>0, predecessor must exist.  
        new_node = _Node(value, prev_node=target.prev, next_node=target)  # New node sits before target.  
        target.prev.next = new_node  # Predecessor now points forward to new node.  
        target.prev = new_node  # Target now points back to new node.  
        self._size += 1  # Update size.  
        return cost  # Traversal cost is the cost to find the insertion position.  

    def remove_at(self, index: int) -> RemoveResult:  # Remove at index and return removed value + traversal cost.  
        self._require_index_in_range(index)  # Validate index.  
        if index == 0:  # Removing head is O(1).  
            return self.pop_front()  # Delegate to pop_front.  
        if index == self._size - 1:  # Removing tail is O(1).  
            return self.pop_back()  # Delegate to pop_back.  

        target, cost = self._node_at_with_cost(index)  # Find node to remove.  
        assert target.prev is not None and target.next is not None  # Middle node must have both neighbors.  
        target.prev.next = target.next  # Bypass target from the left side.  
        target.next.prev = target.prev  # Bypass target from the right side.  
        self._size -= 1  # Update size.  
        return RemoveResult(value=target.value, cost=cost)  # Return removed value + traversal cost.  

    def index_of_with_cost(self, value: int) -> FindResult:  # Find first index of value and return traversal cost.  
        current = self._head  # Start from head.  
        index = 0  # Track current index.  
        hops = 0  # Count `next` traversals.  
        while current is not None:  # Traverse until end.  
            if current.value == value:  # Check for match.  
                return FindResult(index=index, cost=OperationCost(hops=hops))  # Return index + cost so far.  
            current = current.next  # Move forward one node.  
            hops += 1  # Count one hop.  
            index += 1  # Advance index.  
        return FindResult(index=-1, cost=OperationCost(hops=hops))  # Not found: hops equals number of traversals performed.  


def build_ordered_list(n: int) -> DoublyLinkedList:  # Build a list containing [0, 1, ..., n-1].  
    if n < 0:  # Reject invalid sizes early.  
        raise ValueError("n must be >= 0")  # Signal invalid input.  
    lst = DoublyLinkedList()  # Create empty list.  
    for v in range(n):  # Append in increasing order (push_back is O(1) with tail).  
        lst.push_back(v)  # Append one value.  
    return lst  # Return built list.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_push_pop_table(ns: Sequence[int]) -> str:  # Compare push/pop costs at both ends for multiple sizes.  
    header = f"{'n':>6} | {'pushB':>6} | {'popB':>6} | {'pushF':>6} | {'popF':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for n in ns:  # Render one row per n.  
        lst1 = build_ordered_list(n)  # Build list of size n.  
        push_back_cost = lst1.push_back(999).hops  # Measure pushBack hops (should be 0).  
        pop_back_cost = lst1.pop_back().cost.hops if lst1.size > 0 else 0  # Measure popBack hops (0; handle n=0).  
        lst2 = build_ordered_list(n)  # Build another list of size n.  
        push_front_cost = lst2.push_front(999).hops  # Measure pushFront hops (should be 0).  
        pop_front_cost = lst2.pop_front().cost.hops if lst2.size > 0 else 0  # Measure popFront hops (0; handle n=0).  
        lines.append(f"{n:6d} | {push_back_cost:6d} | {pop_back_cost:6d} | {push_front_cost:6d} | {pop_front_cost:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines into printable table.  


def format_get_hops_table(n: int, indices: Sequence[int]) -> str:  # Show get(i) hops on an ordered list of size n.  
    lst = build_ordered_list(n)  # Build ordered list.  
    header = f"{'i':>6} | {'value':>6} | {'hops':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for i in indices:  # Render one row per index.  
        r = lst.get_with_cost(i)  # Fetch value + cost.  
        lines.append(f"{i:6d} | {r.value:6d} | {r.cost.hops:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ns = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8, 16])  # Use args as n list, or defaults.  
    print("=== push/pop hops at both ends ===")  # Print section title.  
    print(format_push_pop_table(ns))  # Print push/pop table.  
    print()  # Blank line between sections.  
    print("=== get(i) hops on list of size 16 ===")  # Print section title.  
    print(format_get_hops_table(16, indices=[0, 1, 2, 4, 8, 15]))  # Print get-cost table.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  

