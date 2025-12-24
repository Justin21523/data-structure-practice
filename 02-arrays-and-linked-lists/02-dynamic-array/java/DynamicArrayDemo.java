// 02 動態陣列示範（Java）/ Dynamic array demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store demo size lists (m values).  
import java.util.List;  // Use List for type clarity and consistency.  

public final class DynamicArrayDemo {  // Provide a demo + a dynamic array implementation with explicit resizing.  
    private DynamicArrayDemo() {  // Prevent instantiation because this class is a container for static logic.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class OperationCost {  // Record per-operation copy/shift counts.  
        public final int copied;  // How many elements were copied due to resize (0 if no resize).  
        public final int moved;  // How many elements were shifted due to insert/remove (0 for append).  

        public OperationCost(int copied, int moved) {  // Construct an immutable cost record.  
            this.copied = copied;  // Store copied count.  
            this.moved = moved;  // Store moved count.  
        }  // Close constructor.  
    }  // End OperationCost.  

    public static final class RemoveResult {  // Return removed value plus its operation cost.  
        public final int value;  // The removed element value.  
        public final OperationCost cost;  // Copy/shift cost for this remove operation.  

        public RemoveResult(int value, OperationCost cost) {  // Construct an immutable remove result.  
            this.value = value;  // Store removed value.  
            this.cost = cost;  // Store cost record.  
        }  // Close constructor.  
    }  // End RemoveResult.  

    public static final class DynamicArray {  // A simple dynamic array with doubling growth (teaching-oriented).  
        private int size;  // Track number of stored elements.  
        private int capacity;  // Track current capacity (allocated slots).  
        private int[] data;  // Backing buffer for contiguous-array behavior.  
        private long totalCopies;  // Accumulate total copied elements across all resizes.  

        public DynamicArray() {  // Initialize empty array with capacity 1.  
            this.size = 0;  // Start with no elements.  
            this.capacity = 1;  // Start with capacity 1 for deterministic doubling.  
            this.data = new int[this.capacity];  // Allocate backing buffer.  
            this.totalCopies = 0;  // Start with zero total copies.  
        }  // Close constructor.  

        public int size() {  // Expose size for callers/tests.  
            return this.size;  // Return current size.  
        }  // End size().  

        public int capacity() {  // Expose capacity for callers/tests.  
            return this.capacity;  // Return current capacity.  
        }  // End capacity().  

        public long totalCopies() {  // Expose total resize copies for analysis/tests.  
            return this.totalCopies;  // Return total copied elements.  
        }  // End totalCopies().  

        private void requireIndexInRange(int index) {  // Validate index in [0, size-1].  
            if (index < 0 || index >= this.size) {  // Reject invalid indices for access/remove.  
                throw new IndexOutOfBoundsException("index out of range");  // Signal invalid index.  
            }  // Close validation.  
        }  // End requireIndexInRange.  

        private void requireInsertIndexInRange(int index) {  // Validate insert index in [0, size].  
            if (index < 0 || index > this.size) {  // Reject invalid insertion indices.  
                throw new IndexOutOfBoundsException("index out of range for insert");  // Signal invalid index.  
            }  // Close validation.  
        }  // End requireInsertIndexInRange.  

        private int resize(int newCapacity) {  // Resize backing buffer and return how many elements were copied.  
            if (newCapacity < this.size) {  // Ensure new capacity can hold all existing elements.  
                throw new IllegalArgumentException("newCapacity must be >= size");  // Signal invalid request.  
            }  // Close validation.  
            if (newCapacity < 1) {  // Keep capacity positive.  
                throw new IllegalArgumentException("newCapacity must be >= 1");  // Signal invalid request.  
            }  // Close validation.  

            int[] newData = new int[newCapacity];  // Allocate new buffer.  
            int copied = 0;  // Count copied elements deterministically.  
            for (int i = 0; i < this.size; i++) {  // Copy used portion.  
                newData[i] = this.data[i];  // Copy one element.  
                copied += 1;  // Count one copy.  
            }  // Close copy loop.  
            this.data = newData;  // Swap buffer.  
            this.capacity = newCapacity;  // Update capacity.  
            this.totalCopies += copied;  // Accumulate total copies.  
            return copied;  // Return copy count for this resize.  
        }  // End resize.  

        private int ensureCapacityForOneMore() {  // Ensure capacity for one more element; return resize copies.  
            if (this.size < this.capacity) {  // Fast path: there is free space.  
                return 0;  // No resize needed.  
            }  // Close fast path.  
            return resize(this.capacity * 2);  // Double capacity and return copies count.  
        }  // End ensureCapacityForOneMore.  

        public int get(int index) {  // Return element at index (O(1)).  
            requireIndexInRange(index);  // Validate index.  
            return this.data[index];  // Read from backing buffer.  
        }  // End get.  

        public void set(int index, int value) {  // Set element at index (O(1)).  
            requireIndexInRange(index);  // Validate index.  
            this.data[index] = value;  // Write to backing buffer.  
        }  // End set.  

        public int[] toArray() {  // Return a copy of the used portion as a normal Java array.  
            int[] result = new int[this.size];  // Allocate output array sized to used portion.  
            for (int i = 0; i < this.size; i++) {  // Copy elements.  
                result[i] = this.data[i];  // Copy one element.  
            }  // Close loop.  
            return result;  // Return copy.  
        }  // End toArray.  

        public int indexOf(int value) {  // Return first index of value or -1 (O(n)).  
            for (int i = 0; i < this.size; i++) {  // Linear scan.  
                if (this.data[i] == value) {  // Compare with target.  
                    return i;  // Return first match.  
                }  // Close match check.  
            }  // Close loop.  
            return -1;  // Not found sentinel.  
        }  // End indexOf.  

        public OperationCost append(int value) {  // Append at end and return resize cost (moved=0).  
            int copied = ensureCapacityForOneMore();  // Resize if needed and get copies count.  
            this.data[this.size] = value;  // Write new element.  
            this.size += 1;  // Increase size.  
            return new OperationCost(copied, 0);  // Return cost record.  
        }  // End append.  

        public OperationCost insertAt(int index, int value) {  // Insert at index and return cost (copies + shifts).  
            requireInsertIndexInRange(index);  // Validate index.  
            int copied = ensureCapacityForOneMore();  // Resize if needed before shifting.  

            int moved = this.size - index;  // Shifting right moves (size - index) elements.  
            for (int i = this.size; i > index; i--) {  // Shift right from tail toward index.  
                this.data[i] = this.data[i - 1];  // Move one element right.  
            }  // Close shift loop.  
            this.data[index] = value;  // Write new value.  
            this.size += 1;  // Increase size.  
            return new OperationCost(copied, moved);  // Return cost record.  
        }  // End insertAt.  

        public RemoveResult removeAt(int index) {  // Remove at index and return removed value + shift cost.  
            requireIndexInRange(index);  // Validate index.  
            int removedValue = this.data[index];  // Capture removed value.  
            int moved = this.size - index - 1;  // Shifting left moves (size - index - 1) elements.  
            for (int i = index; i < this.size - 1; i++) {  // Shift left to fill gap.  
                this.data[i] = this.data[i + 1];  // Move one element left.  
            }  // Close shift loop.  
            this.size -= 1;  // Decrease size.  
            return new RemoveResult(removedValue, new OperationCost(0, moved));  // Return removed value and cost.  
        }  // End removeAt.  
    }  // End DynamicArray.  

    public static final class AppendSummary {  // Summarize a sequence of appends (growth behavior).  
        public final int m;  // Number of appends performed.  
        public final int finalSize;  // Final size after appends.  
        public final int finalCapacity;  // Final capacity after growth.  
        public final long totalCopies;  // Total copied elements due to resizes.  
        public final long totalActualCost;  // Total cost if each append costs 1 write + copies.  
        public final int maxCopiedInOneOp;  // Maximum copied elements in any single append.  

        public AppendSummary(int m, int finalSize, int finalCapacity, long totalCopies, long totalActualCost, int maxCopiedInOneOp) {  // Construct immutable summary.
            this.m = m;  // Store m.
            this.finalSize = finalSize;  // Store final size.
            this.finalCapacity = finalCapacity;  // Store final capacity.
            this.totalCopies = totalCopies;  // Store total copies.
            this.totalActualCost = totalActualCost;  // Store total actual cost.
            this.maxCopiedInOneOp = maxCopiedInOneOp;  // Store max copied.
        }  // Close constructor.  
    }  // End AppendSummary.  

    public static AppendSummary simulateAppends(int m) {  // Simulate m appends and summarize growth/copy costs.  
        if (m < 0) {  // Reject invalid negative counts.  
            throw new IllegalArgumentException("m must be >= 0");  // Signal invalid input.  
        }  // Close validation.  

        DynamicArray a = new DynamicArray();  // Use fresh array so results depend only on m.  
        long totalActual = 0;  // Accumulate total actual cost (1 write + copied).  
        int maxCopied = 0;  // Track max copies in a single append.  
        for (int i = 0; i < m; i++) {  // Perform m appends.  
            OperationCost cost = a.append(i);  // Append deterministic value.  
            totalActual += 1L + (long) cost.copied;  // Add write cost + copy cost.  
            maxCopied = Math.max(maxCopied, cost.copied);  // Track max copied.  
        }  // Close loop.  
        return new AppendSummary(m, a.size(), a.capacity(), a.totalCopies(), totalActual, maxCopied);  // Return summary.  
    }  // End simulateAppends.  

    private static DynamicArray buildFilledArray(int n) {  // Build a dynamic array filled with 0..n-1.  
        if (n < 0) {  // Reject invalid sizes.  
            throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        DynamicArray a = new DynamicArray();  // Start empty.  
        for (int i = 0; i < n; i++) {  // Fill with deterministic values.  
            a.append(i);  // Append one value.  
        }  // Close loop.  
        return a;  // Return filled array.  
    }  // End buildFilledArray.  

    public static OperationCost simulateAppendCostAtSize(int n) {  // Build size n then append once and return cost.  
        DynamicArray a = buildFilledArray(n);  // Build size n array.  
        return a.append(999);  // Append sentinel and return cost.  
    }  // End simulateAppendCostAtSize.  

    public static OperationCost simulateInsert0CostAtSize(int n) {  // Build size n then insert at head and return cost.  
        DynamicArray a = buildFilledArray(n);  // Build size n array.  
        return a.insertAt(0, 999);  // Insert sentinel at head and return cost.  
    }  // End simulateInsert0CostAtSize.  

    private static List<Integer> parseMs(String[] args) {  // Parse CLI args into a list of m values.  
        List<Integer> ms = new ArrayList<>();  // Accumulate m values.  
        if (args.length == 0) {  // Use defaults when no args are provided.  
            ms.add(0);  // Include boundary case m=0.  
            ms.add(1);  // Add m=1.  
            ms.add(2);  // Add m=2.  
            ms.add(4);  // Add m=4.  
            ms.add(8);  // Add m=8.  
            ms.add(16);  // Add m=16.  
            ms.add(32);  // Add m=32.  
            return ms;  // Return defaults.  
        }  // Close default branch.  
        for (String raw : args) {  // Parse each CLI token.  
            ms.add(Integer.parseInt(raw));  // Convert token to int.  
        }  // Close loop.  
        return ms;  // Return parsed list.  
    }  // End parseMs.  

    private static String formatAppendSummaryTable(List<Integer> ms) {  // Format growth summary table for multiple m values.  
        String header = String.format("%6s | %6s | %7s | %6s | %6s", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  

        for (int m : ms) {  // Render one row per m.  
            AppendSummary s = simulateAppends(m);  // Simulate and summarize.  
            double avg = (s.m > 0) ? ((double) s.totalActualCost / (double) s.m) : 0.0;  // Compute average cost.  
            sb.append(String.format("%6d | %6d | %7d | %6.2f | %6d", s.m, s.finalCapacity, s.totalCopies, avg, s.maxCopiedInOneOp)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatAppendSummaryTable.  

    private static String formatAppendVsInsert0Table() {  // Format append vs insertAt(0) comparison for small n values.  
        int[] ns = new int[] {0, 1, 2, 4, 8, 16};  // Use small fixed set for readability.  
        String header = String.format("%6s | %6s | %6s | %7s | %7s", "n", "appCp", "appMv", "ins0Cp", "ins0Mv");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  

        for (int n : ns) {  // Render one row per n.  
            OperationCost app = simulateAppendCostAtSize(n);  // Cost of append at size n.  
            OperationCost ins0 = simulateInsert0CostAtSize(n);  // Cost of insertAt(0) at size n.  
            sb.append(String.format("%6d | %6d | %6d | %7d | %7d", n, app.copied, app.moved, ins0.copied, ins0.moved)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatAppendVsInsert0Table.  

    public static void main(String[] args) {  // CLI entry point that prints demo tables.  
        List<Integer> ms = parseMs(args);  // Determine which m values to simulate.  
        System.out.println("=== Append Growth (m appends) ===");  // Print section title.  
        System.out.print(formatAppendSummaryTable(ms));  // Print growth summary table.  
        System.out.println();  // Print blank line between sections.  
        System.out.println("=== Append vs insertAt(0) at size n ===");  // Print section title.  
        System.out.print(formatAppendVsInsert0Table());  // Print comparison table.  
    }  // End main.  
}  // End DynamicArrayDemo.  

