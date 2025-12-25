// 02 二元搜尋樹示範（C#）/ Binary Search Tree (BST) demo (C#).  // Bilingual file header.

using System;  // Provide basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for inorder traversal results.  

namespace BstUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class BstDemo  // Host BinarySearchTree implementation and helpers.  
    {  // Open class scope.  
        internal sealed class Node  // BST node (key + left child + right child).  
        {  // Open class scope.  
            public Node(int key)  // Initialize node with a key and empty children.  
            {  // Open constructor scope.  
                Key = key;  // Store node key.  
                Left = null;  // Initialize left child as missing.  
                Right = null;  // Initialize right child as missing.  
            }  // Close constructor scope.  

            public int Key { get; set; }  // Store key (mutable so delete can replace with successor key).  
            public Node? Left { get; set; }  // Left child pointer (null means no child).  
            public Node? Right { get; set; }  // Right child pointer (null means no child).  
        }  // Close Node.  

        internal readonly struct TreeSummary  // Summarize key properties and inorder traversal for printing/tests.  
        {  // Open struct scope.  
            public TreeSummary(int size, int height, int? minValue, int? maxValue, IReadOnlyList<int> inorder)  // Construct immutable summary.  
            {  // Open constructor scope.  
                Size = size;  // Store node count.  
                Height = height;  // Store height in edges.  
                MinValue = minValue;  // Store minimum key (null if empty).  
                MaxValue = maxValue;  // Store maximum key (null if empty).  
                Inorder = inorder;  // Store inorder traversal (sorted keys for a valid BST).  
            }  // Close constructor scope.  

            public int Size { get; }  // Number of nodes in the BST.  
            public int Height { get; }  // Height measured in edges (empty=-1, leaf=0).  
            public int? MinValue { get; }  // Minimum key (null if empty).  
            public int? MaxValue { get; }  // Maximum key (null if empty).  
            public IReadOnlyList<int> Inorder { get; }  // Inorder traversal (sorted for a valid BST).  
        }  // Close TreeSummary.  

        internal sealed class BinarySearchTree  // A BST wrapper that holds a root node.  
        {  // Open class scope.  
            private Node? _root;  // Store root reference (null means empty tree).  

            public BinarySearchTree(Node? root)  // Initialize BST with an optional root.  
            {  // Open constructor scope.  
                _root = root;  // Store root reference.  
            }  // Close constructor scope.  

            public Node? Root => _root;  // Expose root for demos/tests (read-only semantics by convention).  
            public bool IsEmpty => _root is null;  // Expose emptiness check.  

            public static BinarySearchTree FromValues(int[] values)  // Build a BST by inserting values in order.  
            {  // Open method scope.  
                var t = new BinarySearchTree(root: null);  // Start from an empty tree.  
                foreach (int v in values)  // Insert each value in the given order.  
                {  // Open foreach scope.  
                    t.Insert(v);  // Insert one key (duplicates are ignored by policy).  
                }  // Close foreach scope.  
                return t;  // Return built BST.  
            }  // Close FromValues.  

            public bool Contains(int key)  // Return true if key exists in the BST.  
            {  // Open method scope.  
                Node? node = _root;  // Start searching from the root.  
                while (node is not null)  // Walk down until we hit a missing child.  
                {  // Open loop scope.  
                    if (key < node.Key)  // Go left when key is smaller.  
                    {  // Open branch scope.  
                        node = node.Left;  // Move to left child.  
                    }  // Close branch scope.  
                    else if (key > node.Key)  // Go right when key is larger.  
                    {  // Open branch scope.  
                        node = node.Right;  // Move to right child.  
                    }  // Close branch scope.  
                    else  // key == node.Key means we found it.  
                    {  // Open branch scope.  
                        return true;  // Report found.  
                    }  // Close branch scope.  
                }  // Close loop scope.  
                return false;  // Report not found when search falls off the tree.  
            }  // Close Contains.  

            public bool Insert(int key)  // Insert key; return false when key already exists.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree case: inserted node becomes the root.  
                {  // Open empty case.  
                    _root = new Node(key);  // Create root node.  
                    return true;  // Report inserted.  
                }  // Close empty case.  

                Node? parent = null;  // Track parent so we can attach the new node.  
                Node? node = _root;  // Start from root.  
                while (node is not null)  // Walk down until we find an empty spot.  
                {  // Open loop scope.  
                    parent = node;  // Update parent before moving.  
                    if (key < node.Key)  // Go left when key is smaller.  
                    {  // Open branch scope.  
                        node = node.Left;  // Move to left child.  
                    }  // Close branch scope.  
                    else if (key > node.Key)  // Go right when key is larger.  
                    {  // Open branch scope.  
                        node = node.Right;  // Move to right child.  
                    }  // Close branch scope.  
                    else  // key == node.Key means duplicate key.  
                    {  // Open branch scope.  
                        return false;  // Reject duplicates by policy.  
                    }  // Close branch scope.  
                }  // Close loop scope.  

                var newNode = new Node(key);  // Create the new leaf node.  
                if (key < parent!.Key)  // Attach to the left side when key is smaller.  
                {  // Open attach scope.  
                    parent.Left = newNode;  // Link as left child.  
                }  // Close attach scope.  
                else  // Otherwise attach to the right side.  
                {  // Open attach scope.  
                    parent.Right = newNode;  // Link as right child.  
                }  // Close attach scope.  
                return true;  // Report inserted.  
            }  // Close Insert.  

            private readonly struct DeleteResult  // Return (newRoot, removed) for recursive deletes.  
            {  // Open struct scope.  
                public DeleteResult(Node? newRoot, bool removed)  // Construct immutable result.  
                {  // Open constructor scope.  
                    NewRoot = newRoot;  // Store new subtree root.  
                    Removed = removed;  // Store removed flag.  
                }  // Close constructor scope.  

                public Node? NewRoot { get; }  // The new subtree root after deletion.  
                public bool Removed { get; }  // Whether a node was removed.  
            }  // Close DeleteResult.  

            public bool Delete(int key)  // Delete key; return true when a node was removed.  
            {  // Open method scope.  
                DeleteResult r = DeleteSubtree(_root, key);  // Delete from root.  
                _root = r.NewRoot;  // Update root pointer (may change when deleting root).  
                return r.Removed;  // Return whether deletion occurred.  
            }  // Close Delete.  

            private static DeleteResult DeleteSubtree(Node? node, int key)  // Recursive delete that returns (newRoot, removed).  
            {  // Open method scope.  
                if (node is null)  // Key not found in an empty subtree.  
                {  // Open base case.  
                    return new DeleteResult(newRoot: null, removed: false);  // Nothing removed.  
                }  // Close base case.  

                if (key < node.Key)  // Delete from left subtree when key is smaller.  
                {  // Open left branch.  
                    DeleteResult r = DeleteSubtree(node.Left, key);  // Recurse into left child.  
                    node.Left = r.NewRoot;  // Update left child pointer.  
                    return new DeleteResult(newRoot: node, removed: r.Removed);  // Return updated subtree root.  
                }  // Close left branch.  
                if (key > node.Key)  // Delete from right subtree when key is larger.  
                {  // Open right branch.  
                    DeleteResult r = DeleteSubtree(node.Right, key);  // Recurse into right child.  
                    node.Right = r.NewRoot;  // Update right child pointer.  
                    return new DeleteResult(newRoot: node, removed: r.Removed);  // Return updated subtree root.  
                }  // Close right branch.  

                // Now key == node.Key, so we found the node to delete.  // Explain match branch.  
                if (node.Left is null && node.Right is null)  // Case 1: leaf node (0 children).  
                {  // Open leaf case.  
                    return new DeleteResult(newRoot: null, removed: true);  // Remove leaf by returning null.  
                }  // Close leaf case.  
                if (node.Left is null)  // Case 2a: only right child exists.  
                {  // Open right-only case.  
                    return new DeleteResult(newRoot: node.Right, removed: true);  // Replace node by its right child.  
                }  // Close right-only case.  
                if (node.Right is null)  // Case 2b: only left child exists.  
                {  // Open left-only case.  
                    return new DeleteResult(newRoot: node.Left, removed: true);  // Replace node by its left child.  
                }  // Close left-only case.  

                // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.  
                int successorKey = MinKey(node.Right!);  // Find successor key from right subtree.  
                node.Key = successorKey;  // Copy successor key into current node.  
                DeleteResult rr = DeleteSubtree(node.Right, successorKey);  // Delete successor node from right subtree.  
                node.Right = rr.NewRoot;  // Update right child pointer after successor removal.  
                return new DeleteResult(newRoot: node, removed: true);  // Report successful deletion.  
            }  // Close DeleteSubtree.  

            public int? Min()  // Return minimum key, or null if tree is empty.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has no minimum.  
                {  // Open empty case.  
                    return null;  // Return null for empty.  
                }  // Close empty case.  
                return MinKey(_root);  // Return leftmost key.  
            }  // Close Min.  

            private static int MinKey(Node node)  // Return the minimum key in a non-empty subtree.  
            {  // Open method scope.  
                Node cur = node;  // Start at given node.  
                while (cur.Left is not null)  // Keep going left until we cannot.  
                {  // Open loop scope.  
                    cur = cur.Left;  // Move to left child.  
                }  // Close loop scope.  
                return cur.Key;  // Leftmost node has the minimum key.  
            }  // Close MinKey.  

            public int? Max()  // Return maximum key, or null if tree is empty.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has no maximum.  
                {  // Open empty case.  
                    return null;  // Return null for empty.  
                }  // Close empty case.  
                Node cur = _root;  // Start at root.  
                while (cur.Right is not null)  // Keep going right until we cannot.  
                {  // Open loop scope.  
                    cur = cur.Right;  // Move to right child.  
                }  // Close loop scope.  
                return cur.Key;  // Rightmost node has the maximum key.  
            }  // Close Max.  

            public int Size()  // Return number of nodes in the BST (O(n)).  
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

            public int Height()  // Return height measured in edges (empty=-1, leaf=0).  
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

            public List<int> Inorder()  // Return inorder traversal (sorted for a valid BST).  
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
                outList.Add(node.Key);  // Visit root.  
                WalkInorder(node.Right, outList);  // Visit right subtree.  
            }  // Close WalkInorder.  

            public TreeSummary Summarize()  // Build a snapshot summary for demos/tests.  
            {  // Open method scope.  
                return new TreeSummary(  // Construct immutable summary record.  
                    size: Size(),  // Compute node count.  
                    height: Height(),  // Compute height (edges).  
                    minValue: Min(),  // Compute minimum key (nullable).  
                    maxValue: Max(),  // Compute maximum key (nullable).  
                    inorder: Inorder()  // Compute inorder traversal.  
                );  // Close summary construction.  
            }  // Close Summarize.  
        }  // Close BinarySearchTree.  
    }  // Close class scope.  
}  // Close namespace scope.  

