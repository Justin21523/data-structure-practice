"""攤銷分析（Amortized Analysis）示範 / Amortized analysis demo.  # Module docstring (bilingual).

本單元用「動態陣列（dynamic array）擴容」示範攤銷 O(1)：  # Traditional Chinese overview with context.
- 平常 append 很便宜（只寫入 1 次）  # Explain the usual cheap case.
- 偶爾會擴容，需要搬移很多元素（看起來很貴）  # Explain the occasional expensive resize.
- 但把一長串操作的成本平均分攤後，平均仍是常數  # Explain the amortized conclusion.

We count deterministic "operation costs" (writes + copies) instead of wall-clock time.  # English summary of the counting model.
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references in type hints (future-proofing).  

from dataclasses import dataclass  # Use dataclass to bundle per-operation results for tests/printing.  
import sys  # Read CLI arguments for custom append counts.  
from typing import List, Sequence  # Type hints for readability and tooling.  


def potential(size: int, capacity: int) -> int:  # Compute the potential Φ used by the potential method.  
    """位能函數 Φ / Potential function Φ.  # Short bilingual docstring.

    我們使用：Φ = 2·size - capacity + 1  # State the exact formula used in this unit.
    - 這個 +1 只是常數位移，讓初始狀態 (size=0, cap=1) 的 Φ = 0  # Explain why the +1 exists.
    - 位能法的攤銷成本：amortized = actual + (Φ_after - Φ_before)  # Define the amortized-cost formula.
    """  # End of docstring.
    if capacity < 1:  # Defensively reject invalid capacities to keep the formula meaningful.  
        raise ValueError("capacity must be >= 1")  # Signal invalid input to the caller.  
    return 2 * size - capacity + 1  # Return Φ(size, capacity) using integer arithmetic.  


@dataclass(frozen=True)  # Make step records immutable so tests cannot mutate them accidentally.  
class AppendStep:  # Record one append operation's costs and accounting/potential state.  
    index: int  # 1-based operation index in the sequence (for debugging and tables).  
    size_before: int  # Size before the append (number of stored elements).  
    capacity_before: int  # Capacity before the append (allocated slots).  
    copied: int  # Number of elements copied during resize (0 if no resize).  
    actual_cost: int  # Actual cost for this append: 1 write + copied moves.  
    phi_before: int  # Φ before the operation (potential method).  
    phi_after: int  # Φ after the operation (potential method).  
    amortized_cost: int  # Amortized cost via potential method: actual + ΔΦ.  
    bank_after: int  # Remaining credit after charging 3 and paying the actual cost.  


class AmortizedDynamicArray:  # A tiny dynamic array implementation instrumented for amortized analysis.  
    def __init__(self) -> None:  # Initialize an empty table with a small starting capacity.  
        self._size = 0  # Track the number of elements actually stored.  
        self._capacity = 1  # Start with capacity 1 so the first write is simple and deterministic.  
        self._data: List[int | None] = [None] * self._capacity  # Pre-allocate a fixed-size buffer.  

        self._bank = 0  # Track credits for the accounting method (start with 0 credit).  
        self._total_actual_cost = 0  # Track the total actual cost across all appends.  
        self._total_copies = 0  # Track total number of copied elements across resizes.  
        self._max_actual_cost = 0  # Track the maximum single-operation actual cost.  
        self._steps: List[AppendStep] = []  # Keep a per-append log for tests and optional printing.  

    @property
    def size(self) -> int:  # Expose size as a read-only property for callers/tests.  
        return self._size  # Return current size.  

    @property
    def capacity(self) -> int:  # Expose capacity as a read-only property for callers/tests.  
        return self._capacity  # Return current capacity.  

    @property
    def bank(self) -> int:  # Expose current bank balance (credits) for callers/tests.  
        return self._bank  # Return current bank balance.  

    @property
    def total_actual_cost(self) -> int:  # Expose total actual cost accumulated so far.  
        return self._total_actual_cost  # Return total actual cost.  

    @property
    def total_copies(self) -> int:  # Expose total number of copies performed during resizes.  
        return self._total_copies  # Return total copied elements.  

    @property
    def max_actual_cost(self) -> int:  # Expose the max single-operation actual cost observed.  
        return self._max_actual_cost  # Return the maximum actual cost.  

    @property
    def steps(self) -> Sequence[AppendStep]:  # Expose the per-step log as a read-only sequence.  
        return tuple(self._steps)  # Return an immutable snapshot to prevent accidental mutation.  

    def _resize(self, new_capacity: int) -> int:  # Grow the backing buffer and return the copy cost.  
        if new_capacity < self._size:  # Ensure the new capacity can hold existing elements.  
            raise ValueError("new_capacity must be >= size")  # Reject invalid resize requests.  
        if new_capacity < 1:  # Ensure capacity is never zero to keep invariants simple.  
            raise ValueError("new_capacity must be >= 1")  # Reject invalid capacities.  

        copied = 0  # Count how many element moves we perform during the resize.  
        new_data: List[int | None] = [None] * new_capacity  # Allocate a new fixed-size buffer.  
        for i in range(self._size):  # Copy exactly the current size elements into the new buffer.  
            new_data[i] = self._data[i]  # Copy one element (this models one "move" cost).  
            copied += 1  # Count one copy/move for amortized-cost measurement.  
        self._data = new_data  # Swap the buffer reference to complete the resize.  
        self._capacity = new_capacity  # Update capacity to match the new buffer size.  
        return copied  # Return the number of copied elements as the resize's copy cost.  

    def append(self, value: int) -> AppendStep:  # Append one value and return a detailed cost record.  
        phi_before = potential(self._size, self._capacity)  # Capture Φ before we mutate the structure.  
        copied = 0  # Default: no resize, so no copies are performed.  
        actual_cost = 1  # Always count 1 for writing the new element into the table.  

        if self._size == self._capacity:  # Resize exactly when the table is full (classic doubling rule).  
            new_capacity = self._capacity * 2  # Double the capacity to achieve amortized O(1) growth.  
            copied = self._resize(new_capacity)  # Perform the resize and count copies deterministically.  
            actual_cost += copied  # Add copy cost to the actual cost for this operation.  

        self._data[self._size] = value  # Store the new value into the next free slot.  
        self._size += 1  # Increase the logical size after insertion succeeds.  

        phi_after = potential(self._size, self._capacity)  # Capture Φ after the append completes.  
        amortized_cost = actual_cost + (phi_after - phi_before)  # Compute amortized cost using potential method.  

        self._bank += 3 - actual_cost  # Accounting method: charge 3, then pay the actual cost from credit.  
        if self._bank < 0:  # Credit should never go negative if the amortized charge is sufficient.  
            raise RuntimeError("bank went negative (accounting invariant violated)")  # Fail fast for debugging.  

        self._total_actual_cost += actual_cost  # Accumulate total actual cost across all operations.  
        self._total_copies += copied  # Accumulate total copies so we can validate aggregate bounds.  
        self._max_actual_cost = max(self._max_actual_cost, actual_cost)  # Update max observed cost.  

        step = AppendStep(  # Build an immutable record describing this append's costs and state.  
            index=len(self._steps) + 1,  # Use the next 1-based index for readability in logs.  
            size_before=self._size - 1,  # Size before insertion is the new size minus 1.  
            capacity_before=self._capacity if copied == 0 else self._capacity // 2,  # Recover pre-resize capacity.  
            copied=copied,  # Record how many elements were moved during this operation.  
            actual_cost=actual_cost,  # Record actual cost for this operation.  
            phi_before=phi_before,  # Record Φ before mutation.  
            phi_after=phi_after,  # Record Φ after mutation.  
            amortized_cost=amortized_cost,  # Record amortized cost via potential method.  
            bank_after=self._bank,  # Record bank balance after charging/paying for this operation.  
        )  # Close AppendStep construction.  
        self._steps.append(step)  # Append step to the log so tests can validate per-step invariants.  
        return step  # Return the step record for optional callers.  


@dataclass(frozen=True)  # Make summary immutable so printing/tests are deterministic.  
class SimulationSummary:  # A compact summary for a sequence of appends.  
    m: int  # Number of append operations performed.  
    final_size: int  # Final size after simulation (should equal m).  
    final_capacity: int  # Final capacity after simulation (power of two >= m, with this doubling rule).  
    total_actual_cost: int  # Total actual cost across all appends (writes + copies).  
    total_copies: int  # Total number of copied elements during all resizes.  
    max_actual_cost: int  # Maximum single-operation actual cost in the sequence.  
    final_bank: int  # Final credit balance when charging 3 per operation (accounting method).  


def simulate_appends(m: int) -> SimulationSummary:  # Run the demo for m appends and return a summary.  
    """模擬連續 append / Simulate repeated appends."""  # Short bilingual docstring.  
    if m < 0:  # Reject negative operation counts because they do not make sense.  
        raise ValueError("m must be >= 0")  # Signal invalid input to the caller.  

    table = AmortizedDynamicArray()  # Create a fresh dynamic array so results depend only on m.  
    for i in range(m):  # Perform m appends with deterministic values.  
        table.append(i)  # Append one value and record costs internally.  

    return SimulationSummary(  # Return a compact summary for printing/tests.  
        m=m,  # Record the requested operation count.  
        final_size=table.size,  # Expose the final size.  
        final_capacity=table.capacity,  # Expose the final capacity.  
        total_actual_cost=table.total_actual_cost,  # Expose total actual cost.  
        total_copies=table.total_copies,  # Expose total copies.  
        max_actual_cost=table.max_actual_cost,  # Expose max single-operation cost.  
        final_bank=table.bank,  # Expose final bank balance.  
    )  # Close SimulationSummary construction.  


def _is_power_of_two(x: int) -> bool:  # Check whether x is a power of two (used for validation).  
    return x > 0 and (x & (x - 1) == 0)  # Standard bit trick: powers of two have exactly one set bit.  


def format_summary_table(summaries: List[SimulationSummary]) -> str:  # Format multiple summaries into a printable table.  
    header = f"{'m':>6} | {'total':>8} | {'avg':>6} | {'max':>6} | {'cap':>6} | {'copies':>7} | {'bank':>6}"  # Table header line.  
    separator = "-" * len(header)  # Separator line to improve readability.  
    lines = [header, separator]  # Start with header + separator.  
    for s in summaries:  # Render each summary as one row.  
        avg = (s.total_actual_cost / s.m) if s.m > 0 else 0.0  # Compute average actual cost per op (safe for m=0).  
        lines.append(  # Append one aligned row.  
            f"{s.m:6d} | {s.total_actual_cost:8d} | {avg:6.2f} | {s.max_actual_cost:6d} | {s.final_capacity:6d} | {s.total_copies:7d} | {s.final_bank:6d}"  # Row string.  
        )  # Close row append.  
    return "\n".join(lines)  # Join lines into a printable multi-line string.  


def _parse_ms(argv: List[str]) -> List[int]:  # Parse CLI args into a list of m values.  
    if not argv:  # Use defaults when no args are provided.  
        return [1, 2, 4, 8, 16, 32]  # Default sizes mirror the previous unit for consistency.  
    return [int(x) for x in argv]  # Parse all args as integers (ValueError surfaces on invalid input).  


def main(argv: List[str]) -> int:  # CLI entry point that prints amortized-analysis summaries.  
    ms = _parse_ms(argv)  # Determine which operation counts to simulate.  
    summaries: List[SimulationSummary] = []  # Accumulate results so we can format them together.  
    for m in ms:  # Simulate each requested m independently.  
        summary = simulate_appends(m)  # Run the simulation and capture its summary.  
        if summary.final_size != m:  # Sanity check: final size should always equal number of appends.  
            raise RuntimeError("size mismatch (internal bug)")  # Fail fast if invariants are violated.  
        if not _is_power_of_two(summary.final_capacity):  # Validate the doubling rule created a power-of-two capacity.  
            raise RuntimeError("capacity is not a power of two (internal bug)")  # Fail fast for debugging.  
        if summary.final_capacity < max(1, m):  # Validate capacity is large enough to hold all appended elements.  
            raise RuntimeError("capacity too small (internal bug)")  # Fail fast for debugging.  
        summaries.append(summary)  # Store the validated summary for formatting.  
    print(format_summary_table(summaries))  # Print a formatted table so learners can inspect growth.  
    return 0  # Indicate success to the OS.  


if __name__ == "__main__":  # Execute only when run as a script, not when imported by tests.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with the return code from main().  

