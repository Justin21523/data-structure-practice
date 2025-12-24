// 05 環狀鏈結串列示範程式（C#）/ Circular linked list demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for parsing CLI args.  

namespace CircularLinkedListUnit  // Keep this unit isolated within its own namespace.  
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
            {  // Open scope: push cost tests.  
                var lst = new CircularLinkedListDemo.CircularLinkedList();  // Start with empty list.  
                AssertEqual(0, lst.PushBack(1).Hops, "pushBack hops should be 0");  // Validate O(1) tail insert.  
                AssertListEqual(new List<int> { 1 }, lst.ToList(), "pushBack should append");  // Validate ordering.  
                AssertEqual(0, lst.PushFront(0).Hops, "pushFront hops should be 0");  // Validate O(1) head insert.  
                AssertListEqual(new List<int> { 0, 1 }, lst.ToList(), "pushFront should prepend");  // Validate ordering.  
            }  // Close push cost scope.  

            {  // Open scope: popFront tests.  
                CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(3);  // Build [0,1,2].  
                CircularLinkedListDemo.RemoveResult r = lst.PopFront();  // Pop head.  
                AssertEqual(0, r.Value, "popFront should remove 0");  // Validate removed value.  
                AssertEqual(0, r.Cost.Hops, "popFront hops should be 0");  // Validate O(1) cost.  
                AssertListEqual(new List<int> { 1, 2 }, lst.ToList(), "popFront should remove head");  // Validate ordering.  
            }  // Close popFront scope.  

            {  // Open scope: popBack hop formula tests.  
                foreach (int n in new[] { 1, 2, 3, 5, 10 })  // Multiple sizes including boundary cases.  
                {  // Open foreach scope.  
                    CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(n);  // Build [0..n-1].  
                    CircularLinkedListDemo.RemoveResult r = lst.PopBack();  // Pop tail.  
                    AssertEqual(n - 1, r.Value, "popBack should remove last value");  // Validate removed value.  
                    int expectedHops = (n <= 1) ? 0 : (n - 2);  // Predecessor search traverses n-2 links.  
                    AssertEqual(expectedHops, r.Cost.Hops, "popBack hops should equal max(0,n-2)");  // Validate cost.  
                    var expectedList = new List<int>();  // Build expected [0..n-2].  
                    for (int i = 0; i < n - 1; i++)  // Fill expected list.  
                    {  // Open loop scope.  
                        expectedList.Add(i);  // Append one expected value.  
                    }  // Close loop scope.  
                    AssertListEqual(expectedList, lst.ToList(), "popBack should remove tail");  // Validate ordering.  
                }  // Close foreach scope.  
            }  // Close popBack scope.  

            {  // Open scope: getWithCost hop formula tests.  
                CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(10);  // Build [0..9].  
                foreach (int i in new[] { 0, 1, 5, 9 })  // Sample indices.  
                {  // Open foreach scope.  
                    CircularLinkedListDemo.GetResult r = lst.GetWithCost(i);  // Fetch value + cost.  
                    AssertEqual(i, r.Value, "ordered list value should equal index");  // Validate value.  
                    AssertEqual(i, r.Cost.Hops, "getWithCost hops should equal index");  // Validate cost.  
                }  // Close foreach scope.  
            }  // Close getWithCost scope.  

            {  // Open scope: rotate tests.  
                int size = 4;  // Use small fixed size.  
                var cases = new List<(int steps, List<int> expected)>  // Provide (steps, expected ordering) pairs.  
                {  // Open list literal.  
                    (0, new List<int> { 0, 1, 2, 3 }),  // No rotation.  
                    (1, new List<int> { 1, 2, 3, 0 }),  // Rotate by 1.  
                    (3, new List<int> { 3, 0, 1, 2 }),  // Rotate by 3.  
                    (4, new List<int> { 0, 1, 2, 3 }),  // Full cycle rotation.  
                    (7, new List<int> { 3, 0, 1, 2 }),  // 7%4==3.  
                };  // Close list literal.  

                foreach (var c in cases)  // Iterate cases.  
                {  // Open foreach scope.  
                    CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(size);  // Build [0,1,2,3].  
                    CircularLinkedListDemo.OperationCost cost = lst.Rotate(c.steps);  // Rotate by steps.  
                    AssertEqual(c.steps % size, cost.Hops, "rotate hops should equal steps%size");  // Validate hop count.  
                    AssertListEqual(c.expected, lst.ToList(), "rotate should produce expected ordering");  // Validate ordering.  
                }  // Close foreach scope.  
            }  // Close rotate scope.  

            {  // Open scope: indexOfWithCost tests.  
                CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(5);  // Build [0..4].  
                CircularLinkedListDemo.FindResult f0 = lst.IndexOfWithCost(0);  // Find head value.  
                AssertEqual(0, f0.Index, "indexOf(0) should return 0");  // Validate index.  
                AssertEqual(0, f0.Cost.Hops, "finding head needs 0 hops");  // Validate cost.  
                CircularLinkedListDemo.FindResult f3 = lst.IndexOfWithCost(3);  // Find value at index 3.  
                AssertEqual(3, f3.Index, "indexOf(3) should return 3");  // Validate index.  
                AssertEqual(3, f3.Cost.Hops, "finding index 3 needs 3 hops");  // Validate cost.  
                CircularLinkedListDemo.FindResult fn = lst.IndexOfWithCost(999);  // Search missing.  
                AssertEqual(-1, fn.Index, "indexOf(missing) should return -1");  // Validate not found.  
                AssertEqual(5, fn.Cost.Hops, "missing value should traverse size hops");  // Validate cost.  
            }  // Close indexOfWithCost scope.  

            {  // Open scope: invalid input tests.  
                CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(3);  // Build [0..2].  
                AssertThrows<IndexOutOfRangeException>(() => lst.GetWithCost(3), "getWithCost should throw on index==size");  // Invalid get.  
                AssertThrows<ArgumentOutOfRangeException>(() => lst.Rotate(-1), "rotate should throw on negative steps");  // Invalid rotate.  
                var empty = new CircularLinkedListDemo.CircularLinkedList();  // Create empty list.  
                AssertThrows<IndexOutOfRangeException>(() => empty.PopFront(), "popFront should throw on empty");  // Invalid pop.  
                AssertThrows<IndexOutOfRangeException>(() => empty.PopBack(), "popBack should throw on empty");  // Invalid pop.  
            }  // Close invalid input scope.  
        }  // Close RunTests.  

        private static List<int> ParseNsOrDefault(string[] args)  // Parse CLI args into n values or use defaults.  
        {  // Open method scope.  
            if (args.Length == 0)  // Use defaults when no args are provided.  
            {  // Open default branch.  
                return new List<int> { 0, 1, 2, 4, 8 };  // Default sizes.  
            }  // Close default branch.  

            var ns = new List<int>();  // Accumulate parsed n values.  
            foreach (string raw in args)  // Parse each token.  
            {  // Open foreach scope.  
                ns.Add(int.Parse(raw));  // Convert token to int.  
            }  // Close foreach scope.  
            return ns;  // Return parsed list.  
        }  // Close ParseNsOrDefault.  

        private static string FormatBasicCostTable(IReadOnlyList<int> ns)  // Show push/pop costs at multiple sizes.  
        {  // Open method scope.  
            string header = string.Format("{0,6} | {1,6} | {2,6} | {3,6} | {4,6}", "n", "pushB", "pushF", "popF", "popB");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int n in ns)  // Render one row per n.  
            {  // Open foreach scope.  
                CircularLinkedListDemo.CircularLinkedList a = CircularLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                int pushB = a.PushBack(999).Hops;  // Measure pushBack hops.  
                CircularLinkedListDemo.CircularLinkedList b = CircularLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                int pushF = b.PushFront(999).Hops;  // Measure pushFront hops.  
                CircularLinkedListDemo.CircularLinkedList c = CircularLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                int popF = c.Size > 0 ? c.PopFront().Cost.Hops : 0;  // Measure popFront hops (handle n=0).  
                CircularLinkedListDemo.CircularLinkedList d = CircularLinkedListDemo.BuildOrderedList(n);  // Build list of size n.  
                int popB = d.Size > 0 ? d.PopBack().Cost.Hops : 0;  // Measure popBack hops (handle n=0).  
                lines.Add(string.Format("{0,6} | {1,6} | {2,6} | {3,6} | {4,6}", n, pushB, pushF, popF, popB));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatBasicCostTable.  

        private static string FormatRotateTable(int n, IReadOnlyList<int> stepsList)  // Show rotate(steps) cost and resulting order.  
        {  // Open method scope.  
            CircularLinkedListDemo.CircularLinkedList lst = CircularLinkedListDemo.BuildOrderedList(n);  // Build base list.  
            string header = string.Format("{0,6} | {1,6} | {2,6} | {3,20}", "steps", "hops", "head", "list");  // Header line.  
            string separator = new string('-', header.Length);  // Separator line.  
            var lines = new List<string> { header, separator };  // Start with header + separator.  

            foreach (int steps in stepsList)  // Render one row per rotation.  
            {  // Open foreach scope.  
                CircularLinkedListDemo.OperationCost cost = lst.Rotate(steps);  // Rotate list.  
                List<int> snapshot = lst.ToList();  // Snapshot list for demo readability.  
                int headValue = snapshot.Count > 0 ? snapshot[0] : -1;  // Read head value (or -1 for empty).  
                lines.Add(string.Format("{0,6} | {1,6} | {2,6} | {3,20}", steps, cost.Hops, headValue, "[" + string.Join(", ", snapshot) + "]"));  // Append row.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines.  
        }  // Close FormatRotateTable.  

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
                Console.WriteLine("=== basic costs (push/pop) ===");  // Print section title.  
                Console.WriteLine(FormatBasicCostTable(ns));  // Print cost table.  
                Console.WriteLine();  // Blank line.  
                Console.WriteLine("=== rotate demo on n=5 ===");  // Print section title.  
                Console.WriteLine(FormatRotateTable(5, new List<int> { 0, 1, 2, 5, 7 }));  // Print rotation table.  
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

