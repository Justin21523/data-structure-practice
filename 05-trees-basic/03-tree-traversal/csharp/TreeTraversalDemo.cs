// 03 樹走訪示範（C#）/ Tree traversal demo (C#).  // Bilingual file header.

using System;  // Provide basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for traversal outputs and stack/queue storage.  

namespace TreeTraversalUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class TreeTraversalDemo  // Host BinaryTree implementation and traversal helpers.  
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
            public Node? Left { get; set; }  // Left child pointer (null means no child).  
            public Node? Right { get; set; }  // Right child pointer (null means no child).  
        }  // Close Node.  

        internal readonly struct TraversalSummary  // Summarize recursive/iterative traversal outputs for printing/tests.  
        {  // Open struct scope.  
            public TraversalSummary(IReadOnlyList<int> preorderRecursive, IReadOnlyList<int> preorderIterative, IReadOnlyList<int> inorderRecursive, IReadOnlyList<int> inorderIterative, IReadOnlyList<int> postorderRecursive, IReadOnlyList<int> postorderIterative, IReadOnlyList<int> levelOrder)  // Construct immutable summary.  
            {  // Open constructor scope.  
                PreorderRecursive = preorderRecursive;  // Store recursive preorder.  
                PreorderIterative = preorderIterative;  // Store iterative preorder.  
                InorderRecursive = inorderRecursive;  // Store recursive inorder.  
                InorderIterative = inorderIterative;  // Store iterative inorder.  
                PostorderRecursive = postorderRecursive;  // Store recursive postorder.  
                PostorderIterative = postorderIterative;  // Store iterative postorder.  
                LevelOrder = levelOrder;  // Store level-order traversal.  
            }  // Close constructor scope.  

            public IReadOnlyList<int> PreorderRecursive { get; }  // Preorder via recursion.  
            public IReadOnlyList<int> PreorderIterative { get; }  // Preorder via stack.  
            public IReadOnlyList<int> InorderRecursive { get; }  // Inorder via recursion.  
            public IReadOnlyList<int> InorderIterative { get; }  // Inorder via stack.  
            public IReadOnlyList<int> PostorderRecursive { get; }  // Postorder via recursion.  
            public IReadOnlyList<int> PostorderIterative { get; }  // Postorder via stack+visited.  
            public IReadOnlyList<int> LevelOrder { get; }  // Level-order via BFS queue.  
        }  // Close TraversalSummary.  

        internal sealed class BinaryTree  // A binary tree wrapper that holds a root node.  
        {  // Open class scope.  
            private readonly Node? _root;  // Store root reference (null means empty tree).  

            public BinaryTree(Node? root)  // Initialize tree with an optional root.  
            {  // Open constructor scope.  
                _root = root;  // Store root reference.  
            }  // Close constructor scope.  

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

            public List<int> PreorderRecursive()  // Return preorder traversal via recursion (root, left, right).  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                WalkPreorder(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close PreorderRecursive.  

            private static void WalkPreorder(Node? node, List<int> outList)  // Recursive preorder helper.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                outList.Add(node.Value);  // Visit root first.  
                WalkPreorder(node.Left, outList);  // Visit left subtree.  
                WalkPreorder(node.Right, outList);  // Visit right subtree.  
            }  // Close WalkPreorder.  

            public List<int> InorderRecursive()  // Return inorder traversal via recursion (left, root, right).  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                WalkInorder(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close InorderRecursive.  

            private static void WalkInorder(Node? node, List<int> outList)  // Recursive inorder helper.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                WalkInorder(node.Left, outList);  // Visit left subtree first.  
                outList.Add(node.Value);  // Visit root.  
                WalkInorder(node.Right, outList);  // Visit right subtree.  
            }  // Close WalkInorder.  

            public List<int> PostorderRecursive()  // Return postorder traversal via recursion (left, right, root).  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                WalkPostorder(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close PostorderRecursive.  

            private static void WalkPostorder(Node? node, List<int> outList)  // Recursive postorder helper.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                WalkPostorder(node.Left, outList);  // Visit left subtree first.  
                WalkPostorder(node.Right, outList);  // Visit right subtree next.  
                outList.Add(node.Value);  // Visit root last.  
            }  // Close WalkPostorder.  

            public List<int> PreorderIterative()  // Return preorder traversal via explicit stack (root, left, right).  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has empty traversal.  
                {  // Open empty case.  
                    return new List<int>();  // Return empty list.  
                }  // Close empty case.  

                var outList = new List<int>();  // Accumulate traversal result.  
                var stack = new List<Node> { _root };  // Use List as stack; start with root.  
                while (stack.Count > 0)  // Process until stack is empty.  
                {  // Open loop scope.  
                    int lastIndex = stack.Count - 1;  // Compute last index for pop.  
                    Node node = stack[lastIndex];  // Read top node.  
                    stack.RemoveAt(lastIndex);  // Pop top node (LIFO).  
                    outList.Add(node.Value);  // Visit node when popped (preorder).  
                    if (node.Right is not null)  // Push right child first so left is processed first.  
                    {  // Open right push scope.  
                        stack.Add(node.Right);  // Push right child.  
                    }  // Close right push scope.  
                    if (node.Left is not null)  // Push left child last.  
                    {  // Open left push scope.  
                        stack.Add(node.Left);  // Push left child.  
                    }  // Close left push scope.  
                }  // Close loop scope.  
                return outList;  // Return traversal list.  
            }  // Close PreorderIterative.  

            public List<int> InorderIterative()  // Return inorder traversal via explicit stack (left, root, right).  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                var stack = new List<Node>();  // Use List as stack to store the path to current node.  
                Node? node = _root;  // Start from root.  

                while (node is not null || stack.Count > 0)  // Continue while there is a node to visit or stack is non-empty.  
                {  // Open loop scope.  
                    while (node is not null)  // Walk left as far as possible.  
                    {  // Open left-walk scope.  
                        stack.Add(node);  // Push current node so we can come back later.  
                        node = node.Left;  // Move to left child.  
                    }  // Close left-walk scope.  
                    int lastIndex = stack.Count - 1;  // Compute last index for pop.  
                    node = stack[lastIndex];  // Pop the next node whose left subtree is done.  
                    stack.RemoveAt(lastIndex);  // Remove popped element.  
                    outList.Add(node.Value);  // Visit node (inorder).  
                    node = node.Right;  // Move to right subtree next.  
                }  // Close loop scope.  

                return outList;  // Return traversal list.  
            }  // Close InorderIterative.  

            private readonly struct Frame  // Store a node plus a visited flag for iterative postorder.  
            {  // Open struct scope.  
                public Frame(Node node, bool visited)  // Construct immutable frame.  
                {  // Open constructor scope.  
                    Node = node;  // Store node.  
                    Visited = visited;  // Store visited flag.  
                }  // Close constructor scope.  

                public Node Node { get; }  // Node pointer for this frame.  
                public bool Visited { get; }  // True means children were already processed.  
            }  // Close Frame.  

            public List<int> PostorderIterative()  // Return postorder traversal via explicit stack (left, right, root).  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has empty traversal.  
                {  // Open empty case.  
                    return new List<int>();  // Return empty list.  
                }  // Close empty case.  

                var outList = new List<int>();  // Accumulate traversal result.  
                var stack = new List<Frame> { new Frame(_root, visited: false) };  // Store (node, visited) frames.  
                while (stack.Count > 0)  // Process until stack is empty.  
                {  // Open loop scope.  
                    int lastIndex = stack.Count - 1;  // Compute last index for pop.  
                    Frame f = stack[lastIndex];  // Read top frame.  
                    stack.RemoveAt(lastIndex);  // Pop top frame.  
                    if (f.Visited)  // When visited==true, children were already processed.  
                    {  // Open visited branch.  
                        outList.Add(f.Node.Value);  // Visit node last (postorder).  
                        continue;  // Move to next frame.  
                    }  // Close visited branch.  
                    stack.Add(new Frame(f.Node, visited: true));  // Re-push node marked as visited (to output after children).  
                    if (f.Node.Right is not null)  // Push right child first so left is processed first.  
                    {  // Open right push scope.  
                        stack.Add(new Frame(f.Node.Right, visited: false));  // Process right subtree.  
                    }  // Close right push scope.  
                    if (f.Node.Left is not null)  // Push left child last.  
                    {  // Open left push scope.  
                        stack.Add(new Frame(f.Node.Left, visited: false));  // Process left subtree first.  
                    }  // Close left push scope.  
                }  // Close loop scope.  
                return outList;  // Return traversal list.  
            }  // Close PostorderIterative.  

            public List<int> LevelOrder()  // Return level-order traversal (BFS) via queue.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has empty traversal.  
                {  // Open empty case.  
                    return new List<int>();  // Return empty list.  
                }  // Close empty case.  

                var queue = new List<Node> { _root };  // Use List as queue storage; start with root.  
                int head = 0;  // Maintain head index so dequeue is O(1) without shifting.  
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

            public TraversalSummary Summarize()  // Build a snapshot summary for demos/tests.  
            {  // Open method scope.  
                return new TraversalSummary(  // Construct immutable summary record.  
                    preorderRecursive: PreorderRecursive(),  // Compute recursive preorder.  
                    preorderIterative: PreorderIterative(),  // Compute iterative preorder.  
                    inorderRecursive: InorderRecursive(),  // Compute recursive inorder.  
                    inorderIterative: InorderIterative(),  // Compute iterative inorder.  
                    postorderRecursive: PostorderRecursive(),  // Compute recursive postorder.  
                    postorderIterative: PostorderIterative(),  // Compute iterative postorder.  
                    levelOrder: LevelOrder()  // Compute level-order BFS.  
                );  // Close summary construction.  
            }  // Close Summarize.  
        }  // Close BinaryTree.  
    }  // Close class scope.  
}  // Close namespace scope.  

