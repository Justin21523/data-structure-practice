// 02 佇列示範（Java）/ Queue demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store demo size lists (m values).  
import java.util.Arrays;  // Use Arrays.toString for printing snapshots.  
import java.util.List;  // Use List for type clarity.  

public final class QueueDemo {  // Provide a naive array-backed queue implementation with explicit copy/shift costs.  
    private QueueDemo() {  // Prevent instantiation because this class is a container for static logic.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class OperationCost {  // Record per-operation copy/shift counts.  
        public final int copied;  // How many elements were copied due to resize (0 if no resize).  
        public final int moved;  // How many elements were shifted left due to dequeue (0 for enqueue).  

        public OperationCost(int copied, int moved) {  // Construct an immutable cost record.  
            this.copied = copied;  // Store copy count.  
            this.moved = moved;  // Store move/shift count.  
        }  // Close constructor.  
    }  // End OperationCost.  

    public static final class DequeueResult {  // Return dequeued value plus its operation cost.  
        public final int value;  // The dequeued element value.  
        public final OperationCost cost;  // Copy/shift cost for this dequeue operation.  

        public DequeueResult(int value, OperationCost cost) {  // Construct immutable dequeue result.  
            this.value = value;  // Store dequeued value.  
            this.cost = cost;  // Store cost record.  
        }  // Close constructor.  
    }  // End DequeueResult.  

    public static final class EnqueueSummary {  // Summarize a sequence of enqueues (growth behavior).  
        public final int m;  // Number of enqueues performed.  
        public final int finalSize;  // Final size after enqueues.  
        public final int finalCapacity;  // Final capacity after growth.  
        public final long totalCopies;  // Total copied elements due to resizes.  
        public final long totalActualCost;  // Total cost if each enqueue costs 1 write + copied.  
        public final int maxCopiedInOneOp;  // Maximum copies in any single enqueue.  

        public EnqueueSummary(int m, int finalSize, int finalCapacity, long totalCopies, long totalActualCost, int maxCopiedInOneOp) {  // Construct summary.  
            this.m = m;  // Store operation count.  
            this.finalSize = finalSize;  // Store final size.  
            this.finalCapacity = finalCapacity;  // Store final capacity.  
            this.totalCopies = totalCopies;  // Store total copies.  
            this.totalActualCost = totalActualCost;  // Store total actual cost.  
            this.maxCopiedInOneOp = maxCopiedInOneOp;  // Store max per-op copied.  
        }  // Close constructor.  
    }  // End EnqueueSummary.  

    public static final class ArrayQueue {  // A naive array-backed queue (dequeue shifts left; teaching-oriented).  
        private int size;  // Number of stored elements.  
        private int capacity;  // Allocated slots (always >= 1 in this unit).  
        private int[] data;  // Backing buffer.  
        private long totalCopies;  // Total elements copied due to resizes (for analysis/tests).  

        public ArrayQueue() {  // Initialize an empty queue with capacity 1.  
            this.size = 0;  // Empty queue has size 0.  
            this.capacity = 1;  // Start with capacity 1 for deterministic doubling.  
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

        public int[] toArray() {  // Return a copy of the used portion (front -> rear).  
            int[] out = new int[this.size];  // Allocate array of exact size.  
            for (int i = 0; i < this.size; i++) {  // Copy used portion.  
                out[i] = this.data[i];  // Copy one element.  
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
            for (int i = 0; i < this.size; i++) {  // Copy used portion.  
                newData[i] = this.data[i];  // Copy one element.  
                copied += 1;  // Count one copy.  
            }  // Close copy loop.  
            this.data = newData;  // Swap buffer reference.  
            this.capacity = newCapacity;  // Update capacity.  
            this.totalCopies += copied;  // Accumulate total copies.  
            return copied;  // Return copies performed.  
        }  // End resize().  

        private int ensureCapacityForOneMore() {  // Ensure capacity for one more enqueue; return resize copies.  
            if (this.size < this.capacity) {  // Fast path: free space exists.  
                return 0;  // No resize needed.  
            }  // Close fast path.  
            return resize(this.capacity * 2);  // Double capacity and return copies.  
        }  // End ensureCapacityForOneMore().  

        public OperationCost enqueue(int value) {  // Add to rear (amortized O(1)).  
            int copied = ensureCapacityForOneMore();  // Resize if needed.  
            this.data[this.size] = value;  // Write new value at rear slot.  
            this.size += 1;  // Increase size.  
            return new OperationCost(copied, 0);  // Enqueue does not shift left.  
        }  // End enqueue().  

        public int peek() {  // Return front value without removing it (O(1)).  
            if (this.size == 0) {  // Reject peeking an empty queue.  
                throw new IndexOutOfBoundsException("peek from empty queue");  // Signal invalid operation.  
            }  // Close validation.  
            return this.data[0];  // Return front slot.  
        }  // End peek().  

        public DequeueResult dequeue() {  // Remove and return front value (O(n) due to shifting).  
            if (this.size == 0) {  // Reject dequeuing an empty queue.  
                throw new IndexOutOfBoundsException("dequeue from empty queue");  // Signal invalid operation.  
            }  // Close validation.  
            int removed = this.data[0];  // Capture front value.  
            int moved = this.size - 1;  // Shifting left moves (size - 1) elements.  
            for (int i = 1; i < this.size; i++) {  // Shift elements left by one.  
                this.data[i - 1] = this.data[i];  // Move one element left.  
            }  // Close shift loop.  
            this.size -= 1;  // Decrease size after shifting.  
            return new DequeueResult(removed, new OperationCost(0, moved));  // Return value + shift cost.  
        }  // End dequeue().  
    }  // End ArrayQueue.  

    public static EnqueueSummary simulateEnqueues(int m) {  // Simulate m enqueues and summarize growth/copy costs.  
        if (m < 0) {  // Reject invalid counts.  
            throw new IllegalArgumentException("m must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        ArrayQueue q = new ArrayQueue();  // Fresh queue for deterministic results.  
        long totalActualCost = 0L;  // Accumulate total cost (1 write + copied).  
        int maxCopied = 0;  // Track max copied in one enqueue.  
        for (int i = 0; i < m; i++) {  // Perform m enqueues.  
            OperationCost cost = q.enqueue(i);  // Enqueue deterministic value.  
            totalActualCost += 1L + cost.copied;  // Add write + copy cost.  
            maxCopied = Math.max(maxCopied, cost.copied);  // Update max copied.  
        }  // Close loop.  
        return new EnqueueSummary(m, q.size(), q.capacity(), q.totalCopies(), totalActualCost, maxCopied);  // Return summary.  
    }  // End simulateEnqueues.  

    public static ArrayQueue buildOrderedQueue(int n) {  // Build a queue containing [0, 1, ..., n-1].  
        if (n < 0) {  // Reject invalid sizes.  
            throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        ArrayQueue q = new ArrayQueue();  // Create empty queue.  
        for (int v = 0; v < n; v++) {  // Enqueue in increasing order.  
            q.enqueue(v);  // Enqueue one value.  
        }  // Close loop.  
        return q;  // Return built queue.  
    }  // End buildOrderedQueue.  

    public static OperationCost simulateDequeueCostAtSize(int n) {  // Build size n queue then dequeue once and return cost.  
        ArrayQueue q = buildOrderedQueue(n);  // Build queue of size n.  
        return q.dequeue().cost;  // Dequeue once and return cost.  
    }  // End simulateDequeueCostAtSize.  

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

    private static String formatEnqueueSummaryTable(List<Integer> ms) {  // Format enqueue-growth summaries for multiple m values.  
        String header = String.format("%6s | %6s | %7s | %6s | %6s", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int m : ms) {  // Render one row per m.  
            EnqueueSummary s = simulateEnqueues(m);  // Simulate and summarize.  
            double avg = (s.m > 0) ? ((double) s.totalActualCost / (double) s.m) : 0.0;  // Average cost.  
            sb.append(String.format("%6d | %6d | %7d | %6.2f | %6d", s.m, s.finalCapacity, s.totalCopies, avg, s.maxCopiedInOneOp)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return accumulated output.  
    }  // End formatEnqueueSummaryTable.  

    private static String formatDequeueCostTable(int[] ns) {  // Show dequeue shift costs at multiple sizes.  
        String header = String.format("%6s | %6s", "n", "moved");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int n : ns) {  // Render one row per n.  
            int moved = (n == 0) ? 0 : simulateDequeueCostAtSize(n).moved;  // Skip invalid dequeue on empty queue.  
            sb.append(String.format("%6d | %6d", n, moved)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return accumulated output.  
    }  // End formatDequeueCostTable.  

    private static String formatFifoDemo() {  // Show a small FIFO example with peek/dequeue.  
        ArrayQueue q = new ArrayQueue();  // Create an empty queue.  
        q.enqueue(3);  // Enqueue 3.  
        q.enqueue(7);  // Enqueue 7.  
        q.enqueue(1);  // Enqueue 1.  
        int front = q.peek();  // Peek current front.  
        int d1 = q.dequeue().value;  // Dequeue first (should be 3).  
        int d2 = q.dequeue().value;  // Dequeue second (should be 7).  
        int d3 = q.dequeue().value;  // Dequeue third (should be 1).  
        return "enqueue [3,7,1] => peek=" + front + ", dequeues=" + Arrays.toString(new int[] {d1, d2, d3});  // Return formatted demo line.  
    }  // End formatFifoDemo.  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        List<Integer> ms = parseMs(args);  // Parse m values or use defaults.  
        int[] ns = new int[] {0, 1, 2, 4, 8, 16};  // Fixed n list for dequeue costs.  
        System.out.println("=== Enqueue Growth (m enqueues) ===");  // Print section title.  
        System.out.print(formatEnqueueSummaryTable(ms));  // Print enqueue growth table.  
        System.out.println();  // Blank line.  
        System.out.println("=== Dequeue Shift Cost (dequeue once from size n) ===");  // Print section title.  
        System.out.print(formatDequeueCostTable(ns));  // Print dequeue moved table.  
        System.out.println();  // Blank line.  
        System.out.println("=== FIFO demo ===");  // Print section title.  
        System.out.println(formatFifoDemo());  // Print FIFO demo line.  
    }  // End main.  
}  // End QueueDemo.  

