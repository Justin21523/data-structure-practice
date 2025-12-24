/** 01 漸進符號示範（Java）/ Asymptotic notation demo (Java). */  // Bilingual file header for quick orientation.
public final class AsymptoticDemo {  // Declare a utility class that hosts static counting methods.
    private AsymptoticDemo() {  // Prevent instantiation because this class is only a function container.
        // No instances.  // Explain why the constructor is private.
    }  // Close the constructor block.

    public static long countConstantOps(int n) {  // Simulate a constant-time algorithm independent of n.
        long operations = 0;  // Initialize the simulated operation counter.
        operations += 1;  // Count a basic operation #1 (e.g., one assignment).
        operations += 1;  // Count a basic operation #2.
        operations += 1;  // Count a basic operation #3.
        return operations;  // Return a constant value to illustrate O(1).
    }  // End of countConstantOps.

    public static long countLog2Ops(int n) {  // Simulate logarithmic growth via repeated halving.
        if (n < 1) {  // Guard against invalid inputs because log(n) is not defined here for n < 1.
            throw new IllegalArgumentException("n must be >= 1");  // Fail fast with a clear message.
        }  // Close the validation block.

        int current = n;  // Copy the input so we can repeatedly shrink it without losing the original.
        long operations = 0;  // Initialize the operation counter for the halving loop.
        while (current > 1) {  // Each iteration halves current, giving ~log2(n) iterations.
            current /= 2;  // Reduce the problem size by a factor of 2 (integer division).
            operations += 1;  // Count one iteration as one representative operation.
        }  // End of the halving loop.
        return operations;  // Return floor(log2(n)) for this discrete loop.
    }  // End of countLog2Ops.

    public static long countLinearOps(int n) {  // Simulate linear growth via one pass of n iterations.
        if (n < 0) {  // Guard against invalid negative sizes.
            throw new IllegalArgumentException("n must be >= 0");  // Fail fast for invalid input.
        }  // Close the validation block.

        long operations = 0;  // Initialize the operation counter for the loop.
        for (int i = 0; i < n; i++) {  // Run exactly n iterations to illustrate O(n).
            operations += 1;  // Count one operation per iteration to model linear cost.
        }  // End of the linear loop.
        return operations;  // Return n as the total number of operations.
    }  // End of countLinearOps.

    public static long countNLog2NOps(int n) {  // Simulate n log n growth via an outer n loop and inner halving loop.
        if (n < 0) {  // Guard against invalid negative sizes.
            throw new IllegalArgumentException("n must be >= 0");  // Fail fast for invalid input.
        }  // Close the validation block.
        if (n == 0) {  // Handle the boundary case explicitly so the demo remains intuitive.
            return 0;  // Define 0 * log(0) as 0 work in this discrete demonstration.
        }  // Close the boundary-case block.

        long operations = 0;  // Initialize the operation counter for nested loops.
        for (int i = 0; i < n; i++) {  // Outer loop contributes the linear factor n.
            int current = n;  // Inner loop starts at n each time to produce the logarithmic factor.
            while (current > 1) {  // Inner loop halves current, producing ~log2(n) iterations.
                current /= 2;  // Shrink the inner problem size by half.
                operations += 1;  // Count one operation per inner iteration.
            }  // End of the inner halving loop.
        }  // End of the outer loop.
        return operations;  // Return approximately n * floor(log2(n)).
    }  // End of countNLog2NOps.

    public static long countQuadraticOps(int n) {  // Simulate quadratic growth via two nested loops.
        if (n < 0) {  // Guard against invalid negative sizes.
            throw new IllegalArgumentException("n must be >= 0");  // Fail fast for invalid input.
        }  // Close the validation block.

        long operations = 0;  // Initialize the operation counter for the double loop.
        for (int i = 0; i < n; i++) {  // Outer loop runs n times.
            for (int j = 0; j < n; j++) {  // Inner loop also runs n times, giving n * n iterations.
                operations += 1;  // Count one operation per inner iteration to model O(n^2).
            }  // End of inner loop.
        }  // End of outer loop.
        return operations;  // Return n^2 as the simulated total operation count.
    }  // End of countQuadraticOps.

    private static int[] parseNsOrDefault(String[] args) {  // Parse CLI arguments into n values or use defaults.
        if (args.length == 0) {  // Use a default sequence when the user provides no sizes.
            return new int[] {1, 2, 4, 8, 16, 32};  // Default sizes show clear growth differences.
        }  // Close the default selection block.

        int[] ns = new int[args.length];  // Allocate an array sized to the number of CLI args.
        for (int i = 0; i < args.length; i++) {  // Parse each argument to an integer.
            ns[i] = Integer.parseInt(args[i]);  // Convert from string to int (throws on invalid input).
        }  // End of the parsing loop.
        return ns;  // Return the parsed list so the caller can build the table.
    }  // End of parseNsOrDefault.

    private static void requireAllAtLeastOne(int[] ns) {  // Validate that all n values are valid for log-based demos.
        for (int n : ns) {  // Check each n individually so we can report errors deterministically.
            if (n < 1) {  // Reject non-positive n because log2(n) is not defined here for n < 1.
                throw new IllegalArgumentException("All n values must be >= 1 for this demo");  // Fail fast with a clear message.
            }  // Close the validation block.
        }  // End of validation loop.
    }  // End of requireAllAtLeastOne.

    private static String formatGrowthTable(int[] ns) {  // Build a human-readable table for the chosen n values.
        String header = String.format(  // Create a fixed-width header line for the output table.
            "%8s | %8s | %8s | %8s | %10s | %10s",  // Define aligned columns for easy comparison.
            "n", "O(1)", "O(log n)", "O(n)", "O(n log n)", "O(n^2)"  // Column names for each growth function.
        );  // End of header formatting.
        String separator = "-".repeat(header.length());  // Build a separator line that matches the header width.

        StringBuilder sb = new StringBuilder();  // Use StringBuilder to efficiently build multi-line output.
        sb.append(header).append('\n');  // Append the header line followed by a newline.
        sb.append(separator).append('\n');  // Append the separator line followed by a newline.

        for (int n : ns) {  // Render each n row with computed operation counts.
            long constantOps = countConstantOps(n);  // Compute the O(1) example count for this n.
            long log2Ops = countLog2Ops(n);  // Compute the O(log n) example count for this n.
            long linearOps = countLinearOps(n);  // Compute the O(n) example count for this n.
            long nLog2NOps = countNLog2NOps(n);  // Compute the O(n log n) example count for this n.
            long quadraticOps = countQuadraticOps(n);  // Compute the O(n^2) example count for this n.

            String row = String.format(  // Format one data row with aligned numeric columns.
                "%8d | %8d | %8d | %8d | %10d | %10d",  // Keep widths consistent with the header.
                n, constantOps, log2Ops, linearOps, nLog2NOps, quadraticOps  // Fill the row with computed values.
            );  // End row formatting.
            sb.append(row).append('\n');  // Append the row and a newline to the output buffer.
        }  // End of row loop.

        return sb.toString();  // Return the final table as a single string for printing.
    }  // End of formatGrowthTable.

    public static void main(String[] args) {  // Run the demo as a simple CLI program.
        int[] ns = parseNsOrDefault(args);  // Parse user-provided n values or fall back to defaults.
        requireAllAtLeastOne(ns);  // Ensure all n values are valid for log-based counters in this demo.
        System.out.print(formatGrowthTable(ns));  // Print the formatted table to stdout for study.
    }  // End of main.
}  // End of AsymptoticDemo class.

