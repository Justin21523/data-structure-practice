// 01 漸進符號示範程式（C#）/ Asymptotic notation demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and basic runtime types.
using System.Collections.Generic;  // Provide List<T> for building dynamic lists of n values.

namespace AsymptoticNotation  // Keep this unit isolated within its own namespace.
{  // Open namespace scope.
    internal static class Program  // Console entry point for the demo and the built-in tests.
    {  // Open class scope.
        private static void RequireAllAtLeastOne(IEnumerable<int> ns)  // Validate inputs for the table demo (needs log2).
        {  // Open method scope.
            foreach (int n in ns)  // Validate each n individually for deterministic error reporting.
            {  // Open foreach scope.
                if (n < 1)  // Reject non-positive n because log2(n) is not defined here for n < 1.
                {  // Open validation scope.
                    throw new ArgumentException("All n values must be >= 1 for this demo");  // Fail fast with a clear message.
                }  // Close validation scope.
            }  // Close foreach scope.
        }  // Close method scope.

        private static List<int> ParseNsOrDefault(string[] args)  // Parse CLI args into n values or fall back to defaults.
        {  // Open method scope.
            if (args.Length == 0)  // Use defaults when no CLI args are provided.
            {  // Open default branch.
                return new List<int> { 1, 2, 4, 8, 16, 32 };  // Default sizes show clear growth differences.
            }  // Close default branch.

            var ns = new List<int>();  // Create a list to store parsed n values.
            foreach (string raw in args)  // Parse each CLI token.
            {  // Open foreach scope.
                ns.Add(int.Parse(raw));  // Convert token to int (throws on invalid input).
            }  // Close foreach scope.
            return ns;  // Return parsed values for downstream processing.
        }  // Close method scope.

        private static string FormatGrowthTable(IReadOnlyList<int> ns)  // Build a fixed-width ASCII table for the chosen n values.
        {  // Open method scope.
            string header = string.Format("{0,8} | {1,8} | {2,8} | {3,8} | {4,10} | {5,10}", "n", "O(1)", "O(log n)", "O(n)", "O(n log n)", "O(n^2)");  // Build the aligned header line.
            string separator = new string('-', header.Length);  // Build a separator line matching the header width.

            var lines = new List<string> { header, separator };  // Start the table with header + separator.
            foreach (int n in ns)  // Add one formatted row per n value.
            {  // Open foreach scope.
                long c1 = AsymptoticDemo.CountConstantOps(n);  // Compute the O(1) example count.
                long clog = AsymptoticDemo.CountLog2Ops(n);  // Compute the O(log n) example count.
                long cn = AsymptoticDemo.CountLinearOps(n);  // Compute the O(n) example count.
                long cnlog = AsymptoticDemo.CountNLog2NOps(n);  // Compute the O(n log n) example count.
                long cn2 = AsymptoticDemo.CountQuadraticOps(n);  // Compute the O(n^2) example count.
                lines.Add($"{n,8} | {c1,8} | {clog,8} | {cn,8} | {cnlog,10} | {cn2,10}");  // Append the aligned numeric row.
            }  // Close foreach scope.

            return string.Join(Environment.NewLine, lines);  // Join lines into a single printable string.
        }  // Close method scope.

        private static void AssertEqual(long expected, long actual, string message)  // Minimal assertion helper for tests.
        {  // Open method scope.
            if (expected != actual)  // Fail when values differ.
            {  // Open failure scope.
                throw new InvalidOperationException($"{message} (expected={expected}, actual={actual})");  // Throw with a clear mismatch message.
            }  // Close failure scope.
        }  // Close method scope.

        private static void AssertThrows<TException>(Action action, string message) where TException : Exception  // Minimal assertion helper for exception behavior.
        {  // Open method scope.
            try  // Execute the action and ensure it throws.
            {  // Open try scope.
                action();  // Run the action expected to throw.
            }  // Close try scope.
            catch (TException)  // Accept the expected exception type.
            {  // Open expected-catch scope.
                return;  // Return early because the test passed.
            }  // Close expected-catch scope.
            throw new InvalidOperationException(message);  // Fail if no exception (or wrong exception) was thrown.
        }  // Close method scope.

        private static void RunTests()  // Run a small built-in test suite (no external packages).
        {  // Open method scope.
            AssertEqual(AsymptoticDemo.CountConstantOps(0), AsymptoticDemo.CountConstantOps(10), "O(1) should be constant");  // Verify constant behavior.
            AssertEqual(3, AsymptoticDemo.CountConstantOps(1), "This demo uses exactly 3 operations");  // Verify chosen constant.

            AssertEqual(0, AsymptoticDemo.CountLog2Ops(1), "log2 ops for n=1 should be 0");  // Verify halving count.
            AssertEqual(1, AsymptoticDemo.CountLog2Ops(2), "log2 ops for n=2 should be 1");  // Verify halving count.
            AssertEqual(3, AsymptoticDemo.CountLog2Ops(8), "log2 ops for n=8 should be 3");  // Verify halving count.

            AssertThrows<ArgumentException>(() => AsymptoticDemo.CountLog2Ops(0), "countLog2Ops should reject n=0");  // Verify invalid input handling.
            AssertThrows<ArgumentException>(() => AsymptoticDemo.CountLog2Ops(-1), "countLog2Ops should reject negative n");  // Verify invalid input handling.

            AssertEqual(0, AsymptoticDemo.CountLinearOps(0), "linear ops for n=0 should be 0");  // Verify linear count.
            AssertEqual(5, AsymptoticDemo.CountLinearOps(5), "linear ops for n=5 should be 5");  // Verify linear count.

            AssertEqual(0, AsymptoticDemo.CountQuadraticOps(0), "quadratic ops for n=0 should be 0");  // Verify quadratic count.
            AssertEqual(16, AsymptoticDemo.CountQuadraticOps(4), "quadratic ops for n=4 should be 16");  // Verify quadratic count.

            AssertEqual(0, AsymptoticDemo.CountNLog2NOps(0), "n log n ops for n=0 should be 0");  // Verify boundary case.
            AssertEqual(24, AsymptoticDemo.CountNLog2NOps(8), "n log n ops for n=8 should be 24");  // Verify n * log2(n) pattern.
        }  // Close method scope.

        public static int Main(string[] args)  // Program entry point that supports both demo and test modes.
        {  // Open method scope.
            try  // Catch exceptions to turn failures into non-zero exit codes.
            {  // Open try scope.
                if (args.Length > 0 && args[0] == "--test")  // Run tests when the user passes the test flag.
                {  // Open test branch.
                    RunTests();  // Execute the built-in tests.
                    Console.WriteLine("All tests PASSED.");  // Print a success message when everything passes.
                    return 0;  // Return success exit code.
                }  // Close test branch.

                List<int> ns = ParseNsOrDefault(args);  // Parse n values or use defaults.
                RequireAllAtLeastOne(ns);  // Ensure n values are valid for log2-based counters.
                Console.WriteLine(FormatGrowthTable(ns));  // Print the formatted table for study.
                return 0;  // Return success exit code.
            }  // Close try scope.
            catch (Exception ex)  // Handle errors consistently for CLI usage.
            {  // Open catch scope.
                Console.Error.WriteLine(ex.Message);  // Print the error message to stderr for debugging.
                return 1;  // Return non-zero code to signal failure.
            }  // Close catch scope.
        }  // Close Main scope.
    }  // Close class scope.
}  // Close namespace scope.
