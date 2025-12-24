/* 01 漸進符號示範（JavaScript / Node.js）/ Asymptotic notation demo (JavaScript / Node.js). */  // Bilingual file header.
'use strict';  // Enable strict mode to avoid accidental globals and silent coercions.

function requireNonNegative(n) {  // Validate that n is a non-negative integer for size-based loops.
  if (!Number.isInteger(n)) {  // Reject non-integer inputs to keep the demo purely discrete.
    throw new TypeError('n must be an integer');  // Fail fast with a clear error message.
  }  // Close integer validation.
  if (n < 0) {  // Reject negative sizes because they do not make sense for complexity analysis.
    throw new RangeError('n must be >= 0');  // Fail fast for invalid input.
  }  // Close range validation.
  return n;  // Return the validated value so callers can chain.
}  // End of requireNonNegative.

function requireAtLeastOne(n) {  // Validate that n is at least 1 for log-based counters.
  if (!Number.isInteger(n)) {  // Reject non-integer inputs to keep iteration counts discrete.
    throw new TypeError('n must be an integer');  // Fail fast with a clear error message.
  }  // Close integer validation.
  if (n < 1) {  // Reject n < 1 because log2(n) is not defined here for non-positive n.
    throw new RangeError('n must be >= 1');  // Fail fast for invalid input.
  }  // Close range validation.
  return n;  // Return the validated value for downstream use.
}  // End of requireAtLeastOne.

function countConstantOps(_n) {  // Simulate a constant-time algorithm independent of n.
  let operations = 0;  // Initialize the simulated operation counter.
  operations += 1;  // Count a basic operation #1.
  operations += 1;  // Count a basic operation #2.
  operations += 1;  // Count a basic operation #3.
  return operations;  // Return a constant value to illustrate O(1).
}  // End of countConstantOps.

function countLog2Ops(n) {  // Simulate logarithmic growth via repeated halving.
  n = requireAtLeastOne(n);  // Validate input before the halving loop starts.
  let current = n;  // Copy n so we can shrink it without losing the original.
  let operations = 0;  // Initialize the iteration counter for the halving loop.
  while (current > 1) {  // Each iteration halves current, giving ~log2(n) iterations.
    current = Math.floor(current / 2);  // Halve using integer division semantics.
    operations += 1;  // Count one operation per halving step.
  }  // End of halving loop.
  return operations;  // Return floor(log2(n)) for this discrete process.
}  // End of countLog2Ops.

function countLinearOps(n) {  // Simulate linear growth via one pass of n iterations.
  n = requireNonNegative(n);  // Validate n so the loop bound is meaningful.
  let operations = 0;  // Initialize the operation counter.
  for (let i = 0; i < n; i += 1) {  // Run exactly n iterations to illustrate O(n).
    operations += 1;  // Count one operation per iteration.
  }  // End of linear loop.
  return operations;  // Return n as the total operation count.
}  // End of countLinearOps.

function countNLog2NOps(n) {  // Simulate n log n growth via outer n loop + inner halving loop.
  n = requireNonNegative(n);  // Validate input to keep the loops well-defined.
  if (n === 0) {  // Handle the boundary case explicitly for a clean demo definition.
    return 0;  // Define 0 * log(0) as 0 operations in this discrete demonstration.
  }  // Close boundary-case block.
  let operations = 0;  // Initialize the counter for the nested loops.
  for (let i = 0; i < n; i += 1) {  // Outer loop contributes the linear factor.
    let current = n;  // Inner loop starts at n each time to yield the logarithmic factor.
    while (current > 1) {  // Inner loop halves current, giving ~log2(n) iterations.
      current = Math.floor(current / 2);  // Halve the inner problem size.
      operations += 1;  // Count one operation per inner iteration.
    }  // End of inner loop.
  }  // End of outer loop.
  return operations;  // Return approximately n * floor(log2(n)).
}  // End of countNLog2NOps.

function countQuadraticOps(n) {  // Simulate quadratic growth via two nested loops.
  n = requireNonNegative(n);  // Validate n before building nested loops.
  let operations = 0;  // Initialize the counter for the double loop.
  for (let i = 0; i < n; i += 1) {  // Outer loop runs n times.
    for (let j = 0; j < n; j += 1) {  // Inner loop runs n times, giving n * n iterations.
      operations += 1;  // Count one operation per inner iteration.
    }  // End of inner loop.
  }  // End of outer loop.
  return operations;  // Return n^2 as the operation count for this demo.
}  // End of countQuadraticOps.

function parseNsOrDefault(argv) {  // Parse CLI arguments into an array of n values.
  if (argv.length === 0) {  // Use defaults when no CLI args are provided.
    return [1, 2, 4, 8, 16, 32];  // Default sizes show clear growth differences.
  }  // Close default branch.
  return argv.map((raw) => Number.parseInt(raw, 10));  // Convert each CLI token to an integer.
}  // End of parseNsOrDefault.

function requireAllAtLeastOne(ns) {  // Ensure table inputs are valid for the log-based counters.
  for (const n of ns) {  // Validate each provided n individually.
    if (!Number.isInteger(n) || n < 1) {  // Reject invalid values to keep the demo output meaningful.
      throw new RangeError('All n values must be integers >= 1');  // Fail fast with a clear error.
    }  // Close validation block.
  }  // End of loop.
}  // End of requireAllAtLeastOne.

function formatGrowthTable(ns) {  // Format the computed counts into a printable ASCII table.
  const header = `${'n'.padStart(8)} | ${'O(1)'.padStart(8)} | ${'O(log n)'.padStart(8)} | ${'O(n)'.padStart(8)} | ${'O(n log n)'.padStart(10)} | ${'O(n^2)'.padStart(10)}`;  // Build the table header with aligned columns.
  const separator = '-'.repeat(header.length);  // Build a separator line matching the header width.
  const lines = [header, separator];  // Start the table with header + separator.

  for (const n of ns) {  // Render each row for each input size.
    const row = `${String(n).padStart(8)} | ${String(countConstantOps(n)).padStart(8)} | ${String(countLog2Ops(n)).padStart(8)} | ${String(countLinearOps(n)).padStart(8)} | ${String(countNLog2NOps(n)).padStart(10)} | ${String(countQuadraticOps(n)).padStart(10)}`;  // Format one row of numeric results.
    lines.push(row);  // Append the row to the table.
  }  // End row loop.
  return lines.join('\n');  // Join lines with newlines to produce the final printable string.
}  // End of formatGrowthTable.

function main(argv) {  // CLI entry point for the Node.js demo.
  const ns = parseNsOrDefault(argv);  // Parse arguments or use defaults.
  requireAllAtLeastOne(ns);  // Validate that all n values are valid for log-based counters.
  console.log(formatGrowthTable(ns));  // Print the formatted table to stdout for study.
  return 0;  // Return an exit code for embedding in other scripts if desired.
}  // End of main.

module.exports = {  // Export functions so the built-in Node test runner can import them.
  countConstantOps,  // Export O(1) demo function.
  countLog2Ops,  // Export O(log n) demo function.
  countLinearOps,  // Export O(n) demo function.
  countNLog2NOps,  // Export O(n log n) demo function.
  countQuadraticOps,  // Export O(n^2) demo function.
  formatGrowthTable,  // Export table formatter for optional testing.
  parseNsOrDefault,  // Export parser for optional testing.
  requireAllAtLeastOne,  // Export validator for optional testing.
  requireNonNegative,  // Export validator for optional testing.
  requireAtLeastOne,  // Export validator for optional testing.
  main,  // Export main for optional reuse.
};  // End of module.exports block.

if (require.main === module) {  // Run main only when this file is executed directly.
  process.exitCode = main(process.argv.slice(2));  // Forward CLI args and set process exit code.
}  // End of CLI guard.

