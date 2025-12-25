// 04 雙端佇列示範程式（C#）/ Deque demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for parsing CLI args and expected snapshots.  

namespace DequeUnit  // Keep this unit isolated within its own namespace.  
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
            {  // Open scope: basic operations tests.  
                var d = new DequeDemo.Deque();  // Start with empty deque.  
                d.PushBack(3);  // Push 3 to back.  
                d.PushBack(7);  // Push 7 to back.  
                d.PushFront(1);  // Push 1 to front (deque becomes [1,3,7]).  
                AssertEqual(1, d.PeekFront(), "PeekFront should return 1");  // Validate front peek.  
                AssertEqual(7, d.PeekBack(), "PeekBack should return 7");  // Validate back peek.  
                AssertEqual(1, d.PopFront().Value, "PopFront should return 1");  // Validate popFront.  
                AssertEqual(7, d.PopBack().Value, "PopBack should return 7");  // Validate popBack.  
                AssertEqual(3, d.PopBack().Value, "final PopBack should return 3");  // Validate final pop.  
                AssertTrue(d.IsEmpty, "deque should be empty after removing all");  // Validate emptiness.  
            }  // Close basic operations scope.  

            {  // Open scope: resize copy tests.  
                var d = new DequeDemo.Deque();  // Start with empty deque (capacity=1).  
                AssertEqual(0, d.PushBack(10).Copied, "first push fits without resize");  // No resize.  
                AssertEqual(1, d.PushBack(20).Copied, "resize 1->2 copies 1 element");  // Copy 1.  
                AssertEqual(2, d.PushBack(30).Copied, "resize 2->4 copies 2 elements");  // Copy 2.  
                AssertEqual(0, d.PushBack(40).Copied, "push fits in capacity 4");  // No resize.  
                AssertEqual(4, d.PushBack(50).Copied, "resize 4->8 copies 4 elements");  // Copy 4.  
                AssertEqual(5, d.Size, "final size should be 5");  // Validate size.  
                AssertEqual(8, d.Capacity, "final capacity should be 8");  // Validate capacity.  
                AssertEqual(1 + 2 + 4, d.TotalCopies, "total copies should sum resizes");  // Validate total copies.  
                AssertListEqual(new List<int> { 10, 20, 30, 40, 50 }, d.ToList(), "snapshot should preserve front->back order");  // Validate snapshot.  
            }  // Close resize copy scope.  

            {  // Open scope: wrap-around tests.  
                var d = new DequeDemo.Deque();  // Start with empty deque.  
                for (int v = 0; v < 4; v++)  // Push 0..3 (capacity should become 4).  
                {  // Open loop scope.  
                    d.PushBack(v);  // Push one value.  
                }  // Close loop scope.  
                AssertEqual(0, d.PopFront().Value, "PopFront should return 0");  // Pop 0.  
                AssertEqual(1, d.PopFront().Value, "PopFront should return 1");  // Pop 1 (head offset).  
                d.PushBack(4);  // Push 4 (wrap-around).  
                d.PushBack(5);  // Push 5 (wrap-around).  
                AssertListEqual(new List<int> { 2, 3, 4, 5 }, d.ToList(), "ordering after wrap-around should be [2,3,4,5]");  // Validate order.  
            }  // Close wrap-around scope.  

            {  // Open scope: resize when head != 0 preserves order.  
                var d = new DequeDemo.Deque();  // Start with empty deque.  
                for (int v = 0; v < 4; v++)  // Push 0..3 (capacity should become 4).  
                {  // Open loop scope.  
                    d.PushBack(v);  // Push one value.  
                }  // Close loop scope.  
                d.PopFront();  // Pop 0 (head moves).  
                d.PopFront();  // Pop 1 (head moves).  
                d.PushBack(4);  // Push 4.  
                d.PushBack(5);  // Push 5 (deque is full).  
                DequeDemo.OperationCost cost = d.PushBack(6);  // Push 6 (triggers resize).  
                AssertEqual(4, cost.Copied, "pushBack resize should copy exactly size elements (4)");  // Validate copied count.  
                AssertEqual(0, cost.Moved, "moved should remain 0");  // Validate moved=0.  
                AssertListEqual(new List<int> { 2, 3, 4, 5, 6 }, d.ToList(), "order should be preserved after pushBack resize");  // Validate order.  
                DequeDemo.OperationCost cost2 = d.PushFront(1);  // Push 1 to front (no resize now).  
                AssertEqual(0, cost2.Copied, "pushFront should not resize after growth");  // Validate no resize.  
                AssertListEqual(new List<int> { 1, 2, 3, 4, 5, 6 }, d.ToList(), "pushFront should add element to the front");  // Validate order.  
            }  // Close head-offset resize scope.  

            {  // Open scope: pushFront-triggered resize preserves order.  
                var d = new DequeDemo.Deque();  // Start with empty deque.  
                for (int v = 0; v < 4; v++)  // Push 0..3 (capacity should become 4).  
                {  // Open loop scope.  
                    d.PushBack(v);  // Push one value.  
                }  // Close loop scope.  
                d.PopFront();  // Pop 0 (head moves).  
                d.PopFront();  // Pop 1 (head moves).  
                d.PushBack(4);  // Push 4.  
                d.PushBack(5);  // Push 5 (deque is full).  
                DequeDemo.OperationCost cost = d.PushFront(1);  // Push 1 to front (triggers resize).  
                AssertEqual(4, cost.Copied, "pushFront resize should copy exactly size elements (4)");  // Validate copied count.  
                AssertListEqual(new List<int> { 1, 2, 3, 4, 5 }, d.ToList(), "order should be [1,2,3,4,5] after resize");  // Validate order.  
            }  // Close pushFront-triggered resize scope.  

            {  // Open scope: empty operations throw.  
                var d = new DequeDemo.Deque();  // Create empty deque.  
                AssertThrows<IndexOutOfRangeException>(() => d.PeekFront(), "PeekFront should throw on empty");  // Invalid peek.  
                AssertThrows<IndexOutOfRangeException>(() => d.PeekBack(), "PeekBack should throw on empty");  // Invalid peek.  
                AssertThrows<IndexOutOfRangeException>(() => d.PopFront(), "PopFront should throw on empty");  // Invalid pop.  
                AssertThrows<IndexOutOfRangeException>(() => d.PopBack(), "PopBack should throw on empty");  // Invalid pop.  
            }  // Close empty operations scope.  
        }  // Close RunTests.  

        private static List<int> ParseMsOrDefault(string[] args)  // Parse CLI args into m values or use defaults.  
        {  // Open method scope.  
            if (args.Length == 0)  // Use defaults when no args are provided.  
            {  // Open default scope.  
                return new List<int> { 0, 1, 2, 4, 8, 16, 32 };  // Default m values.  
            }  // Close default scope.  

            var ms = new List<int>();  // Accumulate parsed m values.  
            foreach (string token in args)  // Parse each CLI token.  
            {  // Open foreach scope.  
                ms.Add(int.Parse(token));  // Convert token to int.  
            }  // Close foreach scope.  
            return ms;  // Return parsed list.  
        }  // Close ParseMsOrDefault.  

        private static string FormatPushSummaryTable(IReadOnlyList<int> ms)  // Format push-growth summaries for multiple m values.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,7} | {3,6} | {4,6}", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int m in ms)  // Render one row per m.  
            {  // Open foreach scope.  
                DequeDemo.PushSummary s = DequeDemo.SimulatePushBacks(m);  // Simulate and summarize.  
                double avg = s.M > 0 ? ((double)s.TotalActualCost / (double)s.M) : 0.0;  // Average actual cost per op.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,7} | {3,6:0.00} | {4,6}", s.M, s.FinalCapacity, s.TotalCopies, avg, s.MaxCopiedInOneOp));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatPushSummaryTable.  

        private static string FormatDequeDemo()  // Show a small deque example using both ends.  
        {  // Open method scope.  
            var d = new DequeDemo.Deque();  // Create empty deque.  
            d.PushBack(3);  // Push 3 to back.  
            d.PushBack(7);  // Push 7 to back.  
            d.PushFront(1);  // Push 1 to front (deque becomes [1,3,7]).  
            int front = d.PeekFront();  // Peek front value.  
            int back = d.PeekBack();  // Peek back value.  
            int p1 = d.PopFront().Value;  // Pop front (should be 1).  
            int p2 = d.PopBack().Value;  // Pop back (should be 7).  
            int p3 = d.PopBack().Value;  // Pop back (should be 3).  
            return $"pushBack [3,7], pushFront [1] => peekFront={front}, peekBack={back}, pops=[{p1},{p2},{p3}]";  // Return formatted demo line.  
        }  // Close FormatDequeDemo.  

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
                Console.WriteLine("=== pushBack growth (m pushes) ===");  // Print section title.  
                Console.WriteLine(FormatPushSummaryTable(ms));  // Print growth table.  
                Console.WriteLine();  // Blank line.  
                Console.WriteLine("=== deque demo (both ends) ===");  // Print section title.  
                Console.WriteLine(FormatDequeDemo());  // Print deque demo line.  
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

