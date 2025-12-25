// 01 二元樹示範（C#）/ Binary tree demo (C#).  // Bilingual file header.

using System;  // Provide basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for traversal results and BFS queue storage.  

namespace BinaryTreeUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class BinaryTreeDemo  // Host BinaryTree implementation and helpers.  
    {  // Open class scope.  
        internal sealed class Node  // Binary tree node (value + left child + right child).  
        {  // Open class scope.  
            public Node(int value)  // Initialize node with a value and empty children.  
            {  // Open constructor scope.  
                Value = value;  // Store node value.  
                Left = null;  // Initialize left child as missing.  
                Right = null;  // Initialize right child as missing.  
            }  // Close constructor scope.  

            public int Value { get; }  // Expose node value as read-only.  
            public Node? Left { get; set; }  // Expose left child pointer (null means no child).  
            public Node? Right { get; set; }  // Expose right child pointer (null means no child).  
        }  // Close Node.  

        internal readonly struct TreeSummary  // Summarize key properties and traversal sequences.  
        {  // Open struct scope.  
            public TreeSummary(int size, int height, int leaves, IReadOnlyList<int> preorder, IReadOnlyList<int> inorder, IReadOnlyList<int> postorder, IReadOnlyList<int> levelOrder)  // Construct immutable summary.  
            {  // Open constructor scope.  
                Size = size;  // Store node count.  
                Height = height;  // Store height in edges.  
                Leaves = leaves;  // Store leaf count.  
                Preorder = preorder;  // Store preorder traversal.  
                Inorder = inorder;  // Store inorder traversal.  
                Postorder = postorder;  // Store postorder traversal.  
                LevelOrder = levelOrder;  // Store level-order traversal.  
            }  // Close constructor scope.  

            public int Size { get; }  // Number of nodes in the tree.  
            public int Height { get; }  // Height measured in edges (empty=-1, leaf=0).  
            public int Leaves { get; }  // Number of leaf nodes.  
            public IReadOnlyList<int> Preorder { get; }  // Preorder traversal (root, left, right).  
            public IReadOnlyList<int> Inorder { get; }  // Inorder traversal (left, root, right).  
            public IReadOnlyList<int> Postorder { get; }  // Postorder traversal (left, right, root).  
            public IReadOnlyList<int> LevelOrder { get; }  // Level-order traversal (BFS).  
        }  // Close TreeSummary.  

        internal sealed class BinaryTree  // A binary tree wrapper that holds a root node.  
        {  // Open class scope.  
            private readonly Node? _root;  // Store root reference (null means empty tree).  

            public BinaryTree(Node? root)  // Initialize tree with an optional root.  
            {  // Open constructor scope.  
                _root = root;  // Store root reference.  
            }  // Close constructor scope.  

            public Node? Root => _root;  // Expose root for demos/tests (read-only semantics by convention).  
            public bool IsEmpty => _root is null;  // Expose emptiness check.  

            public static BinaryTree FromLevelOrder(int?[] values)  // Build a tree from level-order array representation (null means missing node).  
            {  // Open method scope.  
                if (values is null || values.Length == 0)  // Treat null/empty array as an empty tree.  
                {  // Open empty branch.  
                    return new BinaryTree(root: null);  // Return empty tree.  
                }  // Close empty branch.  

                var nodes = new Node?[values.Length];  // Allocate parallel node array (null for missing).  
                for (int i = 0; i < values.Length; i++)  // First pass: create nodes for non-null values.  
                {  // Open loop scope.  
                    int? v = values[i];  // Read value at index i.  
                    nodes[i] = v.HasValue ? new Node(v.Value) : null;  // Create node or keep null.  
                }  // Close loop scope.  

                for (int i = 0; i < values.Length; i++)  // Second pass: link children using array indices.  
                {  // Open loop scope.  
                    Node? node = nodes[i];  // Read node at index i.  
                    if (node is null)  // Skip missing nodes.  
                    {  // Open skip branch.  
                        continue;  // Move to next index.  
                    }  // Close skip branch.  
                    int leftIndex = 2 * i + 1;  // Compute left child index.  
                    int rightIndex = 2 * i + 2;  // Compute right child index.  
                    if (leftIndex < nodes.Length)  // Link left child when index is in range.  
                    {  // Open left link scope.  
                        node.Left = nodes[leftIndex];  // Assign left child (may be null).  
                    }  // Close left link scope.  
                    if (rightIndex < nodes.Length)  // Link right child when index is in range.  
                    {  // Open right link scope.  
                        node.Right = nodes[rightIndex];  // Assign right child (may be null).  
                    }  // Close right link scope.  
                }  // Close loop scope.  

                return new BinaryTree(root: nodes[0]);  // Root is the node at index 0 (may be null).  
            }  // Close FromLevelOrder.  

            public int Size()  // Return number of nodes in the tree (O(n)).  
            {  // Open method scope.  
                return SizeOf(_root);  // Count nodes starting from root.  
            }  // Close Size.  

            private static int SizeOf(Node? node)  // Recursive helper that counts nodes in a subtree.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes 0 nodes.  
                {  // Open base case.  
                    return 0;  // Return 0 for empty.  
                }  // Close base case.  
                return 1 + SizeOf(node.Left) + SizeOf(node.Right);  // Count root + left + right.  
            }  // Close SizeOf.  

            public int Height()  // Return tree height measured in edges (empty=-1, leaf=0).  
            {  // Open method scope.  
                return HeightOf(_root);  // Compute height from root.  
            }  // Close Height.  

            private static int HeightOf(Node? node)  // Recursive helper that computes height in edges.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree has height -1 (so leaf has height 0).  
                {  // Open base case.  
                    return -1;  // Return -1 for empty.  
                }  // Close base case.  
                int leftH = HeightOf(node.Left);  // Compute left subtree height.  
                int rightH = HeightOf(node.Right);  // Compute right subtree height.  
                return 1 + Math.Max(leftH, rightH);  // Height is 1 + max(child heights).  
            }  // Close HeightOf.  

            public int CountLeaves()  // Return number of leaf nodes (O(n)).  
            {  // Open method scope.  
                return LeavesOf(_root);  // Count leaves from root.  
            }  // Close CountLeaves.  

            private static int LeavesOf(Node? node)  // Recursive helper that counts leaf nodes in a subtree.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes 0 leaves.  
                {  // Open base case.  
                    return 0;  // Return 0 for empty.  
                }  // Close base case.  
                if (node.Left is null && node.Right is null)  // Node with no children is a leaf.  
                {  // Open leaf case.  
                    return 1;  // Count this leaf.  
                }  // Close leaf case.  
                return LeavesOf(node.Left) + LeavesOf(node.Right);  // Sum leaves in both subtrees.  
            }  // Close LeavesOf.  

            public List<int> Preorder()  // Return preorder traversal as a list of values.  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                WalkPreorder(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close Preorder.  

            private static void WalkPreorder(Node? node, List<int> outList)  // Recursive preorder (root, left, right).  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                outList.Add(node.Value);  // Visit root first.  
                WalkPreorder(node.Left, outList);  // Visit left subtree.  
                WalkPreorder(node.Right, outList);  // Visit right subtree.  
            }  // Close WalkPreorder.  

            public List<int> Inorder()  // Return inorder traversal as a list of values.  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                WalkInorder(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close Inorder.  

            private static void WalkInorder(Node? node, List<int> outList)  // Recursive inorder (left, root, right).  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                WalkInorder(node.Left, outList);  // Visit left subtree first.  
                outList.Add(node.Value);  // Visit root.  
                WalkInorder(node.Right, outList);  // Visit right subtree.  
            }  // Close WalkInorder.  

            public List<int> Postorder()  // Return postorder traversal as a list of values.  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                WalkPostorder(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close Postorder.  

            private static void WalkPostorder(Node? node, List<int> outList)  // Recursive postorder (left, right, root).  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                WalkPostorder(node.Left, outList);  // Visit left subtree first.  
                WalkPostorder(node.Right, outList);  // Visit right subtree next.  
                outList.Add(node.Value);  // Visit root last.  
            }  // Close WalkPostorder.  

            public List<int> LevelOrder()  // Return level-order traversal (BFS) as a list of values.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has empty traversal.  
                {  // Open empty branch.  
                    return new List<int>();  // Return empty list.  
                }  // Close empty branch.  

                var queue = new List<Node> { _root };  // Use List as queue storage (initially contains root).  
                int head = 0;  // Maintain a head index so dequeue is O(1) without shifting.  
                var outList = new List<int>();  // Accumulate traversal result.  

                while (head < queue.Count)  // Process queue until head reaches the end.  
                {  // Open loop scope.  
                    Node node = queue[head];  // Read next node to process.  
                    head += 1;  // Advance head (logical dequeue).  
                    outList.Add(node.Value);  // Visit node in BFS order.  
                    if (node.Left is not null)  // Enqueue left child when present.  
                    {  // Open left enqueue scope.  
                        queue.Add(node.Left);  // Append to queue tail.  
                    }  // Close left enqueue scope.  
                    if (node.Right is not null)  // Enqueue right child when present.  
                    {  // Open right enqueue scope.  
                        queue.Add(node.Right);  // Append to queue tail.  
                    }  // Close right enqueue scope.  
                }  // Close loop scope.  

                return outList;  // Return traversal list.  
            }  // Close LevelOrder.  

            public TreeSummary Summarize()  // Build a snapshot summary for demos/tests.  
            {  // Open method scope.  
                return new TreeSummary(  // Construct immutable summary record.  
                    size: Size(),  // Compute node count.  
                    height: Height(),  // Compute height (edges).  
                    leaves: CountLeaves(),  // Compute leaf count.  
                    preorder: Preorder(),  // Compute preorder traversal.  
                    inorder: Inorder(),  // Compute inorder traversal.  
                    postorder: Postorder(),  // Compute postorder traversal.  
                    levelOrder: LevelOrder()  // Compute level-order traversal.  
                );  // Close summary construction.  
            }  // Close Summarize.  
        }  // Close BinaryTree.  
    }  // Close class scope.  
}  // Close namespace scope.  

