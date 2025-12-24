"""靜態陣列（Array）示範 / Static array demo.  # Module docstring (bilingual).

本檔案用「固定容量陣列（FixedArray）」模擬靜態陣列：  # Traditional Chinese overview for learners.
- 記憶體概念上是連續配置（contiguous）  # Explain contiguous layout.
- `get/set` 可用索引直接存取（O(1)）  # Explain O(1) random access.
- `insert/remove` 需要搬移（shift）元素（O(n)）  # Explain O(n) shifts.

We count "moved elements" during insert/remove to make the O(n) cost visible and deterministic.  # English summary.
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references for type hints.  

from dataclasses import dataclass  # Use dataclass to return structured results for remove operations.  
import sys  # Read CLI arguments for custom demo sizes.  
from typing import List, Optional, Sequence  # Type hints for clarity and tooling.  


@dataclass(frozen=True)  # Make remove results immutable so tests remain deterministic.  
class RemoveResult:  # Return both the removed value and the number of moved elements.  
    value: int  # The element that was removed from the array.  
    moved: int  # How many elements were shifted left to fill the gap.  


class FixedArray:  # A fixed-capacity array with manual shifting (teaching-oriented).  
    def __init__(self, capacity: int) -> None:  # Create an empty fixed array with a given capacity.  
        if capacity < 0:  # Reject negative capacities because they do not make sense.  
            raise ValueError("capacity must be >= 0")  # Signal invalid input to the caller.  
        self._capacity = capacity  # Store capacity so we can detect overflow on insert/append.  
        self._size = 0  # Track the number of valid elements stored in the array.  
        self._data: List[Optional[int]] = [None] * capacity  # Pre-allocate fixed storage slots.  

    @property
    def capacity(self) -> int:  # Expose capacity for inspection/tests.  
        return self._capacity  # Return the fixed capacity.  

    @property
    def size(self) -> int:  # Expose size for inspection/tests.  
        return self._size  # Return the current number of stored elements.  

    def __len__(self) -> int:  # Allow using len(fixed_array) to read size.  
        return self._size  # Return the number of valid elements.  

    def _require_index_in_range(self, index: int) -> None:  # Validate index in [0, size-1].  
        if index < 0 or index >= self._size:  # Reject out-of-range indices for element access.  
            raise IndexError("index out of range")  # Signal invalid index to the caller.  

    def _require_insert_index_in_range(self, index: int) -> None:  # Validate insert index in [0, size].  
        if index < 0 or index > self._size:  # Reject out-of-range indices for insertion.  
            raise IndexError("index out of range for insert")  # Signal invalid index to the caller.  

    def _require_not_full(self) -> None:  # Validate that there is at least one free slot.  
        if self._size >= self._capacity:  # Detect overflow when the array is full.  
            raise OverflowError("fixed array is full")  # Signal that insertion/append cannot proceed.  

    def get(self, index: int) -> int:  # Return the element at a valid index (O(1)).  
        self._require_index_in_range(index)  # Validate index before accessing internal storage.  
        value = self._data[index]  # Read the stored value from the backing buffer.  
        assert value is not None  # Ensure internal invariant: used slots are never None.  
        return value  # Return the value to the caller.  

    def set(self, index: int, value: int) -> None:  # Set the element at a valid index (O(1)).  
        self._require_index_in_range(index)  # Validate index before writing internal storage.  
        self._data[index] = value  # Write value directly into the backing buffer.  

    def to_list(self) -> List[int]:  # Return a Python list of the used portion of the array.  
        result: List[int] = []  # Accumulate elements into a new list for callers.  
        for i in range(self._size):  # Iterate over only the valid portion.  
            value = self._data[i]  # Read element (may be None only outside size).  
            assert value is not None  # Enforce invariant for used slots.  
            result.append(value)  # Append value to the result list.  
        return result  # Return a copy so callers cannot mutate internal storage.  

    def index_of(self, value: int) -> int:  # Return the first index of value, or -1 if not found (O(n)).  
        for i in range(self._size):  # Scan from left to right (linear search).  
            if self._data[i] == value:  # Compare stored element with target value.  
                return i  # Return the first matching index.  
        return -1  # Signal "not found" with a conventional sentinel.  

    def append(self, value: int) -> int:  # Append at the end (O(1) if not full) and return moved count.  
        return self.insert_at(self._size, value)  # Reuse insert_at to keep logic consistent (moves will be 0).  

    def insert_at(self, index: int, value: int) -> int:  # Insert value at index and return moved elements count.  
        self._require_not_full()  # Ensure there is free space in the fixed-capacity buffer.  
        self._require_insert_index_in_range(index)  # Validate insertion index.  

        moved = self._size - index  # Shifting right moves exactly (size - index) elements.  
        for i in range(self._size, index, -1):  # Shift elements right, from tail toward index.  
            self._data[i] = self._data[i - 1]  # Move one element one step to the right.  
        self._data[index] = value  # Write the new value into the insertion position.  
        self._size += 1  # Increase size after successful insertion.  
        return moved  # Return how many elements were shifted (the visible O(n) cost).  

    def remove_at(self, index: int) -> RemoveResult:  # Remove element at index and return removed value + moved count.  
        self._require_index_in_range(index)  # Validate index before removal.  

        removed_value = self.get(index)  # Read the element being removed (also asserts invariants).  
        moved = self._size - index - 1  # Shifting left moves exactly (size - index - 1) elements.  
        for i in range(index, self._size - 1):  # Shift elements left to fill the removed slot.  
            self._data[i] = self._data[i + 1]  # Move one element one step to the left.  
        if self._size > 0:  # Guard against size=0 (though removal requires size>0).  
            self._data[self._size - 1] = None  # Clear the now-unused last slot to keep invariants clean.  
        self._size -= 1  # Decrease size after removal.  
        return RemoveResult(value=removed_value, moved=moved)  # Return both removed value and move count.  


def _build_filled_array(n: int, extra_capacity: int) -> FixedArray:  # Build an array filled with 0..n-1.  
    if n < 0:  # Reject invalid sizes early.  
        raise ValueError("n must be >= 0")  # Signal invalid input.  
    array = FixedArray(capacity=n + extra_capacity)  # Allocate enough capacity to allow insertions.  
    for i in range(n):  # Append n deterministic values so we know the exact content.  
        array.append(i)  # Append one value (no shifting when appending).  
    return array  # Return the filled array for simulation.  


def simulate_insert_moves(n: int, index: int) -> int:  # Simulate inserting into a full prefix and return moved elements.  
    array = _build_filled_array(n, extra_capacity=1)  # Build size n with one extra slot for insertion.  
    moved = array.insert_at(index, 999)  # Insert a sentinel value and record move count.  
    return moved  # Return moved elements count (size-index).  


def simulate_remove_moves(n: int, index: int) -> int:  # Simulate removing from size n and return moved elements.  
    array = _build_filled_array(n, extra_capacity=0)  # Build size n exactly (no extra capacity needed).  
    result = array.remove_at(index)  # Remove element and record move count.  
    return result.moved  # Return moved elements count (size-index-1).  


def _parse_ns(argv: List[str]) -> List[int]:  # Parse CLI args into a list of n values.  
    if not argv:  # Use defaults when no args are provided.  
        return [0, 1, 2, 4, 8, 16]  # Include n=0 to show boundary behavior.  
    return [int(x) for x in argv]  # Convert each token to int (ValueError surfaces on invalid input).  


def format_moves_table(ns: Sequence[int]) -> str:  # Format a table of move counts for common positions.  
    header = f"{'n':>6} | {'ins@0':>6} | {'ins@mid':>7} | {'ins@end':>7} | {'rm@0':>6} | {'rm@mid':>7} | {'rm@end':>7}"  # Header line.  
    separator = "-" * len(header)  # Separator line for readability.  
    lines: List[str] = [header, separator]  # Start with header + separator.  

    for n in ns:  # Render one row per n.  
        if n < 0:  # Reject invalid n values early for clearer errors.  
            raise ValueError("n must be >= 0")  # Signal invalid input.  
        mid = n // 2  # Choose a deterministic middle index.  

        ins0 = simulate_insert_moves(n, 0)  # Moves when inserting at head.  
        insm = simulate_insert_moves(n, mid)  # Moves when inserting near the middle.  
        inse = simulate_insert_moves(n, n)  # Moves when inserting at tail (append).  

        if n == 0:  # Removal is not defined for an empty array.  
            rm0 = "n/a"  # Use a clear marker for not-applicable entries.  
            rmm = "n/a"  # Use a clear marker for not-applicable entries.  
            rme = "n/a"  # Use a clear marker for not-applicable entries.  
        else:  # For n>0 we can remove at valid indices.  
            rm0 = str(simulate_remove_moves(n, 0))  # Moves when removing at head.  
            rmm = str(simulate_remove_moves(n, mid))  # Moves when removing near the middle.  
            rme = str(simulate_remove_moves(n, n - 1))  # Moves when removing at tail.  

        lines.append(  # Append aligned row.  
            f"{n:6d} | {ins0:6d} | {insm:7d} | {inse:7d} | {rm0:>6} | {rmm:>7} | {rme:>7}"  # One row of numeric output.  
        )  # Close append.  

    return "\n".join(lines)  # Join into a printable table string.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo.  
    ns = _parse_ns(argv)  # Determine which n values to show.  
    print(format_moves_table(ns))  # Print the shift/move table.  
    return 0  # Indicate success.  


if __name__ == "__main__":  # Run the demo only when executed directly.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with the code from main().  

