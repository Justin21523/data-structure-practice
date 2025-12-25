// 01 AVL Tree 示範（C#）/ AVL tree demo (C#).  // Bilingual file header.

using System;  // Provide basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for inorder traversal results.  

namespace AvlUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class AvlDemo  // Host AvlTree implementation and helpers.  
    {  // Open class scope.  
        internal sealed class Node  // AVL node (key + left child + right child + cached height).  
        {  // Open class scope.  
            public Node(int key)  // Initialize node with a key and empty children.  
            {  // Open constructor scope.  
                Key = key;  // Store node key.  
                Left = null;  // Initialize left child as missing.  
                Right = null;  // Initialize right child as missing.  
                Height = 0;  // Leaf height is 0 when height is measured in edges.  
            }  // Close constructor scope.  

            public int Key { get; set; }  // Store key (mutable so delete can replace with successor key).  
            public Node? Left { get; set; }  // Left child pointer (null means no child).  
            public Node? Right { get; set; }  // Right child pointer (null means no child).  
            public int Height { get; set; }  // Cached height in edges (leaf=0 by definition).  
        }  // Close Node.  

        internal readonly struct TreeSummary  // Summarize key properties and inorder traversal for printing/tests.  
        {  // Open struct scope.  
            public TreeSummary(int size, int height, int? minValue, int? maxValue, IReadOnlyList<int> inorder, bool isValid)  // Construct immutable summary.  
            {  // Open constructor scope.  
                Size = size;  // Store node count.  
                Height = height;  // Store height in edges.  
                MinValue = minValue;  // Store minimum key (null if empty).  
                MaxValue = maxValue;  // Store maximum key (null if empty).  
                Inorder = inorder;  // Store inorder traversal (sorted keys for a valid BST).  
                IsValid = isValid;  // Store validation flag.  
            }  // Close constructor scope.  

            public int Size { get; }  // Number of nodes in the AVL tree.  
            public int Height { get; }  // Height measured in edges (empty=-1, leaf=0).  
            public int? MinValue { get; }  // Minimum key (null if empty).  
            public int? MaxValue { get; }  // Maximum key (null if empty).  
            public IReadOnlyList<int> Inorder { get; }  // Inorder traversal (sorted for a valid BST).  
            public bool IsValid { get; }  // Whether the structure satisfies AVL + BST invariants.  
        }  // Close TreeSummary.  

        internal sealed class AvlTree  // AVL tree wrapper that holds a root node.  
        {  // Open class scope.  
            private Node? _root;  // Store root reference (null means empty tree).  

            public AvlTree(Node? root)  // Initialize AVL tree with an optional root.  
            {  // Open constructor scope.  
                _root = root;  // Store root reference.  
            }  // Close constructor scope.  

            public Node? Root => _root;  // Expose root for demos/tests (read-only semantics by convention).  
            public bool IsEmpty => _root is null;  // Expose emptiness check.  

            public static AvlTree FromValues(int[] values)  // Build an AVL tree by inserting values in order.  
            {  // Open method scope.  
                var t = new AvlTree(root: null);  // Start from an empty tree.  
                foreach (int v in values)  // Insert each value in the given order.  
                {  // Open foreach scope.  
                    t.Insert(v);  // Insert one key (duplicates are rejected by policy).  
                }  // Close foreach scope.  
                return t;  // Return built AVL tree.  
            }  // Close FromValues.  

            public bool Contains(int key)  // Return true if key exists in the tree.  
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

            private static int H(Node? node)  // Return cached height for node, or -1 for empty subtree.  
            {  // Open method scope.  
                return node is null ? -1 : node.Height;  // Apply repo-wide height convention.  
            }  // Close H.  

            private static void UpdateHeight(Node node)  // Recompute node.Height from children heights.  
            {  // Open method scope.  
                node.Height = 1 + Math.Max(H(node.Left), H(node.Right));  // Height in edges is 1 + max(child heights).  
            }  // Close UpdateHeight.  

            private static int BalanceFactor(Node node)  // Compute balance factor = height(left) - height(right).  
            {  // Open method scope.  
                return H(node.Left) - H(node.Right);  // Positive means left-heavy; negative means right-heavy.  
            }  // Close BalanceFactor.  

            private static Node RotateRight(Node y)  // Right rotation to fix a left-heavy imbalance.  
            {  // Open method scope.  
                Node x = y.Left!;  // Take left child as pivot (must exist).  
                Node? t2 = x.Right;  // Keep pivot's right subtree (will become y's left subtree).  

                x.Right = y;  // Put y as the right child of x.  
                y.Left = t2;  // Attach T2 as y's left child.  

                UpdateHeight(y);  // Update y first because its children changed.  
                UpdateHeight(x);  // Update x after y because x now points to y.  
                return x;  // Return new subtree root.  
            }  // Close RotateRight.  

            private static Node RotateLeft(Node x)  // Left rotation to fix a right-heavy imbalance.  
            {  // Open method scope.  
                Node y = x.Right!;  // Take right child as pivot (must exist).  
                Node? t2 = y.Left;  // Keep pivot's left subtree (will become x's right subtree).  

                y.Left = x;  // Put x as the left child of y.  
                x.Right = t2;  // Attach T2 as x's right child.  

                UpdateHeight(x);  // Update x first because its children changed.  
                UpdateHeight(y);  // Update y after x because y now points to x.  
                return y;  // Return new subtree root.  
            }  // Close RotateLeft.  

            private static Node Rebalance(Node node)  // Rebalance node if it violates AVL balance constraints.  
            {  // Open method scope.  
                int balance = BalanceFactor(node);  // Compute current node's balance factor.  

                if (balance > 1)  // Left-heavy subtree; need either LL or LR rotation(s).  
                {  // Open left-heavy branch.  
                    if (BalanceFactor(node.Left!) < 0)  // LR case: left child is right-heavy.  
                    {  // Open LR pre-rotation.  
                        node.Left = RotateLeft(node.Left!);  // LR case step 1: rotate left on left child.  
                    }  // Close LR pre-rotation.  
                    return RotateRight(node);  // LL/LR step 2: rotate right on current node.  
                }  // Close left-heavy branch.  

                if (balance < -1)  // Right-heavy subtree; need either RR or RL rotation(s).  
                {  // Open right-heavy branch.  
                    if (BalanceFactor(node.Right!) > 0)  // RL case: right child is left-heavy.  
                    {  // Open RL pre-rotation.  
                        node.Right = RotateRight(node.Right!);  // RL case step 1: rotate right on right child.  
                    }  // Close RL pre-rotation.  
                    return RotateLeft(node);  // RR/RL step 2: rotate left on current node.  
                }  // Close right-heavy branch.  

                return node;  // Already balanced; return node unchanged.  
            }  // Close Rebalance.  

            private readonly struct InsertResult  // Return (newRoot, inserted) for recursive inserts.  
            {  // Open struct scope.  
                public InsertResult(Node? newRoot, bool inserted)  // Construct immutable result.  
                {  // Open constructor scope.  
                    NewRoot = newRoot;  // Store new subtree root.  
                    Inserted = inserted;  // Store inserted flag.  
                }  // Close constructor scope.  

                public Node? NewRoot { get; }  // The new subtree root after insertion.  
                public bool Inserted { get; }  // Whether a node was inserted.  
            }  // Close InsertResult.  

            public bool Insert(int key)  // Insert key; return false when key already exists.  
            {  // Open method scope.  
                InsertResult r = InsertSubtree(_root, key);  // Insert into root subtree.  
                _root = r.NewRoot;  // Update root pointer (rotations may change root).  
                return r.Inserted;  // Return whether insertion happened.  
            }  // Close Insert.  

            private static InsertResult InsertSubtree(Node? node, int key)  // Insert into subtree and rebalance.  
            {  // Open method scope.  
                if (node is null)  // Empty spot: create a new leaf node.  
                {  // Open base case.  
                    return new InsertResult(new Node(key), true);  // Return new node and inserted=true.  
                }  // Close base case.  

                bool inserted;  // Track insertion flag from recursion.  
                if (key < node.Key)  // Insert into left subtree when key is smaller.  
                {  // Open left branch.  
                    InsertResult r = InsertSubtree(node.Left, key);  // Recurse into left child slot.  
                    node.Left = r.NewRoot;  // Re-attach possibly-rotated left subtree.  
                    inserted = r.Inserted;  // Propagate inserted flag.  
                }  // Close left branch.  
                else if (key > node.Key)  // Insert into right subtree when key is larger.  
                {  // Open right branch.  
                    InsertResult r = InsertSubtree(node.Right, key);  // Recurse into right child slot.  
                    node.Right = r.NewRoot;  // Re-attach possibly-rotated right subtree.  
                    inserted = r.Inserted;  // Propagate inserted flag.  
                }  // Close right branch.  
                else  // key == node.Key means duplicate key.  
                {  // Open duplicate branch.  
                    return new InsertResult(node, false);  // Reject duplicates by policy (no changes).  
                }  // Close duplicate branch.  

                UpdateHeight(node);  // Update cached height after insertion below.  
                Node newRoot = Rebalance(node);  // Rebalance on the way back up.  
                return new InsertResult(newRoot, inserted);  // Return possibly new root and insertion flag.  
            }  // Close InsertSubtree.  

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
                DeleteResult r = DeleteSubtree(_root, key);  // Delete from root subtree.  
                _root = r.NewRoot;  // Update root pointer (rotations may change root).  
                return r.Removed;  // Return whether deletion happened.  
            }  // Close Delete.  

            private static Node MinNode(Node node)  // Return the leftmost (minimum-key) node in a non-empty subtree.  
            {  // Open method scope.  
                Node cur = node;  // Start at given node.  
                while (cur.Left is not null)  // Keep going left until we cannot.  
                {  // Open loop scope.  
                    cur = cur.Left;  // Move to left child.  
                }  // Close loop scope.  
                return cur;  // Leftmost node has the minimum key.  
            }  // Close MinNode.  

            private static DeleteResult DeleteSubtree(Node? node, int key)  // Delete from subtree and rebalance.  
            {  // Open method scope.  
                if (node is null)  // Key not found in an empty subtree.  
                {  // Open base case.  
                    return new DeleteResult(newRoot: null, removed: false);  // Nothing removed.  
                }  // Close base case.  

                bool removed = false;  // Track whether a node was removed in this subtree.  
                if (key < node.Key)  // Delete from left subtree when key is smaller.  
                {  // Open left branch.  
                    DeleteResult r = DeleteSubtree(node.Left, key);  // Recurse into left child.  
                    node.Left = r.NewRoot;  // Re-attach possibly-rotated left subtree.  
                    removed = r.Removed;  // Propagate removed flag.  
                }  // Close left branch.  
                else if (key > node.Key)  // Delete from right subtree when key is larger.  
                {  // Open right branch.  
                    DeleteResult r = DeleteSubtree(node.Right, key);  // Recurse into right child.  
                    node.Right = r.NewRoot;  // Re-attach possibly-rotated right subtree.  
                    removed = r.Removed;  // Propagate removed flag.  
                }  // Close right branch.  
                else  // key == node.Key means we found the node to delete.  
                {  // Open match branch.  
                    removed = true;  // Mark that we are removing a node.  

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
                    Node successor = MinNode(node.Right!);  // Find successor node from right subtree.  
                    node.Key = successor.Key;  // Copy successor key into current node (keeps tree shape).  
                    DeleteResult rr = DeleteSubtree(node.Right, successor.Key);  // Delete successor node from right subtree.  
                    node.Right = rr.NewRoot;  // Re-attach right subtree after successor removal.  
                }  // Close match branch.  

                UpdateHeight(node);  // Update cached height after deletion below.  
                Node newRoot = Rebalance(node);  // Rebalance on the way back up.  
                return new DeleteResult(newRoot: newRoot, removed: removed);  // Return possibly new root and removed flag.  
            }  // Close DeleteSubtree.  

            public int? Min()  // Return minimum key, or null if tree is empty.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree has no minimum.  
                {  // Open empty case.  
                    return null;  // Return null for empty.  
                }  // Close empty case.  
                return MinNode(_root).Key;  // Return leftmost key.  
            }  // Close Min.  

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

            public int Height()  // Return tree height measured in edges (empty=-1, leaf=0).  
            {  // Open method scope.  
                return _root is null ? -1 : _root.Height;  // Use cached root height (or -1 for empty).  
            }  // Close Height.  

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

            public List<int> Inorder()  // Return inorder traversal (sorted for a valid BST).  
            {  // Open method scope.  
                var outList = new List<int>();  // Accumulate traversal result.  
                InorderWalk(_root, outList);  // Walk from root.  
                return outList;  // Return traversal list.  
            }  // Close Inorder.  

            private static void InorderWalk(Node? node, List<int> outList)  // Recursive inorder (left, root, right).  
            {  // Open method scope.  
                if (node is null)  // Empty subtree contributes nothing.  
                {  // Open base case.  
                    return;  // Stop recursion.  
                }  // Close base case.  
                InorderWalk(node.Left, outList);  // Visit left subtree first.  
                outList.Add(node.Key);  // Visit root.  
                InorderWalk(node.Right, outList);  // Visit right subtree.  
            }  // Close InorderWalk.  

            private readonly struct ValidateResult  // Return (ok, computedHeight) for recursive validation.  
            {  // Open struct scope.  
                public ValidateResult(bool ok, int computedHeight)  // Construct immutable result.  
                {  // Open constructor scope.  
                    Ok = ok;  // Store ok flag.  
                    ComputedHeight = computedHeight;  // Store computed height.  
                }  // Close constructor scope.  

                public bool Ok { get; }  // Whether subtree is valid.  
                public int ComputedHeight { get; }  // Computed height in edges for this subtree.  
            }  // Close ValidateResult.  

            public bool Validate()  // Validate BST ordering + AVL balance + cached height correctness.  
            {  // Open method scope.  
                return ValidateSubtree(_root, null, null).Ok;  // Validate from root with open bounds.  
            }  // Close Validate.  

            private static ValidateResult ValidateSubtree(Node? node, int? low, int? high)  // Validate subtree and return (ok, computedHeight).  
            {  // Open method scope.  
                if (node is null)  // Empty subtree is valid and has height -1 by definition.  
                {  // Open base case.  
                    return new ValidateResult(ok: true, computedHeight: -1);  // Return valid with height -1.  
                }  // Close base case.  

                if (low.HasValue && node.Key <= low.Value)  // Enforce strict lower bound (no duplicates).  
                {  // Open lower-bound check.  
                    return new ValidateResult(ok: false, computedHeight: 0);  // Report invalid (height irrelevant on failure).  
                }  // Close lower-bound check.  
                if (high.HasValue && node.Key >= high.Value)  // Enforce strict upper bound (no duplicates).  
                {  // Open upper-bound check.  
                    return new ValidateResult(ok: false, computedHeight: 0);  // Report invalid (height irrelevant on failure).  
                }  // Close upper-bound check.  

                ValidateResult left = ValidateSubtree(node.Left, low, node.Key);  // Validate left subtree with updated upper bound.  
                if (!left.Ok)  // Early return on left failure to keep logic simple.  
                {  // Open left-failure branch.  
                    return new ValidateResult(ok: false, computedHeight: 0);  // Propagate failure.  
                }  // Close left-failure branch.  
                ValidateResult right = ValidateSubtree(node.Right, node.Key, high);  // Validate right subtree with updated lower bound.  
                if (!right.Ok)  // Early return on right failure to keep logic simple.  
                {  // Open right-failure branch.  
                    return new ValidateResult(ok: false, computedHeight: 0);  // Propagate failure.  
                }  // Close right-failure branch.  

                int computed = 1 + Math.Max(left.ComputedHeight, right.ComputedHeight);  // Compute expected height from children heights.  
                if (node.Height != computed)  // Cached height must match computed height.  
                {  // Open height check.  
                    return new ValidateResult(ok: false, computedHeight: computed);  // Report height mismatch.  
                }  // Close height check.  

                if (Math.Abs(left.ComputedHeight - right.ComputedHeight) > 1)  // AVL balance constraint requires |bf| <= 1.  
                {  // Open balance check.  
                    return new ValidateResult(ok: false, computedHeight: computed);  // Report imbalance.  
                }  // Close balance check.  

                return new ValidateResult(ok: true, computedHeight: computed);  // Report valid subtree with computed height.  
            }  // Close ValidateSubtree.  

            public TreeSummary Summarize()  // Build a summary snapshot for demos/tests.  
            {  // Open method scope.  
                return new TreeSummary(  // Construct summary object.  
                    size: Size(),  // Compute node count.  
                    height: Height(),  // Read tree height (edges).  
                    minValue: Min(),  // Compute minimum key (nullable).  
                    maxValue: Max(),  // Compute maximum key (nullable).  
                    inorder: Inorder(),  // Compute inorder traversal.  
                    isValid: Validate()  // Validate AVL invariants.  
                );  // Close summary construction.  
            }  // Close Summarize.  
        }  // Close AvlTree.  
    }  // Close AvlDemo.  
}  // Close namespace scope.  

