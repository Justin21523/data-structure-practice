// 01 靜態陣列示範（JavaScript）/ Static array demo (JavaScript).  // Bilingual file header.
// We implement a fixed-capacity array and count "moved elements" during shifts to make O(n) visible.  // English overview.

"use strict";  // Enable strict mode to avoid accidental globals.  

class FixedArray {  // A fixed-capacity array with manual shifting (teaching-oriented).  
    constructor(capacity) {  // Create an empty fixed array with a given capacity.  
        if (!Number.isInteger(capacity) || capacity < 0) {  // Validate that capacity is a non-negative integer.  
            throw new RangeError("capacity must be an integer >= 0");  // Signal invalid input.  
        }  // Close validation block.  
        this._capacity = capacity;  // Store fixed capacity for overflow checks.  
        this._size = 0;  // Track the number of valid elements stored.  
        this._data = new Array(capacity);  // Pre-allocate backing storage (conceptually contiguous).  
    }  // Close constructor.  

    get capacity() {  // Expose capacity for callers/tests.  
        return this._capacity;  // Return fixed capacity.  
    }  // End capacity getter.  

    get size() {  // Expose size for callers/tests.  
        return this._size;  // Return current size.  
    }  // End size getter.  

    _requireIndexInRange(index) {  // Validate index in [0, size-1].  
        if (!Number.isInteger(index) || index < 0 || index >= this._size) {  // Reject invalid indices for access/remove.  
            throw new RangeError("index out of range");  // Signal invalid index.  
        }  // Close validation block.  
    }  // End _requireIndexInRange.  

    _requireInsertIndexInRange(index) {  // Validate insert index in [0, size].  
        if (!Number.isInteger(index) || index < 0 || index > this._size) {  // Reject invalid indices for insert.  
            throw new RangeError("index out of range for insert");  // Signal invalid index.  
        }  // Close validation block.  
    }  // End _requireInsertIndexInRange.  

    _requireNotFull() {  // Ensure there is at least one free slot.  
        if (this._size >= this._capacity) {  // Detect overflow when full.  
            throw new Error("fixed array is full");  // Signal overflow condition.  
        }  // Close validation block.  
    }  // End _requireNotFull.  

    get(index) {  // Return element at index (O(1)).  
        this._requireIndexInRange(index);  // Validate index first.  
        return this._data[index];  // Read from backing storage.  
    }  // End get.  

    set(index, value) {  // Set element at index (O(1)).  
        this._requireIndexInRange(index);  // Validate index first.  
        this._data[index] = value;  // Write to backing storage.  
    }  // End set.  

    toArray() {  // Return a copy of the used portion as a normal JS array.  
        const result = [];  // Build result array.  
        for (let i = 0; i < this._size; i += 1) {  // Copy only used elements.  
            result.push(this._data[i]);  // Append element.  
        }  // Close loop.  
        return result;  // Return copy so callers cannot mutate internal storage.  
    }  // End toArray.  

    indexOf(value) {  // Return first index of value or -1 (O(n)).  
        for (let i = 0; i < this._size; i += 1) {  // Linear scan.  
            if (this._data[i] === value) {  // Compare with strict equality.  
                return i;  // Return first match.  
            }  // Close match check.  
        }  // Close loop.  
        return -1;  // Signal not found.  
    }  // End indexOf.  

    append(value) {  // Append at end (O(1) if not full) and return moved count.  
        return this.insertAt(this._size, value);  // Reuse insertAt so behavior is consistent (moves will be 0).  
    }  // End append.  

    insertAt(index, value) {  // Insert value at index and return moved elements count.  
        this._requireNotFull();  // Ensure free space exists.  
        this._requireInsertIndexInRange(index);  // Validate insertion index.  

        const moved = this._size - index;  // Shifting right moves exactly (size - index) elements.  
        for (let i = this._size; i > index; i -= 1) {  // Shift right from tail toward index.  
            this._data[i] = this._data[i - 1];  // Move one element one step to the right.  
        }  // Close shift loop.  
        this._data[index] = value;  // Write new value into the gap.  
        this._size += 1;  // Increase size after insertion.  
        return moved;  // Return moved count so learners can see O(n) cost.  
    }  // End insertAt.  

    removeAt(index) {  // Remove element at index and return removed value + moved count.  
        this._requireIndexInRange(index);  // Validate index.  

        const removedValue = this._data[index];  // Capture removed value before shifting.  
        const moved = this._size - index - 1;  // Shifting left moves exactly (size - index - 1) elements.  
        for (let i = index; i < this._size - 1; i += 1) {  // Shift left to fill removed slot.  
            this._data[i] = this._data[i + 1];  // Move one element one step to the left.  
        }  // Close shift loop.  
        if (this._size > 0) {  // Guard against size=0 (though removal requires size>0).  
            this._data[this._size - 1] = undefined;  // Clear last slot to keep state tidy.  
        }  // Close clear branch.  
        this._size -= 1;  // Decrease size after removal.  
        return { value: removedValue, moved: moved };  // Return both removed value and move count.  
    }  // End removeAt.  
}  // End FixedArray.  

function buildFilledArray(n, extraCapacity) {  // Build an array filled with 0..n-1.  
    if (!Number.isInteger(n) || n < 0) {  // Validate size parameter.  
        throw new RangeError("n must be an integer >= 0");  // Signal invalid input.  
    }  // Close validation block.  
    const a = new FixedArray(n + extraCapacity);  // Allocate enough capacity to allow insertion when needed.  
    for (let i = 0; i < n; i += 1) {  // Fill with deterministic values.  
        a.append(i);  // Append one value.  
    }  // Close loop.  
    return a;  // Return filled array.  
}  // End buildFilledArray.  

function simulateInsertMoves(n, index) {  // Simulate insert and return moved count.  
    const a = buildFilledArray(n, 1);  // Build size n with one extra slot.  
    return a.insertAt(index, 999);  // Insert sentinel and return moved count.  
}  // End simulateInsertMoves.  

function simulateRemoveMoves(n, index) {  // Simulate remove and return moved count.  
    const a = buildFilledArray(n, 0);  // Build size n exactly.  
    return a.removeAt(index).moved;  // Remove and return moved count.  
}  // End simulateRemoveMoves.  

function parseNs(argv) {  // Parse CLI args into a list of n values.  
    if (argv.length === 0) {  // Use defaults when no args are provided.  
        return [0, 1, 2, 4, 8, 16];  // Include n=0 boundary case.  
    }  // Close default branch.  
    return argv.map((s) => Number.parseInt(s, 10));  // Parse each token as integer.  
}  // End parseNs.  

function formatTable(ns) {  // Format move counts into a fixed-width ASCII table.  
    const header = `${"n".padStart(6)} | ${"ins@0".padStart(6)} | ${"ins@mid".padStart(7)} | ${"ins@end".padStart(7)} | ${"rm@0".padStart(6)} | ${"rm@mid".padStart(7)} | ${"rm@end".padStart(7)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  

    for (const n of ns) {  // Render one row per n.  
        if (!Number.isInteger(n) || n < 0) {  // Validate n values.  
            throw new RangeError("n must be an integer >= 0");  // Signal invalid input.  
        }  // Close validation.  
        const mid = Math.floor(n / 2);  // Choose deterministic middle index.  

        const ins0 = simulateInsertMoves(n, 0);  // Moves for head insert.  
        const insm = simulateInsertMoves(n, mid);  // Moves for middle insert.  
        const inse = simulateInsertMoves(n, n);  // Moves for tail insert.  

        let rm0 = "n/a";  // Default to not applicable for n=0.  
        let rmm = "n/a";  // Default to not applicable for n=0.  
        let rme = "n/a";  // Default to not applicable for n=0.  
        if (n > 0) {  // Only compute removes when n is positive.  
            rm0 = String(simulateRemoveMoves(n, 0));  // Moves for head removal.  
            rmm = String(simulateRemoveMoves(n, mid));  // Moves for middle removal.  
            rme = String(simulateRemoveMoves(n, n - 1));  // Moves for tail removal.  
        }  // Close branch.  

        lines.push(  // Append aligned row.  
            `${String(n).padStart(6)} | ${String(ins0).padStart(6)} | ${String(insm).padStart(7)} | ${String(inse).padStart(7)} | ${rm0.padStart(6)} | ${rmm.padStart(7)} | ${rme.padStart(7)}`  // Row string.  
        );  // Close push.  
    }  // Close loop.  

    return lines.join("\n");  // Join lines into printable output.  
}  // End formatTable.  

function main(argv) {  // CLI entry point.  
    const ns = parseNs(argv);  // Determine n values.  
    console.log(formatTable(ns));  // Print move-count table.  
    return 0;  // Return success exit code.  
}  // End main.  

module.exports = {  // Export public API for tests.  
    FixedArray: FixedArray,  // Export FixedArray class.  
    buildFilledArray: buildFilledArray,  // Export builder helper.  
    simulateInsertMoves: simulateInsertMoves,  // Export insert simulation.  
    simulateRemoveMoves: simulateRemoveMoves,  // Export remove simulation.  
};  // End exports.  

if (require.main === module) {  // Run CLI only when executed directly.  
    process.exitCode = main(process.argv.slice(2));  // Set exit code for shell integration.  
}  // Close main guard.  
