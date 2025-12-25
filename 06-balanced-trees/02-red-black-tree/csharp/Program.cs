// 02 Red-Black Tree 示範程式（C#，LLRB）/ Red-black tree demo program (C#, LLRB).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for expected inorder sequences.  

namespace RbtUnit  // Keep this unit isolated within its own namespace.  
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

        private static void AssertEqual(int expected, int actual, string message)  // Minimal int equality assertion helper.  
        {  // Open method scope.  
            if (expected != actual)  // Fail when values differ.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (expected={expected}, actual={actual})");  // Throw mismatch.  
            }  // Close failure scope.  
        }  // Close AssertEqual.  

        private static void AssertNullableEqual(int? expected, int? actual, string message)  // Minimal nullable-int equality helper.  
        {  // Open method scope.  
            if (expected.HasValue != actual.HasValue)  // Fail on presence mismatch.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (presence mismatch)");  // Throw mismatch.  
            }  // Close failure scope.  
            if (expected.HasValue && actual.HasValue && expected.Value != actual.Value)  // Fail on value mismatch.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (expected={expected}, actual={actual})");  // Throw mismatch.  
            }  // Close failure scope.  
        }  // Close AssertNullableEqual.  

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

        private static void AssertLessEqual(int upperBound, int actual, string message)  // Minimal <= assertion helper.  
        {  // Open method scope.  
            if (actual > upperBound)  // Fail when value is larger than bound.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (upperBound={upperBound}, actual={actual})");  // Throw mismatch.  
            }  // Close failure scope.  
        }  // Close AssertLessEqual.  

        private static void RunTests()  // Run built-in tests (no external packages).  
        {  // Open method scope.  
            {  // Open scope: empty input test.  
                var t = RbtDemo.RedBlackTree.FromValues(Array.Empty<int>());  // Build tree from empty array.  
                AssertTrue(t.IsEmpty, "tree should be empty");  // Validate emptiness.  
                AssertEqual(0, t.Size(), "size should be 0");  // Validate size.  
                AssertEqual(-1, t.Height(), "height should be -1 for empty");  // Validate height definition.  
                AssertNullableEqual(null, t.Min(), "min should be null for empty");  // Validate min.  
                AssertNullableEqual(null, t.Max(), "max should be null for empty");  // Validate max.  
                AssertListEqual(new List<int>(), t.Inorder(), "inorder should be empty");  // Validate inorder.  
                AssertTrue(t.Validate(), "empty tree should be valid");  // Empty tree is valid.  
                AssertTrue(!t.Contains(1), "contains should be false on empty");  // Validate contains.  
                AssertTrue(!t.Delete(1), "delete should be false when key is missing");  // Validate delete missing.  
            }  // Close empty scope.  

            {  // Open scope: duplicate insert test.  
                var t = RbtDemo.RedBlackTree.FromValues(Array.Empty<int>());  // Start from empty tree.  
                AssertTrue(t.Insert(5), "first insert should succeed");  // Insert 5.  
                AssertTrue(!t.Insert(5), "duplicate insert should be rejected");  // Duplicate insert.  
                AssertTrue(t.Contains(5), "contains should find inserted key");  // Validate contains.  
                AssertListEqual(new List<int> { 5 }, t.Inorder(), "inorder should contain one key");  // Validate inorder.  
                AssertTrue(t.Validate(), "tree should remain valid after duplicate attempt");  // Validate invariants.  
            }  // Close duplicate scope.  

            {  // Open scope: ascending inserts height bound.  
                var values = new List<int>();  // Build 1..15 input list.  
                for (int i = 1; i <= 15; i++)  // Fill with increasing values.  
                {  // Open loop scope.  
                    values.Add(i);  // Append i.  
                }  // Close loop scope.  
                var t = RbtDemo.RedBlackTree.FromValues(values.ToArray());  // Build tree by inserting in order.  
                AssertListEqual(values, t.Inorder(), "inorder should be sorted for ascending inserts");  // inorder must stay sorted.  
                AssertTrue(t.Validate(), "tree should satisfy red-black invariants");  // Validate invariants.  
                AssertLessEqual(7, t.Height(), "for n=15, red-black height (edges) must be <= 7");  // Height bound check.  
            }  // Close height-bound scope.  

            {  // Open scope: delete cases and root delete.  
                int[] values = new int[] { 8, 3, 10, 1, 6, 14, 4, 7, 13 };  // Classic insert order.  
                var t = RbtDemo.RedBlackTree.FromValues(values);  // Build tree by inserting in order.  
                AssertTrue(t.Validate(), "initial tree should be valid");  // Sanity-check initial validity.  

                AssertTrue(!t.Delete(999), "delete missing key should return false");  // Deleting missing key should fail.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 7, 8, 10, 13, 14 }, t.Inorder(), "inorder should be unchanged after missing delete");  // inorder unchanged.  
                AssertTrue(t.Validate(), "tree should remain valid after missing delete");  // Validate invariants.  

                AssertTrue(t.Delete(7), "delete leaf (7) should succeed");  // Delete a leaf node.  
                AssertTrue(!t.Contains(7), "deleted key (7) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 8, 10, 13, 14 }, t.Inorder(), "inorder after deleting 7 should match");  // Validate inorder.  
                AssertTrue(t.Validate(), "tree should remain valid after deleting 7");  // Validate invariants.  

                AssertTrue(t.Delete(14), "delete one-child node (14) should succeed");  // Delete a one-child node in this data set.  
                AssertTrue(!t.Contains(14), "deleted key (14) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 8, 10, 13 }, t.Inorder(), "inorder after deleting 14 should match");  // Validate inorder.  
                AssertTrue(t.Validate(), "tree should remain valid after deleting 14");  // Validate invariants.  

                AssertTrue(t.Delete(3), "delete two-child node (3) should succeed");  // Delete a two-child node.  
                AssertTrue(!t.Contains(3), "deleted key (3) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 4, 6, 8, 10, 13 }, t.Inorder(), "inorder after deleting 3 should match");  // Validate inorder.  
                AssertTrue(t.Validate(), "tree should remain valid after deleting 3");  // Validate invariants.  

                var t2 = RbtDemo.RedBlackTree.FromValues(values);  // Build a fresh tree for root deletion test.  
                AssertTrue(t2.Delete(8), "delete root (8) should succeed");  // Delete original root.  
                AssertTrue(!t2.Contains(8), "deleted root key (8) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 7, 10, 13, 14 }, t2.Inorder(), "inorder should remain sorted without 8");  // Validate inorder.  
                AssertTrue(t2.Validate(), "tree should remain valid after root deletion");  // Validate invariants.  
            }  // Close delete scope.  
        }  // Close RunTests.  

        private static string FormatList(IReadOnlyList<int> values)  // Format list as [a, b, c].  
        {  // Open method scope.  
            return "[" + string.Join(", ", values) + "]";  // Join values with comma+space.  
        }  // Close FormatList.  

        private static string FormatNullable(int? v)  // Format nullable int as number or "null".  
        {  // Open method scope.  
            return v.HasValue ? v.Value.ToString() : "null";  // Return number or null marker.  
        }  // Close FormatNullable.  

        private static string FormatDemo()  // Format a small demo output using a classic insert sequence.  
        {  // Open method scope.  
            int[] values = new int[] { 8, 3, 10, 1, 6, 14, 4, 7, 13 };  // Classic textbook insert order.  
            var tree = RbtDemo.RedBlackTree.FromValues(values);  // Build tree by inserting in order.  
            RbtDemo.TreeSummary s = tree.Summarize();  // Summarize properties and inorder traversal.  
            var lines = new List<string>();  // Accumulate printable lines.  
            lines.Add("=== Red-Black Tree Demo (C#, LLRB) ===");  // Print header.  
            lines.Add("insert order: [8, 3, 10, 1, 6, 14, 4, 7, 13]");  // Print insertion order (fixed for this demo).  
            lines.Add($"size={s.Size}, height={s.Height}, min={FormatNullable(s.MinValue)}, max={FormatNullable(s.MaxValue)}, valid={s.IsValid}");  // Print key properties.  
            lines.Add($"inorder(sorted)={FormatList(s.Inorder)}");  // Print inorder traversal.  
            lines.Add($"contains(6)={tree.Contains(6)}, contains(99)={tree.Contains(99)}");  // Print search examples.  
            tree.Delete(7);  // Delete a leaf (7).  
            tree.Delete(14);  // Delete a one-child node in this data set.  
            tree.Delete(3);  // Delete a two-child node (3) and rebalance.  
            lines.Add($"after deletes (7,14,3) inorder={FormatList(tree.Inorder())}, valid={tree.Validate()}");  // Show results after deletions.  
            return string.Join(Environment.NewLine, lines);  // Join lines into one printable string.  
        }  // Close FormatDemo.  

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

                Console.WriteLine(FormatDemo());  // Print demo output.  
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

