// 03 單向鏈結串列示範程式（C#）/ Singly linked list demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for collecting n values and output lines.  

namespace SinglyLinkedListUnit  // Keep this unit isolated within its own namespace.  
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
            var lst = new SinglyLinkedListDemo.SinglyLinkedList();  // Create empty list.  
            foreach (int v in new[] { 1, 2, 3, 4 })  // Insert a few values at head.  
            {  // Open foreach scope.  
                SinglyLinkedListDemo.OperationCost cost = lst.PushFront(v);  // Insert at head.  
                AssertEqual(0, cost.Hops, "pushFront hops should be 0");  // Validate cost.  
            }  // Close foreach scope.  

            foreach (int n in new[] { 0, 1, 2, 3, 5, 10 })  // Validate pushBack traversal cost for multiple sizes.  
            {  // Open foreach scope.  
                SinglyLinkedListDemo.SinglyLinkedList a = SinglyLinkedListDemo.BuildOrderedList(n);  // Build [0..n-1].  
                SinglyLinkedListDemo.OperationCost cost = a.PushBack(999);  // Append sentinel.  
                int expected = n == 0 ? 0 : n - 1;  // Expected hops.  
                AssertEqual(expected, cost.Hops, "pushBack hops should equal max(0,n-1)");  // Validate traversal cost.  
                AssertEqual(n + 1, a.Size, "size should increase by one");  // Validate size update.  
                AssertTrue(a.ToList()[a.Size - 1] == 999, "pushBack should append to tail");  // Validate tail value.  
            }  // Close foreach scope.  

            SinglyLinkedListDemo.SinglyLinkedList g = SinglyLinkedListDemo.BuildOrderedList(10);  // Build [0..9].  
            foreach (int i in new[] { 0, 1, 5, 9 })  // Validate getWithCost behavior.  
            {  // Open foreach scope.  
                SinglyLinkedListDemo.GetResult r = g.GetWithCost(i);  // Fetch value + cost.  
                AssertEqual(i, r.Value, "value should equal index for ordered list");  // Validate value.  
                AssertEqual(i, r.Cost.Hops, "getWithCost hops should equal index");  // Validate cost.  
            }  // Close foreach scope.  

            SinglyLinkedListDemo.SinglyLinkedList ia = SinglyLinkedListDemo.BuildOrderedList(5);  // Build [0..4].  
            SinglyLinkedListDemo.OperationCost c0 = ia.InsertAt(0, 99);  // Insert at head.  
            AssertEqual(0, c0.Hops, "insertAt(0) hops should be 0");  // Validate cost.  
            AssertTrue(ia.ToList()[0] == 99, "insertAt(0) should insert at head");  // Validate ordering.  

            SinglyLinkedListDemo.SinglyLinkedList ib = SinglyLinkedListDemo.BuildOrderedList(5);  // Build [0..4] again.  
            SinglyLinkedListDemo.OperationCost c3 = ib.InsertAt(3, 77);  // Insert at index 3.  
            AssertEqual(2, c3.Hops, "insertAt(3) hops should be 2");  // Validate cost.  
            AssertTrue(ib.ToList()[3] == 77, "insertAt(3) should place value at index");  // Validate ordering.  

            SinglyLinkedListDemo.SinglyLinkedList ra = SinglyLinkedListDemo.BuildOrderedList(5);  // Build [0..4].  
            SinglyLinkedListDemo.RemoveResult r0 = ra.RemoveAt(0);  // Remove head.  
            AssertEqual(0, r0.Value, "removeAt(0) should remove value 0");  // Validate removed value.  
            AssertEqual(0, r0.Cost.Hops, "removeAt(0) hops should be 0");  // Validate cost.  
            AssertTrue(ra.ToList()[0] == 1, "removeAt(0) should remove head");  // Validate ordering.  

            SinglyLinkedListDemo.SinglyLinkedList rb = SinglyLinkedListDemo.BuildOrderedList(5);  // Build [0..4] again.  
            SinglyLinkedListDemo.RemoveResult r3 = rb.RemoveAt(3);  // Remove value 3.  
            AssertEqual(3, r3.Value, "removeAt(3) should remove value 3");  // Validate removed value.  
            AssertEqual(2, r3.Cost.Hops, "removeAt(3) hops should be 2");  // Validate cost.  
            AssertTrue(rb.ToList()[3] == 4, "removeAt(3) should bypass removed node");  // Validate ordering.  

            SinglyLinkedListDemo.SinglyLinkedList f = SinglyLinkedListDemo.BuildOrderedList(5);  // Build [0..4].  
            SinglyLinkedListDemo.FindResult f0 = f.IndexOfWithCost(0);  // Find head value.  
            AssertEqual(0, f0.Index, "indexOf(0) should return 0");  // Validate index.  
            AssertEqual(0, f0.Cost.Hops, "finding head needs 0 hops");  // Validate cost.  
            SinglyLinkedListDemo.FindResult f3 = f.IndexOfWithCost(3);  // Find index 3.  
            AssertEqual(3, f3.Index, "indexOf(3) should return 3");  // Validate index.  
            AssertEqual(3, f3.Cost.Hops, "finding index 3 needs 3 hops");  // Validate cost.  
            SinglyLinkedListDemo.FindResult fn = f.IndexOfWithCost(999);  // Search missing.  
            AssertEqual(-1, fn.Index, "indexOf(missing) should return -1");  // Validate not found.  
            AssertEqual(5, fn.Cost.Hops, "missing value should traverse n hops");  // Validate cost.  

            bool threw = false;  // Track invalid get exception.  
            try  // Attempt invalid get.  
            {  // Open try scope.  
                g.GetWithCost(999);  // Should throw.  
            }  // Close try scope.  
            catch (Exception)  // Catch expected exception.  
            {  // Open catch scope.  
                threw = true;  // Mark as thrown.  
            }  // Close catch scope.  
            AssertTrue(threw, "getWithCost should throw on invalid index");  // Validate exception behavior.  
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

        private static string FormatPushFrontVsBackTable(IReadOnlyList<int> ns)  // Format pushFront vs pushBack hops table.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,6}", "n", "frontH", "backH");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int n in ns)  // Render one row per n.  
            {  // Open foreach scope.  
                SinglyLinkedListDemo.SinglyLinkedList a = SinglyLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                SinglyLinkedListDemo.OperationCost front = a.PushFront(999);  // Measure head insertion.  
                SinglyLinkedListDemo.SinglyLinkedList b = SinglyLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                SinglyLinkedListDemo.OperationCost back = b.PushBack(999);  // Measure tail insertion.  
                lines.Add(string.Format("{0,6} | {1,6} | {2,6}", n, front.Hops, back.Hops));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatPushFrontVsBackTable.  

        private static string FormatGetHopsTable()  // Format get(i) hops table for a fixed list size.  
        {  // Open method scope.  
            SinglyLinkedListDemo.SinglyLinkedList lst = SinglyLinkedListDemo.BuildOrderedList(16);  // Build [0..15].  
            int[] indices = new[] { 0, 1, 2, 4, 8, 15 };  // Fixed indices.  
            string header = string.Format("{0,6} | {1,6} | {2,6}", "i", "value", "hops");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int i in indices)  // Render one row per index.  
            {  // Open foreach scope.  
                SinglyLinkedListDemo.GetResult r = lst.GetWithCost(i);  // Fetch value + cost.  
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
                Console.WriteLine("=== pushFront vs pushBack hops at size n ===");  // Print section title.  
                Console.WriteLine(FormatPushFrontVsBackTable(ns));  // Print table.  
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

