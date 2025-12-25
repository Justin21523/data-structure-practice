// 04 雙端佇列示範（JavaScript）/ Deque demo (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer, more explicit JavaScript.

class Deque {  // A circular-buffer deque with doubling growth (teaching-oriented).
    constructor() {  // Initialize an empty deque with capacity 1.
        this._size = 0;  // Track number of stored elements.
        this._capacity = 1;  // Start with capacity 1 for deterministic doubling.
        this._head = 0;  // Head index (front position) within the buffer.
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

    _indexAt(offset) {  // Map logical offset [0..size) to physical buffer index.
        return (this._head + offset) % this._capacity;  // Wrap around using modulo.
    }  // End _indexAt().

    toArray() {  // Return a copy of the used portion (front -> back).
        const out = [];  // Accumulate values in order.
        for (let i = 0; i < this._size; i += 1) {  // Copy exactly size elements in deque order.
            out.push(this._data[this._indexAt(i)]);  // Append one element.
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
        for (let i = 0; i < this._size; i += 1) {  // Copy elements in deque order.
            newData[i] = this._data[this._indexAt(i)];  // Copy one element into compact layout.
            copied += 1;  // Count one copy.
        }  // Close copy loop.
        this._data = newData;  // Swap buffer reference.
        this._capacity = newCapacity;  // Update capacity.
        this._head = 0;  // Reset head so deque starts at index 0 after resize.
        this._totalCopies += copied;  // Accumulate total copies.
        return copied;  // Return copies performed for this resize.
    }  // End _resize().

    _ensureCapacityForOneMore() {  // Ensure capacity for one more push; return resize copies.
        if (this._size < this._capacity) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return this._resize(this._capacity * 2);  // Double capacity and return copies.
    }  // End _ensureCapacityForOneMore().

    pushBack(value) {  // Push at back (amortized O(1)).
        const copied = this._ensureCapacityForOneMore();  // Resize if needed.
        const tailIndex = this._indexAt(this._size);  // Compute tail index where new element is written.
        this._data[tailIndex] = value;  // Store value at tail position.
        this._size += 1;  // Increase size.
        return { copied, moved: 0 };  // moved stays 0 (no shifting).
    }  // End pushBack().

    pushFront(value) {  // Push at front (amortized O(1)).
        const copied = this._ensureCapacityForOneMore();  // Resize if needed.
        this._head = (this._head - 1 + this._capacity) % this._capacity;  // Move head left by one with wrap-around.
        this._data[this._head] = value;  // Store value at new head position.
        this._size += 1;  // Increase size.
        return { copied, moved: 0 };  // moved stays 0 (no shifting).
    }  // End pushFront().

    peekFront() {  // Return front value without removing it (O(1)).
        if (this._size === 0) {  // Reject peeking an empty deque.
            throw new RangeError("peek from empty deque");  // Signal invalid operation.
        }  // Close validation.
        return this._data[this._head];  // Return front slot directly.
    }  // End peekFront().

    peekBack() {  // Return back value without removing it (O(1)).
        if (this._size === 0) {  // Reject peeking an empty deque.
            throw new RangeError("peek from empty deque");  // Signal invalid operation.
        }  // Close validation.
        const tailIndex = this._indexAt(this._size - 1);  // Compute index of last element.
        return this._data[tailIndex];  // Return back slot.
    }  // End peekBack().

    popFront() {  // Pop from front (O(1); no shifting).
        if (this._size === 0) {  // Reject popping an empty deque.
            throw new RangeError("pop from empty deque");  // Signal invalid operation.
        }  // Close validation.
        const value = this._data[this._head];  // Capture front value.
        this._data[this._head] = undefined;  // Clear slot for debug friendliness.
        this._head = (this._head + 1) % this._capacity;  // Advance head by one with wrap-around.
        this._size -= 1;  // Decrease size.
        if (this._size === 0) {  // Normalize head for deterministic state when deque becomes empty.
            this._head = 0;  // Reset head (optional but stable).
        }  // Close normalize branch.
        return { value, cost: { copied: 0, moved: 0 } };  // moved stays 0 (no shift).
    }  // End popFront().

    popBack() {  // Pop from back (O(1); no shifting).
        if (this._size === 0) {  // Reject popping an empty deque.
            throw new RangeError("pop from empty deque");  // Signal invalid operation.
        }  // Close validation.
        const tailIndex = this._indexAt(this._size - 1);  // Compute index of last element.
        const value = this._data[tailIndex];  // Capture back value.
        this._data[tailIndex] = undefined;  // Clear slot for debug friendliness.
        this._size -= 1;  // Decrease size.
        if (this._size === 0) {  // Normalize head for deterministic state when deque becomes empty.
            this._head = 0;  // Reset head (optional but stable).
        }  // Close normalize branch.
        return { value, cost: { copied: 0, moved: 0 } };  // moved stays 0 (no shift).
    }  // End popBack().
}  // End Deque.

function simulatePushBacks(m) {  // Simulate m pushBack operations and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw new RangeError("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    const d = new Deque();  // Fresh deque for deterministic results.
    let totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    let maxCopied = 0;  // Track maximum copied elements in a single push.
    for (let i = 0; i < m; i += 1) {  // Perform m pushes.
        const cost = d.pushBack(i);  // Push deterministic value and get per-op cost.
        totalActualCost += 1 + cost.copied;  // Add write cost + resize copy cost.
        maxCopied = Math.max(maxCopied, cost.copied);  // Update maximum copied.
    }  // Close loop.
    return {  // Return a compact summary object.
        m,  // Store operation count.
        finalSize: d.size,  // Store final size.
        finalCapacity: d.capacity,  // Store final capacity.
        totalCopies: d.totalCopies,  // Store total copies.
        totalActualCost,  // Store total actual cost.
        maxCopiedInOneOp: maxCopied,  // Store max per-op copied.
    };  // Close summary.
}  // End simulatePushBacks().

function parseMsOrDefault(argv) {  // Parse CLI args into m values or use defaults.
    if (argv.length === 0) {  // Use defaults when no args are provided.
        return [0, 1, 2, 4, 8, 16, 32];  // Default m values.
    }  // Close default branch.
    return argv.map((x) => Number.parseInt(x, 10));  // Parse each token to int.
}  // End parseMsOrDefault().

function formatPushSummaryTable(ms) {  // Format push-growth summaries for multiple m values.
    const header = ["m", "cap", "copies", "avg", "maxCp"].map((s) => s.padStart(6)).join(" | ");  // Header line.
    const separator = "-".repeat(header.length);  // Separator line.
    const lines = [header, separator];  // Start with header + separator.
    for (const m of ms) {  // Render one row per m.
        const s = simulatePushBacks(m);  // Simulate and summarize.
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

function formatDequeDemo() {  // Show a small deque example using both ends.
    const d = new Deque();  // Create empty deque.
    d.pushBack(3);  // Push 3 to back.
    d.pushBack(7);  // Push 7 to back.
    d.pushFront(1);  // Push 1 to front (deque becomes [1,3,7]).
    const front = d.peekFront();  // Peek front value.
    const back = d.peekBack();  // Peek back value.
    const p1 = d.popFront().value;  // Pop front (should be 1).
    const p2 = d.popBack().value;  // Pop back (should be 7).
    const p3 = d.popBack().value;  // Pop back (should be 3).
    return `pushBack [3,7], pushFront [1] => peekFront=${front}, peekBack=${back}, pops=[${p1},${p2},${p3}]`;  // Return formatted demo line.
}  // End formatDequeDemo().

function main(argv) {  // CLI entry point for the demo.
    const ms = parseMsOrDefault(argv);  // Parse m values or use defaults.
    console.log("=== pushBack growth (m pushes) ===");  // Print section title.
    console.log(formatPushSummaryTable(ms));  // Print growth table.
    console.log("");  // Blank line.
    console.log("=== deque demo (both ends) ===");  // Print section title.
    console.log(formatDequeDemo());  // Print deque demo line.
}  // End main().

module.exports = {  // Export public API for tests and reuse.
    Deque,  // Export Deque class.
};  // Close exports.

if (require.main === module) {  // Execute demo only when run directly.
    main(process.argv.slice(2));  // Run main with CLI args.
}  // Close direct-run guard.

