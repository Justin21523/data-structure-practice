// 02 Red-Black Tree 示範（C#，LLRB）/ Red-black tree demo (C#, LLRB).  // Bilingual file header.

using System;  // Provide basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for inorder traversal results.  

namespace RbtUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class RbtDemo  // Host RedBlackTree implementation and helpers.  
    {  // Open class scope.  
        private const bool RedColor = true;  // Represent red nodes/links with true for readability.  
        private const bool BlackColor = false;  // Represent black nodes/links with false for readability.  

        internal sealed class Node  // LLRB node (key + left child + right child + color).  
        {  // Open class scope.  
            public Node(int key, bool red)  // Initialize node with a key, color, and empty children.  
            {  // Open constructor scope.  
                Key = key;  // Store node key.  
                Left = null;  // Initialize left child as missing.  
                Right = null;  // Initialize right child as missing.  
                Red = red;  // Store node color (true=RED, false=BLACK).  
            }  // Close constructor scope.  

            public int Key { get; set; }  // Store key (mutable so delete can replace with successor key).  
            public Node? Left { get; set; }  // Left child pointer (null means no child).  
            public Node? Right { get; set; }  // Right child pointer (null means no child).  
            public bool Red { get; set; }  // Node color (true=RED, false=BLACK).  
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

            public int Size { get; }  // Number of nodes in the tree.  
            public int Height { get; }  // Height measured in edges (empty=-1, leaf=0).  
            public int? MinValue { get; }  // Minimum key (null if empty).  
            public int? MaxValue { get; }  // Maximum key (null if empty).  
            public IReadOnlyList<int> Inorder { get; }  // Inorder traversal (sorted for a valid BST).  
            public bool IsValid { get; }  // Whether the structure satisfies RB + BST invariants.  
        }  // Close TreeSummary.  

        internal sealed class RedBlackTree  // Red-black tree wrapper (implemented as Left-Leaning Red-Black, LLRB).  
        {  // Open class scope.  
            private Node? _root;  // Store root reference (null means empty tree).  

            public RedBlackTree(Node? root)  // Initialize tree with an optional root.  
            {  // Open constructor scope.  
                _root = root;  // Store root reference.  
            }  // Close constructor scope.  

            public bool IsEmpty => _root is null;  // Expose emptiness check.  

            public static RedBlackTree FromValues(int[] values)  // Build a tree by inserting values in order.  
            {  // Open method scope.  
                var t = new RedBlackTree(root: null);  // Start from an empty tree.  
                foreach (int v in values)  // Insert each value in the given order.  
                {  // Open foreach scope.  
                    t.Insert(v);  // Insert one key (duplicates are rejected by policy).  
                }  // Close foreach scope.  
                return t;  // Return built tree.  
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

            private static bool IsRed(Node? node)  // Return true when node is red; null is treated as black.  
            {  // Open method scope.  
                return node is not null && node.Red == RedColor;  // Null links are black by definition.  
            }  // Close IsRed.  

            private static Node RotateLeft(Node h)  // Rotate left to eliminate a right-leaning red link.  
            {  // Open method scope.  
                Node x = h.Right!;  // Take right child as pivot (must exist).  
                h.Right = x.Left;  // Move pivot's left subtree to h's right slot.  
                x.Left = h;  // Put h as pivot's left child.  
                x.Red = h.Red;  // Preserve original color on the new root.  
                h.Red = RedColor;  // Mark the moved-down node as red.  
                return x;  // Return new subtree root.  
            }  // Close RotateLeft.  

            private static Node RotateRight(Node h)  // Rotate right to fix two consecutive left-leaning red links.  
            {  // Open method scope.  
                Node x = h.Left!;  // Take left child as pivot (must exist).  
                h.Left = x.Right;  // Move pivot's right subtree to h's left slot.  
                x.Right = h;  // Put h as pivot's right child.  
                x.Red = h.Red;  // Preserve original color on the new root.  
                h.Red = RedColor;  // Mark the moved-down node as red.  
                return x;  // Return new subtree root.  
            }  // Close RotateRight.  

            private static void FlipColors(Node h)  // Flip colors to split or merge 4-nodes (LLRB <-> 2-3 tree).  
            {  // Open method scope.  
                h.Red = !h.Red;  // Toggle parent color.  
                if (h.Left is not null)  // Guard for safety (children should exist in normal LLRB usage).  
                {  // Open left guard.  
                    h.Left.Red = !h.Left.Red;  // Toggle left child color.  
                }  // Close left guard.  
                if (h.Right is not null)  // Guard for safety (children should exist in normal LLRB usage).  
                {  // Open right guard.  
                    h.Right.Red = !h.Right.Red;  // Toggle right child color.  
                }  // Close right guard.  
            }  // Close FlipColors.  

            private static Node FixUp(Node h)  // Restore LLRB invariants on the way back up after insert/delete.  
            {  // Open method scope.  
                if (IsRed(h.Right) && !IsRed(h.Left))  // Fix right-leaning red link.  
                {  // Open right-red fix.  
                    h = RotateLeft(h);  // Rotate left to make red link lean left.  
                }  // Close right-red fix.  
                if (IsRed(h.Left) && IsRed(h.Left!.Left))  // Fix two consecutive left reds.  
                {  // Open left-left fix.  
                    h = RotateRight(h);  // Rotate right to balance the 4-node shape.  
                }  // Close left-left fix.  
                if (IsRed(h.Left) && IsRed(h.Right))  // Split 4-node when both children are red.  
                {  // Open split case.  
                    FlipColors(h);  // Flip colors to push red up one level.  
                }  // Close split case.  
                return h;  // Return possibly rotated/flipped subtree root.  
            }  // Close FixUp.  

            private static Node MoveRedLeft(Node h)  // Ensure h.Left is red when we need to delete from the left side.  
            {  // Open method scope.  
                FlipColors(h);  // Push red down so we can borrow from the right side.  
                if (h.Right is not null && IsRed(h.Right.Left))  // Borrow from right-left red link when available.  
                {  // Open borrow case.  
                    h.Right = RotateRight(h.Right);  // Rotate right on right child to create a right-leaning red link.  
                    h = RotateLeft(h);  // Rotate left on h to move red link to the left side.  
                    FlipColors(h);  // Re-flip to restore colors after rotations.  
                }  // Close borrow case.  
                return h;  // Return adjusted subtree root.  
            }  // Close MoveRedLeft.  

            private static Node MoveRedRight(Node h)  // Ensure h.Right is red when we need to delete from the right side.  
            {  // Open method scope.  
                FlipColors(h);  // Push red down so we can borrow from the left side.  
                if (h.Left is not null && IsRed(h.Left.Left))  // Borrow from left-left red link when available.  
                {  // Open borrow case.  
                    h = RotateRight(h);  // Rotate right to move red link to the right side.  
                    FlipColors(h);  // Re-flip to restore colors after rotation.  
                }  // Close borrow case.  
                return h;  // Return adjusted subtree root.  
            }  // Close MoveRedRight.  

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
                _root = r.NewRoot;  // Update root pointer.  
                if (_root is not null)  // Guard for empty tree.  
                {  // Open guard.  
                    _root.Red = BlackColor;  // Root must always be black.  
                }  // Close guard.  
                return r.Inserted;  // Return whether insertion happened.  
            }  // Close Insert.  

            private static InsertResult InsertSubtree(Node? h, int key)  // Insert key into subtree and rebalance.  
            {  // Open method scope.  
                if (h is null)  // Empty spot: create a new red leaf node.  
                {  // Open base case.  
                    return new InsertResult(new Node(key, RedColor), true);  // Return new node and inserted=true.  
                }  // Close base case.  

                bool inserted = false;  // Track insertion flag from recursion.  
                if (key < h.Key)  // Insert into left subtree when key is smaller.  
                {  // Open left branch.  
                    InsertResult r = InsertSubtree(h.Left, key);  // Recurse into left child slot.  
                    h.Left = r.NewRoot;  // Re-attach possibly rebalanced left subtree.  
                    inserted = r.Inserted;  // Propagate inserted flag.  
                }  // Close left branch.  
                else if (key > h.Key)  // Insert into right subtree when key is larger.  
                {  // Open right branch.  
                    InsertResult r = InsertSubtree(h.Right, key);  // Recurse into right child slot.  
                    h.Right = r.NewRoot;  // Re-attach possibly rebalanced right subtree.  
                    inserted = r.Inserted;  // Propagate inserted flag.  
                }  // Close right branch.  
                else  // key == h.Key means duplicate key.  
                {  // Open duplicate branch.  
                    return new InsertResult(h, false);  // Reject duplicates by policy (no changes).  
                }  // Close duplicate branch.  

                h = FixUp(h);  // Restore invariants after insertion below.  
                return new InsertResult(h, inserted);  // Return possibly rebalanced subtree root and insertion flag.  
            }  // Close InsertSubtree.  

            private static Node MinNode(Node node)  // Return the leftmost (minimum-key) node in a non-empty subtree.  
            {  // Open method scope.  
                Node cur = node;  // Start at given node.  
                while (cur.Left is not null)  // Keep going left until we cannot.  
                {  // Open loop scope.  
                    cur = cur.Left;  // Move to left child.  
                }  // Close loop scope.  
                return cur;  // Leftmost node has the minimum key.  
            }  // Close MinNode.  

            private static Node? DeleteMin(Node h)  // Delete the minimum node in this subtree and return new subtree root.  
            {  // Open method scope.  
                if (h.Left is null)  // Reached the minimum node (no left child).  
                {  // Open base case.  
                    return null;  // Delete this node by returning null.  
                }  // Close base case.  

                if (!IsRed(h.Left) && !IsRed(h.Left.Left))  // Ensure we do not descend into a 2-node.  
                {  // Open move case.  
                    h = MoveRedLeft(h);  // Move a red link to the left side.  
                }  // Close move case.  

                h.Left = DeleteMin(h.Left!);  // Recurse into left subtree.  
                return FixUp(h);  // Fix up invariants on the way back up.  
            }  // Close DeleteMin.  

            public bool Delete(int key)  // Delete key; return true when a node was removed.  
            {  // Open method scope.  
                if (_root is null)  // Empty tree cannot delete anything.  
                {  // Open empty case.  
                    return false;  // Report not removed.  
                }  // Close empty case.  
                if (!Contains(key))  // Avoid running LLRB delete when key is missing.  
                {  // Open missing-key guard.  
                    return false;  // Report not removed.  
                }  // Close missing-key guard.  

                if (!IsRed(_root.Left) && !IsRed(_root.Right))  // Detect 2-node root situation.  
                {  // Open pre-step.  
                    _root.Red = RedColor;  // Temporarily color root red to simplify deletion logic.  
                }  // Close pre-step.  

                _root = DeleteSubtree(_root, key);  // Delete key from root subtree (guaranteed to exist).  
                if (_root is not null)  // Restore invariant when tree is non-empty.  
                {  // Open root-color guard.  
                    _root.Red = BlackColor;  // Root must be black.  
                }  // Close root-color guard.  
                return true;  // Report removed.  
            }  // Close Delete.  

            private static Node? DeleteSubtree(Node h, int key)  // Delete key from subtree (assumes key exists).  
            {  // Open method scope.  
                if (key < h.Key)  // Key is in the left subtree.  
                {  // Open left-side branch.  
	                    if (h.Left is not null)  // Only recurse when left subtree exists.  
	                    {  // Open left guard.  
	                        if (!IsRed(h.Left) && !IsRed(h.Left.Left))  // Avoid descending into a 2-node.  
	                        {  // Open move case.  
	                            h = MoveRedLeft(h);  // Move a red link to the left side.  
	                        }  // Close move case.  
	                        Node left = h.Left!;  // Capture non-null left child (we are inside a null-checked branch).  
	                        h.Left = DeleteSubtree(left, key);  // Recurse into left subtree.  
	                    }  // Close left guard.  
	                }  // Close left-side branch.  
                else  // Key is in the current node or right subtree.  
                {  // Open right-side branch.  
                    if (IsRed(h.Left))  // Rotate right to maintain delete preconditions.  
                    {  // Open rotate-right pre-step.  
                        h = RotateRight(h);  // Rotate right to make sure we can move red right when needed.  
                    }  // Close rotate-right pre-step.  

                    if (key == h.Key && h.Right is null)  // Case: match and no right child => can delete this node.  
                    {  // Open leaf-right-null case.  
                        return null;  // Delete node by returning null.  
                    }  // Close leaf-right-null case.  

	                    if (h.Right is not null)  // Only work on right side when it exists.  
	                    {  // Open right guard.  
	                        if (!IsRed(h.Right) && !IsRed(h.Right.Left))  // Avoid descending into a 2-node.  
	                        {  // Open move case.  
	                            h = MoveRedRight(h);  // Move a red link to the right side.  
	                        }  // Close move case.  
	                        if (key == h.Key)  // Match case with a right subtree => replace with successor.  
	                        {  // Open match case.  
	                            Node right = h.Right!;  // Capture non-null right child (we are inside a null-checked branch).  
	                            Node successor = MinNode(right);  // Find successor node from right subtree.  
	                            h.Key = successor.Key;  // Copy successor key into current node.  
	                            h.Right = DeleteMin(right);  // Delete successor node from right subtree.  
	                        }  // Close match case.  
	                        else  // Otherwise delete in the right subtree.  
	                        {  // Open descend case.  
	                            Node right = h.Right!;  // Capture non-null right child (we are inside a null-checked branch).  
	                            h.Right = DeleteSubtree(right, key);  // Recurse into right subtree.  
	                        }  // Close descend case.  
	                    }  // Close right guard.  
	                }  // Close right-side branch.  

                return FixUp(h);  // Fix up invariants on the way back up.  
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

            public int Height()  // Return height measured in edges (empty=-1, leaf=0).  
            {  // Open method scope.  
                return HeightOf(_root);  // Compute height from root.  
            }  // Close Height.  

            private static int HeightOf(Node? node)  // Recursive helper that computes height in edges.  
            {  // Open method scope.  
                if (node is null)  // Empty subtree has height -1.  
                {  // Open base case.  
                    return -1;  // Return -1 for empty.  
                }  // Close base case.  
                return 1 + Math.Max(HeightOf(node.Left), HeightOf(node.Right));  // Height is 1 + max(child heights).  
            }  // Close HeightOf.  

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

            private readonly struct ValidateResult  // Return (ok, blackHeight) for recursive validation.  
            {  // Open struct scope.  
                public ValidateResult(bool ok, int blackHeight)  // Construct immutable result.  
                {  // Open constructor scope.  
                    Ok = ok;  // Store ok flag.  
                    BlackHeight = blackHeight;  // Store black-height.  
                }  // Close constructor scope.  

                public bool Ok { get; }  // Whether subtree is valid.  
                public int BlackHeight { get; }  // Computed black-height for this subtree.  
            }  // Close ValidateResult.  

            public bool Validate()  // Validate BST ordering + red-black properties (LLRB variant).  
            {  // Open method scope.  
                if (_root is null)  // Empty tree is valid.  
                {  // Open empty case.  
                    return true;  // Vacuously valid.  
                }  // Close empty case.  
                if (_root.Red)  // Root must be black.  
                {  // Open root check.  
                    return false;  // Report invalid root color.  
                }  // Close root check.  
                return ValidateSubtree(_root, null, null).Ok;  // Validate from root with open bounds.  
            }  // Close Validate.  

            private static ValidateResult ValidateSubtree(Node? node, int? low, int? high)  // Return (ok, blackHeight).  
            {  // Open method scope.  
                if (node is null)  // Null links are black and contribute a base black-height of 1.  
                {  // Open base case.  
                    return new ValidateResult(ok: true, blackHeight: 1);  // Treat null as a black leaf sentinel.  
                }  // Close base case.  

                if (low.HasValue && node.Key <= low.Value)  // Enforce strict lower bound (no duplicates).  
                {  // Open lower-bound check.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Report invalid ordering.  
                }  // Close lower-bound check.  
                if (high.HasValue && node.Key >= high.Value)  // Enforce strict upper bound (no duplicates).  
                {  // Open upper-bound check.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Report invalid ordering.  
                }  // Close upper-bound check.  

                if (IsRed(node.Right))  // LLRB rule: red links must lean left (no red right child).  
                {  // Open right-red check.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Report invalid right-leaning red link.  
                }  // Close right-red check.  

                if (IsRed(node) && (IsRed(node.Left) || IsRed(node.Right)))  // RB rule: no two consecutive reds.  
                {  // Open consecutive-red check.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Report invalid consecutive red links.  
                }  // Close consecutive-red check.  

                ValidateResult left = ValidateSubtree(node.Left, low, node.Key);  // Validate left subtree with updated upper bound.  
                if (!left.Ok)  // Early return on left failure to keep logic simple.  
                {  // Open left-failure branch.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Propagate failure.  
                }  // Close left-failure branch.  
                ValidateResult right = ValidateSubtree(node.Right, node.Key, high);  // Validate right subtree with updated lower bound.  
                if (!right.Ok)  // Early return on right failure to keep logic simple.  
                {  // Open right-failure branch.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Propagate failure.  
                }  // Close right-failure branch.  

                if (left.BlackHeight != right.BlackHeight)  // RB rule: all root-to-leaf paths have same number of black nodes.  
                {  // Open black-height check.  
                    return new ValidateResult(ok: false, blackHeight: 0);  // Report black-height mismatch.  
                }  // Close black-height check.  

                int blackHere = node.Red ? 0 : 1;  // Count black nodes; red nodes do not add to black-height.  
                return new ValidateResult(ok: true, blackHeight: left.BlackHeight + blackHere);  // Return ok with computed black-height.  
            }  // Close ValidateSubtree.  

            public TreeSummary Summarize()  // Build a summary snapshot for demos/tests.  
            {  // Open method scope.  
                return new TreeSummary(  // Construct summary object.  
                    size: Size(),  // Compute node count.  
                    height: Height(),  // Compute height (edges).  
                    minValue: Min(),  // Compute minimum key (nullable).  
                    maxValue: Max(),  // Compute maximum key (nullable).  
                    inorder: Inorder(),  // Compute inorder traversal.  
                    isValid: Validate()  // Validate red-black invariants.  
                );  // Close summary construction.  
            }  // Close Summarize.  
        }  // Close RedBlackTree.  
    }  // Close RbtDemo.  
}  // Close namespace scope.  
