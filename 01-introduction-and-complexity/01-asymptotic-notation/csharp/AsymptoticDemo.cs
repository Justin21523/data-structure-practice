// 01 漸進符號示範（C#）/ Asymptotic notation demo (C#).  // Bilingual file header.

using System;  // Provide basic runtime types and Console I/O.

namespace AsymptoticNotation  // Namespace to keep this unit isolated from other chapters.
{  // Open namespace scope.
    public static class AsymptoticDemo  // Static helper class containing operation-counting methods.
    {  // Open class scope.
        public static long CountConstantOps(int n)  // Simulate a constant-time algorithm independent of n.
        {  // Open method scope.
            _ = n;  // Explicitly ignore n to demonstrate O(1) independence.
            long operations = 0;  // Initialize the simulated operation counter.
            operations += 1;  // Count a basic operation #1.
            operations += 1;  // Count a basic operation #2.
            operations += 1;  // Count a basic operation #3.
            return operations;  // Return a constant value to illustrate O(1).
        }  // Close method scope.

        public static long CountLog2Ops(int n)  // Simulate logarithmic growth via repeated halving.
        {  // Open method scope.
            if (n < 1)  // Reject invalid inputs because log2(n) is not defined here for n < 1.
            {  // Open validation scope.
                throw new ArgumentException("n must be >= 1", nameof(n));  // Fail fast with a clear message.
            }  // Close validation scope.

            int current = n;  // Copy n so we can shrink it while counting iterations.
            long operations = 0;  // Initialize the counter for the halving loop.
            while (current > 1)  // Each iteration halves current, producing ~log2(n) iterations.
            {  // Open loop scope.
                current /= 2;  // Reduce the problem size by a factor of 2.
                operations += 1;  // Count one operation per halving step.
            }  // Close loop scope.
            return operations;  // Return floor(log2(n)) for this discrete loop.
        }  // Close method scope.

        public static long CountLinearOps(int n)  // Simulate linear growth via one pass of n iterations.
        {  // Open method scope.
            if (n < 0)  // Reject invalid negative sizes.
            {  // Open validation scope.
                throw new ArgumentException("n must be >= 0", nameof(n));  // Fail fast for invalid input.
            }  // Close validation scope.

            long operations = 0;  // Initialize the operation counter for the linear loop.
            for (int i = 0; i < n; i++)  // Run exactly n iterations to illustrate O(n).
            {  // Open loop scope.
                operations += 1;  // Count one operation per iteration.
            }  // Close loop scope.
            return operations;  // Return n as the total count.
        }  // Close method scope.

        public static long CountNLog2NOps(int n)  // Simulate n log n growth via outer n loop + inner halving loop.
        {  // Open method scope.
            if (n < 0)  // Reject invalid negative sizes.
            {  // Open validation scope.
                throw new ArgumentException("n must be >= 0", nameof(n));  // Fail fast for invalid input.
            }  // Close validation scope.
            if (n == 0)  // Handle boundary case explicitly for a clean demo definition.
            {  // Open boundary-case scope.
                return 0;  // Define 0 * log(0) as 0 operations in this discrete demonstration.
            }  // Close boundary-case scope.

            long operations = 0;  // Initialize the counter for nested loops.
            for (int i = 0; i < n; i++)  // Outer loop contributes the linear factor.
            {  // Open outer loop scope.
                int current = n;  // Inner loop starts at n each time to produce the logarithmic factor.
                while (current > 1)  // Inner loop halves current, producing ~log2(n) iterations.
                {  // Open inner loop scope.
                    current /= 2;  // Halve the inner problem size.
                    operations += 1;  // Count one operation per inner iteration.
                }  // Close inner loop scope.
            }  // Close outer loop scope.
            return operations;  // Return approximately n * floor(log2(n)).
        }  // Close method scope.

        public static long CountQuadraticOps(int n)  // Simulate quadratic growth via two nested loops.
        {  // Open method scope.
            if (n < 0)  // Reject invalid negative sizes.
            {  // Open validation scope.
                throw new ArgumentException("n must be >= 0", nameof(n));  // Fail fast for invalid input.
            }  // Close validation scope.

            long operations = 0;  // Initialize the counter for the double loop.
            for (int i = 0; i < n; i++)  // Outer loop runs n times.
            {  // Open outer loop scope.
                for (int j = 0; j < n; j++)  // Inner loop runs n times, giving n * n iterations.
                {  // Open inner loop scope.
                    operations += 1;  // Count one operation per inner iteration.
                }  // Close inner loop scope.
            }  // Close outer loop scope.
            return operations;  // Return n^2 as the total operation count.
        }  // Close method scope.
    }  // Close class scope.
}  // Close namespace scope.

