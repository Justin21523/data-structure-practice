// 02 動態陣列示範程式（C#）/ Dynamic array demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for collecting m values and output lines.  

namespace DynamicArrayUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class Program  // Console entry point for the demo and the built-in tests.  
    {  // Open class scope.  
        private static void AssertTrue(bool condition, string message)  // Minimal boolean assertion helper.  
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

        private static void RunTests()  // Run built-in tests (no external packages).  
        {  // Open method scope.  
            foreach (int m in new[] { 0, 1, 2, 3, 4, 5, 8, 9, 16, 33, 100 })  // Validate capacity invariant.  
            {  // Open foreach scope.  
                DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.SimulateAppends(m);  // Simulate m appends.  
                AssertEqual(m, s.FinalSize, "final size should equal m");  // Validate size.  
                AssertTrue(DynamicArrayDemo.IsPowerOfTwo(s.FinalCapacity), "capacity should be power of two");  // Validate capacity form.  
                AssertTrue(s.FinalCapacity >= Math.Max(1, m), "capacity should be >= max(1,m)");  // Validate bound.  
            }  // Close foreach scope.  

            foreach (int m in new[] { 0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33 })  // Validate copy-sum property.  
            {  // Open foreach scope.  
                DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.SimulateAppends(m);  // Simulate m appends.  
                AssertEqual(s.FinalCapacity - 1, s.TotalCopies, "total copies should equal finalCapacity - 1");  // Copies sum to cap-1.  
            }  // Close foreach scope.  

            foreach (int m in new[] { 0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256 })  // Validate aggregate bound.  
            {  // Open foreach scope.  
                DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.SimulateAppends(m);  // Simulate m appends.  
                AssertTrue(s.TotalActualCost <= 3L * m, "total actual cost should be <= 3m");  // Validate bound.  
            }  // Close foreach scope.  

            var a = new DynamicArrayDemo.DynamicArray();  // Create empty array for insert/remove tests.  
            a.Append(1);  // [1]  
            a.Append(2);  // [1,2]  
            a.Append(3);  // [1,2,3]  
            DynamicArrayDemo.OperationCost insertCost = a.InsertAt(1, 99);  // Insert at index 1.  
            AssertEqual(2, insertCost.Moved, "insertAt moved should equal size-index");  // Validate shift count.  
            AssertTrue(a.ToList()[1] == 99, "insertAt should place value at index");  // Validate ordering.  

            var b = new DynamicArrayDemo.DynamicArray();  // Create array for remove test.  
            foreach (int v in new[] { 10, 20, 30, 40 })  // Fill with deterministic values.  
            {  // Open foreach scope.  
                b.Append(v);  // Append one value.  
            }  // Close foreach scope.  
            DynamicArrayDemo.RemoveResult rr = b.RemoveAt(1);  // Remove 20.  
            AssertEqual(20, rr.Value, "removeAt should return removed value");  // Validate removed value.  
            AssertEqual(2, rr.Cost.Moved, "removeAt moved should equal size-index-1");  // Validate shift count.  
            AssertTrue(b.ToList()[1] == 30, "removeAt should shift left");  // Validate ordering.  

            bool threw = false;  // Track invalid get exception.  
            try  // Attempt invalid get.  
            {  // Open try scope.  
                a.Get(999);  // Should throw.  
            }  // Close try scope.  
            catch (Exception)  // Catch expected exception.  
            {  // Open catch scope.  
                threw = true;  // Mark as thrown.  
            }  // Close catch scope.  
            AssertTrue(threw, "get should throw on invalid index");  // Validate exception behavior.  
        }  // Close RunTests.  

        private static List<int> ParseMsOrDefault(string[] args)  // Parse CLI args into m values or use defaults.  
        {  // Open method scope.  
            if (args.Length == 0)  // Use defaults when no args are provided.  
            {  // Open default branch.  
                return new List<int> { 0, 1, 2, 4, 8, 16, 32 };  // Default m values.  
            }  // Close default branch.  

            var ms = new List<int>();  // Accumulate parsed m values.  
            foreach (string raw in args)  // Parse each token.  
            {  // Open foreach scope.  
                ms.Add(int.Parse(raw));  // Convert token to int.  
            }  // Close foreach scope.  
            return ms;  // Return parsed list.  
        }  // Close ParseMsOrDefault.  

        private static string FormatAppendSummaryTable(IReadOnlyList<int> ms)  // Format growth summary table.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,7} | {3,6} | {4,6}", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int m in ms)  // Render one row per m.  
            {  // Open foreach scope.  
                DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.SimulateAppends(m);  // Simulate and summarize.  
                double avg = s.M > 0 ? (double)s.TotalActualCost / (double)s.M : 0.0;  // Average cost per op.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,7} | {3,6:F2} | {4,6}", s.M, s.FinalCapacity, s.TotalCopies, avg, s.MaxCopiedInOneOp));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatAppendSummaryTable.  

        private static string FormatAppendVsInsert0Table()  // Format append vs insertAt(0) comparison table.  
        {  // Open method scope.  
            int[] ns = new[] { 0, 1, 2, 4, 8, 16 };  // Fixed n list.  
            string header = string.Format("{0,6} | {1,6} | {2,6} | {3,7} | {4,7}", "n", "appCp", "appMv", "ins0Cp", "ins0Mv");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int n in ns)  // Render one row per n.  
            {  // Open foreach scope.  
                DynamicArrayDemo.OperationCost app = DynamicArrayDemo.SimulateAppendCostAtSize(n);  // Cost of append at size n.  
                DynamicArrayDemo.OperationCost ins0 = DynamicArrayDemo.SimulateInsert0CostAtSize(n);  // Cost of insert@0 at size n.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,6} | {3,7} | {4,7}", n, app.Copied, app.Moved, ins0.Copied, ins0.Moved));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatAppendVsInsert0Table.  

        public static int Main(string[] args)  // Entry point supporting demo and test modes.  
        {  // Open method scope.  
            try  // Catch exceptions for consistent CLI behavior.  
            {  // Open try scope.  
                if (args.Length > 0 && args[0] == "--test")  // Run tests when flag is provided.  
                {  // Open test branch.  
                    RunTests();  // Execute tests.  
                    Console.WriteLine("All tests PASSED.");  // Report success.  
                    return 0;  // Exit success.  
                }  // Close test branch.  

                List<int> ms = ParseMsOrDefault(args);  // Parse m values or use defaults.  
                Console.WriteLine("=== Append Growth (m appends) ===");  // Print section title.  
                Console.WriteLine(FormatAppendSummaryTable(ms));  // Print summary table.  
                Console.WriteLine();  // Print blank line.  
                Console.WriteLine("=== Append vs insertAt(0) at size n ===");  // Print section title.  
                Console.WriteLine(FormatAppendVsInsert0Table());  // Print comparison table.  
                return 0;  // Exit success.  
            }  // Close try scope.  
            catch (Exception ex)  // Print errors consistently.  
            {  // Open catch scope.  
                Console.Error.WriteLine(ex.Message);  // Print error message.  
                return 1;  // Exit failure.  
            }  // Close catch scope.  
        }  // Close Main.  
    }  // Close class scope.  
}  // Close namespace scope.  

