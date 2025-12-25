// 01 二元樹示範程式（C#）/ Binary tree demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for expected traversal sequences.  

namespace BinaryTreeUnit  // Keep this unit isolated within its own namespace.  
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
                var t = BinaryTreeDemo.BinaryTree.FromLevelOrder(Array.Empty<int?>());  // Build tree from empty array.  
                AssertTrue(t.IsEmpty, "tree should be empty");  // Validate emptiness.  
                AssertEqual(0, t.Size(), "size should be 0");  // Validate size.  
                AssertEqual(-1, t.Height(), "height should be -1 for empty");  // Validate height definition.  
                AssertEqual(0, t.CountLeaves(), "leaves should be 0 for empty");  // Validate leaves.  
                AssertListEqual(new List<int>(), t.Preorder(), "preorder should be empty");  // Validate preorder.  
                AssertListEqual(new List<int>(), t.Inorder(), "inorder should be empty");  // Validate inorder.  
                AssertListEqual(new List<int>(), t.Postorder(), "postorder should be empty");  // Validate postorder.  
                AssertListEqual(new List<int>(), t.LevelOrder(), "level_order should be empty");  // Validate BFS.  
            }  // Close empty scope.  

            {  // Open scope: single node test.  
                var t = BinaryTreeDemo.BinaryTree.FromLevelOrder(new int?[] { 1 });  // Build tree from one value.  
                AssertTrue(!t.IsEmpty, "tree should not be empty");  // Validate non-empty.  
                AssertEqual(1, t.Size(), "size should be 1");  // Validate size.  
                AssertEqual(0, t.Height(), "height should be 0 for leaf");  // Validate height.  
                AssertEqual(1, t.CountLeaves(), "leaves should be 1");  // Validate leaves.  
                AssertListEqual(new List<int> { 1 }, t.Preorder(), "preorder should be [1]");  // Validate preorder.  
                AssertListEqual(new List<int> { 1 }, t.Inorder(), "inorder should be [1]");  // Validate inorder.  
                AssertListEqual(new List<int> { 1 }, t.Postorder(), "postorder should be [1]");  // Validate postorder.  
                AssertListEqual(new List<int> { 1 }, t.LevelOrder(), "level_order should be [1]");  // Validate BFS.  
            }  // Close single-node scope.  

            {  // Open scope: sample tree test.  
                var t = BinaryTreeDemo.BinaryTree.FromLevelOrder(new int?[] { 1, 2, 3, 4, 5 });  // Build sample tree.  
                AssertEqual(5, t.Size(), "size should be 5");  // Validate size.  
                AssertEqual(2, t.Height(), "height should be 2 (edges)");  // Validate height.  
                AssertEqual(3, t.CountLeaves(), "leaves should be 3");  // Validate leaves.  
                AssertListEqual(new List<int> { 1, 2, 4, 5, 3 }, t.Preorder(), "preorder should match");  // Validate preorder.  
                AssertListEqual(new List<int> { 4, 2, 5, 1, 3 }, t.Inorder(), "inorder should match");  // Validate inorder.  
                AssertListEqual(new List<int> { 4, 5, 2, 3, 1 }, t.Postorder(), "postorder should match");  // Validate postorder.  
                AssertListEqual(new List<int> { 1, 2, 3, 4, 5 }, t.LevelOrder(), "level_order should match");  // Validate BFS.  
            }  // Close sample scope.  

            {  // Open scope: holes test.  
                var t = BinaryTreeDemo.BinaryTree.FromLevelOrder(new int?[] { 1, 2, 3, null, 5, null, 7 });  // Build tree with holes.  
                AssertEqual(5, t.Size(), "size should be 5");  // Validate size.  
                AssertEqual(2, t.Height(), "height should be 2 (edges)");  // Validate height.  
                AssertEqual(2, t.CountLeaves(), "leaves should be 2");  // Validate leaves.  
                AssertListEqual(new List<int> { 1, 2, 5, 3, 7 }, t.Preorder(), "preorder should match");  // Validate preorder.  
                AssertListEqual(new List<int> { 2, 5, 1, 3, 7 }, t.Inorder(), "inorder should match");  // Validate inorder.  
                AssertListEqual(new List<int> { 5, 2, 7, 3, 1 }, t.Postorder(), "postorder should match");  // Validate postorder.  
                AssertListEqual(new List<int> { 1, 2, 3, 5, 7 }, t.LevelOrder(), "level_order should match");  // Validate BFS.  
            }  // Close holes scope.  

            {  // Open scope: null root test.  
                var t = BinaryTreeDemo.BinaryTree.FromLevelOrder(new int?[] { null, 1, 2 });  // Root is missing.  
                AssertTrue(t.IsEmpty, "tree should be empty when root is null");  // Validate emptiness.  
                AssertEqual(0, t.Size(), "size should be 0");  // Validate size.  
                AssertEqual(-1, t.Height(), "height should be -1 for empty");  // Validate height.  
                AssertEqual(0, t.CountLeaves(), "leaves should be 0");  // Validate leaves.  
                AssertListEqual(new List<int>(), t.Preorder(), "preorder should be empty");  // Validate preorder.  
                AssertListEqual(new List<int>(), t.Inorder(), "inorder should be empty");  // Validate inorder.  
                AssertListEqual(new List<int>(), t.Postorder(), "postorder should be empty");  // Validate postorder.  
                AssertListEqual(new List<int>(), t.LevelOrder(), "level_order should be empty");  // Validate BFS.  
            }  // Close null-root scope.  
        }  // Close RunTests.  

        private static string FormatList(IReadOnlyList<int> values)  // Format list as [a, b, c].  
        {  // Open method scope.  
            return "[" + string.Join(", ", values) + "]";  // Join values with comma+space.  
        }  // Close FormatList.  

        private static string FormatDemo()  // Format a small demo output using a sample tree.  
        {  // Open method scope.  
            int?[] values = new int?[] { 1, 2, 3, 4, 5 };  // Sample tree: 1 with children 2 and 3; 2 has 4 and 5.  
            var tree = BinaryTreeDemo.BinaryTree.FromLevelOrder(values);  // Build tree from level-order list.  
            BinaryTreeDemo.TreeSummary s = tree.Summarize();  // Summarize properties and traversals.  
            var lines = new List<string>();  // Accumulate printable lines.  
            lines.Add("=== Binary Tree Demo (C#) ===");  // Print header.  
            lines.Add("level_order input: [1, 2, 3, 4, 5]");  // Print input (fixed for this demo).  
            lines.Add($"size={s.Size}, height={s.Height}, leaves={s.Leaves}");  // Print key properties.  
            lines.Add($"preorder={FormatList(s.Preorder)}");  // Print preorder list.  
            lines.Add($"inorder={FormatList(s.Inorder)}");  // Print inorder list.  
            lines.Add($"postorder={FormatList(s.Postorder)}");  // Print postorder list.  
            lines.Add($"level_order={FormatList(s.LevelOrder)}");  // Print BFS list.  
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

