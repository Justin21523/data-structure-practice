// 04 雙端佇列示範（Java）/ Deque demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store demo size lists (m values).  
import java.util.Arrays;  // Use Arrays.toString for printing snapshots.  
import java.util.List;  // Use List for type clarity.  

public final class DequeDemo {  // Provide a circular-buffer deque implementation with explicit copy/shift costs.  
    private DequeDemo() {  // Prevent instantiation because this class is a container for static logic.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class OperationCost {  // Record per-operation copy/shift counts.  
        public final int copied;  // How many elements were copied due to resize (0 if no resize).  
        public final int moved;  // How many elements were shifted (should be 0 here).  

        public OperationCost(int copied, int moved) {  // Construct an immutable cost record.  
            this.copied = copied;  // Store copy count.  
            this.moved = moved;  // Store move/shift count.  
        }  // Close constructor.  
    }  // End OperationCost.  

    public static final class PopResult {  // Return popped value plus its operation cost.  
        public final int value;  // The popped element value.  
        public final OperationCost cost;  // Copy/shift cost for this pop.  

        public PopResult(int value, OperationCost cost) {  // Construct immutable pop result.  
            this.value = value;  // Store popped value.  
            this.cost = cost;  // Store cost record.  
        }  // Close constructor.  
    }  // End PopResult.  

    public static final class PushSummary {  // Summarize a sequence of pushes (growth behavior).  
        public final int m;  // Number of pushes performed.  
        public final int finalSize;  // Final size after pushes.  
        public final int finalCapacity;  // Final capacity after growth.  
        public final long totalCopies;  // Total copied elements due to resizes.  
        public final long totalActualCost;  // Total cost if each push costs 1 write + copied.  
        public final int maxCopiedInOneOp;  // Maximum copies in any single push.  

        public PushSummary(int m, int finalSize, int finalCapacity, long totalCopies, long totalActualCost, int maxCopiedInOneOp) {  // Construct summary.  
            this.m = m;  // Store operation count.  
            this.finalSize = finalSize;  // Store final size.  
            this.finalCapacity = finalCapacity;  // Store final capacity.  
            this.totalCopies = totalCopies;  // Store total copies.  
            this.totalActualCost = totalActualCost;  // Store total actual cost.  
            this.maxCopiedInOneOp = maxCopiedInOneOp;  // Store max per-op copied.  
        }  // Close constructor.  
    }  // End PushSummary.  

    public static final class Deque {  // A circular-buffer deque with doubling growth (teaching-oriented).  
        private int size;  // Number of stored elements.  
        private int capacity;  // Allocated slots (always >= 1 in this unit).  
        private int head;  // Head index (front position) within the buffer.  
        private int[] data;  // Backing buffer.  
        private long totalCopies;  // Total elements copied due to resizes (for analysis/tests).  

        public Deque() {  // Initialize an empty deque with capacity 1.  
            this.size = 0;  // Empty deque has size 0.  
            this.capacity = 1;  // Start with capacity 1 for deterministic doubling.  
            this.head = 0;  // Head starts at index 0.  
            this.data = new int[this.capacity];  // Allocate backing buffer.  
            this.totalCopies = 0L;  // Start with zero total copies.  
        }  // Close constructor.  

        public int size() {  // Expose size for callers/tests.  
            return this.size;  // Return stored size.  
        }  // End size().  

        public int capacity() {  // Expose capacity for callers/tests.  
            return this.capacity;  // Return current capacity.  
        }  // End capacity().  

        public long totalCopies() {  // Expose total copies due to resizes.  
            return this.totalCopies;  // Return total copied elements.  
        }  // End totalCopies().  

        public boolean isEmpty() {  // Convenience check.  
            return this.size == 0;  // Empty iff size is zero.  
        }  // End isEmpty().  

        private int indexAt(int offset) {  // Map logical offset [0..size) to physical buffer index.  
            return (this.head + offset) % this.capacity;  // Wrap around using modulo.  
        }  // End indexAt().  

        public int[] toArray() {  // Return a copy of the used portion (front -> back).  
            int[] out = new int[this.size];  // Allocate array of exact size.  
            for (int i = 0; i < this.size; i++) {  // Copy elements in deque order.  
                out[i] = this.data[indexAt(i)];  // Copy one element.  
            }  // Close copy loop.  
            return out;  // Return copied array.  
        }  // End toArray().  

        private int resize(int newCapacity) {  // Resize buffer and return number of copied elements.  
            if (newCapacity < this.size) {  // Ensure new buffer can hold all elements.  
                throw new IllegalArgumentException("newCapacity must be >= size");  // Signal invalid request.  
            }  // Close validation.  
            if (newCapacity < 1) {  // Keep capacity positive.  
                throw new IllegalArgumentException("newCapacity must be >= 1");  // Signal invalid request.  
            }  // Close validation.  
            int[] newData = new int[newCapacity];  // Allocate new buffer.  
            int copied = 0;  // Count copied elements.  
            for (int i = 0; i < this.size; i++) {  // Copy elements in deque order.  
                newData[i] = this.data[indexAt(i)];  // Copy one element into compact layout.  
                copied += 1;  // Count one copy.  
            }  // Close copy loop.  
            this.data = newData;  // Swap buffer reference.  
            this.capacity = newCapacity;  // Update capacity.  
            this.head = 0;  // Reset head so deque starts at index 0 after resize.  
            this.totalCopies += copied;  // Accumulate total copies.  
            return copied;  // Return copies performed.  
        }  // End resize().  

        private int ensureCapacityForOneMore() {  // Ensure capacity for one more push; return resize copies.  
            if (this.size < this.capacity) {  // Fast path: free space exists.  
                return 0;  // No resize needed.  
            }  // Close fast path.  
            return resize(this.capacity * 2);  // Double capacity and return copies.  
        }  // End ensureCapacityForOneMore().  

        public OperationCost pushBack(int value) {  // Push at back (amortized O(1)).  
            int copied = ensureCapacityForOneMore();  // Resize if needed.  
            int tailIndex = indexAt(this.size);  // Compute tail index where new element is written.  
            this.data[tailIndex] = value;  // Store value at tail position.  
            this.size += 1;  // Increase size.  
            return new OperationCost(copied, 0);  // moved stays 0 (no shifting).  
        }  // End pushBack().  

        public OperationCost pushFront(int value) {  // Push at front (amortized O(1)).  
            int copied = ensureCapacityForOneMore();  // Resize if needed.  
            this.head = (this.head - 1 + this.capacity) % this.capacity;  // Move head left by one with wrap-around.  
            this.data[this.head] = value;  // Store value at new head position.  
            this.size += 1;  // Increase size.  
            return new OperationCost(copied, 0);  // moved stays 0 (no shifting).  
        }  // End pushFront().  

        public int peekFront() {  // Return front value without removing it (O(1)).  
            if (this.size == 0) {  // Reject peeking an empty deque.  
                throw new IndexOutOfBoundsException("peek from empty deque");  // Signal invalid operation.  
            }  // Close validation.  
            return this.data[this.head];  // Return front slot directly.  
        }  // End peekFront().  

        public int peekBack() {  // Return back value without removing it (O(1)).  
            if (this.size == 0) {  // Reject peeking an empty deque.  
                throw new IndexOutOfBoundsException("peek from empty deque");  // Signal invalid operation.  
            }  // Close validation.  
            int tailIndex = indexAt(this.size - 1);  // Compute index of last element.  
            return this.data[tailIndex];  // Return back slot.  
        }  // End peekBack().  

        public PopResult popFront() {  // Pop from front (O(1); no shifting).  
            if (this.size == 0) {  // Reject popping an empty deque.  
                throw new IndexOutOfBoundsException("pop from empty deque");  // Signal invalid operation.  
            }  // Close validation.  
            int removed = this.data[this.head];  // Capture front value.  
            this.head = (this.head + 1) % this.capacity;  // Advance head by one with wrap-around.  
            this.size -= 1;  // Decrease size.  
            if (this.size == 0) {  // Normalize head for deterministic state when deque becomes empty.  
                this.head = 0;  // Reset head (optional but stable).  
            }  // Close normalize branch.  
            return new PopResult(removed, new OperationCost(0, 0));  // moved stays 0 (no shift).  
        }  // End popFront().  

        public PopResult popBack() {  // Pop from back (O(1); no shifting).  
            if (this.size == 0) {  // Reject popping an empty deque.  
                throw new IndexOutOfBoundsException("pop from empty deque");  // Signal invalid operation.  
            }  // Close validation.  
            int tailIndex = indexAt(this.size - 1);  // Compute index of last element.  
            int removed = this.data[tailIndex];  // Capture back value.  
            this.size -= 1;  // Decrease size.  
            if (this.size == 0) {  // Normalize head for deterministic state when deque becomes empty.  
                this.head = 0;  // Reset head (optional but stable).  
            }  // Close normalize branch.  
            return new PopResult(removed, new OperationCost(0, 0));  // moved stays 0 (no shift).  
        }  // End popBack().  
    }  // End Deque.  

    public static PushSummary simulatePushBacks(int m) {  // Simulate m pushBack operations and summarize growth/copy costs.  
        if (m < 0) {  // Reject invalid counts.  
            throw new IllegalArgumentException("m must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        Deque d = new Deque();  // Fresh deque for deterministic results.  
        long totalActualCost = 0L;  // Accumulate total cost (1 write + copied).  
        int maxCopied = 0;  // Track max copied in one push.  
        for (int i = 0; i < m; i++) {  // Perform m pushes.  
            OperationCost cost = d.pushBack(i);  // Push deterministic value.  
            totalActualCost += 1L + cost.copied;  // Add write + copy cost.  
            maxCopied = Math.max(maxCopied, cost.copied);  // Update max copied.  
        }  // Close loop.  
        return new PushSummary(m, d.size(), d.capacity(), d.totalCopies(), totalActualCost, maxCopied);  // Return summary.  
    }  // End simulatePushBacks.  

    private static List<Integer> parseMs(String[] args) {  // Parse CLI args into a list of m values.  
        List<Integer> ms = new ArrayList<>();  // Accumulate m values.  
        if (args.length == 0) {  // Use defaults when no args are provided.  
            ms.add(0);  // Include m=0.  
            ms.add(1);  // Include m=1.  
            ms.add(2);  // Include m=2.  
            ms.add(4);  // Include m=4.  
            ms.add(8);  // Include m=8.  
            ms.add(16);  // Include m=16.  
            ms.add(32);  // Include m=32.  
            return ms;  // Return defaults.  
        }  // Close default branch.  
        for (String raw : args) {  // Parse each CLI token.  
            ms.add(Integer.parseInt(raw));  // Convert token to int.  
        }  // Close loop.  
        return ms;  // Return parsed list.  
    }  // End parseMs.  

    private static String formatPushSummaryTable(List<Integer> ms) {  // Format push-growth summaries for multiple m values.  
        String header = String.format("%6s | %6s | %7s | %6s | %6s", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int m : ms) {  // Render one row per m.  
            PushSummary s = simulatePushBacks(m);  // Simulate and summarize.  
            double avg = (s.m > 0) ? ((double) s.totalActualCost / (double) s.m) : 0.0;  // Average cost.  
            sb.append(String.format("%6d | %6d | %7d | %6.2f | %6d", s.m, s.finalCapacity, s.totalCopies, avg, s.maxCopiedInOneOp)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return accumulated output.  
    }  // End formatPushSummaryTable.  

    private static String formatDequeDemo() {  // Show a small deque example using both ends.  
        Deque d = new Deque();  // Create empty deque.  
        d.pushBack(3);  // Push 3 to back.  
        d.pushBack(7);  // Push 7 to back.  
        d.pushFront(1);  // Push 1 to front (deque becomes [1,3,7]).  
        int front = d.peekFront();  // Peek front value.  
        int back = d.peekBack();  // Peek back value.  
        int p1 = d.popFront().value;  // Pop front (should be 1).  
        int p2 = d.popBack().value;  // Pop back (should be 7).  
        int p3 = d.popBack().value;  // Pop back (should be 3).  
        return "pushBack [3,7], pushFront [1] => peekFront=" + front + ", peekBack=" + back + ", pops=" + Arrays.toString(new int[] {p1, p2, p3});  // Return formatted demo line.  
    }  // End formatDequeDemo.  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        List<Integer> ms = parseMs(args);  // Parse m values or use defaults.  
        System.out.println("=== pushBack growth (m pushes) ===");  // Print section title.  
        System.out.print(formatPushSummaryTable(ms));  // Print growth table.  
        System.out.println();  // Blank line.  
        System.out.println("=== deque demo (both ends) ===");  // Print section title.  
        System.out.println(formatDequeDemo());  // Print deque demo line.  
    }  // End main.  
}  // End DequeDemo.  

