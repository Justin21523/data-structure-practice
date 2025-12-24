"""佇列（Queue）示範 / Queue demo.  # Bilingual module header.

本單元用「教學版」陣列佇列（naive array queue）來呈現成本來源：  # Traditional Chinese overview.
- enqueue：尾端加入，容量不足時倍增擴容（doubling）  # Explain enqueue + resize.
- dequeue：從前端移除後，必須把剩下元素全部左移（shift left）  # Explain why dequeue is O(n).

We track two deterministic costs:  # English overview.
- copied: number of elements copied during a resize (0 if no resize)  # Resize cost.
- moved: number of elements shifted left during dequeue (typically size-1)  # Shift cost.
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references for type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable result records.  
import sys  # Read CLI arguments for custom demo sizes.  
from typing import List, Optional, Sequence  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make cost records immutable so tests remain deterministic.  
class OperationCost:  # Record per-operation copy/shift counts.  
    copied: int  # How many elements were copied due to resize (0 if no resize).  
    moved: int  # How many elements were shifted left due to dequeue (0 for enqueue).  


@dataclass(frozen=True)  # Make dequeue results immutable.  
class DequeueResult:  # Return dequeued value plus its operation cost.  
    value: int  # The dequeued element value.  
    cost: OperationCost  # Copy/shift cost for this dequeue operation.  


@dataclass(frozen=True)  # Make summary records immutable for stable demos/tests.  
class EnqueueSummary:  # Summarize a sequence of enqueues (growth behavior).  
    m: int  # Number of enqueues performed.  
    final_size: int  # Final size after enqueues.  
    final_capacity: int  # Final capacity after growth.  
    total_copies: int  # Total copied elements due to resizes.  
    total_actual_cost: int  # Total cost if each enqueue costs 1 write + copies.  
    max_copied_in_one_op: int  # Maximum copied elements in any single enqueue.  


class ArrayQueue:  # A naive array-backed queue (dequeue shifts left; teaching-oriented).  
    def __init__(self) -> None:  # Initialize an empty queue with capacity 1.  
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

    def to_list(self) -> List[int]:  # Return a copy of the used portion (front -> rear).  
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

    def _ensure_capacity_for_one_more(self) -> int:  # Ensure capacity for one more enqueue; return resize copies.  
        if self._size < self._capacity:  # Fast path: free space exists.  
            return 0  # No resize needed.  
        return self._resize(self._capacity * 2)  # Double capacity and return copies.  

    def enqueue(self, value: int) -> OperationCost:  # Add to rear (amortized O(1)).  
        copied = self._ensure_capacity_for_one_more()  # Resize if needed.  
        self._data[self._size] = value  # Write new value at rear slot.  
        self._size += 1  # Increase size.  
        return OperationCost(copied=copied, moved=0)  # Enqueue does not shift left.  

    def peek(self) -> int:  # Return front value without removing it (O(1)).  
        if self._size == 0:  # Reject peeking an empty queue.  
            raise IndexError("peek from empty queue")  # Signal invalid operation.  
        v = self._data[0]  # Read front slot.  
        assert v is not None  # Front slot must contain a value.  
        return v  # Return front value.  

    def dequeue(self) -> DequeueResult:  # Remove and return front value (O(n) due to shifting).  
        if self._size == 0:  # Reject dequeuing an empty queue.  
            raise IndexError("dequeue from empty queue")  # Signal invalid operation.  

        removed = self._data[0]  # Capture front value.  
        assert removed is not None  # Front slot must contain a value.  

        moved = self._size - 1  # Shifting left moves (size - 1) elements.  
        for i in range(1, self._size):  # Shift elements left by one.  
            self._data[i - 1] = self._data[i]  # Move one element left.  
        self._size -= 1  # Decrease size after shifting.  
        self._data[self._size] = None  # Clear last used slot for GC/debug friendliness.  
        return DequeueResult(value=removed, cost=OperationCost(copied=0, moved=moved))  # Return value + shift cost.  


def simulate_enqueues(m: int) -> EnqueueSummary:  # Simulate m enqueues and summarize growth/copy costs.  
    if m < 0:  # Reject invalid counts.  
        raise ValueError("m must be >= 0")  # Signal invalid input.  
    q = ArrayQueue()  # Use a fresh queue so results depend only on m.  
    total_actual = 0  # Accumulate total actual cost (1 write + copies).  
    max_copied = 0  # Track maximum copied elements in a single enqueue.  
    for i in range(m):  # Perform m enqueues.  
        cost = q.enqueue(i)  # Enqueue deterministic value and get per-op cost.  
        total_actual += 1 + cost.copied  # Add write cost + resize copy cost.  
        max_copied = max(max_copied, cost.copied)  # Update maximum copied.  
    return EnqueueSummary(  # Return a compact summary record.  
        m=m,  # Store operation count.  
        final_size=q.size,  # Store final size.  
        final_capacity=q.capacity,  # Store final capacity.  
        total_copies=q.total_copies,  # Store total copies.  
        total_actual_cost=total_actual,  # Store total actual cost.  
        max_copied_in_one_op=max_copied,  # Store max per-op copied.  
    )  # Close summary construction.  


def build_ordered_queue(n: int) -> ArrayQueue:  # Build a queue containing [0, 1, ..., n-1].  
    if n < 0:  # Reject invalid sizes early.  
        raise ValueError("n must be >= 0")  # Signal invalid input.  
    q = ArrayQueue()  # Create empty queue.  
    for v in range(n):  # Enqueue in increasing order.  
        q.enqueue(v)  # Enqueue one value.  
    return q  # Return built queue.  


def simulate_dequeue_cost_at_size(n: int) -> OperationCost:  # Build size n queue then dequeue once and return cost.  
    q = build_ordered_queue(n)  # Build queue of size n.  
    return q.dequeue().cost  # Dequeue once and return cost.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_enqueue_summary_table(ms: Sequence[int]) -> str:  # Format enqueue-growth summaries for multiple m values.  
    header = f"{'m':>6} | {'cap':>6} | {'copies':>7} | {'avg':>6} | {'maxCp':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for m in ms:  # Render one row per m.  
        s = simulate_enqueues(m)  # Simulate and summarize.  
        avg = (s.total_actual_cost / s.m) if s.m > 0 else 0.0  # Average actual cost per op.  
        lines.append(f"{s.m:6d} | {s.final_capacity:6d} | {s.total_copies:7d} | {avg:6.2f} | {s.max_copied_in_one_op:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines into printable table.  


def format_dequeue_cost_table(ns: Sequence[int]) -> str:  # Show dequeue shift costs at multiple sizes.  
    header = f"{'n':>6} | {'moved':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for n in ns:  # Render one row per n.  
        if n == 0:  # Skip invalid dequeue on empty queue for demo.  
            moved = 0  # Show 0 as placeholder.  
        else:  # Non-empty case.  
            moved = simulate_dequeue_cost_at_size(n).moved  # moved should equal n-1.  
        lines.append(f"{n:6d} | {moved:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines.  


def format_fifo_demo() -> str:  # Show a small FIFO example with peek/dequeue.  
    q = ArrayQueue()  # Create an empty queue.  
    q.enqueue(3)  # Enqueue 3.  
    q.enqueue(7)  # Enqueue 7.  
    q.enqueue(1)  # Enqueue 1.  
    front = q.peek()  # Peek current front.  
    d1 = q.dequeue().value  # Dequeue first (should be 3).  
    d2 = q.dequeue().value  # Dequeue second (should be 7).  
    d3 = q.dequeue().value  # Dequeue third (should be 1).  
    return f"enqueue [3,7,1] => peek={front}, dequeues=[{d1},{d2},{d3}]"  # Return formatted demo line.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ms = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8, 16, 32])  # Use args as m list, or defaults.  
    ns = [0, 1, 2, 4, 8, 16]  # Use a small fixed set for dequeue shift costs.  
    print("=== Enqueue Growth (m enqueues) ===")  # Print section title.  
    print(format_enqueue_summary_table(ms))  # Print enqueue growth table.  
    print()  # Blank line.  
    print("=== Dequeue Shift Cost (dequeue once from size n) ===")  # Print section title.  
    print(format_dequeue_cost_table(ns))  # Print dequeue moved table.  
    print()  # Blank line.  
    print("=== FIFO demo ===")  # Print section title.  
    print(format_fifo_demo())  # Print small FIFO demo line.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  

