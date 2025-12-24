"""堆疊（Stack）示範 / Stack demo.  # Bilingual module header.

本單元用「動態陣列（dynamic array）」實作堆疊（stack）：  # Traditional Chinese overview.
- LIFO：最後進來的元素（Last In）會最先出去（First Out）  # Explain LIFO.
- top 位於陣列尾端（index = size-1），push/pop 都是 O(1)  # Explain why operations are O(1).
- 若底層陣列滿了，就用「容量倍增（doubling）」擴容  # Explain resizing.

We track one deterministic cost for each push:  # English overview.
- copied: number of elements copied during a resize (0 if no resize)  # Define the metric.
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references for type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable result records.  
import sys  # Read CLI arguments for custom demo sizes.  
from typing import List, Optional, Sequence  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make cost records immutable so tests remain deterministic.  
class OperationCost:  # Record per-operation resize copy counts.  
    copied: int  # How many elements were copied due to resize (0 if no resize).  


@dataclass(frozen=True)  # Make pop results immutable.  
class PopResult:  # Return popped value plus its operation cost.  
    value: int  # The popped element value.  
    cost: OperationCost  # Resize-copy cost for this pop (always 0 in this unit).  


@dataclass(frozen=True)  # Make summary records immutable for stable demos/tests.  
class PushSummary:  # Summarize a sequence of pushes (growth behavior).  
    m: int  # Number of pushes performed.  
    final_size: int  # Final size after pushes.  
    final_capacity: int  # Final capacity after growth.  
    total_copies: int  # Total copied elements due to resizes.  
    total_actual_cost: int  # Total cost if each push costs 1 write + copied.  
    max_copied_in_one_op: int  # Maximum copied elements in any single push.  


class ArrayStack:  # An array-backed stack with doubling growth (teaching-oriented).  
    def __init__(self) -> None:  # Initialize an empty stack with capacity 1.  
        self._size = 0  # Track number of stored elements.  
        self._capacity = 1  # Start with capacity 1 for deterministic doubling.  
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

    def to_list(self) -> List[int]:  # Return a copy of the used portion (bottom -> top).  
        out: List[int] = []  # Accumulate values in order.  
        for i in range(self._size):  # Copy only the used elements.  
            v = self._data[i]  # Read one slot from buffer.  
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
        for i in range(self._size):  # Copy exactly the used portion.  
            new_data[i] = self._data[i]  # Copy one element.  
            copied += 1  # Count one copy.  
        self._data = new_data  # Swap buffer reference.  
        self._capacity = new_capacity  # Update capacity.  
        self._total_copies += copied  # Accumulate total copies.  
        return copied  # Return copies performed for this resize.  

    def _ensure_capacity_for_one_more(self) -> int:  # Ensure capacity for one more push; return resize copies.  
        if self._size < self._capacity:  # Fast path: free space exists.  
            return 0  # No resize needed.  
        return self._resize(self._capacity * 2)  # Double capacity and return copies.  

    def push(self, value: int) -> OperationCost:  # Push to top (amortized O(1)).  
        copied = self._ensure_capacity_for_one_more()  # Resize if needed.  
        self._data[self._size] = value  # Write new value at the top slot.  
        self._size += 1  # Increase size.  
        return OperationCost(copied=copied)  # Return deterministic resize-copy cost.  

    def peek(self) -> int:  # Return top value without removing it (O(1)).  
        if self._size == 0:  # Reject peeking an empty stack.  
            raise IndexError("peek from empty stack")  # Signal invalid operation.  
        v = self._data[self._size - 1]  # Read top slot.  
        assert v is not None  # Top slot must contain a value.  
        return v  # Return top value.  

    def pop(self) -> PopResult:  # Pop top value (O(1) in this unit; no shrinking).  
        if self._size == 0:  # Reject popping an empty stack.  
            raise IndexError("pop from empty stack")  # Signal invalid operation.  
        self._size -= 1  # Decrease size first so top index becomes size.  
        v = self._data[self._size]  # Read removed slot.  
        assert v is not None  # Removed slot must contain a value.  
        self._data[self._size] = None  # Clear slot for GC/debug friendliness.  
        return PopResult(value=v, cost=OperationCost(copied=0))  # Pop does not resize/copy in this unit.  


def simulate_pushes(m: int) -> PushSummary:  # Simulate m pushes and summarize growth/copy costs.  
    if m < 0:  # Reject invalid counts.  
        raise ValueError("m must be >= 0")  # Signal invalid input.  
    s = ArrayStack()  # Use a fresh stack so results depend only on m.  
    total_actual = 0  # Accumulate total actual cost (1 write + copies).  
    max_copied = 0  # Track maximum copied elements in a single push.  
    for i in range(m):  # Perform m pushes.  
        cost = s.push(i)  # Push deterministic value and get per-op cost.  
        total_actual += 1 + cost.copied  # Add write cost + resize copy cost.  
        max_copied = max(max_copied, cost.copied)  # Update maximum copied.  
    return PushSummary(  # Return a compact summary record.  
        m=m,  # Store operation count.  
        final_size=s.size,  # Store final size.  
        final_capacity=s.capacity,  # Store final capacity.  
        total_copies=s.total_copies,  # Store total copies.  
        total_actual_cost=total_actual,  # Store total actual cost.  
        max_copied_in_one_op=max_copied,  # Store max per-op copied.  
    )  # Close summary construction.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_push_summary_table(ms: Sequence[int]) -> str:  # Format growth summaries for multiple m values.  
    header = f"{'m':>6} | {'cap':>6} | {'copies':>7} | {'avg':>6} | {'maxCp':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for m in ms:  # Render one row per m.  
        s = simulate_pushes(m)  # Simulate and summarize.  
        avg = (s.total_actual_cost / s.m) if s.m > 0 else 0.0  # Average actual cost per op.  
        lines.append(f"{s.m:6d} | {s.final_capacity:6d} | {s.total_copies:7d} | {avg:6.2f} | {s.max_copied_in_one_op:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines into printable table.  


def format_lifo_demo() -> str:  # Show a small LIFO example with peek/pop.  
    s = ArrayStack()  # Create an empty stack.  
    s.push(3)  # Push 3.  
    s.push(7)  # Push 7.  
    s.push(1)  # Push 1 (becomes top).  
    top = s.peek()  # Peek current top.  
    p1 = s.pop().value  # Pop first (should be 1).  
    p2 = s.pop().value  # Pop second (should be 7).  
    p3 = s.pop().value  # Pop third (should be 3).  
    return f"push [3,7,1] => peek={top}, pops=[{p1},{p2},{p3}]"  # Return formatted demo line.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ms = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8, 16, 32])  # Use args as m list, or defaults.  
    print("=== Push Growth (m pushes) ===")  # Print section title.  
    print(format_push_summary_table(ms))  # Print growth summary table.  
    print()  # Print blank line between sections.  
    print("=== LIFO demo ===")  # Print section title.  
    print(format_lifo_demo())  # Print small LIFO demo line.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  

