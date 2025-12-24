// 02 攤銷分析示範（JavaScript）/ Amortized analysis demo (JavaScript).  // Bilingual file header for this unit.
// We simulate a dynamic array that doubles capacity when full, and we count writes + copies.  // English overview of the model.

"use strict";  // Enable strict mode so accidental globals are avoided.  

function potential(size, capacity) {  // Compute Φ for the potential method.  
  if (capacity < 1) {  // Reject invalid capacities so invariants remain well-defined.  
    throw new RangeError("capacity must be >= 1");  // Fail fast with a clear message.  
  }  // Close validation block.  
  return 2 * size - capacity + 1;  // Use Φ = 2·size - capacity + 1 so Φ(0,1)=0.  
}  // End of potential.  

class AmortizedDynamicArray {  // Minimal dynamic array with instrumentation for amortized analysis.  
  constructor() {  // Initialize an empty table with capacity 1.  
    this._size = 0;  // Track logical size (stored elements).  
    this._capacity = 1;  // Track allocated capacity (slots).  
    this._data = new Array(this._capacity);  // Allocate a fixed-size backing store.  

    this._bank = 0;  // Accounting-method credit balance (charge 3 per append).  
    this._totalActualCost = 0;  // Aggregate total actual cost.  
    this._totalCopies = 0;  // Aggregate total copy count across resizes.  
    this._maxActualCost = 0;  // Track maximum single-operation actual cost.  
    this._steps = [];  // Keep a per-operation log for tests/inspection.  
  }  // Close constructor.  

  get size() {  // Expose size for callers/tests.  
    return this._size;  // Return current size.  
  }  // End of size getter.  

  get capacity() {  // Expose capacity for callers/tests.  
    return this._capacity;  // Return current capacity.  
  }  // End of capacity getter.  

  get bank() {  // Expose bank (credit) for callers/tests.  
    return this._bank;  // Return current bank balance.  
  }  // End of bank getter.  

  get totalActualCost() {  // Expose total actual cost for aggregate checks.  
    return this._totalActualCost;  // Return total actual cost.  
  }  // End of totalActualCost getter.  

  get totalCopies() {  // Expose total copies for aggregate checks.  
    return this._totalCopies;  // Return total copies.  
  }  // End of totalCopies getter.  

  get maxActualCost() {  // Expose maximum single-operation actual cost.  
    return this._maxActualCost;  // Return max actual cost.  
  }  // End of maxActualCost getter.  

  get steps() {  // Expose per-operation steps for tests.  
    return this._steps;  // Return step log array (tests treat it as read-only).  
  }  // End of steps getter.  

  _resize(newCapacity) {  // Resize the backing store and return the number of copies performed.  
    if (newCapacity < this._size) {  // Ensure we do not lose existing elements.  
      throw new RangeError("newCapacity must be >= size");  // Reject invalid resize requests.  
    }  // Close validation block.  
    if (newCapacity < 1) {  // Keep capacity positive.  
      throw new RangeError("newCapacity must be >= 1");  // Reject invalid capacities.  
    }  // Close validation block.  

    const newData = new Array(newCapacity);  // Allocate a new fixed-size array.  
    let copied = 0;  // Count how many elements we copy.  
    for (let i = 0; i < this._size; i += 1) {  // Copy exactly the stored elements.  
      newData[i] = this._data[i];  // Copy one element (one "move" in our cost model).  
      copied += 1;  // Increment copy counter.  
    }  // Close copy loop.  
    this._data = newData;  // Swap backing store.  
    this._capacity = newCapacity;  // Update capacity to match new store.  
    return copied;  // Return copy count as resize cost.  
  }  // End of _resize.  

  append(value) {  // Append one element and return a detailed step record.  
    const phiBefore = potential(this._size, this._capacity);  // Capture Φ before mutation.  
    const sizeBefore = this._size;  // Capture size before insertion.  
    const capacityBefore = this._capacity;  // Capture capacity before possible resize.  

    let copied = 0;  // Default: no resize means zero copies.  
    let actualCost = 1;  // Always pay 1 for writing the new element.  
    if (this._size === this._capacity) {  // Resize exactly when full (doubling rule).  
      copied = this._resize(this._capacity * 2);  // Resize and count copies.  
      actualCost += copied;  // Add copy cost to this operation's actual cost.  
    }  // Close resize branch.  

    this._data[this._size] = value;  // Write the new element to the next slot.  
    this._size += 1;  // Increase logical size after successful write.  

    const phiAfter = potential(this._size, this._capacity);  // Capture Φ after mutation.  
    const amortizedCost = actualCost + (phiAfter - phiBefore);  // Compute amortized cost via potential method.  

    this._bank += 3 - actualCost;  // Accounting method: charge 3 and pay actual cost from credit.  
    if (this._bank < 0) {  // Credit should never be negative with this amortized charge.  
      throw new Error("bank went negative (accounting invariant violated)");  // Fail fast for debugging.  
    }  // Close invariant check.  

    this._totalActualCost += actualCost;  // Accumulate total actual cost.  
    this._totalCopies += copied;  // Accumulate total copies.  
    this._maxActualCost = Math.max(this._maxActualCost, actualCost);  // Track maximum per-op cost.  

    const step = {  // Create a plain object step record (simple and serializable).  
      index: this._steps.length + 1,  // 1-based operation index.  
      sizeBefore: sizeBefore,  // Size before.  
      capacityBefore: capacityBefore,  // Capacity before.  
      copied: copied,  // Copies performed in this op.  
      actualCost: actualCost,  // Actual cost.  
      phiBefore: phiBefore,  // Φ before.  
      phiAfter: phiAfter,  // Φ after.  
      amortizedCost: amortizedCost,  // Amortized cost via potential method (should be 3).  
      bankAfter: this._bank,  // Bank after charging/paying.  
    };  // Close step object.  
    this._steps.push(step);  // Append step record to the log.  
    return step;  // Return step record to callers.  
  }  // End of append.  
}  // End of AmortizedDynamicArray.  

function simulateAppends(m) {  // Run m appends and return a summary object.  
  if (!Number.isInteger(m) || m < 0) {  // Validate input so simulation is well-defined.  
    throw new RangeError("m must be an integer >= 0");  // Signal invalid input.  
  }  // Close validation block.  

  const table = new AmortizedDynamicArray();  // Use a fresh table for deterministic results.  
  for (let i = 0; i < m; i += 1) {  // Perform m appends.  
    table.append(i);  // Append a deterministic value.  
  }  // Close loop.  

  return {  // Return a compact summary for printing/tests.  
    m: m,  // Number of operations.  
    finalSize: table.size,  // Final size.  
    finalCapacity: table.capacity,  // Final capacity.  
    totalActualCost: table.totalActualCost,  // Total actual cost.  
    totalCopies: table.totalCopies,  // Total copies.  
    maxActualCost: table.maxActualCost,  // Max per-op cost.  
    finalBank: table.bank,  // Final bank balance.  
  };  // Close summary object.  
}  // End of simulateAppends.  

function isPowerOfTwo(x) {  // Check whether x is a power of two.  
  return Number.isInteger(x) && x > 0 && (x & (x - 1)) === 0;  // Use bit logic for power-of-two detection.  
}  // End of isPowerOfTwo.  

function parseMs(argv) {  // Parse CLI args into a list of m values.  
  if (argv.length === 0) {  // Use defaults when no args are provided.  
    return [1, 2, 4, 8, 16, 32];  // Default sizes match other units for consistency.  
  }  // Close default branch.  
  return argv.map((s) => Number.parseInt(s, 10));  // Parse each token as an integer.  
}  // End of parseMs.  

function formatTable(summaries) {  // Format summaries into a fixed-width ASCII table.  
  const header = `${"m".padStart(6)} | ${"total".padStart(8)} | ${"avg".padStart(6)} | ${"max".padStart(6)} | ${"cap".padStart(6)} | ${"copies".padStart(7)} | ${"bank".padStart(6)}`;  // Header line.  
  const separator = "-".repeat(header.length);  // Separator line.  
  const lines = [header, separator];  // Start with header + separator.  
  for (const s of summaries) {  // Render one row per summary.  
    const avg = s.m > 0 ? s.totalActualCost / s.m : 0;  // Compute average safely for m=0.  
    lines.push(  // Append aligned row.  
      `${String(s.m).padStart(6)} | ${String(s.totalActualCost).padStart(8)} | ${avg.toFixed(2).padStart(6)} | ${String(s.maxActualCost).padStart(6)} | ${String(s.finalCapacity).padStart(6)} | ${String(s.totalCopies).padStart(7)} | ${String(s.finalBank).padStart(6)}`  // Row string.  
    );  // Close push call.  
  }  // Close row loop.  
  return lines.join("\n");  // Join lines with newlines for printing.  
}  // End of formatTable.  

function main(argv) {  // CLI entry point.  
  const ms = parseMs(argv);  // Determine which m values to simulate.  
  const summaries = [];  // Accumulate summary rows.  
  for (const m of ms) {  // Simulate each m.  
    const s = simulateAppends(m);  // Run simulation and collect summary.  
    if (s.finalSize !== m) {  // Sanity check size invariant.  
      throw new Error("size mismatch (internal bug)");  // Fail fast.  
    }  // Close check.  
    if (!isPowerOfTwo(s.finalCapacity)) {  // Validate power-of-two capacity invariant.  
      throw new Error("capacity not power-of-two (internal bug)");  // Fail fast.  
    }  // Close check.  
    if (s.finalCapacity < Math.max(1, m)) {  // Validate capacity bound.  
      throw new Error("capacity too small (internal bug)");  // Fail fast.  
    }  // Close check.  
    summaries.push(s);  // Store summary for printing.  
  }  // Close loop.  
  console.log(formatTable(summaries));  // Print the formatted table to stdout.  
  return 0;  // Return a success code for consistency with other languages.  
}  // End of main.  

module.exports = {  // Export functions/classes for tests and reuse.  
  potential: potential,  // Export potential function.  
  AmortizedDynamicArray: AmortizedDynamicArray,  // Export dynamic array class.  
  simulateAppends: simulateAppends,  // Export simulator function.  
  isPowerOfTwo: isPowerOfTwo,  // Export power-of-two helper.  
};  // End of exports.  

if (require.main === module) {  // Run the CLI only when executed directly.  
  process.exitCode = main(process.argv.slice(2));  // Use Node's exitCode so we can still flush console output.  
}  // Close main guard.  

