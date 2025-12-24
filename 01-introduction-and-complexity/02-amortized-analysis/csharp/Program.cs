// 02 攤銷分析示範程式（C#）/ Amortized analysis demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for collecting m values and summaries.  

namespace AmortizedAnalysis  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class Program  // Console entry point for the demo and the built-in tests.  
    {  // Open class scope.  
        private static void AssertTrue(bool condition, string message)  // Minimal assertion helper.  
        {  // Open method scope.  
            if (!condition)  // Fail when condition is false.  
            {  // Open failure scope.  
                throw new InvalidOperationException("FAIL: " + message);  // Throw to signal test failure.  
            }  // Close failure scope.  
        }  // Close AssertTrue.  

        private static void AssertEqual(long expected, long actual, string message)  // Minimal equality assertion helper.  
        {  // Open method scope.  
            if (expected != actual)  // Fail when values differ.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (expected={expected}, actual={actual})");  // Throw mismatch.  
            }  // Close failure scope.  
        }  // Close AssertEqual.  

        private static void RunTests()  // Run a built-in test suite (no external packages).  
        {  // Open method scope.  
            AssertEqual(0, AmortizedDemo.Potential(0, 1), "potential(0,1) should be 0");  // Validate Φ offset.  

            foreach (int m in new[] { 0, 1, 2, 3, 4, 5, 6, 8, 16, 33, 100 })  // Validate capacity invariants across sizes.  
            {  // Open foreach scope.  
                AmortizedDemo.SimulationSummary s = AmortizedDemo.SimulateAppends(m);  // Simulate m appends.  
                AssertEqual(m, s.FinalSize, "final size should equal m");  // Validate size.  
                AssertTrue(AmortizedDemo.IsPowerOfTwo(s.FinalCapacity), "capacity should be power of two");  // Validate capacity form.  
                AssertTrue(s.FinalCapacity >= Math.Max(1, m), "capacity should be >= max(1,m)");  // Validate capacity bound.  
            }  // Close foreach scope.  

            foreach (int m in new[] { 0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33 })  // Validate deterministic copy sum.  
            {  // Open foreach scope.  
                AmortizedDemo.SimulationSummary s = AmortizedDemo.SimulateAppends(m);  // Simulate m appends.  
                AssertEqual(s.FinalCapacity - 1, s.TotalCopies, "total copies should equal finalCapacity - 1");  // Copies sum to cap-1.  
            }  // Close foreach scope.  

            foreach (int m in new[] { 0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256 })  // Validate aggregate bound.  
            {  // Open foreach scope.  
                AmortizedDemo.SimulationSummary s = AmortizedDemo.SimulateAppends(m);  // Simulate m appends.  
                AssertTrue(s.TotalActualCost <= 3L * m, "total actual cost should be <= 3m");  // Aggregate method result.  
            }  // Close foreach scope.  

            var table = new AmortizedDemo.AmortizedDynamicArray();  // Create a table for per-step invariants.  
            for (int i = 0; i < 256; i++)  // Run enough operations to hit multiple resizes.  
            {  // Open loop scope.  
                AmortizedDemo.AppendStep step = table.Append(i);  // Append and capture step record.  
                AssertTrue(step.BankAfter >= 0, "bank should never be negative");  // Validate accounting invariant.  
                AssertEqual(3, step.AmortizedCost, "amortized cost should be 3 each step");  // Validate potential method result.  
            }  // Close loop scope.  
        }  // Close RunTests.  

        private static List<int> ParseMsOrDefault(string[] args)  // Parse CLI args into m values or use defaults.  
        {  // Open method scope.  
            if (args.Length == 0)  // Use defaults when no args are provided.  
            {  // Open default branch.  
                return new List<int> { 1, 2, 4, 8, 16, 32 };  // Default sizes match other units for consistency.  
            }  // Close default branch.  

            var ms = new List<int>();  // Accumulate parsed m values.  
            foreach (string raw in args)  // Parse each CLI token.  
            {  // Open foreach scope.  
                ms.Add(int.Parse(raw));  // Convert token to int (throws on invalid input).  
            }  // Close foreach scope.  
            return ms;  // Return parsed list.  
        }  // Close ParseMsOrDefault.  

        private static string FormatSummaryTable(IReadOnlyList<AmortizedDemo.SimulationSummary> summaries)  // Format summaries into a printable table.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,8} | {2,6} | {3,6} | {4,6} | {5,7} | {6,6}", "m", "total", "avg", "max", "cap", "copies", "bank");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (AmortizedDemo.SimulationSummary s in summaries)  // Render one row per summary.  
            {  // Open foreach scope.  
                double avg = s.M > 0 ? (double)s.TotalActualCost / (double)s.M : 0.0;  // Compute average safely for m=0.  
                lines.Add(  // Append formatted row.  
                    string.Format(  // Format row with aligned columns.  
                        "{0,6} | {1,8} | {2,6:F2} | {3,6} | {4,6} | {5,7} | {6,6}",  // Row format.  
                        s.M,  // Operation count.  
                        s.TotalActualCost,  // Total actual cost.  
                        avg,  // Average cost.  
                        s.MaxActualCost,  // Max cost.  
                        s.FinalCapacity,  // Capacity.  
                        s.TotalCopies,  // Copies.  
                        s.FinalBank  // Bank.  
                    )  // Close string.Format call.  
                );  // Close Add call.  
            }  // Close foreach scope.  

            return string.Join(Environment.NewLine, lines);  // Join lines into final printable string.  
        }  // Close FormatSummaryTable.  

        public static int Main(string[] args)  // Program entry point supporting demo and test modes.  
        {  // Open method scope.  
            try  // Catch exceptions to turn failures into non-zero exit codes.  
            {  // Open try scope.  
                if (args.Length > 0 && args[0] == "--test")  // Run tests when flag is provided.  
                {  // Open test branch.  
                    RunTests();  // Execute built-in tests.  
                    Console.WriteLine("All tests PASSED.");  // Report success.  
                    return 0;  // Exit success.  
                }  // Close test branch.  

                List<int> ms = ParseMsOrDefault(args);  // Parse m values or use defaults.  
                var summaries = new List<AmortizedDemo.SimulationSummary>();  // Collect summaries for printing.  
                foreach (int m in ms)  // Simulate each requested m.  
                {  // Open foreach scope.  
                    AmortizedDemo.SimulationSummary s = AmortizedDemo.SimulateAppends(m);  // Run simulation.  
                    summaries.Add(s);  // Store summary for printing.  
                }  // Close foreach scope.  
                Console.WriteLine(FormatSummaryTable(summaries));  // Print formatted table.  
                return 0;  // Exit success.  
            }  // Close try scope.  
            catch (Exception ex)  // Report errors consistently.  
            {  // Open catch scope.  
                Console.Error.WriteLine(ex.Message);  // Print error message.  
                return 1;  // Exit failure.  
            }  // Close catch scope.  
        }  // Close Main.  
    }  // Close class scope.  
}  // Close namespace scope.  

