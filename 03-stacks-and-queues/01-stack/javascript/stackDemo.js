// 01 堆疊示範（JavaScript）/ Stack demo (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer, more explicit JavaScript.

class ArrayStack {  // An array-backed stack with doubling growth (teaching-oriented).
    constructor() {  // Initialize an empty stack with capacity 1.
        this._size = 0;  // Track number of stored elements.
        this._capacity = 1;  // Start with capacity 1 for deterministic doubling.
        this._data = new Array(this._capacity);  // Allocate backing buffer.
        this._totalCopies = 0;  // Accumulate total copied elements across all resizes.
    }  // Close constructor.

    get size() {  // Expose current size for callers/tests.
        return this._size;  // Return number of stored items.
    }  // End size getter.

    get capacity() {  // Expose current capacity for callers/tests.
        return this._capacity;  // Return allocated slots.
    }  // End capacity getter.

    get totalCopies() {  // Expose total resize copies for analysis-style checks.
        return this._totalCopies;  // Return total copied elements.
    }  // End totalCopies getter.

    isEmpty() {  // Convenience helper to check emptiness.
        return this._size === 0;  // Empty iff size is zero.
    }  // End isEmpty().

    toArray() {  // Return a copy of the used portion (bottom -> top).
        const out = [];  // Accumulate values in order.
        for (let i = 0; i < this._size; i += 1) {  // Copy only the used elements.
            out.push(this._data[i]);  // Append one value.
        }  // Close loop.
        return out;  // Return copied array.
    }  // End toArray().

    _resize(newCapacity) {  // Resize buffer and return number of copied elements.
        if (newCapacity < this._size) {  // Ensure new buffer can hold all existing elements.
            throw new RangeError("newCapacity must be >= size");  // Signal invalid resize request.
        }  // Close validation.
        if (newCapacity < 1) {  // Keep capacity positive for simplicity.
            throw new RangeError("newCapacity must be >= 1");  // Signal invalid capacity.
        }  // Close validation.

        const newData = new Array(newCapacity);  // Allocate new buffer.
        let copied = 0;  // Count copied elements deterministically.
        for (let i = 0; i < this._size; i += 1) {  // Copy exactly the used portion.
            newData[i] = this._data[i];  // Copy one element.
            copied += 1;  // Count one copy.
        }  // Close copy loop.
        this._data = newData;  // Swap buffer reference.
        this._capacity = newCapacity;  // Update capacity.
        this._totalCopies += copied;  // Accumulate total copies.
        return copied;  // Return copies performed for this resize.
    }  // End _resize().

    _ensureCapacityForOneMore() {  // Ensure capacity for one more push; return resize copies.
        if (this._size < this._capacity) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return this._resize(this._capacity * 2);  // Double capacity and return copies.
    }  // End _ensureCapacityForOneMore().

    push(value) {  // Push to top (amortized O(1)).
        const copied = this._ensureCapacityForOneMore();  // Resize if needed.
        this._data[this._size] = value;  // Write new value at the top slot.
        this._size += 1;  // Increase size.
        return { copied };  // Return deterministic resize-copy cost.
    }  // End push().

    peek() {  // Return top value without removing it (O(1)).
        if (this._size === 0) {  // Reject peeking an empty stack.
            throw new RangeError("peek from empty stack");  // Signal invalid operation.
        }  // Close validation.
        return this._data[this._size - 1];  // Return top slot.
    }  // End peek().

    pop() {  // Pop top value (O(1) in this unit; no shrinking).
        if (this._size === 0) {  // Reject popping an empty stack.
            throw new RangeError("pop from empty stack");  // Signal invalid operation.
        }  // Close validation.
        this._size -= 1;  // Decrease size first so top index becomes size.
        const value = this._data[this._size];  // Read removed slot.
        this._data[this._size] = undefined;  // Clear slot for debug friendliness.
        return { value, cost: { copied: 0 } };  // Pop does not resize/copy in this unit.
    }  // End pop().
}  // End ArrayStack.

function simulatePushes(m) {  // Simulate m pushes and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw new RangeError("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    const s = new ArrayStack();  // Fresh stack for deterministic results.
    let totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    let maxCopied = 0;  // Track maximum copied elements in a single push.
    for (let i = 0; i < m; i += 1) {  // Perform m pushes.
        const cost = s.push(i);  // Push deterministic value and get per-op cost.
        totalActualCost += 1 + cost.copied;  // Add write cost + resize copy cost.
        maxCopied = Math.max(maxCopied, cost.copied);  // Update maximum copied.
    }  // Close loop.
    return {  // Return a compact summary object.
        m,  // Store operation count.
        finalSize: s.size,  // Store final size.
        finalCapacity: s.capacity,  // Store final capacity.
        totalCopies: s.totalCopies,  // Store total copies.
        totalActualCost,  // Store total actual cost.
        maxCopiedInOneOp: maxCopied,  // Store max per-op copied.
    };  // Close summary.
}  // End simulatePushes().

function parseMsOrDefault(argv) {  // Parse CLI args into m values or use defaults.
    if (argv.length === 0) {  // Use defaults when no args are provided.
        return [0, 1, 2, 4, 8, 16, 32];  // Default m values.
    }  // Close default branch.
    return argv.map((x) => Number.parseInt(x, 10));  // Parse each token to int.
}  // End parseMsOrDefault().

function formatPushSummaryTable(ms) {  // Format growth summaries for multiple m values.
    const header = ["m", "cap", "copies", "avg", "maxCp"].map((s) => s.padStart(6)).join(" | ");  // Header line.
    const separator = "-".repeat(header.length);  // Separator line.
    const lines = [header, separator];  // Start with header + separator.
    for (const m of ms) {  // Render one row per m.
        const s = simulatePushes(m);  // Simulate and summarize.
        const avg = s.m > 0 ? (s.totalActualCost / s.m) : 0.0;  // Average actual cost per op.
        lines.push(  // Append formatted row.
            String(s.m).padStart(6) + " | " +  // Column: m.
            String(s.finalCapacity).padStart(6) + " | " +  // Column: capacity.
            String(s.totalCopies).padStart(6) + " | " +  // Column: total copies.
            avg.toFixed(2).padStart(6) + " | " +  // Column: avg.
            String(s.maxCopiedInOneOp).padStart(6)  // Column: max copied.
        );  // Close push.
    }  // Close loop.
    return lines.join("\n");  // Join lines into printable table.
}  // End formatPushSummaryTable().

function formatLifoDemo() {  // Show a small LIFO example with peek/pop.
    const s = new ArrayStack();  // Create an empty stack.
    s.push(3);  // Push 3.
    s.push(7);  // Push 7.
    s.push(1);  // Push 1 (becomes top).
    const top = s.peek();  // Peek current top.
    const p1 = s.pop().value;  // Pop first (should be 1).
    const p2 = s.pop().value;  // Pop second (should be 7).
    const p3 = s.pop().value;  // Pop third (should be 3).
    return `push [3,7,1] => peek=${top}, pops=[${p1},${p2},${p3}]`;  // Return formatted demo line.
}  // End formatLifoDemo().

function main(argv) {  // CLI entry point for the demo.
    const ms = parseMsOrDefault(argv);  // Parse m values or use defaults.
    console.log("=== Push Growth (m pushes) ===");  // Print section title.
    console.log(formatPushSummaryTable(ms));  // Print table.
    console.log("");  // Blank line.
    console.log("=== LIFO demo ===");  // Print section title.
    console.log(formatLifoDemo());  // Print LIFO demo line.
}  // End main().

module.exports = {  // Export public API for tests and reuse.
    ArrayStack,  // Export ArrayStack class.
    simulatePushes,  // Export simulator helper.
};  // Close exports.

if (require.main === module) {  // Execute demo only when run directly.
    main(process.argv.slice(2));  // Run main with CLI args.
}  // Close direct-run guard.

