// 03 樹走訪示範程式（C#）/ Tree traversal demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for expected traversal sequences.  

namespace TreeTraversalUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class Program  // Console entry point for the demo and the built-in tests.  
    {  // Open class scope.  
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
            {  // Open scope: empty tree test.  
                var t = TreeTraversalDemo.BinaryTree.FromLevelOrder(Array.Empty<int?>());  // Build empty tree.  
                AssertListEqual(new List<int>(), t.PreorderRecursive(), "preorderRecursive should be empty");  // Validate preorderRecursive.  
                AssertListEqual(new List<int>(), t.PreorderIterative(), "preorderIterative should be empty");  // Validate preorderIterative.  
                AssertListEqual(new List<int>(), t.InorderRecursive(), "inorderRecursive should be empty");  // Validate inorderRecursive.  
                AssertListEqual(new List<int>(), t.InorderIterative(), "inorderIterative should be empty");  // Validate inorderIterative.  
                AssertListEqual(new List<int>(), t.PostorderRecursive(), "postorderRecursive should be empty");  // Validate postorderRecursive.  
                AssertListEqual(new List<int>(), t.PostorderIterative(), "postorderIterative should be empty");  // Validate postorderIterative.  
                AssertListEqual(new List<int>(), t.LevelOrder(), "levelOrder should be empty");  // Validate levelOrder.  
            }  // Close empty scope.  

            {  // Open scope: sample tree test.  
                var t = TreeTraversalDemo.BinaryTree.FromLevelOrder(new int?[] { 1, 2, 3, 4, 5 });  // Build sample tree.  
                var expectedPre = new List<int> { 1, 2, 4, 5, 3 };  // Expected preorder.  
                var expectedIn = new List<int> { 4, 2, 5, 1, 3 };  // Expected inorder.  
                var expectedPost = new List<int> { 4, 5, 2, 3, 1 };  // Expected postorder.  
                var expectedLevel = new List<int> { 1, 2, 3, 4, 5 };  // Expected level-order.  
                AssertListEqual(expectedPre, t.PreorderRecursive(), "preorderRecursive should match");  // Validate preorderRecursive.  
                AssertListEqual(expectedPre, t.PreorderIterative(), "preorderIterative should match");  // Validate preorderIterative.  
                AssertListEqual(expectedIn, t.InorderRecursive(), "inorderRecursive should match");  // Validate inorderRecursive.  
                AssertListEqual(expectedIn, t.InorderIterative(), "inorderIterative should match");  // Validate inorderIterative.  
                AssertListEqual(expectedPost, t.PostorderRecursive(), "postorderRecursive should match");  // Validate postorderRecursive.  
                AssertListEqual(expectedPost, t.PostorderIterative(), "postorderIterative should match");  // Validate postorderIterative.  
                AssertListEqual(expectedLevel, t.LevelOrder(), "levelOrder should match");  // Validate levelOrder.  
            }  // Close sample scope.  

            {  // Open scope: holes tree test.  
                var t = TreeTraversalDemo.BinaryTree.FromLevelOrder(new int?[] { 1, 2, 3, null, 5, null, 7 });  // Build tree with holes.  
                var expectedPre = new List<int> { 1, 2, 5, 3, 7 };  // Expected preorder.  
                var expectedIn = new List<int> { 2, 5, 1, 3, 7 };  // Expected inorder.  
                var expectedPost = new List<int> { 5, 2, 7, 3, 1 };  // Expected postorder.  
                var expectedLevel = new List<int> { 1, 2, 3, 5, 7 };  // Expected level-order.  
                AssertListEqual(expectedPre, t.PreorderRecursive(), "preorderRecursive (holes) should match");  // Validate preorderRecursive.  
                AssertListEqual(expectedPre, t.PreorderIterative(), "preorderIterative (holes) should match");  // Validate preorderIterative.  
                AssertListEqual(expectedIn, t.InorderRecursive(), "inorderRecursive (holes) should match");  // Validate inorderRecursive.  
                AssertListEqual(expectedIn, t.InorderIterative(), "inorderIterative (holes) should match");  // Validate inorderIterative.  
                AssertListEqual(expectedPost, t.PostorderRecursive(), "postorderRecursive (holes) should match");  // Validate postorderRecursive.  
                AssertListEqual(expectedPost, t.PostorderIterative(), "postorderIterative (holes) should match");  // Validate postorderIterative.  
                AssertListEqual(expectedLevel, t.LevelOrder(), "levelOrder (holes) should match");  // Validate levelOrder.  
            }  // Close holes scope.  
        }  // Close RunTests.  

        private static string FormatList(IReadOnlyList<int> values)  // Format list as [a, b, c].  
        {  // Open method scope.  
            return "[" + string.Join(", ", values) + "]";  // Join values with comma+space.  
        }  // Close FormatList.  

        private static string FormatDemo()  // Format a small demo output using a sample tree.  
        {  // Open method scope.  
            int?[] values = new int?[] { 1, 2, 3, 4, 5 };  // Sample tree used across this repo for traversal examples.  
            var tree = TreeTraversalDemo.BinaryTree.FromLevelOrder(values);  // Build tree from level-order list.  
            TreeTraversalDemo.TraversalSummary s = tree.Summarize();  // Summarize all traversal variants.  
            var lines = new List<string>();  // Accumulate printable lines.  
            lines.Add("=== Tree Traversal Demo (C#) ===");  // Print header.  
            lines.Add("level_order input: [1, 2, 3, 4, 5]");  // Print input (fixed for this demo).  
            lines.Add($"preorder_recursive={FormatList(s.PreorderRecursive)}");  // Print recursive preorder.  
            lines.Add($"preorder_iterative={FormatList(s.PreorderIterative)}");  // Print iterative preorder.  
            lines.Add($"inorder_recursive={FormatList(s.InorderRecursive)}");  // Print recursive inorder.  
            lines.Add($"inorder_iterative={FormatList(s.InorderIterative)}");  // Print iterative inorder.  
            lines.Add($"postorder_recursive={FormatList(s.PostorderRecursive)}");  // Print recursive postorder.  
            lines.Add($"postorder_iterative={FormatList(s.PostorderIterative)}");  // Print iterative postorder.  
            lines.Add($"level_order={FormatList(s.LevelOrder)}");  // Print level-order BFS.  
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

