// 01 AVL Tree 示範（Java）/ AVL tree demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to build traversals before converting to arrays.  
import java.util.Arrays;  // Use Arrays.toString for readable demo output.  
import java.util.List;  // Use List for type clarity when collecting values.  

public final class AvlTreeDemo {  // Provide a small, explicit AVL implementation for study.  
    private AvlTreeDemo() {  // Prevent instantiation because this class is a static container.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class Node {  // AVL node (key + left child + right child + cached height).  
        public int key;  // Store node key (mutable so delete can replace with successor key).  
        public Node left;  // Store reference to left child (null means no child).  
        public Node right;  // Store reference to right child (null means no child).  
        public int height;  // Store cached height in edges (leaf=0 by definition).  

        public Node(int key) {  // Initialize node with a key and empty children.  
            this.key = key;  // Assign key.  
            this.left = null;  // Initialize left child as missing.  
            this.right = null;  // Initialize right child as missing.  
            this.height = 0;  // Leaf height is 0 when height is measured in edges.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class TreeSummary {  // Summarize key properties and inorder traversal for printing/tests.  
        public final int size;  // Number of nodes in the AVL tree.  
        public final int height;  // Height measured in edges (empty=-1, leaf=0).  
        public final Integer minValue;  // Minimum key (null if empty).  
        public final Integer maxValue;  // Maximum key (null if empty).  
        public final int[] inorder;  // Inorder traversal (sorted keys for a valid BST).  
        public final boolean isValid;  // Whether the tree satisfies AVL invariants.  

        public TreeSummary(int size, int height, Integer minValue, Integer maxValue, int[] inorder, boolean isValid) {  // Construct immutable summary.  
            this.size = size;  // Store size.  
            this.height = height;  // Store height.  
            this.minValue = minValue;  // Store minimum value (nullable).  
            this.maxValue = maxValue;  // Store maximum value (nullable).  
            this.inorder = inorder;  // Store inorder traversal.  
            this.isValid = isValid;  // Store validation flag.  
        }  // Close constructor.  
    }  // End TreeSummary.  

    public static final class AvlTree {  // AVL tree wrapper that holds a root node.  
        private Node root;  // Store root reference (null means empty tree).  

        public AvlTree(Node root) {  // Initialize AVL tree with an optional root.  
            this.root = root;  // Store root reference.  
        }  // Close constructor.  

        public static AvlTree fromValues(int[] values) {  // Build an AVL tree by inserting values in order.  
            AvlTree t = new AvlTree(null);  // Start from an empty tree.  
            for (int v : values) {  // Insert each value in the given order.  
                t.insert(v);  // Insert one key (duplicates are rejected by policy).  
            }  // Close loop.  
            return t;  // Return built AVL tree.  
        }  // End fromValues().  

        public boolean isEmpty() {  // Check whether the tree has no nodes.  
            return this.root == null;  // Empty iff root is null.  
        }  // End isEmpty().  

        public int height() {  // Return tree height measured in edges (empty=-1, leaf=0).  
            return this.root == null ? -1 : this.root.height;  // Use cached root height (or -1 for empty).  
        }  // End height().  

        public boolean contains(int key) {  // Return true if key exists in the tree.  
            Node node = this.root;  // Start searching from the root.  
            while (node != null) {  // Walk down until we hit a missing child.  
                if (key < node.key) {  // Go left when key is smaller.  
                    node = node.left;  // Move to left child.  
                } else if (key > node.key) {  // Go right when key is larger.  
                    node = node.right;  // Move to right child.  
                } else {  // key == node.key means we found it.  
                    return true;  // Report found.  
                }  // Close compare branch.  
            }  // Close loop.  
            return false;  // Report not found when search falls off the tree.  
        }  // End contains().  

        private static int h(Node node) {  // Return cached height for node, or -1 for empty subtree.  
            return node == null ? -1 : node.height;  // Apply repo-wide height convention.  
        }  // End h().  

        private static void updateHeight(Node node) {  // Recompute node.height from children heights.  
            node.height = 1 + Math.max(h(node.left), h(node.right));  // Height in edges is 1 + max(child heights).  
        }  // End updateHeight().  

        private static int balanceFactor(Node node) {  // Compute balance factor = height(left) - height(right).  
            return h(node.left) - h(node.right);  // Positive means left-heavy; negative means right-heavy.  
        }  // End balanceFactor().  

        private static Node rotateRight(Node y) {  // Right rotation to fix a left-heavy imbalance.  
            Node x = y.left;  // Take left child as pivot.  
            Node t2 = x.right;  // Keep pivot's right subtree (will become y's left subtree).  

            x.right = y;  // Put y as the right child of x.  
            y.left = t2;  // Attach T2 as y's left child.  

            updateHeight(y);  // Update y first because its children changed.  
            updateHeight(x);  // Update x after y because x now points to y.  
            return x;  // Return new subtree root.  
        }  // End rotateRight().  

        private static Node rotateLeft(Node x) {  // Left rotation to fix a right-heavy imbalance.  
            Node y = x.right;  // Take right child as pivot.  
            Node t2 = y.left;  // Keep pivot's left subtree (will become x's right subtree).  

            y.left = x;  // Put x as the left child of y.  
            x.right = t2;  // Attach T2 as x's right child.  

            updateHeight(x);  // Update x first because its children changed.  
            updateHeight(y);  // Update y after x because y now points to x.  
            return y;  // Return new subtree root.  
        }  // End rotateLeft().  

        private static Node rebalance(Node node) {  // Rebalance node if it violates AVL balance constraints.  
            int balance = balanceFactor(node);  // Compute current node's balance factor.  

            if (balance > 1) {  // Left-heavy subtree; need either LL or LR rotation(s).  
                if (balanceFactor(node.left) >= 0) {  // LL case: left child is not right-heavy.  
                    return rotateRight(node);  // Single right rotation fixes LL.  
                }  // Close LL branch.  
                node.left = rotateLeft(node.left);  // LR case step 1: rotate left on left child.  
                return rotateRight(node);  // LR case step 2: rotate right on current node.  
            }  // Close left-heavy branch.  

            if (balance < -1) {  // Right-heavy subtree; need either RR or RL rotation(s).  
                if (balanceFactor(node.right) <= 0) {  // RR case: right child is not left-heavy.  
                    return rotateLeft(node);  // Single left rotation fixes RR.  
                }  // Close RR branch.  
                node.right = rotateRight(node.right);  // RL case step 1: rotate right on right child.  
                return rotateLeft(node);  // RL case step 2: rotate left on current node.  
            }  // Close right-heavy branch.  

            return node;  // Already balanced; return node unchanged.  
        }  // End rebalance().  

        private static final class InsertResult {  // Return (newSubtreeRoot, inserted) for recursive inserts.  
            public final Node newRoot;  // The new subtree root after insertion.  
            public final boolean inserted;  // Whether a node was inserted.  

            public InsertResult(Node newRoot, boolean inserted) {  // Construct immutable result.  
                this.newRoot = newRoot;  // Store new root.  
                this.inserted = inserted;  // Store inserted flag.  
            }  // Close constructor.  
        }  // End InsertResult.  

        public boolean insert(int key) {  // Insert key; return false when key already exists.  
            InsertResult r = insertSubtree(this.root, key);  // Insert into root subtree.  
            this.root = r.newRoot;  // Update root pointer (rotations may change root).  
            return r.inserted;  // Return whether insertion happened.  
        }  // End insert().  

        private static InsertResult insertSubtree(Node node, int key) {  // Insert into subtree and rebalance.  
            if (node == null) {  // Empty spot: create a new leaf node.  
                return new InsertResult(new Node(key), true);  // Return new node and inserted=true.  
            }  // Close base case.  

            boolean inserted;  // Track insertion flag from recursion.  
            if (key < node.key) {  // Insert into left subtree when key is smaller.  
                InsertResult r = insertSubtree(node.left, key);  // Recurse into left child slot.  
                node.left = r.newRoot;  // Re-attach possibly-rotated left subtree.  
                inserted = r.inserted;  // Propagate inserted flag.  
            } else if (key > node.key) {  // Insert into right subtree when key is larger.  
                InsertResult r = insertSubtree(node.right, key);  // Recurse into right child slot.  
                node.right = r.newRoot;  // Re-attach possibly-rotated right subtree.  
                inserted = r.inserted;  // Propagate inserted flag.  
            } else {  // key == node.key means duplicate key.  
                return new InsertResult(node, false);  // Reject duplicates by policy (no changes).  
            }  // Close compare branch.  

            updateHeight(node);  // Update cached height after insertion below.  
            Node newRoot = rebalance(node);  // Rebalance on the way back up.  
            return new InsertResult(newRoot, inserted);  // Return possibly new root and insertion flag.  
        }  // End insertSubtree().  

        private static final class DeleteResult {  // Return (newSubtreeRoot, removed) for recursive deletes.  
            public final Node newRoot;  // The new subtree root after deletion.  
            public final boolean removed;  // Whether a node was removed.  

            public DeleteResult(Node newRoot, boolean removed) {  // Construct immutable result.  
                this.newRoot = newRoot;  // Store new root.  
                this.removed = removed;  // Store removed flag.  
            }  // Close constructor.  
        }  // End DeleteResult.  

        public boolean delete(int key) {  // Delete key; return true when a node was removed.  
            DeleteResult r = deleteSubtree(this.root, key);  // Delete from root subtree.  
            this.root = r.newRoot;  // Update root pointer (rotations may change root).  
            return r.removed;  // Return whether deletion happened.  
        }  // End delete().  

        private static Node minNode(Node node) {  // Return the leftmost (minimum-key) node in a non-empty subtree.  
            Node cur = node;  // Start at given node.  
            while (cur.left != null) {  // Keep going left until we cannot.  
                cur = cur.left;  // Move to left child.  
            }  // Close loop.  
            return cur;  // Leftmost node has the minimum key.  
        }  // End minNode().  

        private static DeleteResult deleteSubtree(Node node, int key) {  // Delete from subtree and rebalance.  
            if (node == null) {  // Key not found in an empty subtree.  
                return new DeleteResult(null, false);  // Nothing removed.  
            }  // Close base case.  

            boolean removed = false;  // Track whether a node was removed in this subtree.  
            if (key < node.key) {  // Delete from left subtree when key is smaller.  
                DeleteResult r = deleteSubtree(node.left, key);  // Recurse into left child.  
                node.left = r.newRoot;  // Re-attach possibly-rotated left subtree.  
                removed = r.removed;  // Propagate removed flag.  
            } else if (key > node.key) {  // Delete from right subtree when key is larger.  
                DeleteResult r = deleteSubtree(node.right, key);  // Recurse into right child.  
                node.right = r.newRoot;  // Re-attach possibly-rotated right subtree.  
                removed = r.removed;  // Propagate removed flag.  
            } else {  // key == node.key means we found the node to delete.  
                removed = true;  // Mark that we are removing a node.  

                if (node.left == null && node.right == null) {  // Case 1: leaf node (0 children).  
                    return new DeleteResult(null, true);  // Remove leaf by returning null.  
                }  // Close leaf case.  
                if (node.left == null) {  // Case 2a: only right child exists.  
                    return new DeleteResult(node.right, true);  // Replace node by its right child.  
                }  // Close right-only case.  
                if (node.right == null) {  // Case 2b: only left child exists.  
                    return new DeleteResult(node.left, true);  // Replace node by its left child.  
                }  // Close left-only case.  

                // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.  
                Node successor = minNode(node.right);  // Find successor node from right subtree.  
                node.key = successor.key;  // Copy successor key into current node (keeps tree shape).  
                DeleteResult rr = deleteSubtree(node.right, successor.key);  // Delete successor node from right subtree.  
                node.right = rr.newRoot;  // Re-attach right subtree after successor removal.  
            }  // Close match branch.  

            updateHeight(node);  // Update cached height after deletion below.  
            Node newRoot = rebalance(node);  // Rebalance on the way back up.  
            return new DeleteResult(newRoot, removed);  // Return possibly new root and removed flag.  
        }  // End deleteSubtree().  

        public Integer min() {  // Return minimum key, or null if tree is empty.  
            if (this.root == null) {  // Empty tree has no minimum.  
                return null;  // Return null for empty.  
            }  // Close empty case.  
            return minNode(this.root).key;  // Return leftmost key.  
        }  // End min().  

        public Integer max() {  // Return maximum key, or null if tree is empty.  
            if (this.root == null) {  // Empty tree has no maximum.  
                return null;  // Return null for empty.  
            }  // Close empty case.  
            Node cur = this.root;  // Start at root.  
            while (cur.right != null) {  // Keep going right until we cannot.  
                cur = cur.right;  // Move to right child.  
            }  // Close loop.  
            return cur.key;  // Rightmost node has the maximum key.  
        }  // End max().  

        public int size() {  // Return number of nodes in the tree (O(n)).  
            return sizeOf(this.root);  // Count nodes starting from root.  
        }  // End size().  

        private static int sizeOf(Node node) {  // Recursive helper that counts nodes in a subtree.  
            if (node == null) {  // Empty subtree contributes 0 nodes.  
                return 0;  // Return 0 for empty.  
            }  // Close base case.  
            return 1 + sizeOf(node.left) + sizeOf(node.right);  // Count root + left + right.  
        }  // End sizeOf().  

        public int[] inorder() {  // Return inorder traversal (sorted for a valid BST).  
            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            inorderWalk(this.root, out);  // Walk from root.  
            int[] arr = new int[out.size()];  // Allocate primitive int array for tests/demo.  
            for (int i = 0; i < out.size(); i++) {  // Copy boxed Integers into primitive int array.  
                arr[i] = out.get(i);  // Unbox into result array.  
            }  // Close copy loop.  
            return arr;  // Return traversal array.  
        }  // End inorder().  

        private static void inorderWalk(Node node, List<Integer> out) {  // Recursive inorder (left, root, right).  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            inorderWalk(node.left, out);  // Visit left subtree first.  
            out.add(node.key);  // Visit root.  
            inorderWalk(node.right, out);  // Visit right subtree.  
        }  // End inorderWalk().  

        private static final class ValidateResult {  // Return (ok, computedHeight) for recursive validation.  
            public final boolean ok;  // Whether subtree is valid.  
            public final int computedHeight;  // Computed height in edges for this subtree.  

            public ValidateResult(boolean ok, int computedHeight) {  // Construct immutable result.  
                this.ok = ok;  // Store ok flag.  
                this.computedHeight = computedHeight;  // Store computed height.  
            }  // Close constructor.  
        }  // End ValidateResult.  

        public boolean validate() {  // Validate BST ordering + AVL balance + cached height correctness.  
            ValidateResult r = validateSubtree(this.root, null, null);  // Validate from root with open bounds.  
            return r.ok;  // Return validation result.  
        }  // End validate().  

        private static ValidateResult validateSubtree(Node node, Integer low, Integer high) {  // Validate subtree and return (ok, computedHeight).  
            if (node == null) {  // Empty subtree is valid and has height -1 by definition.  
                return new ValidateResult(true, -1);  // Return valid with height -1.  
            }  // Close base case.  

            if (low != null && node.key <= low) {  // Enforce strict lower bound (no duplicates).  
                return new ValidateResult(false, 0);  // Report invalid (height value is irrelevant on failure).  
            }  // Close lower-bound check.  
            if (high != null && node.key >= high) {  // Enforce strict upper bound (no duplicates).  
                return new ValidateResult(false, 0);  // Report invalid (height value is irrelevant on failure).  
            }  // Close upper-bound check.  

            ValidateResult left = validateSubtree(node.left, low, node.key);  // Validate left subtree with updated upper bound.  
            if (!left.ok) {  // Early return on left failure to keep logic simple.  
                return new ValidateResult(false, 0);  // Propagate failure.  
            }  // Close left-failure branch.  
            ValidateResult right = validateSubtree(node.right, node.key, high);  // Validate right subtree with updated lower bound.  
            if (!right.ok) {  // Early return on right failure to keep logic simple.  
                return new ValidateResult(false, 0);  // Propagate failure.  
            }  // Close right-failure branch.  

            int computed = 1 + Math.max(left.computedHeight, right.computedHeight);  // Compute expected height from children heights.  
            if (node.height != computed) {  // Cached height must match computed height.  
                return new ValidateResult(false, computed);  // Report height mismatch.  
            }  // Close height check.  

            if (Math.abs(left.computedHeight - right.computedHeight) > 1) {  // AVL balance constraint requires |bf| <= 1.  
                return new ValidateResult(false, computed);  // Report imbalance.  
            }  // Close balance check.  

            return new ValidateResult(true, computed);  // Report valid subtree with computed height.  
        }  // End validateSubtree().  

        public TreeSummary summarize() {  // Build a summary snapshot for demos/tests.  
            return new TreeSummary(  // Construct summary object.  
                size(),  // Compute node count.  
                height(),  // Read tree height (edges).  
                min(),  // Compute minimum key (nullable).  
                max(),  // Compute maximum key (nullable).  
                inorder(),  // Compute inorder traversal.  
                validate()  // Validate AVL invariants.  
            );  // Close summary construction.  
        }  // End summarize().  
    }  // End AvlTree.  

    public static String formatDemo() {  // Format a small demo output using a classic insert sequence.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic textbook insert order.  
        AvlTree t = AvlTree.fromValues(values);  // Build AVL tree by inserting in order.  
        TreeSummary s = t.summarize();  // Summarize properties and inorder traversal.  

        List<String> lines = new ArrayList<>();  // Accumulate printable lines.  
        lines.add("=== AVL Tree Demo (Java) ===");  // Print header.  
        lines.add("insert order: " + Arrays.toString(values));  // Print insertion order.  
        lines.add("size=" + s.size + ", height=" + s.height + ", min=" + s.minValue + ", max=" + s.maxValue + ", valid=" + s.isValid);  // Print key properties.  
        lines.add("inorder(sorted)=" + Arrays.toString(s.inorder));  // Print inorder traversal (should be sorted).  
        lines.add("contains(6)=" + t.contains(6) + ", contains(99)=" + t.contains(99));  // Print search examples.  

        t.delete(7);  // Delete a leaf (7) to show delete on AVL.  
        t.delete(14);  // Delete a one-child node (14 has left child 13).  
        t.delete(3);  // Delete a two-child node (3) and rebalance.  
        lines.add("after deletes (7,14,3) inorder=" + Arrays.toString(t.inorder()) + ", valid=" + t.validate());  // Show results after deletions.  
        return String.join(System.lineSeparator(), lines);  // Join lines into one printable string.  
    }  // End formatDemo().  

    public static void main(String[] args) {  // Run demo output.  
        System.out.println(formatDemo());  // Print demo output.  
    }  // End main().  
}  // End AvlTreeDemo.  

