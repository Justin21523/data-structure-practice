"""動態陣列（Dynamic Array）示範 / Dynamic array demo.  # Module docstring (bilingual).

本單元用「容量倍增（doubling）」的策略實作動態陣列：  # Traditional Chinese overview.
- 底層是固定容量的連續儲存（conceptually contiguous buffer）  # Explain the underlying contiguous buffer.
- 當 `size == capacity` 時，配置更大的 buffer 並搬移既有元素（copies）  # Explain resize copies.
- `append` 因為不是每次都擴容，所以平均/攤銷成本是 O(1)  # Explain amortized behavior.
- `insertAt/removeAt` 除了可能擴容，還需要 shift，因此通常是 O(n)  # Explain shifts.

We track two deterministic costs:  # English summary.
- copied: number of elements copied during resize  # Resize cost.
- moved: number of elements shifted during insert/remove  # Shift cost.
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references for type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable result records.  
import sys  # Read CLI arguments for custom demo sizes.  
from typing import List, Optional, Sequence  # Type hints for readability.  


@dataclass(frozen=True)  # Make cost records immutable so tests remain deterministic.  
class OperationCost:  # Record per-operation copy/shift counts.  
    copied: int  # How many elements were copied due to resize (0 if no resize).  
    moved: int  # How many elements were shifted due to insert/remove (0 for append).  


@dataclass(frozen=True)  # Make remove results immutable.  
class RemoveResult:  # Return removed value plus its operation cost.  
    value: int  # The removed element value.  
    cost: OperationCost  # Copy/shift cost for this remove operation.  


class DynamicArray:  # A simple dynamic array with doubling growth (teaching-oriented).  
    def __init__(self) -> None:  # Initialize an empty array with capacity 1.  
        self._size = 0  # Track number of stored elements.  
        self._capacity = 1  # Start with capacity 1 to keep growth behavior deterministic.  
        self._data: List[Optional[int]] = [None] * self._capacity  # Allocate backing buffer.  
        self._total_copies = 0  # Accumulate total copied elements across all resizes.  

    @property
    def size(self) -> int:  # Expose size for callers/tests.  
        return self._size  # Return current size.  

    @property
    def capacity(self) -> int:  # Expose capacity for callers/tests.  
        return self._capacity  # Return current capacity.  

    @property
    def total_copies(self) -> int:  # Expose total resize copies for amortized-analysis style checks.  
        return self._total_copies  # Return total copied elements.  

    def _require_index_in_range(self, index: int) -> None:  # Validate index in [0, size-1].  
        if index < 0 or index >= self._size:  # Reject invalid indices for access/remove.  
            raise IndexError("index out of range")  # Signal invalid index.  

    def _require_insert_index_in_range(self, index: int) -> None:  # Validate insert index in [0, size].  
        if index < 0 or index > self._size:  # Reject invalid indices for insert.  
            raise IndexError("index out of range for insert")  # Signal invalid index.  

    def _resize(self, new_capacity: int) -> int:  # Resize backing buffer and return how many elements were copied.  
        if new_capacity < self._size:  # Ensure new buffer can hold all existing elements.  
            raise ValueError("new_capacity must be >= size")  # Signal invalid resize request.  
        if new_capacity < 1:  # Keep capacity positive for simplicity.  
            raise ValueError("new_capacity must be >= 1")  # Signal invalid capacity.  

        new_data: List[Optional[int]] = [None] * new_capacity  # Allocate new buffer.  
        copied = 0  # Count copied elements deterministically.  
        for i in range(self._size):  # Copy exactly the used portion.  
            new_data[i] = self._data[i]  # Copy one element.  
            copied += 1  # Count one copy/move.  
        self._data = new_data  # Swap buffer reference.  
        self._capacity = new_capacity  # Update capacity.  
        self._total_copies += copied  # Accumulate total copies.  
        return copied  # Return copies performed for this resize.  

    def _ensure_capacity_for_one_more(self) -> int:  # Ensure capacity for one more element; return resize copies.  
        if self._size < self._capacity:  # Fast path: there is already free space.  
            return 0  # No resize needed.  
        new_capacity = self._capacity * 2  # Doubling growth strategy for amortized O(1) append.  
        return self._resize(new_capacity)  # Resize and return copies count.  

    def get(self, index: int) -> int:  # Return element at index (O(1)).  
        self._require_index_in_range(index)  # Validate index before access.  
        value = self._data[index]  # Read from backing buffer.  
        assert value is not None  # Enforce invariant: used slots are not None.  
        return value  # Return value.  

    def set(self, index: int, value: int) -> None:  # Set element at index (O(1)).  
        self._require_index_in_range(index)  # Validate index before write.  
        self._data[index] = value  # Write to backing buffer.  

    def to_list(self) -> List[int]:  # Return a copy of the used portion as a Python list.  
        result: List[int] = []  # Accumulate values into a new list.  
        for i in range(self._size):  # Copy only used elements.  
            value = self._data[i]  # Read element.  
            assert value is not None  # Enforce invariant for used slots.  
            result.append(value)  # Append to result list.  
        return result  # Return copy.  

    def index_of(self, value: int) -> int:  # Return first index of value or -1 (O(n)).  
        for i in range(self._size):  # Linear scan.  
            if self._data[i] == value:  # Compare with target.  
                return i  # Return first match.  
        return -1  # Signal not found.  

    def append(self, value: int) -> OperationCost:  # Append at end and return resize cost (moved=0).  
        copied = self._ensure_capacity_for_one_more()  # Resize if needed and get copies count.  
        self._data[self._size] = value  # Write new element into next free slot.  
        self._size += 1  # Increase size after write.  
        return OperationCost(copied=copied, moved=0)  # Return per-operation cost.  

    def insert_at(self, index: int, value: int) -> OperationCost:  # Insert at index and return cost (copies + shifts).  
        self._require_insert_index_in_range(index)  # Validate insertion index.  
        copied = self._ensure_capacity_for_one_more()  # Resize if needed before shifting.  

        moved = self._size - index  # Shifting right moves exactly (size - index) elements.  
        for i in range(self._size, index, -1):  # Shift right from tail toward index.  
            self._data[i] = self._data[i - 1]  # Move one element one step to the right.  
        self._data[index] = value  # Write new value into gap.  
        self._size += 1  # Increase size after insertion.  
        return OperationCost(copied=copied, moved=moved)  # Return per-operation cost.  

    def remove_at(self, index: int) -> RemoveResult:  # Remove at index and return removed value + shift cost.  
        self._require_index_in_range(index)  # Validate index before removal.  
        removed_value = self.get(index)  # Read value (also enforces invariants).  

        moved = self._size - index - 1  # Shifting left moves exactly (size - index - 1) elements.  
        for i in range(index, self._size - 1):  # Shift left to fill the removed slot.  
            self._data[i] = self._data[i + 1]  # Move one element one step to the left.  
        self._data[self._size - 1] = None  # Clear last slot to keep state tidy.  
        self._size -= 1  # Decrease size after removal.  
        return RemoveResult(value=removed_value, cost=OperationCost(copied=0, moved=moved))  # Return removed value and cost.  


@dataclass(frozen=True)  # Make summary immutable for deterministic printing/tests.  
class AppendSummary:  # Summarize a sequence of appends (growth behavior).  
    m: int  # Number of appends performed.  
    final_size: int  # Final size after appends.  
    final_capacity: int  # Final capacity after growth.  
    total_copies: int  # Total copied elements due to resizes.  
    total_actual_cost: int  # Total cost if each append costs 1 write + copies.  
    max_copied_in_one_op: int  # Maximum copied elements in any single append.  


def simulate_appends(m: int) -> AppendSummary:  # Simulate m appends and summarize growth/copy costs.  
    if m < 0:  # Reject invalid negative counts.  
        raise ValueError("m must be >= 0")  # Signal invalid input.  
    a = DynamicArray()  # Use a fresh array so results depend only on m.  
    total_actual = 0  # Accumulate total actual cost (1 write + copied).  
    max_copied = 0  # Track maximum copied elements in a single append.  
    for i in range(m):  # Perform m appends.  
        cost = a.append(i)  # Append deterministic value and get per-op cost.  
        total_actual += 1 + cost.copied  # Add write cost + resize copy cost.  
        max_copied = max(max_copied, cost.copied)  # Update maximum copied.  
    return AppendSummary(  # Return a compact summary record.  
        m=m,  # Store operation count.  
        final_size=a.size,  # Store final size.  
        final_capacity=a.capacity,  # Store final capacity.  
        total_copies=a.total_copies,  # Store total copies.  
        total_actual_cost=total_actual,  # Store total actual cost.  
        max_copied_in_one_op=max_copied,  # Store max per-op copied.  
    )  # Close summary construction.  


def _build_filled_array(n: int) -> DynamicArray:  # Build a dynamic array filled with 0..n-1.  
    if n < 0:  # Reject invalid sizes early.  
        raise ValueError("n must be >= 0")  # Signal invalid input.  
    a = DynamicArray()  # Start from an empty array with capacity 1.  
    for i in range(n):  # Fill with deterministic values.  
        a.append(i)  # Append one value.  
    return a  # Return filled array.  


def simulate_append_cost_at_size(n: int) -> OperationCost:  # Build size n array then append once and return cost.  
    a = _build_filled_array(n)  # Build array of size n.  
    return a.append(999)  # Append sentinel and return its cost.  


def simulate_insert0_cost_at_size(n: int) -> OperationCost:  # Build size n array then insert at head and return cost.  
    a = _build_filled_array(n)  # Build array of size n.  
    return a.insert_at(0, 999)  # Insert sentinel at head and return cost.  


def _parse_numbers(argv: List[str], defaults: Sequence[int]) -> List[int]:  # Parse CLI args into ints with defaults.  
    if not argv:  # Use defaults when no args are provided.  
        return list(defaults)  # Return defaults as a list.  
    return [int(x) for x in argv]  # Parse each token to int.  


def format_append_summary_table(ms: Sequence[int]) -> str:  # Format growth summaries for multiple m values.  
    header = f"{'m':>6} | {'cap':>6} | {'copies':>7} | {'avg':>6} | {'maxCp':>6}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for m in ms:  # Render one row per m.  
        s = simulate_appends(m)  # Simulate and summarize.  
        avg = (s.total_actual_cost / s.m) if s.m > 0 else 0.0  # Average actual cost per op.  
        lines.append(f"{s.m:6d} | {s.final_capacity:6d} | {s.total_copies:7d} | {avg:6.2f} | {s.max_copied_in_one_op:6d}")  # Append row.  
    return "\n".join(lines)  # Join lines into printable table.  


def format_append_vs_insert0_table(ns: Sequence[int]) -> str:  # Compare append vs insert-at-head costs at different sizes.  
    header = f"{'n':>6} | {'appCp':>6} | {'appMv':>6} | {'ins0Cp':>7} | {'ins0Mv':>7}"  # Header line.  
    separator = "-" * len(header)  # Separator line.  
    lines: List[str] = [header, separator]  # Start with header + separator.  
    for n in ns:  # Render one row per n.  
        if n < 0:  # Reject invalid sizes early.  
            raise ValueError("n must be >= 0")  # Signal invalid input.  
        app = simulate_append_cost_at_size(n)  # Cost of append at size n.  
        ins0 = simulate_insert0_cost_at_size(n)  # Cost of insertAt(0) at size n.  
        lines.append(f"{n:6d} | {app.copied:6d} | {app.moved:6d} | {ins0.copied:7d} | {ins0.moved:7d}")  # Append row.  
    return "\n".join(lines)  # Join lines into printable table.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ms = _parse_numbers(argv, defaults=[0, 1, 2, 4, 8, 16, 32])  # Use args as m list, or defaults.  
    ns = [0, 1, 2, 4, 8, 16]  # Use a small fixed set for append vs insertAt(0) comparison.  
    print("=== Append Growth (m appends) ===")  # Print section title.  
    print(format_append_summary_table(ms))  # Print growth summary table.  
    print()  # Print blank line between sections.  
    print("=== Append vs insertAt(0) at size n ===")  # Print section title.  
    print(format_append_vs_insert0_table(ns))  # Print comparison table.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with code returned by main().  

