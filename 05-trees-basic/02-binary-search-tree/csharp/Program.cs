// 02 二元搜尋樹示範程式（C#）/ Binary Search Tree (BST) demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for expected inorder sequences.  

namespace BstUnit  // Keep this unit isolated within its own namespace.  
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

        private static void RunTests()  // Run built-in tests (no external packages).  
        {  // Open method scope.  
            {  // Open scope: empty input test.  
                var t = BstDemo.BinarySearchTree.FromValues(Array.Empty<int>());  // Build tree from empty array.  
                AssertTrue(t.IsEmpty, "tree should be empty");  // Validate emptiness.  
                AssertEqual(0, t.Size(), "size should be 0");  // Validate size.  
                AssertEqual(-1, t.Height(), "height should be -1 for empty");  // Validate height definition.  
                AssertNullableEqual(null, t.Min(), "min should be null for empty");  // Validate min.  
                AssertNullableEqual(null, t.Max(), "max should be null for empty");  // Validate max.  
                AssertListEqual(new List<int>(), t.Inorder(), "inorder should be empty");  // Validate inorder.  
                AssertTrue(!t.Contains(1), "contains should be false on empty");  // Validate contains.  
                AssertTrue(!t.Delete(1), "delete should be false when key is missing");  // Validate delete missing.  
            }  // Close empty scope.  

            {  // Open scope: insert/contains/duplicate test.  
                var t = BstDemo.BinarySearchTree.FromValues(Array.Empty<int>());  // Start from empty tree.  
                AssertTrue(t.Insert(5), "first insert should succeed");  // Insert 5.  
                AssertTrue(t.Contains(5), "contains should find inserted key");  // Validate contains.  
                AssertTrue(!t.Insert(5), "duplicate insert should be rejected");  // Duplicate insert.  
                AssertEqual(1, t.Size(), "size should remain 1 after duplicate");  // Validate size.  
                AssertTrue(!t.Contains(99), "contains should be false for missing key");  // Validate missing.  
            }  // Close insert scope.  

            {  // Open scope: inorder/min/max test.  
                int[] values = new int[] { 8, 3, 10, 1, 6, 14, 4, 7, 13 };  // Classic BST insert order.  
                var t = BstDemo.BinarySearchTree.FromValues(values);  // Build BST by inserting in order.  
                AssertEqual(9, t.Size(), "size should be 9");  // Validate size.  
                AssertEqual(3, t.Height(), "height should be 3");  // Validate height.  
                AssertNullableEqual(1, t.Min(), "min should be 1");  // Validate min.  
                AssertNullableEqual(14, t.Max(), "max should be 14");  // Validate max.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 7, 8, 10, 13, 14 }, t.Inorder(), "inorder should be sorted");  // Validate inorder.  
            }  // Close inorder scope.  

            {  // Open scope: delete cases test.  
                int[] values = new int[] { 8, 3, 10, 1, 6, 14, 4, 7, 13 };  // Build a tree with multiple delete cases.  
                var t = BstDemo.BinarySearchTree.FromValues(values);  // Build BST by insertion.  
                AssertTrue(t.Delete(7), "delete leaf (7) should succeed");  // Delete leaf node.  
                AssertTrue(!t.Contains(7), "deleted key (7) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 8, 10, 13, 14 }, t.Inorder(), "inorder after deleting 7 should match");  // Validate inorder.  
                AssertTrue(t.Delete(14), "delete one-child node (14) should succeed");  // Delete one-child node.  
                AssertTrue(!t.Contains(14), "deleted key (14) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 8, 10, 13 }, t.Inorder(), "inorder after deleting 14 should match");  // Validate inorder.  
                AssertTrue(t.Delete(3), "delete two-child node (3) should succeed");  // Delete two-child node.  
                AssertTrue(!t.Contains(3), "deleted key (3) should be missing");  // Validate deletion.  
                AssertListEqual(new List<int> { 1, 4, 6, 8, 10, 13 }, t.Inorder(), "inorder after deleting 3 should match");  // Validate inorder.  
                AssertEqual(2, t.Height(), "height should be 2 after deletions");  // Validate height change.  
                AssertNullableEqual(1, t.Min(), "min should remain 1");  // Validate min.  
                AssertNullableEqual(13, t.Max(), "max should become 13");  // Validate max.  
            }  // Close delete-cases scope.  

            {  // Open scope: delete root test.  
                int[] values = new int[] { 8, 3, 10, 1, 6, 14, 4, 7, 13 };  // Root (8) has two children.  
                var t = BstDemo.BinarySearchTree.FromValues(values);  // Build BST by insertion.  
                AssertTrue(t.Delete(8), "delete root (8) should succeed");  // Delete root.  
                AssertTrue(!t.Contains(8), "deleted root key (8) should be missing");  // Validate deletion.  
                AssertEqual(8, t.Size(), "size should be 8 after deleting root");  // Validate size.  
                AssertListEqual(new List<int> { 1, 3, 4, 6, 7, 10, 13, 14 }, t.Inorder(), "inorder should remain sorted without 8");  // Validate inorder.  
                AssertNullableEqual(1, t.Min(), "min should stay 1");  // Validate min.  
                AssertNullableEqual(14, t.Max(), "max should stay 14");  // Validate max.  
            }  // Close delete-root scope.  
        }  // Close RunTests.  

        private static string FormatList(IReadOnlyList<int> values)  // Format list as [a, b, c].  
        {  // Open method scope.  
            return "[" + string.Join(", ", values) + "]";  // Join values with comma+space.  
        }  // Close FormatList.  

        private static string FormatNullable(int? v)  // Format nullable int as number or "null".  
        {  // Open method scope.  
            return v.HasValue ? v.Value.ToString() : "null";  // Return number or null marker.  
        }  // Close FormatNullable.  

        private static string FormatDemo()  // Format a small demo output using a classic BST example.  
        {  // Open method scope.  
            int[] values = new int[] { 8, 3, 10, 1, 6, 14, 4, 7, 13 };  // Classic BST insert order used in many textbooks.  
            var tree = BstDemo.BinarySearchTree.FromValues(values);  // Build BST by inserting in order.  
            BstDemo.TreeSummary s = tree.Summarize();  // Summarize properties and inorder traversal.  
            var lines = new List<string>();  // Accumulate printable lines.  
            lines.Add("=== Binary Search Tree Demo (C#) ===");  // Print header.  
            lines.Add("insert order: [8, 3, 10, 1, 6, 14, 4, 7, 13]");  // Print insertion order (fixed for this demo).  
            lines.Add($"size={s.Size}, height={s.Height}, min={FormatNullable(s.MinValue)}, max={FormatNullable(s.MaxValue)}");  // Print key properties.  
            lines.Add($"inorder(sorted)={FormatList(s.Inorder)}");  // Print inorder traversal.  
            lines.Add($"contains(6)={tree.Contains(6)}, contains(99)={tree.Contains(99)}");  // Print search examples.  
            tree.Delete(7);  // Delete a leaf (7).  
            tree.Delete(14);  // Delete a one-child node (14 has left child 13).  
            tree.Delete(3);  // Delete a two-child node (3) using successor replacement.  
            lines.Add($"after deletes (7,14,3) inorder={FormatList(tree.Inorder())}");  // Show inorder after deletions.  
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

