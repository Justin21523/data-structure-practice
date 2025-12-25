"""雙端佇列（Deque）示範 / Deque demo.  # Bilingual module header.

本單元用「環狀陣列（circular buffer）」實作雙端佇列（deque）：  # Traditional Chinese overview.
- 只維護 head（front index）與 size  # Explain representation.
- pushFront / pushBack 只需要算出要寫入的位置  # Explain O(1) inserts at both ends.
- popFront / popBack 只需要調整 head 或 size  # Explain O(1) removals at both ends.

We track deterministic costs for learning/testing:  # English overview.
- copied: number of elements copied during a resize (0 if no resize)  # Resize cost.
- moved: extra element shifts (should always be 0 with a circular buffer)  # Shift cost (eliminated).
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references for type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable result records.  
import sys  # Read CLI arguments for custom demo sizes.  
from typing import List, Optional, Sequence  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make cost records immutable so tests remain deterministic.  
class OperationCost:  # Record per-operation copy/shift counts.  
    copied: int  # How many elements were copied due to resize (0 if no resize).  
    moved: int  # How many elements were shifted (should be 0 in this unit).  


@dataclass(frozen=True)  # Make pop results immutable.  
class PopResult:  # Return popped value plus its operation cost.  
    value: int  # The popped element value.  
    cost: OperationCost  # Copy/shift cost for this pop.  


@dataclass(frozen=True)  # Make summary records immutable for stable demos/tests.  
class PushSummary:  # Summarize a sequence of pushes (growth behavior).  
    m: int  # Number of pushes performed.  
    final_size: int  # Final size after pushes.  
    final_capacity: int  # Final capacity after growth.  
    total_copies: int  # Total copied elements due to resizes.  
    total_actual_cost: int  # Total cost if each push costs 1 write + copies.  
    max_copied_in_one_op: int  # Maximum copies in any single push.  


class Deque:  # A circular-buffer deque with doubling growth (teaching-oriented).  
    def __init__(self) -> None:  # Initialize an empty deque with capacity 1.  
        self._size = 0  # Track number of stored elements.  
        self._capacity = 1  # Start with capacity 1 for deterministic doubling.  
        self._head = 0  # Head index (front position) within the buffer.  
        self._data: List[Optional[int]] = [None] * self._capacity  # Allocate backing buffer.  
        self._total_copies = 0  # Accumulate total copied elements across all resizes.  

    @property
    def size(self) -> int:  # Expose current size for callers/tests.  
        return self._size  # Return number of stored items.  

    @property
    def capacity(self) -> int:  # Expose current capacity for callers/tests.  
        return self._capacity  # Return allocated slots.  

    @property
    def total_copies(self) -> int:  # Expose total resize copies for analysis-style checks.  
        return self._total_copies  # Return total copied elements.  

    def is_empty(self) -> bool:  # Convenience helper to check emptiness.  
        return self._size == 0  # Empty iff size is zero.  

    def _index_at(self, offset: int) -> int:  # Map logical offset [0..size) to physical buffer index.  
        return (self._head + offset) % self._capacity  # Wrap around using modulo.  

    def to_list(self) -> List[int]:  # Return a copy of the used portion (front -> back).  
        out: List[int] = []  # Accumulate values in order.  
        for i in range(self._size):  # Copy exactly size elements in deque order.  
            idx = self._index_at(i)  # Compute physical index for logical position i.  
            v = self._data[idx]  # Read buffer slot.  
            assert v is not None  # Used slots must contain ints.  
            out.append(v)  # Append value to output list.  
        return out  # Return copied list.  

    def _resize(self, new_capacity: int) -> int:  # Resize buffer and return number of copied elements.  
        if new_capacity < self._size:  # Ensure new buffer can hold all existing elements.  
            raise ValueError("new_capacity must be >= size")  # Signal invalid resize request.  
        if new_capacity < 1:  # Keep capacity positive for simplicity.  
            raise ValueError("new_capacity must be >= 1")  # Signal invalid capacity.  

        new_data: List[Optional[int]] = [None] * new_capacity  # Allocate new buffer.  
        copied = 0  # Count copied elements deterministically.  
        for i in range(self._size):  # Copy elements in deque order.  
            old_idx = self._index_at(i)  # Map logical i to old physical index.  
            new_data[i] = self._data[old_idx]  # Copy one element into compact layout.  
            copied += 1  # Count one copy.  
        self._data = new_data  # Swap buffer reference.  
        self._capacity = new_capacity  # Update capacity.  
        self._head = 0  # Reset head so deque starts at index 0 after resize.  
        self._total_copies += copied  # Accumulate total copies.  
        return copied  # Return copies performed for this resize.  

    def _ensure_capacity_for_one_more(self) -> int:  # Ensure capacity for one more push; return resize copies.  
        if self._size < self._capacity:  # Fast path: free space exists.  
            return 0  # No resize needed.  
        return self._resize(self._capacity * 2)  # Double capacity and return copies.  

    def push_back(self, value: int) -> OperationCost:  # Push at back (amortized O(1)).  
        copied = self._ensure_capacity_for_one_more()  # Resize if needed.  
        tail_idx = self._index_at(self._size)  # Compute tail index where new element is written.  
        self._data[tail_idx] = value  # Store value at tail position.  
        self._size += 1  # Increase size.  
        return OperationCost(copied=copied, moved=0)  # moved stays 0 (no shifting).  

    def push_front(self, value: int) -> OperationCost:  # Push at front (amortized O(1)).  
        copied = self._ensure_capacity_for_one_more()  # Resize if needed.  
        self._head = (self._head - 1) % self._capacity  # Move head left by one with wrap-around.  
        self._data[self._head] = value  # Store value at new head position.  
        self._size += 1  # Increase size.  
        return OperationCost(copied=copied, moved=0)  # moved stays 0 (no shifting).  

    def peek_front(self) -> int:  # Return front value without removing it (O(1)).  
        if self._size == 0:  # Reject peeking an empty deque.  
            raise IndexError("peek from empty deque")  # Signal invalid operation.  
        v = self._data[self._head]  # Read front slot directly.  
        assert v is not None  # Front slot must contain a value.  
        return v  # Return front value.  

    def peek_back(self) -> int:  # Return back value without removing it (O(1)).  
        if self._size == 0:  # Reject peeking an empty deque.  
            raise IndexError("peek from empty deque")  # Signal invalid operation.  
        tail_idx = self._index_at(self._size - 1)  # Compute index of last element.  
        v = self._data[tail_idx]  # Read back slot.  
        assert v is not None  # Back slot must contain a value.  
        return v  # Return back value.  

    def pop_front(self) -> PopResult:  # Pop from front (O(1); no shifting).  
        if self._size == 0:  # Reject popping an empty deque.  
            raise IndexError("pop from empty deque")  # Signal invalid operation.  
        removed = self._data[self._head]  # Capture front value.  
        assert removed is not None  # Front slot must contain a value.  
        self._data[self._head] = None  # Clear slot for GC/debug friendliness.  
        self._head = (self._head + 1) % self._capacity  # Advance head by one with wrap-around.  
        self._size -= 1  # Decrease size.  
        if self._size == 0:  # Normalize head for deterministic state when deque becomes empty.  
            self._head = 0  # Reset head so next push starts at index 0 (optional but stable).  
        return PopResult(value=removed, cost=OperationCost(copied=0, moved=0))  # moved stays 0 (no shift).  

    def pop_back(self) -> PopResult:  # Pop from back (O(1); no shifting).  
        if self._size == 0:  # Reject popping an empty deque.  
            raise IndexError("pop from empty deque")  # Signal invalid operation.  
        tail_idx = self._index_at(self._size - 1)  # Compute index of last element.  
        removed = self._data[tail_idx]  # Capture back value.  
        assert removed is not None  # Back slot must contain a value.  
        self._data[tail_idx] = None  # Clear slot for GC/debug friendliness.  
        self._size -= 1  # Decrease size.  
        if self._size == 0:  # Normalize head for deterministic state when deque becomes empty.  
            self._head = 0  # Reset head (optional but stable).  
        return PopResult(value=removed, cost=OperationCost(copied=0, moved=0))  # moved stays 0 (no shift).  


def simulate_push_backs(m: int) -> PushSummary:  # Simulate m push_back operations and summarize growth/copy costs.  
    if m < 0:  # Reject invalid counts.  
        raise ValueError("m must be >= 0")  # Signal invalid input.  
    d = Deque()  # Use a fresh deque so results depend only on m.  
    total_actual = 0  # Accumulate total actual cost (1 write + copies).  
    max_copied = 0  # Track maximum copied elements in a single push.  
    for i in range(m):  # Perform m pushes.  
        cost = d.push_back(i)  # Push deterministic value and get per-op cost.  
        total_actual += 1 + cost.copied  # Add write cost + resize copy cost.  
        max_copied = max(max_copied, cost.copied)  # Update maximum copied.  
    return PushSummary(  # Return a compact summary record.  
        m=m,  # Store operation count.  
        final_size=d.size,  # Store final size.  
        final_capacity=d.capacity,  # Store final capacity.  
        total_copies=d.total_copies,  # Store total copies.  
        total_actual_cost=total_actual,  # Store total actual cost.  
        max_copied_in_one_op=max_copied,  # Store max per-op copied.  
    )  # Close summary construction.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_push_summary_table(ms: Sequence[int]) -> str:  # Format push-growth summaries for multiple m values.  
    header = f"{'m':>6} | {'cap':>6} | {'copies':>7} | {'avg':>6} | {'maxCp':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for m in ms:  # Render one row per m.  
        s = simulate_push_backs(m)  # Simulate and summarize.  
        avg = (s.total_actual_cost / s.m) if s.m > 0 else 0.0  # Average actual cost per op.  
        lines.append(f"{s.m:6d} | {s.final_capacity:6d} | {s.total_copies:7d} | {avg:6.2f} | {s.max_copied_in_one_op:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines into printable table.  


def format_deque_demo() -> str:  # Show a small deque example using both ends.  
    d = Deque()  # Create empty deque.  
    d.push_back(3)  # Push 3 to back.  
    d.push_back(7)  # Push 7 to back.  
    d.push_front(1)  # Push 1 to front (deque becomes [1,3,7]).  
    front = d.peek_front()  # Peek front value.  
    back = d.peek_back()  # Peek back value.  
    p1 = d.pop_front().value  # Pop front (should be 1).  
    p2 = d.pop_back().value  # Pop back (should be 7).  
    p3 = d.pop_back().value  # Pop back (should be 3).  
    return f"pushBack [3,7], pushFront [1] => peekFront={front}, peekBack={back}, pops=[{p1},{p2},{p3}]"  # Return formatted demo line.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ms = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8, 16, 32])  # Use args as m list, or defaults.  
    print("=== pushBack growth (m pushes) ===")  # Print section title.  
    print(format_push_summary_table(ms))  # Print growth table.  
    print()  # Blank line.  
    print("=== deque demo (both ends) ===")  # Print section title.  
    print(format_deque_demo())  # Print deque demo line.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  

