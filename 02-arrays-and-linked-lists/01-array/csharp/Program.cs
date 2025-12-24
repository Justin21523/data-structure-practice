// 01 靜態陣列示範程式（C#）/ Static array demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for collecting n values and output lines.  

namespace StaticArray  // Keep this unit isolated within its own namespace.  
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

        private static void RunTests()  // Run a built-in test suite (no external packages).  
        {  // Open method scope.  
            var a = new FixedArrayDemo.FixedArray(3);  // Create a small array for basic operations test.  
            AssertEqual(0, a.Append(10), "append moved should be 0");  // Append at end moves 0.  
            AssertEqual(0, a.Append(20), "append moved should be 0");  // Append at end moves 0.  
            AssertEqual(10, a.Get(0), "get(0) should be 10");  // Validate first element.  
            AssertEqual(20, a.Get(1), "get(1) should be 20");  // Validate second element.  
            AssertTrue(a.ToList().Count == 2, "toList should have 2 elements");  // Validate list length.  

            a.Set(1, 99);  // Update element at index 1.  
            AssertEqual(99, a.Get(1), "set should update value");  // Validate update.  

            var b = new FixedArrayDemo.FixedArray(5);  // Create array with capacity for insert.  
            b.Append(1);  // [1]  
            b.Append(2);  // [1,2]  
            b.Append(3);  // [1,2,3]  
            int movedInsert = b.InsertAt(1, 99);  // Insert at index 1; should move 2 elements.  
            AssertEqual(2, movedInsert, "insertAt moved should equal size-index");  // Validate move count.  
            AssertTrue(b.ToList()[1] == 99, "insertAt should place value at index");  // Validate placement.  

            var c = new FixedArrayDemo.FixedArray(4);  // Create array for remove test.  
            c.Append(10);  // Fill.  
            c.Append(20);  // Fill.  
            c.Append(30);  // Fill.  
            c.Append(40);  // Fill.  
            FixedArrayDemo.RemoveResult r = c.RemoveAt(1);  // Remove 20; should move 2 elements.  
            AssertEqual(20, r.Value, "removeAt should return removed value");  // Validate removed value.  
            AssertEqual(2, r.Moved, "removeAt moved should equal size-index-1");  // Validate moved count.  

            var d = new FixedArrayDemo.FixedArray(5);  // Create array for indexOf test.  
            d.Append(7);  // [7]  
            d.Append(8);  // [7,8]  
            d.Append(7);  // [7,8,7]  
            AssertEqual(0, d.IndexOf(7), "indexOf should return first occurrence");  // Validate first match.  
            AssertEqual(1, d.IndexOf(8), "indexOf should return correct index");  // Validate.  
            AssertEqual(-1, d.IndexOf(999), "indexOf should return -1 when not found");  // Validate not found.  

            var e = new FixedArrayDemo.FixedArray(1);  // Capacity 1 for overflow test.  
            e.Append(1);  // Fill.  
            bool overflowThrew = false;  // Track overflow exception.  
            try  // Attempt overflow append.  
            {  // Open try scope.  
                e.Append(2);  // Should throw when full.  
            }  // Close try scope.  
            catch (Exception)  // Catch expected exception.  
            {  // Open catch scope.  
                overflowThrew = true;  // Mark as thrown.  
            }  // Close catch scope.  
            AssertTrue(overflowThrew, "append should throw when full");  // Validate overflow behavior.  

            AssertEqual(0, FixedArrayDemo.SimulateInsertMoves(0, 0), "insert into empty moves 0");  // n=0 boundary.  
            AssertEqual(4, FixedArrayDemo.SimulateInsertMoves(4, 0), "insert at head moves n");  // head insert formula.  
            AssertEqual(0, FixedArrayDemo.SimulateInsertMoves(4, 4), "insert at tail moves 0");  // tail insert formula.  
            AssertEqual(3, FixedArrayDemo.SimulateRemoveMoves(4, 0), "remove at head moves n-1");  // head remove formula.  
            AssertEqual(0, FixedArrayDemo.SimulateRemoveMoves(4, 3), "remove at tail moves 0");  // tail remove formula.  
        }  // Close RunTests.  

        private static List<int> ParseNsOrDefault(string[] args)  // Parse CLI args into n values or use defaults.  
        {  // Open method scope.  
            if (args.Length == 0)  // Use defaults when no args are provided.  
            {  // Open default branch.  
                return new List<int> { 0, 1, 2, 4, 8, 16 };  // Include boundary case n=0.  
            }  // Close default branch.  

            var ns = new List<int>();  // Accumulate parsed n values.  
            foreach (string raw in args)  // Parse each CLI token.  
            {  // Open foreach scope.  
                ns.Add(int.Parse(raw));  // Convert token to int (throws on invalid input).  
            }  // Close foreach scope.  
            return ns;  // Return parsed list.  
        }  // Close ParseNsOrDefault.  

        private static string FormatMovesTable(IReadOnlyList<int> ns)  // Format move counts into a printable table.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,7} | {3,7} | {4,6} | {5,7} | {6,7}", "n", "ins@0", "ins@mid", "ins@end", "rm@0", "rm@mid", "rm@end");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int n in ns)  // Render one row per n.  
            {  // Open foreach scope.  
                if (n < 0)  // Reject invalid n values.  
                {  // Open validation scope.  
                    throw new ArgumentException("n must be >= 0");  // Signal invalid input.  
                }  // Close validation scope.  
                int mid = n / 2;  // Choose deterministic middle index.  

                int ins0 = FixedArrayDemo.SimulateInsertMoves(n, 0);  // Moves for head insert.  
                int insm = FixedArrayDemo.SimulateInsertMoves(n, mid);  // Moves for middle insert.  
                int inse = FixedArrayDemo.SimulateInsertMoves(n, n);  // Moves for tail insert.  

                if (n == 0)  // Removal is not defined for empty arrays.  
                {  // Open branch.  
                    lines.Add(string.Format("{0,6} | {1,6} | {2,7} | {3,7} | {4,6} | {5,7} | {6,7}", n, ins0, insm, inse, "n/a", "n/a", "n/a"));  // Print n/a for removes.  
                }  // Close branch.  
                else  // For n>0 we can compute remove moves.  
                {  // Open branch.  
                    int rm0 = FixedArrayDemo.SimulateRemoveMoves(n, 0);  // Moves for head remove.  
                    int rmm = FixedArrayDemo.SimulateRemoveMoves(n, mid);  // Moves for middle remove.  
                    int rme = FixedArrayDemo.SimulateRemoveMoves(n, n - 1);  // Moves for tail remove.  
                    lines.Add(string.Format("{0,6} | {1,6} | {2,7} | {3,7} | {4,6} | {5,7} | {6,7}", n, ins0, insm, inse, rm0, rmm, rme));  // Print numeric row.  
                }  // Close branch.  
            }  // Close foreach scope.  

            return string.Join(Environment.NewLine, lines);  // Join lines into a printable table string.  
        }  // Close FormatMovesTable.  

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

                List<int> ns = ParseNsOrDefault(args);  // Parse n values or use defaults.  
                Console.WriteLine(FormatMovesTable(ns));  // Print move-count table.  
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

