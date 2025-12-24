// 01 靜態陣列示範（Java）/ Static array demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to collect demo sizes (n values).  
import java.util.List;  // Use List for type clarity and consistency.  

public final class FixedArrayDemo {  // Provide a demo + a small fixed-capacity array implementation.  
    private FixedArrayDemo() {  // Prevent instantiation because this class is a container for static logic.  
        // No instances.  // Clarify the intent of the private constructor.  
    }  // Close constructor.  

    public static final class RemoveResult {  // Return both removed value and moved elements count.  
        public final int value;  // The removed element value.  
        public final int moved;  // How many elements were shifted left after removal.  

        public RemoveResult(int value, int moved) {  // Construct an immutable result record.  
            this.value = value;  // Store removed value.  
            this.moved = moved;  // Store moved count.  
        }  // Close constructor.  
    }  // End of RemoveResult.  

    public static final class FixedArray {  // A fixed-capacity array with manual shifting (teaching-oriented).  
        private final int capacity;  // Store fixed capacity for overflow checks.  
        private int size;  // Track how many elements are currently stored.  
        private final int[] data;  // Backing storage for contiguous-array behavior.  

        public FixedArray(int capacity) {  // Create an empty fixed array with a given capacity.  
            if (capacity < 0) {  // Reject negative capacity.  
                throw new IllegalArgumentException("capacity must be >= 0");  // Signal invalid input.  
            }  // Close validation block.  
            this.capacity = capacity;  // Store capacity.  
            this.size = 0;  // Start empty.  
            this.data = new int[capacity];  // Allocate contiguous backing array.  
        }  // End constructor.  

        public int capacity() {  // Expose capacity for callers/tests.  
            return this.capacity;  // Return fixed capacity.  
        }  // End capacity().  

        public int size() {  // Expose size for callers/tests.  
            return this.size;  // Return current size.  
        }  // End size().  

        private void requireIndexInRange(int index) {  // Validate index in [0, size-1].  
            if (index < 0 || index >= this.size) {  // Reject invalid indices for element access.  
                throw new IndexOutOfBoundsException("index out of range");  // Signal invalid index.  
            }  // Close validation block.  
        }  // End requireIndexInRange.  

        private void requireInsertIndexInRange(int index) {  // Validate index in [0, size] for insertion.  
            if (index < 0 || index > this.size) {  // Reject invalid indices for insertion.  
                throw new IndexOutOfBoundsException("index out of range for insert");  // Signal invalid index.  
            }  // Close validation block.  
        }  // End requireInsertIndexInRange.  

        private void requireNotFull() {  // Ensure there is at least one free slot.  
            if (this.size >= this.capacity) {  // Detect overflow when full.  
                throw new IllegalStateException("fixed array is full");  // Signal overflow condition.  
            }  // Close validation block.  
        }  // End requireNotFull.  

        public int get(int index) {  // Return element at index (O(1)).  
            requireIndexInRange(index);  // Validate index.  
            return this.data[index];  // Read from contiguous array storage.  
        }  // End get.  

        public void set(int index, int value) {  // Set element at index (O(1)).  
            requireIndexInRange(index);  // Validate index.  
            this.data[index] = value;  // Write into contiguous array storage.  
        }  // End set.  

        public int[] toArray() {  // Return a copy of the used portion as a normal Java array.  
            int[] result = new int[this.size];  // Allocate output array sized to used portion.  
            for (int i = 0; i < this.size; i++) {  // Copy elements from the backing store.  
                result[i] = this.data[i];  // Copy one element.  
            }  // Close copy loop.  
            return result;  // Return copy so callers cannot mutate internal storage.  
        }  // End toArray.  

        public int indexOf(int value) {  // Return first index of value or -1 (O(n) linear search).  
            for (int i = 0; i < this.size; i++) {  // Scan from left to right.  
                if (this.data[i] == value) {  // Compare each element with target.  
                    return i;  // Return first matching index.  
                }  // Close match check.  
            }  // Close search loop.  
            return -1;  // Signal not found.  
        }  // End indexOf.  

        public int append(int value) {  // Append at end (O(1) if not full) and return moved count.  
            return insertAt(this.size, value);  // Reuse insertAt so behavior is consistent (moves will be 0).  
        }  // End append.  

        public int insertAt(int index, int value) {  // Insert value at index and return moved elements count.  
            requireNotFull();  // Ensure free space exists.  
            requireInsertIndexInRange(index);  // Validate insert index.  

            int moved = this.size - index;  // Moving right shifts exactly (size - index) elements.  
            for (int i = this.size; i > index; i--) {  // Shift right from tail toward index.  
                this.data[i] = this.data[i - 1];  // Move one element one step to the right.  
            }  // Close shift loop.  
            this.data[index] = value;  // Write new value into the gap.  
            this.size += 1;  // Increase size after insertion.  
            return moved;  // Return moved count so learners can see O(n) cost.  
        }  // End insertAt.  

        public RemoveResult removeAt(int index) {  // Remove element at index and return removed value + moved count.  
            requireIndexInRange(index);  // Validate index.  

            int removedValue = this.data[index];  // Capture removed value before shifting.  
            int moved = this.size - index - 1;  // Moving left shifts exactly (size - index - 1) elements.  
            for (int i = index; i < this.size - 1; i++) {  // Shift left to fill the removed slot.  
                this.data[i] = this.data[i + 1];  // Move one element one step to the left.  
            }  // Close shift loop.  
            this.size -= 1;  // Decrease size after removal.  
            return new RemoveResult(removedValue, moved);  // Return removed value and move count.  
        }  // End removeAt.  
    }  // End of FixedArray.  

    private static FixedArray buildFilledArray(int n, int extraCapacity) {  // Build array filled with 0..n-1.  
        if (n < 0) {  // Reject invalid sizes early.  
            throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
        }  // Close validation block.  
        FixedArray a = new FixedArray(n + extraCapacity);  // Allocate capacity to allow insertion when needed.  
        for (int i = 0; i < n; i++) {  // Fill with deterministic values.  
            a.append(i);  // Append one value.  
        }  // Close fill loop.  
        return a;  // Return filled array.  
    }  // End of buildFilledArray.  

    public static int simulateInsertMoves(int n, int index) {  // Simulate insert and return moved count.  
        FixedArray a = buildFilledArray(n, 1);  // Build size n with one extra slot.  
        return a.insertAt(index, 999);  // Insert sentinel and return moved elements count.  
    }  // End simulateInsertMoves.  

    public static int simulateRemoveMoves(int n, int index) {  // Simulate remove and return moved count.  
        FixedArray a = buildFilledArray(n, 0);  // Build size n exactly.  
        return a.removeAt(index).moved;  // Remove and return moved count.  
    }  // End simulateRemoveMoves.  

    private static List<Integer> parseNs(String[] args) {  // Parse CLI args into a list of n values.  
        List<Integer> ns = new ArrayList<>();  // Accumulate n values.  
        if (args.length == 0) {  // Use defaults when no args are provided.  
            ns.add(0);  // Include boundary case n=0.  
            ns.add(1);  // Add n=1.  
            ns.add(2);  // Add n=2.  
            ns.add(4);  // Add n=4.  
            ns.add(8);  // Add n=8.  
            ns.add(16);  // Add n=16.  
            return ns;  // Return defaults.  
        }  // Close default branch.  
        for (String raw : args) {  // Parse each CLI token.  
            ns.add(Integer.parseInt(raw));  // Convert token to int.  
        }  // Close parse loop.  
        return ns;  // Return parsed list.  
    }  // End parseNs.  

    private static String formatTable(List<Integer> ns) {  // Format move counts into a fixed-width table.  
        String header = String.format("%6s | %6s | %7s | %7s | %6s | %7s | %7s", "n", "ins@0", "ins@mid", "ins@end", "rm@0", "rm@mid", "rm@end");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header line.  
        sb.append(separator).append('\n');  // Append separator line.  

        for (int n : ns) {  // Render one row per n.  
            if (n < 0) {  // Reject invalid n values early.  
                throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
            }  // Close validation block.  
            int mid = n / 2;  // Choose deterministic middle index.  

            int ins0 = simulateInsertMoves(n, 0);  // Moves for head insert.  
            int insm = simulateInsertMoves(n, mid);  // Moves for middle insert.  
            int inse = simulateInsertMoves(n, n);  // Moves for tail insert.  

            String rm0;  // Store remove@0 field as a string (needs "n/a" for n=0).  
            String rmm;  // Store remove@mid field as a string.  
            String rme;  // Store remove@end field as a string.  
            if (n == 0) {  // Removal is not defined for empty arrays.  
                rm0 = "n/a";  // Mark as not applicable.  
                rmm = "n/a";  // Mark as not applicable.  
                rme = "n/a";  // Mark as not applicable.  
            } else {  // For n>0 we can remove at valid indices.  
                rm0 = Integer.toString(simulateRemoveMoves(n, 0));  // Moves for head removal.  
                rmm = Integer.toString(simulateRemoveMoves(n, mid));  // Moves for middle removal.  
                rme = Integer.toString(simulateRemoveMoves(n, n - 1));  // Moves for tail removal.  
            }  // Close branch.  

            sb.append(String.format("%6d | %6d | %7d | %7d | %6s | %7s | %7s", n, ins0, insm, inse, rm0, rmm, rme)).append('\n');  // Append aligned row.  
        }  // Close row loop.  

        return sb.toString();  // Return full table string.  
    }  // End formatTable.  

    public static void main(String[] args) {  // CLI entry point that prints move-count table.  
        List<Integer> ns = parseNs(args);  // Determine which n values to show.  
        System.out.print(formatTable(ns));  // Print the table to stdout.  
    }  // End main.  
}  // End FixedArrayDemo.  

