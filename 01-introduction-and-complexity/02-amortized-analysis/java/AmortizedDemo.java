// 02 攤銷分析示範（Java）/ Amortized analysis demo (Java).  // Bilingual file header for this unit.

import java.util.ArrayList;  // Use ArrayList to store per-step logs and summary rows.  
import java.util.List;  // Use List for API types and readability.  

public final class AmortizedDemo {  // Keep everything in one file for easy compilation without build tools.  
    private AmortizedDemo() {  // Prevent instantiation of this utility/demo class.  
    }  // Close constructor scope.  

    public static int potential(int size, int capacity) {  // Compute Φ for the potential method.  
        if (capacity < 1) {  // Reject invalid capacities to keep invariants simple.  
            throw new IllegalArgumentException("capacity must be >= 1");  // Fail fast with a clear message.  
        }  // Close validation block.  
        return (2 * size) - capacity + 1;  // Use Φ = 2·size - capacity + 1 (offset so Φ(0,1)=0).  
    }  // End of potential.  

    public static final class AppendStep {  // Record one append's costs and amortized-analysis state.  
        public final int index;  // 1-based operation index in the sequence.  
        public final int sizeBefore;  // Size before append.  
        public final int capacityBefore;  // Capacity before append.  
        public final int copied;  // Number of copied elements during resize (0 if no resize).  
        public final int actualCost;  // Actual cost: 1 write + copied moves.  
        public final int phiBefore;  // Φ before the operation.  
        public final int phiAfter;  // Φ after the operation.  
        public final int amortizedCost;  // Amortized cost: actual + ΔΦ (should be 3).  
        public final long bankAfter;  // Credit after charging 3 and paying actual cost (accounting method).  

        public AppendStep(  // Construct an immutable step record.  
                int index,  // Operation index.  
                int sizeBefore,  // Size before.  
                int capacityBefore,  // Capacity before.  
                int copied,  // Copies performed.  
                int actualCost,  // Actual cost.  
                int phiBefore,  // Potential before.  
                int phiAfter,  // Potential after.  
                int amortizedCost,  // Amortized cost.  
                long bankAfter  // Credit after.  
        ) {  // Open constructor scope.  
            this.index = index;  // Store operation index.  
            this.sizeBefore = sizeBefore;  // Store size before.  
            this.capacityBefore = capacityBefore;  // Store capacity before.  
            this.copied = copied;  // Store copy count.  
            this.actualCost = actualCost;  // Store actual cost.  
            this.phiBefore = phiBefore;  // Store Φ before.  
            this.phiAfter = phiAfter;  // Store Φ after.  
            this.amortizedCost = amortizedCost;  // Store amortized cost.  
            this.bankAfter = bankAfter;  // Store bank after.  
        }  // Close constructor scope.  
    }  // End of AppendStep.  

    public static final class SimulationSummary {  // Store summary metrics for a sequence of m appends.  
        public final int m;  // Number of appends performed.  
        public final int finalSize;  // Final size (should equal m).  
        public final int finalCapacity;  // Final capacity (power of two >= m, with this rule).  
        public final long totalActualCost;  // Total actual cost across all operations.  
        public final long totalCopies;  // Total copies across all resizes.  
        public final long maxActualCost;  // Maximum single-operation actual cost.  
        public final long finalBank;  // Final credit balance (3m - totalActualCost).  

        public SimulationSummary(  // Construct an immutable summary record.  
                int m,  // Operation count.  
                int finalSize,  // Final size.  
                int finalCapacity,  // Final capacity.  
                long totalActualCost,  // Total cost.  
                long totalCopies,  // Total copies.  
                long maxActualCost,  // Max cost.  
                long finalBank  // Final credit.  
        ) {  // Open constructor scope.  
            this.m = m;  // Store m.  
            this.finalSize = finalSize;  // Store final size.  
            this.finalCapacity = finalCapacity;  // Store final capacity.  
            this.totalActualCost = totalActualCost;  // Store total actual cost.  
            this.totalCopies = totalCopies;  // Store total copy count.  
            this.maxActualCost = maxActualCost;  // Store max actual cost.  
            this.finalBank = finalBank;  // Store final bank.  
        }  // Close constructor scope.  
    }  // End of SimulationSummary.  

    public static final class AmortizedDynamicArray {  // A tiny dynamic array instrumented for amortized analysis.  
        private int size;  // Track logical size (number of stored elements).  
        private int capacity;  // Track current capacity (allocated slots).  
        private int[] data;  // Backing fixed-size buffer (manually resized).  

        private long bank;  // Accounting-method credit balance.  
        private long totalActualCost;  // Aggregate total actual cost.  
        private long totalCopies;  // Aggregate total copy count.  
        private long maxActualCost;  // Track the maximum per-operation actual cost.  
        private final List<AppendStep> steps;  // Store per-operation steps for tests/inspection.  

        public AmortizedDynamicArray() {  // Initialize an empty table with capacity 1.  
            this.size = 0;  // Start with no elements.  
            this.capacity = 1;  // Start with one slot (keeps initial Φ = 0 with our formula).  
            this.data = new int[this.capacity];  // Allocate backing buffer.  

            this.bank = 0;  // Start with zero credit.  
            this.totalActualCost = 0;  // Start with zero total cost.  
            this.totalCopies = 0;  // Start with zero total copies.  
            this.maxActualCost = 0;  // Start max at zero.  
            this.steps = new ArrayList<>();  // Prepare a list for step logs.  
        }  // Close constructor scope.  

        public int getSize() {  // Expose size for callers/tests.  
            return this.size;  // Return current size.  
        }  // End of getSize.  

        public int getCapacity() {  // Expose capacity for callers/tests.  
            return this.capacity;  // Return current capacity.  
        }  // End of getCapacity.  

        public long getBank() {  // Expose bank for callers/tests.  
            return this.bank;  // Return current credit balance.  
        }  // End of getBank.  

        public long getTotalActualCost() {  // Expose total actual cost for aggregate checks.  
            return this.totalActualCost;  // Return total actual cost.  
        }  // End of getTotalActualCost.  

        public long getTotalCopies() {  // Expose total copy count for aggregate checks.  
            return this.totalCopies;  // Return total copies.  
        }  // End of getTotalCopies.  

        public long getMaxActualCost() {  // Expose maximum actual cost.  
            return this.maxActualCost;  // Return maximum per-operation cost.  
        }  // End of getMaxActualCost.  

        public List<AppendStep> getSteps() {  // Expose per-step log for tests.  
            return this.steps;  // Return the internal list (tests treat it as read-only).  
        }  // End of getSteps.  

        private int resize(int newCapacity) {  // Resize backing buffer and return number of copies performed.  
            if (newCapacity < this.size) {  // Ensure we do not lose data during resize.  
                throw new IllegalArgumentException("newCapacity must be >= size");  // Reject invalid requests.  
            }  // Close validation block.  
            if (newCapacity < 1) {  // Ensure capacity stays positive.  
                throw new IllegalArgumentException("newCapacity must be >= 1");  // Reject invalid requests.  
            }  // Close validation block.  

            int[] newData = new int[newCapacity];  // Allocate new fixed-size array.  
            int copied = 0;  // Count the number of element moves performed.  
            for (int i = 0; i < this.size; i++) {  // Copy each stored element into the new buffer.  
                newData[i] = this.data[i];  // Copy one element (one "move" for our cost model).  
                copied += 1;  // Increment copy counter.  
            }  // Close copy loop.  
            this.data = newData;  // Swap the backing buffer reference.  
            this.capacity = newCapacity;  // Update capacity to match the new buffer.  
            return copied;  // Return copy count as the resize cost.  
        }  // End of resize.  

        public AppendStep append(int value) {  // Append one value and return a per-step cost record.  
            int phiBefore = potential(this.size, this.capacity);  // Capture Φ before mutation.  
            int capacityBefore = this.capacity;  // Remember capacity before resize so we can log it correctly.  
            int sizeBefore = this.size;  // Remember size before insertion.  

            int copied = 0;  // Default: no resize, so no copies.  
            int actualCost = 1;  // Always pay 1 for writing the new element.  
            if (this.size == this.capacity) {  // Grow only when the table is full (doubling rule).  
                copied = resize(this.capacity * 2);  // Resize and count copies.  
                actualCost += copied;  // Add copy cost to this operation's actual cost.  
            }  // Close resize branch.  

            this.data[this.size] = value;  // Write the new element into the first free slot.  
            this.size += 1;  // Increase logical size after successful write.  

            int phiAfter = potential(this.size, this.capacity);  // Capture Φ after mutation.  
            int amortizedCost = actualCost + (phiAfter - phiBefore);  // Compute amortized cost via potential method.  

            this.bank += 3L - (long) actualCost;  // Accounting method: charge 3, then subtract actual cost.  
            if (this.bank < 0) {  // Credit should never be negative for this classic analysis.  
                throw new IllegalStateException("bank went negative (accounting invariant violated)");  // Fail fast.  
            }  // Close invariant check.  

            this.totalActualCost += (long) actualCost;  // Accumulate total actual cost.  
            this.totalCopies += (long) copied;  // Accumulate total copy count.  
            this.maxActualCost = Math.max(this.maxActualCost, (long) actualCost);  // Track maximum cost observed.  

            AppendStep step = new AppendStep(  // Create a step record for tests/inspection.  
                    this.steps.size() + 1,  // 1-based index.  
                    sizeBefore,  // Size before.  
                    capacityBefore,  // Capacity before.  
                    copied,  // Copies this step.  
                    actualCost,  // Actual cost.  
                    phiBefore,  // Φ before.  
                    phiAfter,  // Φ after.  
                    amortizedCost,  // Amortized cost.  
                    this.bank  // Bank after.  
            );  // Close constructor call.  
            this.steps.add(step);  // Append to the step log.  
            return step;  // Return the record to callers.  
        }  // End of append.  
    }  // End of AmortizedDynamicArray.  

    public static SimulationSummary simulateAppends(int m) {  // Run m appends and return a summary.  
        if (m < 0) {  // Reject invalid negative counts.  
            throw new IllegalArgumentException("m must be >= 0");  // Signal invalid input.  
        }  // Close validation block.  
        AmortizedDynamicArray table = new AmortizedDynamicArray();  // Use a fresh table for deterministic results.  
        for (int i = 0; i < m; i++) {  // Perform m appends.  
            table.append(i);  // Append a deterministic value to keep behavior reproducible.  
        }  // Close loop.  
        return new SimulationSummary(  // Build a summary record.  
                m,  // Record operation count.  
                table.getSize(),  // Record final size.  
                table.getCapacity(),  // Record final capacity.  
                table.getTotalActualCost(),  // Record total actual cost.  
                table.getTotalCopies(),  // Record total copies.  
                table.getMaxActualCost(),  // Record maximum actual cost.  
                table.getBank()  // Record final bank.  
        );  // Close summary construction.  
    }  // End of simulateAppends.  

    private static boolean isPowerOfTwo(int x) {  // Check whether x is a power of two (capacity invariant).  
        return x > 0 && (x & (x - 1)) == 0;  // Standard bit trick for powers of two.  
    }  // End of isPowerOfTwo.  

    private static List<Integer> parseMs(String[] args) {  // Parse CLI args into a list of m values.  
        List<Integer> ms = new ArrayList<>();  // Accumulate m values.  
        if (args.length == 0) {  // Use defaults when no args are provided.  
            ms.add(1);  // Add default m=1.  
            ms.add(2);  // Add default m=2.  
            ms.add(4);  // Add default m=4.  
            ms.add(8);  // Add default m=8.  
            ms.add(16);  // Add default m=16.  
            ms.add(32);  // Add default m=32.  
            return ms;  // Return defaults.  
        }  // Close default branch.  
        for (String raw : args) {  // Parse each CLI token.  
            ms.add(Integer.parseInt(raw));  // Parse int (throws on invalid input).  
        }  // Close parse loop.  
        return ms;  // Return parsed list.  
    }  // End of parseMs.  

    private static String formatTable(List<SimulationSummary> summaries) {  // Format summaries into a printable table.  
        String header = String.format("%6s | %8s | %6s | %6s | %6s | %7s | %6s", "m", "total", "avg", "max", "cap", "copies", "bank");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator matching header width.  
        StringBuilder sb = new StringBuilder();  // Accumulate output lines efficiently.  
        sb.append(header).append('\n');  // Append header line.  
        sb.append(separator).append('\n');  // Append separator line.  
        for (SimulationSummary s : summaries) {  // Render one row per summary.  
            double avg = (s.m > 0) ? ((double) s.totalActualCost / (double) s.m) : 0.0;  // Average actual cost per op.  
            sb.append(String.format(  // Append one formatted row.  
                    "%6d | %8d | %6.2f | %6d | %6d | %7d | %6d",  // Row format spec.  
                    s.m,  // Operation count.  
                    s.totalActualCost,  // Total actual cost.  
                    avg,  // Average cost.  
                    s.maxActualCost,  // Max single cost.  
                    s.finalCapacity,  // Final capacity.  
                    s.totalCopies,  // Total copies.  
                    s.finalBank  // Final bank.  
            )).append('\n');  // End the row with newline.  
        }  // Close row loop.  
        return sb.toString();  // Return the complete table string.  
    }  // End of formatTable.  

    public static void main(String[] args) {  // CLI entry point that prints amortized-analysis summaries.  
        List<Integer> ms = parseMs(args);  // Determine which m values to demonstrate.  
        List<SimulationSummary> summaries = new ArrayList<>();  // Collect summaries to print as one table.  
        for (int m : ms) {  // Simulate each requested m independently.  
            SimulationSummary summary = simulateAppends(m);  // Run the simulation.  
            if (summary.finalSize != m) {  // Sanity check size invariant.  
                throw new IllegalStateException("size mismatch (internal bug)");  // Fail fast.  
            }  // Close check.  
            if (!isPowerOfTwo(summary.finalCapacity)) {  // Ensure capacity is power-of-two.  
                throw new IllegalStateException("capacity not power-of-two (internal bug)");  // Fail fast.  
            }  // Close check.  
            if (summary.finalCapacity < Math.max(1, m)) {  // Ensure capacity is sufficient.  
                throw new IllegalStateException("capacity too small (internal bug)");  // Fail fast.  
            }  // Close check.  
            summaries.add(summary);  // Store for printing.  
        }  // Close loop.  
        System.out.print(formatTable(summaries));  // Print the formatted table to stdout.  
    }  // End of main.  
}  // End of AmortizedDemo.  

