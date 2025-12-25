// 03 環狀佇列示範程式（C#）/ Circular queue demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for parsing CLI args and expected snapshots.  

namespace CircularQueueUnit  // Keep this unit isolated within its own namespace.  
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
            {  // Open scope: FIFO tests.  
                var q = new CircularQueueDemo.CircularQueue();  // Start with empty queue.  
                q.Enqueue(10);  // Enqueue 10.  
                q.Enqueue(20);  // Enqueue 20.  
                q.Enqueue(30);  // Enqueue 30.  
                AssertEqual(10, q.Peek(), "peek should return first enqueued value");  // Validate peek.  
                AssertEqual(10, q.Dequeue().Value, "first dequeue should return 10");  // Validate first dequeue.  
                AssertEqual(20, q.Dequeue().Value, "second dequeue should return 20");  // Validate second dequeue.  
                AssertEqual(30, q.Dequeue().Value, "third dequeue should return 30");  // Validate third dequeue.  
                AssertTrue(q.IsEmpty, "queue should be empty after removing all");  // Validate emptiness.  
            }  // Close FIFO scope.  

            {  // Open scope: resize copy tests.  
                var q = new CircularQueueDemo.CircularQueue();  // Start with empty queue (capacity=1).  
                AssertEqual(0, q.Enqueue(10).Copied, "first enqueue fits without resize");  // No resize.  
                AssertEqual(1, q.Enqueue(20).Copied, "resize 1->2 copies 1 element");  // Copy 1.  
                AssertEqual(2, q.Enqueue(30).Copied, "resize 2->4 copies 2 elements");  // Copy 2.  
                AssertEqual(0, q.Enqueue(40).Copied, "enqueue fits in capacity 4");  // No resize.  
                AssertEqual(4, q.Enqueue(50).Copied, "resize 4->8 copies 4 elements");  // Copy 4.  
                AssertEqual(5, q.Size, "final size should be 5");  // Validate size.  
                AssertEqual(8, q.Capacity, "final capacity should be 8");  // Validate capacity.  
                AssertEqual(1 + 2 + 4, q.TotalCopies, "total copies should sum resizes");  // Validate total copies.  
                AssertListEqual(new List<int> { 10, 20, 30, 40, 50 }, q.ToList(), "ToList should preserve FIFO order");  // Validate order.  
            }  // Close resize scope.  

            {  // Open scope: dequeue moved tests.  
                foreach (int n in new[] { 1, 2, 3, 5, 10 })  // Multiple sizes.  
                {  // Open foreach scope.  
                    CircularQueueDemo.CircularQueue q = CircularQueueDemo.BuildOrderedQueue(n);  // Build [0..n-1].  
                    CircularQueueDemo.DequeueResult r = q.Dequeue();  // Dequeue once.  
                    AssertEqual(0, r.Value, "dequeued value should be 0");  // Validate removed value.  
                    AssertEqual(0, r.Cost.Moved, "dequeue moved should be 0");  // Validate moved=0.  
                    var expected = new List<int>();  // Build expected remaining queue.  
                    for (int i = 1; i < n; i++)  // Fill expected values 1..n-1.  
                    {  // Open loop scope.  
                        expected.Add(i);  // Append one value.  
                    }  // Close loop scope.  
                    AssertListEqual(expected, q.ToList(), "after dequeue, queue should be 1..n-1");  // Validate remaining order.  
                }  // Close foreach scope.  
            }  // Close dequeue moved scope.  

            {  // Open scope: wrap-around tests.  
                var q = new CircularQueueDemo.CircularQueue();  // Start with empty queue.  
                foreach (int v in new[] { 0, 1, 2, 3 })  // Fill queue to capacity 4 (with resizes).  
                {  // Open foreach scope.  
                    q.Enqueue(v);  // Enqueue one value.  
                }  // Close foreach scope.  
                AssertListEqual(new List<int> { 0, 1, 2, 3 }, q.ToList(), "initial ordering should match");  // Validate ordering.  
                AssertEqual(0, q.Dequeue().Value, "dequeue should return 0");  // Dequeue 0.  
                AssertEqual(1, q.Dequeue().Value, "dequeue should return 1");  // Dequeue 1 (head offset).  
                q.Enqueue(4);  // Enqueue 4 (wrap-around).  
                q.Enqueue(5);  // Enqueue 5 (wrap-around).  
                AssertListEqual(new List<int> { 2, 3, 4, 5 }, q.ToList(), "ordering after wrap-around should match");  // Validate ordering.  
            }  // Close wrap-around scope.  

            {  // Open scope: resize-with-offset-head test.  
                var q = new CircularQueueDemo.CircularQueue();  // Start with empty queue.  
                foreach (int v in new[] { 0, 1, 2, 3 })  // Fill queue to capacity 4.  
                {  // Open foreach scope.  
                    q.Enqueue(v);  // Enqueue one value.  
                }  // Close foreach scope.  
                q.Dequeue();  // Remove 0 (head moves).  
                q.Dequeue();  // Remove 1 (head moves).  
                q.Enqueue(4);  // Enqueue 4 (wrap-around into freed slots).  
                q.Enqueue(5);  // Enqueue 5 (queue is full again).  
                CircularQueueDemo.OperationCost cost = q.Enqueue(6);  // Trigger resize and enqueue 6.  
                AssertEqual(4, cost.Copied, "resize should copy exactly size elements (4)");  // Validate copied count.  
                AssertEqual(0, cost.Moved, "moved should remain 0");  // Validate moved=0.  
                AssertListEqual(new List<int> { 2, 3, 4, 5, 6 }, q.ToList(), "order should be preserved after resize");  // Validate ordering.  
            }  // Close resize-with-offset-head scope.  

            {  // Open scope: empty-operation tests.  
                var q = new CircularQueueDemo.CircularQueue();  // Create empty queue.  
                AssertThrows<IndexOutOfRangeException>(() => q.Peek(), "peek should throw on empty");  // Invalid peek.  
                AssertThrows<IndexOutOfRangeException>(() => q.Dequeue(), "dequeue should throw on empty");  // Invalid dequeue.  
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

        private static string FormatEnqueueSummaryTable(IReadOnlyList<int> ms)  // Format enqueue-growth summaries for multiple m values.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,7} | {3,6} | {4,6}", "m", "cap", "copies", "avg", "maxCp");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int m in ms)  // Render one row per m.  
            {  // Open foreach scope.  
                CircularQueueDemo.EnqueueSummary s = CircularQueueDemo.SimulateEnqueues(m);  // Simulate and summarize.  
                double avg = s.M > 0 ? ((double)s.TotalActualCost / (double)s.M) : 0.0;  // Average actual cost per op.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,7} | {3,6:0.00} | {4,6}", s.M, s.FinalCapacity, s.TotalCopies, avg, s.MaxCopiedInOneOp));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatEnqueueSummaryTable.  

        private static string FormatDequeueCostTable(IReadOnlyList<int> ns)  // Show dequeue moved costs at multiple sizes (should be 0).  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6}", "n", "moved");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int n in ns)  // Render one row per n.  
            {  // Open foreach scope.  
                int moved = n == 0 ? 0 : CircularQueueDemo.SimulateDequeueCostAtSize(n).Moved;  // Skip invalid dequeue on empty queue.  
                lines.Add(string.Format("{0,6} | {1,6}", n, moved));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatDequeueCostTable.  

        private static string FormatFifoDemo()  // Show a small FIFO example with peek/dequeue.  
        {  // Open method scope.  
            var q = new CircularQueueDemo.CircularQueue();  // Create an empty queue.  
            q.Enqueue(3);  // Enqueue 3.  
            q.Enqueue(7);  // Enqueue 7.  
            q.Enqueue(1);  // Enqueue 1.  
            int front = q.Peek();  // Peek current front.  
            int d1 = q.Dequeue().Value;  // Dequeue first (should be 3).  
            int d2 = q.Dequeue().Value;  // Dequeue second (should be 7).  
            int d3 = q.Dequeue().Value;  // Dequeue third (should be 1).  
            return $"enqueue [3,7,1] => peek={front}, dequeues=[{d1},{d2},{d3}]";  // Return formatted demo line.  
        }  // Close FormatFifoDemo.  

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
                List<int> ns = new List<int> { 0, 1, 2, 4, 8, 16 };  // Fixed n list for dequeue costs.  
                Console.WriteLine("=== Enqueue Growth (m enqueues) ===");  // Print section title.  
                Console.WriteLine(FormatEnqueueSummaryTable(ms));  // Print enqueue growth table.  
                Console.WriteLine();  // Blank line.  
                Console.WriteLine("=== Dequeue Shift Cost (should be 0) ===");  // Print section title.  
                Console.WriteLine(FormatDequeueCostTable(ns));  // Print dequeue moved table.  
                Console.WriteLine();  // Blank line.  
                Console.WriteLine("=== FIFO demo ===");  // Print section title.  
                Console.WriteLine(FormatFifoDemo());  // Print FIFO demo line.  
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

