// 04 雙向鏈結串列示範程式（C#）/ Doubly linked list demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for collecting n values and output lines.  

namespace DoublyLinkedListUnit  // Keep this unit isolated within its own namespace.  
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

        private static void RunTests()  // Run built-in tests (no external packages).  
        {  // Open method scope.  
            foreach (int n in new[] { 0, 1, 2, 5 })  // Validate push/pop at both ends are O(1).  
            {  // Open foreach scope.  
                DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.BuildOrderedList(n);  // Build [0..n-1].  
                AssertEqual(0, lst.PushBack(999).Hops, "pushBack hops should be 0");  // Validate O(1) tail insert.  
                AssertEqual(0, lst.PushFront(888).Hops, "pushFront hops should be 0");  // Validate O(1) head insert.  
                AssertEqual(0, lst.PopBack().Cost.Hops, "popBack hops should be 0");  // Validate O(1) tail remove.  
                AssertEqual(0, lst.PopFront().Cost.Hops, "popFront hops should be 0");  // Validate O(1) head remove.  
            }  // Close foreach scope.  

            int size = 16;  // Fixed size for get coverage.  
            DoublyLinkedListDemo.DoublyLinkedList g = DoublyLinkedListDemo.BuildOrderedList(size);  // Build [0..15].  
            foreach (int i in new[] { 0, 1, 2, 4, 8, 15 })  // Validate getWithCost hops formula.  
            {  // Open foreach scope.  
                DoublyLinkedListDemo.GetResult r = g.GetWithCost(i);  // Fetch value + cost.  
                AssertEqual(i, r.Value, "ordered list value should equal index");  // Validate value.  
                AssertEqual(DoublyLinkedListDemo.ExpectedGetHops(size, i), r.Cost.Hops, "getWithCost hops should match nearest-end formula");  // Validate hops.  
            }  // Close foreach scope.  

            DoublyLinkedListDemo.DoublyLinkedList ia = DoublyLinkedListDemo.BuildOrderedList(6);  // Build [0..5].  
            DoublyLinkedListDemo.OperationCost c0 = ia.InsertAt(0, 99);  // Insert at head.  
            AssertEqual(0, c0.Hops, "insertAt(0) hops should be 0");  // Validate cost.  
            AssertListEqual(new List<int> { 99, 0, 1, 2, 3, 4, 5 }, ia.ToList(), "insertAt(0) ordering should match");  // Validate ordering.  

            DoublyLinkedListDemo.DoublyLinkedList ib = DoublyLinkedListDemo.BuildOrderedList(6);  // Build [0..5] again.  
            DoublyLinkedListDemo.OperationCost c3 = ib.InsertAt(3, 77);  // Insert at index 3.  
            AssertEqual(DoublyLinkedListDemo.ExpectedGetHops(6, 3), c3.Hops, "insertAt(3) hops should match nearest-end formula");  // Validate hops.  
            AssertListEqual(new List<int> { 0, 1, 2, 77, 3, 4, 5 }, ib.ToList(), "insertAt(3) ordering should match");  // Validate ordering.  

            DoublyLinkedListDemo.DoublyLinkedList ic = DoublyLinkedListDemo.BuildOrderedList(6);  // Build [0..5] again.  
            DoublyLinkedListDemo.OperationCost ce = ic.InsertAt(6, 55);  // Insert at tail.  
            AssertEqual(0, ce.Hops, "insertAt(size) hops should be 0");  // Validate hops.  
            AssertListEqual(new List<int> { 0, 1, 2, 3, 4, 5, 55 }, ic.ToList(), "insertAt(size) ordering should match");  // Validate ordering.  

            DoublyLinkedListDemo.DoublyLinkedList ra = DoublyLinkedListDemo.BuildOrderedList(6);  // Build [0..5].  
            DoublyLinkedListDemo.RemoveResult r0 = ra.RemoveAt(0);  // Remove head.  
            AssertEqual(0, r0.Value, "removeAt(0) should remove value 0");  // Validate value.  
            AssertEqual(0, r0.Cost.Hops, "removeAt(0) hops should be 0");  // Validate hops.  
            AssertListEqual(new List<int> { 1, 2, 3, 4, 5 }, ra.ToList(), "removeAt(0) ordering should match");  // Validate ordering.  

            DoublyLinkedListDemo.DoublyLinkedList rb = DoublyLinkedListDemo.BuildOrderedList(6);  // Build [0..5] again.  
            DoublyLinkedListDemo.RemoveResult r5 = rb.RemoveAt(5);  // Remove tail.  
            AssertEqual(5, r5.Value, "removeAt(tail) should remove value 5");  // Validate value.  
            AssertEqual(0, r5.Cost.Hops, "removeAt(tail) hops should be 0");  // Validate hops.  
            AssertListEqual(new List<int> { 0, 1, 2, 3, 4 }, rb.ToList(), "removeAt(tail) ordering should match");  // Validate ordering.  

            DoublyLinkedListDemo.DoublyLinkedList rc = DoublyLinkedListDemo.BuildOrderedList(6);  // Build [0..5] again.  
            DoublyLinkedListDemo.RemoveResult r3 = rc.RemoveAt(3);  // Remove value 3.  
            AssertEqual(3, r3.Value, "removeAt(3) should remove value 3");  // Validate value.  
            AssertEqual(DoublyLinkedListDemo.ExpectedGetHops(6, 3), r3.Cost.Hops, "removeAt(3) hops should match nearest-end formula");  // Validate hops.  
            AssertListEqual(new List<int> { 0, 1, 2, 4, 5 }, rc.ToList(), "removeAt(3) ordering should match");  // Validate ordering.  

            DoublyLinkedListDemo.DoublyLinkedList f = DoublyLinkedListDemo.BuildOrderedList(5);  // Build [0..4].  
            DoublyLinkedListDemo.FindResult f0 = f.IndexOfWithCost(0);  // Find head value.  
            AssertEqual(0, f0.Index, "indexOf(0) should return 0");  // Validate index.  
            AssertEqual(0, f0.Cost.Hops, "finding head needs 0 hops");  // Validate cost.  
            DoublyLinkedListDemo.FindResult f3 = f.IndexOfWithCost(3);  // Find index 3.  
            AssertEqual(3, f3.Index, "indexOf(3) should return 3");  // Validate index.  
            AssertEqual(3, f3.Cost.Hops, "finding index 3 needs 3 hops");  // Validate cost.  
            DoublyLinkedListDemo.FindResult fn = f.IndexOfWithCost(999);  // Search missing.  
            AssertEqual(-1, fn.Index, "indexOf(missing) should return -1");  // Validate not found.  
            AssertEqual(5, fn.Cost.Hops, "missing value should traverse n hops");  // Validate cost.  

            bool threw = false;  // Track invalid get exception.  
            try  // Attempt invalid get.  
            {  // Open try scope.  
                f.GetWithCost(999);  // Should throw.  
            }  // Close try scope.  
            catch (Exception)  // Catch expected exception.  
            {  // Open catch scope.  
                threw = true;  // Mark as thrown.  
            }  // Close catch scope.  
            AssertTrue(threw, "getWithCost should throw on invalid index");  // Validate exception behavior.  

            threw = false;  // Track invalid popBack exception.  
            try  // Attempt invalid popBack.  
            {  // Open try scope.  
                new DoublyLinkedListDemo.DoublyLinkedList().PopBack();  // Pop on empty should throw.  
            }  // Close try scope.  
            catch (Exception)  // Catch expected exception.  
            {  // Open catch scope.  
                threw = true;  // Mark as thrown.  
            }  // Close catch scope.  
            AssertTrue(threw, "popBack should throw on empty list");  // Validate exception behavior.  
        }  // Close RunTests.  

        private static List<int> ParseNsOrDefault(string[] args)  // Parse CLI args into n values or use defaults.  
        {  // Open method scope.  
            if (args.Length == 0)  // Use defaults when no args are provided.  
            {  // Open default branch.  
                return new List<int> { 0, 1, 2, 4, 8, 16 };  // Default sizes.  
            }  // Close default branch.  

            var ns = new List<int>();  // Accumulate parsed n values.  
            foreach (string raw in args)  // Parse each token.  
            {  // Open foreach scope.  
                ns.Add(int.Parse(raw));  // Convert token to int.  
            }  // Close foreach scope.  
            return ns;  // Return parsed list.  
        }  // Close ParseNsOrDefault.  

        private static string FormatPushPopTable(IReadOnlyList<int> ns)  // Format push/pop costs at both ends for multiple sizes.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,6} | {3,6} | {4,6}", "n", "pushB", "popB", "pushF", "popF");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int n in ns)  // Render one row per n.  
            {  // Open foreach scope.  
                DoublyLinkedListDemo.DoublyLinkedList a = DoublyLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                int pushB = a.PushBack(999).Hops;  // Measure pushBack hops.  
                int popB = a.Size > 0 ? a.PopBack().Cost.Hops : 0;  // Measure popBack hops (handle n=0).  
                DoublyLinkedListDemo.DoublyLinkedList b = DoublyLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                int pushF = b.PushFront(999).Hops;  // Measure pushFront hops.  
                int popF = b.Size > 0 ? b.PopFront().Cost.Hops : 0;  // Measure popFront hops (handle n=0).  
                lines.Add(string.Format("{0,6} | {1,6} | {2,6} | {3,6} | {4,6}", n, pushB, popB, pushF, popF));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatPushPopTable.  

        private static string FormatGetHopsTable()  // Format get(i) hops table for a fixed list size.  
        {  // Open method scope.  
            DoublyLinkedListDemo.DoublyLinkedList lst = DoublyLinkedListDemo.BuildOrderedList(16);  // Build [0..15].  
            int[] indices = new[] { 0, 1, 2, 4, 8, 15 };  // Fixed indices.  
            string header = string.Format("{0,6} | {1,6} | {2,6}", "i", "value", "hops");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int i in indices)  // Render one row per index.  
            {  // Open foreach scope.  
                DoublyLinkedListDemo.GetResult r = lst.GetWithCost(i);  // Fetch value + cost.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,6}", i, r.Value, r.Cost.Hops));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatGetHopsTable.  

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

                List<int> ns = ParseNsOrDefault(args);  // Parse n values or use defaults.  
                Console.WriteLine("=== push/pop hops at both ends ===");  // Print section title.  
                Console.WriteLine(FormatPushPopTable(ns));  // Print table.  
                Console.WriteLine();  // Print blank line.  
                Console.WriteLine("=== get(i) hops on list of size 16 ===");  // Print section title.  
                Console.WriteLine(FormatGetHopsTable());  // Print get-cost table.  
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

