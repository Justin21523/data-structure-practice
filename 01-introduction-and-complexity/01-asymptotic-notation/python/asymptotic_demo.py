"""漸進符號（Asymptotic Notation）示範 / Asymptotic notation demo.  # Module docstring (bilingual).

本檔案用「操作次數（operation counts）」而非實際時間來展示成長率 / Use operation counts rather than wall time to show growth rates.  # Motivation and approach.
"""  # End of module docstring.

from __future__ import annotations  # Enable forward references in type hints (future-proofing).  

from dataclasses import dataclass  # Use dataclass to bundle results for printing.  
import sys  # Read CLI arguments for custom n values.  
from typing import Iterable, List  # Type hints for readability and tooling.  


@dataclass(frozen=True)  # Make rows immutable so the demo output is deterministic.  
class GrowthRow:  # Represent one row of the growth table for a specific n.  
    n: int  # Input size for this row (problem size).  
    constant_ops: int  # Simulated O(1) operation count.  
    log2_ops: int  # Simulated O(log n) operation count.  
    linear_ops: int  # Simulated O(n) operation count.  
    n_log2_n_ops: int  # Simulated O(n log n) operation count.  
    quadratic_ops: int  # Simulated O(n^2) operation count.  


def _require_non_negative(n: int) -> int:  # Validate inputs so counts remain well-defined.  
    """檢查 n >= 0 / Require n >= 0."""  # Short bilingual docstring for this helper.  
    if n < 0:  # Reject negative sizes because they do not make sense for complexity.  
        raise ValueError("n must be >= 0")  # Signal invalid input to the caller.  
    return n  # Return the validated value so callers can chain calls.  


def _require_at_least_one(n: int) -> int:  # Validate inputs for log-based demos.  
    """檢查 n >= 1 / Require n >= 1."""  # Short bilingual docstring for this helper.  
    if n < 1:  # log(n) is not meaningful for n <= 0 in this discrete demo.  
        raise ValueError("n must be >= 1")  # Signal invalid input to the caller.  
    return n  # Return the validated value so callers can proceed safely.  


def count_constant_ops(_: int) -> int:  # Provide a constant-time example that ignores input size.  
    """固定次數操作（O(1)）/ Constant operation count (O(1))."""  # Explain what this returns.  
    operations = 0  # Initialize the simulated operation counter.  
    operations += 1  # Simulate a basic operation (e.g., one assignment).  
    operations += 1  # Simulate a second basic operation.  
    operations += 1  # Simulate a third basic operation.  
    return operations  # Return a value independent of n to illustrate O(1).  


def count_log2_ops(n: int) -> int:  # Provide a logarithmic example via repeated halving.  
    """反覆除以 2 的迴圈（O(log n)）/ Repeated halving loop (O(log n))."""  # Describe the pattern.  
    n = _require_at_least_one(n)  # Ensure n is valid before we start halving.  
    current = n  # Copy n so we do not mutate the caller's variable accidentally.  
    operations = 0  # Initialize the simulated operation counter.  
    while current > 1:  # Each iteration reduces the problem size by half (logarithmic).  
        current //= 2  # Halve the remaining problem size using integer division.  
        operations += 1  # Count one loop iteration as one basic operation.  
    return operations  # Return the number of halvings (~ floor(log2(n))).  


def count_linear_ops(n: int) -> int:  # Provide a linear example via a single loop.  
    """走訪 n 次（O(n)）/ Single pass loop (O(n))."""  # Describe the pattern.  
    n = _require_non_negative(n)  # Validate n so range(n) is well-defined.  
    operations = 0  # Initialize the simulated operation counter.  
    for _ in range(n):  # Iterate exactly n times (linear growth).  
        operations += 1  # Count one basic operation per iteration.  
    return operations  # Return n to illustrate O(n).  


def count_n_log2_n_ops(n: int) -> int:  # Provide an n log n example via nested (n, log n) loops.  
    """外層 n 次、內層 log n 次（O(n log n)）/ Outer n loop, inner log n loop (O(n log n))."""  # Describe the pattern.  
    n = _require_non_negative(n)  # Validate n so the outer loop is well-defined.  
    if n == 0:  # Handle the boundary case explicitly to keep the demo intuitive.  
        return 0  # Zero work for zero-sized input.  
    operations = 0  # Initialize the simulated operation counter.  
    for _ in range(n):  # Outer loop runs n times (linear factor).  
        current = n  # Inner loop starts with size n each time (same order).  
        while current > 1:  # Inner loop halves current each time (logarithmic factor).  
            current //= 2  # Halve the inner problem size.  
            operations += 1  # Count one basic operation per inner iteration.  
    return operations  # Return approximately n * floor(log2(n)).  


def count_quadratic_ops(n: int) -> int:  # Provide a quadratic example via two nested loops.  
    """雙重迴圈（O(n^2)）/ Double nested loops (O(n^2))."""  # Describe the pattern.  
    n = _require_non_negative(n)  # Validate n so both loops are well-defined.  
    operations = 0  # Initialize the simulated operation counter.  
    for _ in range(n):  # Outer loop runs n times.  
        for _ in range(n):  # Inner loop also runs n times (n * n total iterations).  
            operations += 1  # Count one basic operation per inner iteration.  
    return operations  # Return n^2 to illustrate quadratic growth.  


def build_growth_rows(ns: Iterable[int]) -> List[GrowthRow]:  # Compute all growth counts for each n.  
    """建立成長率表格資料 / Build growth table rows."""  # Explain the purpose at a high level.  
    rows: List[GrowthRow] = []  # Accumulate rows so we can print them later.  
    for n in ns:  # Process each input size requested by the user.  
        n = _require_non_negative(int(n))  # Normalize and validate n values from user input.  
        row = GrowthRow(  # Build one row of results for this n.  
            n=n,  # Store the input size.  
            constant_ops=count_constant_ops(n),  # Compute O(1) example count.  
            log2_ops=count_log2_ops(max(1, n)),  # Compute O(log n) with a safe minimum of 1.  
            linear_ops=count_linear_ops(n),  # Compute O(n) example count.  
            n_log2_n_ops=count_n_log2_n_ops(n),  # Compute O(n log n) example count.  
            quadratic_ops=count_quadratic_ops(n),  # Compute O(n^2) example count.  
        )  # Close the dataclass construction.  
        rows.append(row)  # Append this row to the output list.  
    return rows  # Return the full set of rows for formatting/printing.  


def format_growth_table(rows: List[GrowthRow]) -> str:  # Convert rows into a human-readable table string.  
    """把結果格式化成表格字串 / Format rows into a printable table string."""  # Explain the output.  
    header = f"{'n':>8} | {'O(1)':>8} | {'O(log n)':>8} | {'O(n)':>8} | {'O(n log n)':>10} | {'O(n^2)':>10}"  # Column header line.  
    separator = "-" * len(header)  # Build a separator line that matches the header width.  
    lines = [header, separator]  # Start the table with header + separator.  
    for row in rows:  # Render each row into aligned columns.  
        lines.append(  # Append a formatted line for this row.  
            f"{row.n:8d} | {row.constant_ops:8d} | {row.log2_ops:8d} | {row.linear_ops:8d} | {row.n_log2_n_ops:10d} | {row.quadratic_ops:10d}"  # One row of numeric output.  
        )  # Close append call.  
    return "\n".join(lines)  # Join lines with newlines so it can be printed as-is.  


def _parse_ns(argv: List[str]) -> List[int]:  # Convert CLI arguments into a list of n values.  
    """解析命令列 n 值 / Parse CLI n values."""  # Describe what this helper does.  
    if not argv:  # If no args are provided, use a reasonable default sequence.  
        return [1, 2, 4, 8, 16, 32]  # Default sizes show clear growth differences.  
    ns: List[int] = []  # Accumulate parsed integers.  
    for raw in argv:  # Parse each argument individually to provide better error messages.  
        ns.append(int(raw))  # Convert string to int (ValueError will surface for invalid input).  
    return ns  # Return the parsed list for downstream processing.  


def main(argv: List[str]) -> int:  # CLI entry point for the demo script.  
    """主程式 / Main entry point."""  # Describe the function role succinctly.  
    ns = _parse_ns(argv)  # Determine which input sizes to demonstrate.  
    rows = build_growth_rows(ns)  # Compute growth counts for each n.  
    print(format_growth_table(rows))  # Print a formatted table to stdout for study.  
    return 0  # Indicate success to the OS.  


if __name__ == "__main__":  # Run the demo only when executed as a script.  
    raise SystemExit(main(sys.argv[1:]))  # Exit with the return code from main().  
