// 02 動態陣列示範（JavaScript）/ Dynamic array demo (JavaScript).  // Bilingual file header.
// We implement a dynamic array with doubling growth and count deterministic costs (copies + shifts).  // English overview.

"use strict";  // Enable strict mode to avoid accidental globals.  

class DynamicArray {  // A simple dynamic array with doubling growth (teaching-oriented).  
    constructor() {  // Initialize an empty array with capacity 1.  
        this._size = 0;  // Track number of stored elements.  
        this._capacity = 1;  // Start with capacity 1 for deterministic doubling.  
        this._data = new Array(this._capacity);  // Allocate backing buffer.  
        this._totalCopies = 0;  // Accumulate total copied elements across all resizes.  
    }  // Close constructor.  

    get size() {  // Expose size for callers/tests.  
        return this._size;  // Return current size.  
    }  // End size getter.  

    get capacity() {  // Expose capacity for callers/tests.  
        return this._capacity;  // Return current capacity.  
    }  // End capacity getter.  

    get totalCopies() {  // Expose total resize copies for analysis/tests.  
        return this._totalCopies;  // Return total copied elements.  
    }  // End totalCopies getter.  

    _requireIndexInRange(index) {  // Validate index in [0, size-1].  
        if (!Number.isInteger(index) || index < 0 || index >= this._size) {  // Reject invalid indices for access/remove.  
            throw new RangeError("index out of range");  // Signal invalid index.  
        }  // Close validation.  
    }  // End _requireIndexInRange.  

    _requireInsertIndexInRange(index) {  // Validate insert index in [0, size].  
        if (!Number.isInteger(index) || index < 0 || index > this._size) {  // Reject invalid insertion indices.  
            throw new RangeError("index out of range for insert");  // Signal invalid index.  
        }  // Close validation.  
    }  // End _requireInsertIndexInRange.  

    _resize(newCapacity) {  // Resize backing buffer and return how many elements were copied.  
        if (!Number.isInteger(newCapacity) || newCapacity < this._size) {  // Ensure new capacity can hold existing elements.  
            throw new RangeError("newCapacity must be an integer >= size");  // Signal invalid request.  
        }  // Close validation.  
        if (newCapacity < 1) {  // Keep capacity positive.  
            throw new RangeError("newCapacity must be >= 1");  // Signal invalid request.  
        }  // Close validation.  

        const newData = new Array(newCapacity);  // Allocate new buffer.  
        let copied = 0;  // Count copied elements deterministically.  
        for (let i = 0; i < this._size; i += 1) {  // Copy used portion.  
            newData[i] = this._data[i];  // Copy one element.  
            copied += 1;  // Count one copy.  
        }  // Close loop.  
        this._data = newData;  // Swap buffer.  
        this._capacity = newCapacity;  // Update capacity.  
        this._totalCopies += copied;  // Accumulate total copies.  
        return copied;  // Return copy count for this resize.  
    }  // End _resize.  

    _ensureCapacityForOneMore() {  // Ensure capacity for one more element; return resize copies.  
        if (this._size < this._capacity) {  // Fast path: free space exists.  
            return 0;  // No resize needed.  
        }  // Close fast path.  
        return this._resize(this._capacity * 2);  // Double capacity and return copies count.  
    }  // End _ensureCapacityForOneMore.  

    get(index) {  // Return element at index (O(1)).  
        this._requireIndexInRange(index);  // Validate index.  
        return this._data[index];  // Read from backing buffer.  
    }  // End get.  

    set(index, value) {  // Set element at index (O(1)).  
        this._requireIndexInRange(index);  // Validate index.  
        this._data[index] = value;  // Write to backing buffer.  
    }  // End set.  

    toArray() {  // Return a copy of the used portion as a normal JS array.  
        const result = [];  // Build result array.  
        for (let i = 0; i < this._size; i += 1) {  // Copy used portion.  
            result.push(this._data[i]);  // Append one element.  
        }  // Close loop.  
        return result;  // Return copy.  
    }  // End toArray.  

    indexOf(value) {  // Return first index of value or -1 (O(n)).  
        for (let i = 0; i < this._size; i += 1) {  // Linear scan.  
            if (this._data[i] === value) {  // Compare with strict equality.  
                return i;  // Return first match.  
            }  // Close match check.  
        }  // Close loop.  
        return -1;  // Not found sentinel.  
    }  // End indexOf.  

    append(value) {  // Append at end and return resize cost (moved=0).  
        const copied = this._ensureCapacityForOneMore();  // Resize if needed.  
        this._data[this._size] = value;  // Write new element.  
        this._size += 1;  // Increase size.  
        return { copied: copied, moved: 0 };  // Return per-operation cost record.  
    }  // End append.  

    insertAt(index, value) {  // Insert at index and return cost (copies + shifts).  
        this._requireInsertIndexInRange(index);  // Validate insertion index.  
        const copied = this._ensureCapacityForOneMore();  // Resize if needed before shifting.  

        const moved = this._size - index;  // Shifting right moves (size - index) elements.  
        for (let i = this._size; i > index; i -= 1) {  // Shift right from tail toward index.  
            this._data[i] = this._data[i - 1];  // Move one element right.  
        }  // Close loop.  
        this._data[index] = value;  // Write new value.  
        this._size += 1;  // Increase size.  
        return { copied: copied, moved: moved };  // Return cost record.  
    }  // End insertAt.  

    removeAt(index) {  // Remove at index and return removed value + shift cost.  
        this._requireIndexInRange(index);  // Validate index.  
        const removedValue = this._data[index];  // Capture removed value.  
        const moved = this._size - index - 1;  // Shifting left moves (size - index - 1) elements.  
        for (let i = index; i < this._size - 1; i += 1) {  // Shift left to fill gap.  
            this._data[i] = this._data[i + 1];  // Move one element left.  
        }  // Close loop.  
        this._size -= 1;  // Decrease size.  
        return { value: removedValue, cost: { copied: 0, moved: moved } };  // Return removed value and cost.  
    }  // End removeAt.  
}  // End DynamicArray.  

function isPowerOfTwo(x) {  // Helper for verifying doubling-capacity behavior.  
    return Number.isInteger(x) && x > 0 && (x & (x - 1)) === 0;  // Standard bit trick.  
}  // End isPowerOfTwo.  

function simulateAppends(m) {  // Simulate m appends and summarize growth/copy costs.  
    if (!Number.isInteger(m) || m < 0) {  // Validate input.  
        throw new RangeError("m must be an integer >= 0");  // Signal invalid input.  
    }  // Close validation.  
    const a = new DynamicArray();  // Fresh array for deterministic results.  
    let totalActualCost = 0;  // Accumulate total cost (1 write + copied).  
    let maxCopiedInOneOp = 0;  // Track maximum copies in a single append.  
    for (let i = 0; i < m; i += 1) {  // Perform m appends.  
        const cost = a.append(i);  // Append deterministic value.  
        totalActualCost += 1 + cost.copied;  // Add write + copy cost.  
        maxCopiedInOneOp = Math.max(maxCopiedInOneOp, cost.copied);  // Update max copied.  
    }  // Close loop.  
    return {  // Return summary object.  
        m: m,  // Store operation count.  
        finalSize: a.size,  // Store final size.  
        finalCapacity: a.capacity,  // Store final capacity.  
        totalCopies: a.totalCopies,  // Store total copies.  
        totalActualCost: totalActualCost,  // Store total actual cost.  
        maxCopiedInOneOp: maxCopiedInOneOp,  // Store max copied.  
    };  // Close summary.  
}  // End simulateAppends.  

function buildFilledArray(n) {  // Build a dynamic array filled with 0..n-1.  
    if (!Number.isInteger(n) || n < 0) {  // Validate n.  
        throw new RangeError("n must be an integer >= 0");  // Signal invalid input.  
    }  // Close validation.  
    const a = new DynamicArray();  // Start empty.  
    for (let i = 0; i < n; i += 1) {  // Fill with deterministic values.  
        a.append(i);  // Append one value.  
    }  // Close loop.  
    return a;  // Return filled array.  
}  // End buildFilledArray.  

function simulateAppendCostAtSize(n) {  // Build size n then append once and return cost.  
    const a = buildFilledArray(n);  // Build array.  
    return a.append(999);  // Append sentinel and return cost.  
}  // End simulateAppendCostAtSize.  

function simulateInsert0CostAtSize(n) {  // Build size n then insert at head and return cost.  
    const a = buildFilledArray(n);  // Build array.  
    return a.insertAt(0, 999);  // Insert sentinel at head.  
}  // End simulateInsert0CostAtSize.  

function parseMs(argv) {  // Parse CLI args into m values.  
    if (argv.length === 0) {  // Use defaults when no args.  
        return [0, 1, 2, 4, 8, 16, 32];  // Default m values.  
    }  // Close default branch.  
    return argv.map((s) => Number.parseInt(s, 10));  // Parse each token.  
}  // End parseMs.  

function formatAppendSummaryTable(ms) {  // Format growth summary table.  
    const header = `${"m".padStart(6)} | ${"cap".padStart(6)} | ${"copies".padStart(7)} | ${"avg".padStart(6)} | ${"maxCp".padStart(6)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const m of ms) {  // Render one row per m.  
        const s = simulateAppends(m);  // Simulate and summarize.  
        const avg = s.m > 0 ? s.totalActualCost / s.m : 0;  // Average actual cost per op.  
        lines.push(`${String(s.m).padStart(6)} | ${String(s.finalCapacity).padStart(6)} | ${String(s.totalCopies).padStart(7)} | ${avg.toFixed(2).padStart(6)} | ${String(s.maxCopiedInOneOp).padStart(6)}`);  // Row string.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatAppendSummaryTable.  

function formatAppendVsInsert0Table() {  // Format append vs insertAt(0) comparison for small n values.  
    const ns = [0, 1, 2, 4, 8, 16];  // Fixed list for readability.  
    const header = `${"n".padStart(6)} | ${"appCp".padStart(6)} | ${"appMv".padStart(6)} | ${"ins0Cp".padStart(7)} | ${"ins0Mv".padStart(7)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const n of ns) {  // Render one row per n.  
        const app = simulateAppendCostAtSize(n);  // Cost of append at size n.  
        const ins0 = simulateInsert0CostAtSize(n);  // Cost of insertAt(0) at size n.  
        lines.push(`${String(n).padStart(6)} | ${String(app.copied).padStart(6)} | ${String(app.moved).padStart(6)} | ${String(ins0.copied).padStart(7)} | ${String(ins0.moved).padStart(7)}`);  // Row string.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatAppendVsInsert0Table.  

function main(argv) {  // CLI entry point.  
    const ms = parseMs(argv);  // Determine m values.  
    console.log("=== Append Growth (m appends) ===");  // Print section title.  
    console.log(formatAppendSummaryTable(ms));  // Print summary table.  
    console.log("");  // Print blank line between sections.  
    console.log("=== Append vs insertAt(0) at size n ===");  // Print section title.  
    console.log(formatAppendVsInsert0Table());  // Print comparison table.  
    return 0;  // Return success code.  
}  // End main.  

module.exports = {  // Export API for tests.  
    DynamicArray: DynamicArray,  // Export DynamicArray class.  
    isPowerOfTwo: isPowerOfTwo,  // Export helper.  
    simulateAppends: simulateAppends,  // Export simulator.  
    buildFilledArray: buildFilledArray,  // Export builder.  
    simulateAppendCostAtSize: simulateAppendCostAtSize,  // Export append cost helper.  
    simulateInsert0CostAtSize: simulateInsert0CostAtSize,  // Export insert@0 cost helper.  
};  // End exports.  

if (require.main === module) {  // Run CLI only when executed directly.  
    process.exitCode = main(process.argv.slice(2));  // Set exit code for shell integration.  
}  // Close main guard.  

