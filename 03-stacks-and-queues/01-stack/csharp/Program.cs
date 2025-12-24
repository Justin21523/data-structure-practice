// 01 堆疊示範程式（C#）/ Stack demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for parsing CLI args and expected snapshots.  

namespace StackUnit  // Keep this unit isolated within its own namespace.  
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

        private static void AssertListEqual(IReadOnlyList<int> expected, IReadOnlyList<int> actual, string message)  // Minimal list equality helper.  
        {  // Open method scope.  
            if (expected.Count != actual.Count)  // Fail on length mismatch.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (len expected={expected.Count}, actual={actual.Count})");  // Throw mismatch.  
            }  // Close failure scope.  
            for (int i = 0; i < expected.Count; i++)  // Compare each element.  
            {  // Open loop scope.  
                if (expected[i] != actual[i])  // Fail on value mismatch.  
                {  // Open failure scope.  
                    throw new InvalidOperationException($"{message} (i={i}, expected={expected[i]}, actual={actual[i]})");  // Throw mismatch.  
                }  // Close failure scope.  
            }  // Close loop scope.  
        }  // Close AssertListEqual.  

        private static void AssertThrows<T>(Action action, string message) where T : Exception  // Minimal exception assertion helper.  
        {  // Open method scope.  
            try  // Execute and expect an exception.  
            {  // Open try scope.  
                action();  // Run action.  
            }  // Close try scope.  
            catch (Exception ex)  // Catch any exception.  
            {  // Open catch scope.  
                if (ex is T)  // Accept expected type.  
                {  // Open expected-type scope.  
                    return;  // Test passed.  
                }  // Close expected-type scope.  
                throw new InvalidOperationException($"{message} (threw {ex.GetType().Name})");  // Wrong type.  
            }  // Close catch scope.  
            throw new InvalidOperationException($"{message} (no exception thrown)");  // Fail if nothing thrown.  
        }  // Close AssertThrows.  

        private static void RunTests()  // Run built-in tests (no external packages).  
        {  // Open method scope.  
            {  // Open scope: LIFO tests.  
                var s = new StackDemo.ArrayStack();  // Start with empty stack.  
                s.Push(10);  // Push 10.  
                s.Push(20);  // Push 20.  
                s.Push(30);  // Push 30.  
                AssertEqual(30, s.Peek(), "peek should return last pushed value");  // Validate peek.  
                AssertEqual(30, s.Pop().Value, "first pop should return 30");  // Validate first pop.  
                AssertEqual(20, s.Pop().Value, "second pop should return 20");  // Validate second pop.  
                AssertEqual(10, s.Pop().Value, "third pop should return 10");  // Validate third pop.  
                AssertTrue(s.IsEmpty, "stack should be empty after popping all");  // Validate emptiness.  
            }  // Close LIFO scope.  

            {  // Open scope: resize cost tests.  
                var s = new StackDemo.ArrayStack();  // Start with empty stack (capacity=1).  
                AssertEqual(0, s.Push(10).Copied, "first push fits without resize");  // No resize.  
                AssertEqual(1, s.Push(20).Copied, "resize 1->2 copies 1 element");  // Copy 1.  
                AssertEqual(2, s.Push(30).Copied, "resize 2->4 copies 2 elements");  // Copy 2.  
                AssertEqual(0, s.Push(40).Copied, "push fits in capacity 4");  // No resize.  
                AssertEqual(4, s.Push(50).Copied, "resize 4->8 copies 4 elements");  // Copy 4.  
                AssertEqual(5, s.Size, "final size should be 5");  // Validate size.  
                AssertEqual(8, s.Capacity, "final capacity should be 8");  // Validate capacity.  
                AssertEqual(1 + 2 + 4, s.TotalCopies, "total copies should sum resizes");  // Validate total copies.  
                AssertListEqual(new List<int> { 10, 20, 30, 40, 50 }, s.ToList(), "ToList should preserve bottom->top order");  // Validate snapshot.  
            }  // Close resize scope.  

            {  // Open scope: empty-operation tests.  
                var s = new StackDemo.ArrayStack();  // Create empty stack.  
                AssertThrows<IndexOutOfRangeException>(() => s.Peek(), "peek should throw on empty");  // Invalid peek.  
                AssertThrows<IndexOutOfRangeException>(() => s.Pop(), "pop should throw on empty");  // Invalid pop.  
            }  // Close empty-operation scope.  
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

        private static string FormatPushSummaryTable(IReadOnlyList<int> ms)  // Format growth summaries for multiple m values.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,7} | {3,6} | {4,6}", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int m in ms)  // Render one row per m.  
            {  // Open foreach scope.  
                StackDemo.PushSummary s = StackDemo.SimulatePushes(m);  // Simulate and summarize.  
                double avg = s.M > 0 ? ((double)s.TotalActualCost / (double)s.M) : 0.0;  // Average actual cost per op.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,7} | {3,6:0.00} | {4,6}", s.M, s.FinalCapacity, s.TotalCopies, avg, s.MaxCopiedInOneOp));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatPushSummaryTable.  

        private static string FormatLifoDemo()  // Show a small LIFO example with peek/pop.  
        {  // Open method scope.  
            var s = new StackDemo.ArrayStack();  // Create an empty stack.  
            s.Push(3);  // Push 3.  
            s.Push(7);  // Push 7.  
            s.Push(1);  // Push 1 (becomes top).  
            int top = s.Peek();  // Peek current top.  
            int p1 = s.Pop().Value;  // Pop first (should be 1).  
            int p2 = s.Pop().Value;  // Pop second (should be 7).  
            int p3 = s.Pop().Value;  // Pop third (should be 3).  
            return $"push [3,7,1] => peek={top}, pops=[{p1},{p2},{p3}]";  // Return formatted demo line.  
        }  // Close FormatLifoDemo.  

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
                Console.WriteLine("=== Push Growth (m pushes) ===");  // Print section title.  
                Console.WriteLine(FormatPushSummaryTable(ms));  // Print table.  
                Console.WriteLine();  // Blank line.  
                Console.WriteLine("=== LIFO demo ===");  // Print section title.  
                Console.WriteLine(FormatLifoDemo());  // Print LIFO demo line.  
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

