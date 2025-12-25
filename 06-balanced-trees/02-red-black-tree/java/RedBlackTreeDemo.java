// 02 Red-Black Tree 示範（Java, LLRB）/ Red-black tree demo (Java, LLRB).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to build traversals before converting to arrays.  
import java.util.Arrays;  // Use Arrays.toString for readable demo output.  
import java.util.List;  // Use List for type clarity when collecting values.  

public final class RedBlackTreeDemo {  // Provide a small, explicit red-black tree implementation for study.  
    private RedBlackTreeDemo() {  // Prevent instantiation because this class is a static container.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    private static final boolean RED = true;  // Represent red nodes/links with true for readability.  
    private static final boolean BLACK = false;  // Represent black nodes/links with false for readability.  

    public static final class Node {  // LLRB node (key + left child + right child + color).  
        public int key;  // Store node key (mutable so delete can replace with successor key).  
        public Node left;  // Store reference to left child (null means no child).  
        public Node right;  // Store reference to right child (null means no child).  
        public boolean red;  // Store node color (true=RED, false=BLACK).  

        public Node(int key, boolean red) {  // Initialize node with a key, color, and empty children.  
            this.key = key;  // Assign key.  
            this.left = null;  // Initialize left child as missing.  
            this.right = null;  // Initialize right child as missing.  
            this.red = red;  // Assign node color.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class TreeSummary {  // Summarize key properties and inorder traversal for printing/tests.  
        public final int size;  // Number of nodes in the tree.  
        public final int height;  // Height measured in edges (empty=-1, leaf=0).  
        public final Integer minValue;  // Minimum key (null if empty).  
        public final Integer maxValue;  // Maximum key (null if empty).  
        public final int[] inorder;  // Inorder traversal (sorted keys for a valid BST).  
        public final boolean isValid;  // Whether the structure satisfies RB + BST invariants.  

        public TreeSummary(int size, int height, Integer minValue, Integer maxValue, int[] inorder, boolean isValid) {  // Construct immutable summary.  
            this.size = size;  // Store size.  
            this.height = height;  // Store height.  
            this.minValue = minValue;  // Store minimum value (nullable).  
            this.maxValue = maxValue;  // Store maximum value (nullable).  
            this.inorder = inorder;  // Store inorder traversal.  
            this.isValid = isValid;  // Store validation flag.  
        }  // Close constructor.  
    }  // End TreeSummary.  

    public static final class RedBlackTree {  // Red-black tree wrapper (implemented as Left-Leaning Red-Black, LLRB).  
        private Node root;  // Store root reference (null means empty tree).  

        public RedBlackTree(Node root) {  // Initialize tree with an optional root.  
            this.root = root;  // Store root reference.  
        }  // Close constructor.  

        public static RedBlackTree fromValues(int[] values) {  // Build a tree by inserting values in order.  
            RedBlackTree t = new RedBlackTree(null);  // Start from an empty tree.  
            for (int v : values) {  // Insert each value in the given order.  
                t.insert(v);  // Insert one key (duplicates are rejected by policy).  
            }  // Close loop.  
            return t;  // Return built tree.  
        }  // End fromValues().  

        public boolean isEmpty() {  // Check whether the tree has no nodes.  
            return this.root == null;  // Empty iff root is null.  
        }  // End isEmpty().  

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

        private static boolean isRed(Node node) {  // Return true when node is red; null is treated as black.  
            return node != null && node.red == RED;  // Null links are black by definition.  
        }  // End isRed().  

        private static Node rotateLeft(Node h) {  // Rotate left to eliminate a right-leaning red link.  
            Node x = h.right;  // Take right child as pivot (must exist).  
            h.right = x.left;  // Move pivot's left subtree to h's right slot.  
            x.left = h;  // Put h as pivot's left child.  
            x.red = h.red;  // Preserve original color on the new root.  
            h.red = RED;  // Mark the moved-down node as red.  
            return x;  // Return new subtree root.  
        }  // End rotateLeft().  

        private static Node rotateRight(Node h) {  // Rotate right to fix two consecutive left-leaning red links.  
            Node x = h.left;  // Take left child as pivot (must exist).  
            h.left = x.right;  // Move pivot's right subtree to h's left slot.  
            x.right = h;  // Put h as pivot's right child.  
            x.red = h.red;  // Preserve original color on the new root.  
            h.red = RED;  // Mark the moved-down node as red.  
            return x;  // Return new subtree root.  
        }  // End rotateRight().  

        private static void flipColors(Node h) {  // Flip colors to split or merge 4-nodes (LLRB <-> 2-3 tree).  
            h.red = !h.red;  // Toggle parent color.  
            if (h.left != null) {  // Guard for safety (children should exist in normal LLRB usage).  
                h.left.red = !h.left.red;  // Toggle left child color.  
            }  // Close left guard.  
            if (h.right != null) {  // Guard for safety (children should exist in normal LLRB usage).  
                h.right.red = !h.right.red;  // Toggle right child color.  
            }  // Close right guard.  
        }  // End flipColors().  

        private static Node fixUp(Node h) {  // Restore LLRB invariants on the way back up after insert/delete.  
            if (isRed(h.right) && !isRed(h.left)) {  // Fix right-leaning red link.  
                h = rotateLeft(h);  // Rotate left to make red link lean left.  
            }  // Close right-red fix.  
            if (isRed(h.left) && isRed(h.left.left)) {  // Fix two consecutive left reds.  
                h = rotateRight(h);  // Rotate right to balance the 4-node shape.  
            }  // Close left-left fix.  
            if (isRed(h.left) && isRed(h.right)) {  // Split 4-node when both children are red.  
                flipColors(h);  // Flip colors to push red up one level.  
            }  // Close split case.  
            return h;  // Return possibly rotated/flipped subtree root.  
        }  // End fixUp().  

        private static Node moveRedLeft(Node h) {  // Ensure h.left is red when we need to delete from the left side.  
            flipColors(h);  // Push red down so we can borrow from the right side.  
            if (h.right != null && isRed(h.right.left)) {  // Borrow from right-left red link when available.  
                h.right = rotateRight(h.right);  // Rotate right on right child to create a right-leaning red link.  
                h = rotateLeft(h);  // Rotate left on h to move red link to the left side.  
                flipColors(h);  // Re-flip to restore colors after rotations.  
            }  // Close borrow case.  
            return h;  // Return adjusted subtree root.  
        }  // End moveRedLeft().  

        private static Node moveRedRight(Node h) {  // Ensure h.right is red when we need to delete from the right side.  
            flipColors(h);  // Push red down so we can borrow from the left side.  
            if (h.left != null && isRed(h.left.left)) {  // Borrow from left-left red link when available.  
                h = rotateRight(h);  // Rotate right to move red link to the right side.  
                flipColors(h);  // Re-flip to restore colors after rotation.  
            }  // Close borrow case.  
            return h;  // Return adjusted subtree root.  
        }  // End moveRedRight().  

        private static final class InsertResult {  // Return (newRoot, inserted) for recursive inserts.  
            public final Node newRoot;  // The new subtree root after insertion.  
            public final boolean inserted;  // Whether a node was inserted.  

            public InsertResult(Node newRoot, boolean inserted) {  // Construct immutable result.  
                this.newRoot = newRoot;  // Store new root.  
                this.inserted = inserted;  // Store inserted flag.  
            }  // Close constructor.  
        }  // End InsertResult.  

        public boolean insert(int key) {  // Insert key; return false when key already exists.  
            InsertResult r = insertSubtree(this.root, key);  // Insert into root subtree.  
            this.root = r.newRoot;  // Update root pointer.  
            if (this.root != null) {  // Guard for empty tree.  
                this.root.red = BLACK;  // Root must always be black.  
            }  // Close root-color guard.  
            return r.inserted;  // Return whether insertion happened.  
        }  // End insert().  

        private static InsertResult insertSubtree(Node h, int key) {  // Insert key into subtree and rebalance.  
            if (h == null) {  // Empty spot: create a new red leaf node.  
                return new InsertResult(new Node(key, RED), true);  // Return new node and inserted=true.  
            }  // Close base case.  

            boolean inserted = false;  // Track insertion flag from recursion.  
            if (key < h.key) {  // Insert into left subtree when key is smaller.  
                InsertResult r = insertSubtree(h.left, key);  // Recurse into left child slot.  
                h.left = r.newRoot;  // Re-attach possibly rebalanced left subtree.  
                inserted = r.inserted;  // Propagate inserted flag.  
            } else if (key > h.key) {  // Insert into right subtree when key is larger.  
                InsertResult r = insertSubtree(h.right, key);  // Recurse into right child slot.  
                h.right = r.newRoot;  // Re-attach possibly rebalanced right subtree.  
                inserted = r.inserted;  // Propagate inserted flag.  
            } else {  // key == h.key means duplicate key.  
                return new InsertResult(h, false);  // Reject duplicates by policy (no changes).  
            }  // Close compare branch.  

            h = fixUp(h);  // Restore invariants after insertion below.  
            return new InsertResult(h, inserted);  // Return possibly rebalanced subtree root and insertion flag.  
        }  // End insertSubtree().  

        private static Node minNode(Node node) {  // Return the leftmost (minimum-key) node in a non-empty subtree.  
            Node cur = node;  // Start at given node.  
            while (cur.left != null) {  // Keep going left until we cannot.  
                cur = cur.left;  // Move to left child.  
            }  // Close loop.  
            return cur;  // Leftmost node has the minimum key.  
        }  // End minNode().  

        private static Node deleteMin(Node h) {  // Delete the minimum node in this subtree and return new subtree root.  
            if (h.left == null) {  // Reached the minimum node (no left child).  
                return null;  // Delete this node by returning null.  
            }  // Close base case.  

            if (!isRed(h.left) && !isRed(h.left.left)) {  // Ensure we do not descend into a 2-node.  
                h = moveRedLeft(h);  // Move a red link to the left side.  
            }  // Close move case.  

            h.left = deleteMin(h.left);  // Recurse into left subtree.  
            return fixUp(h);  // Fix up invariants on the way back up.  
        }  // End deleteMin().  

        public boolean delete(int key) {  // Delete key; return true when a node was removed.  
            if (this.root == null) {  // Empty tree cannot delete anything.  
                return false;  // Report not removed.  
            }  // Close empty case.  
            if (!contains(key)) {  // Avoid running LLRB delete when key is missing.  
                return false;  // Report not removed.  
            }  // Close missing-key guard.  

            if (!isRed(this.root.left) && !isRed(this.root.right)) {  // Detect 2-node root situation.  
                this.root.red = RED;  // Temporarily color root red to simplify deletion logic.  
            }  // Close pre-step.  

            this.root = deleteSubtree(this.root, key);  // Delete key from root subtree (guaranteed to exist).  
            if (this.root != null) {  // Restore invariant when tree is non-empty.  
                this.root.red = BLACK;  // Root must be black.  
            }  // Close root-color guard.  
            return true;  // Report removed.  
        }  // End delete().  

        private static Node deleteSubtree(Node h, int key) {  // Delete key from subtree (assumes key exists).  
            if (key < h.key) {  // Key is in the left subtree.  
                if (h.left != null) {  // Only recurse when left subtree exists.  
                    if (!isRed(h.left) && !isRed(h.left.left)) {  // Avoid descending into a 2-node.  
                        h = moveRedLeft(h);  // Move a red link to the left side.  
                    }  // Close move case.  
                    h.left = deleteSubtree(h.left, key);  // Recurse into left subtree.  
                }  // Close left guard.  
            } else {  // Key is in the current node or right subtree.  
                if (isRed(h.left)) {  // Rotate right to maintain delete preconditions.  
                    h = rotateRight(h);  // Rotate right to make sure we can move red right when needed.  
                }  // Close rotate-right pre-step.  

                if (key == h.key && h.right == null) {  // Case: match and no right child => can delete this node.  
                    return null;  // Delete node by returning null.  
                }  // Close leaf-right-null case.  

                if (h.right != null) {  // Only work on right side when it exists.  
                    if (!isRed(h.right) && !isRed(h.right.left)) {  // Avoid descending into a 2-node.  
                        h = moveRedRight(h);  // Move a red link to the right side.  
                    }  // Close move case.  
                    if (key == h.key) {  // Match case with a right subtree => replace with successor.  
                        Node successor = minNode(h.right);  // Find successor node from right subtree.  
                        h.key = successor.key;  // Copy successor key into current node.  
                        h.right = deleteMin(h.right);  // Delete successor node from right subtree.  
                    } else {  // Otherwise delete in the right subtree.  
                        h.right = deleteSubtree(h.right, key);  // Recurse into right subtree.  
                    }  // Close match/descend branch.  
                }  // Close right guard.  
            }  // Close key-side branch.  

            return fixUp(h);  // Fix up invariants on the way back up.  
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

        public int height() {  // Return height measured in edges (empty=-1, leaf=0).  
            return heightOf(this.root);  // Compute height from root.  
        }  // End height().  

        private static int heightOf(Node node) {  // Recursive helper that computes height in edges.  
            if (node == null) {  // Empty subtree has height -1.  
                return -1;  // Return -1 for empty.  
            }  // Close base case.  
            return 1 + Math.max(heightOf(node.left), heightOf(node.right));  // Height is 1 + max(child heights).  
        }  // End heightOf().  

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

        private static final class ValidateResult {  // Return (ok, blackHeight) for recursive validation.  
            public final boolean ok;  // Whether subtree is valid.  
            public final int blackHeight;  // Computed black-height for this subtree.  

            public ValidateResult(boolean ok, int blackHeight) {  // Construct immutable result.  
                this.ok = ok;  // Store ok flag.  
                this.blackHeight = blackHeight;  // Store black-height.  
            }  // Close constructor.  
        }  // End ValidateResult.  

        public boolean validate() {  // Validate BST ordering + red-black properties (LLRB variant).  
            if (this.root == null) {  // Empty tree is valid.  
                return true;  // Vacuously valid.  
            }  // Close empty case.  
            if (this.root.red) {  // Root must be black.  
                return false;  // Report invalid root color.  
            }  // Close root check.  
            return validateSubtree(this.root, null, null).ok;  // Validate from root with open bounds.  
        }  // End validate().  

        private static ValidateResult validateSubtree(Node node, Integer low, Integer high) {  // Return (ok, blackHeight).  
            if (node == null) {  // Null links are black and contribute a base black-height of 1.  
                return new ValidateResult(true, 1);  // Treat null as a black leaf sentinel.  
            }  // Close base case.  

            if (low != null && node.key <= low) {  // Enforce strict lower bound (no duplicates).  
                return new ValidateResult(false, 0);  // Report invalid ordering.  
            }  // Close lower-bound check.  
            if (high != null && node.key >= high) {  // Enforce strict upper bound (no duplicates).  
                return new ValidateResult(false, 0);  // Report invalid ordering.  
            }  // Close upper-bound check.  

            if (isRed(node.right)) {  // LLRB rule: red links must lean left (no red right child).  
                return new ValidateResult(false, 0);  // Report invalid right-leaning red link.  
            }  // Close right-red check.  

            if (isRed(node) && (isRed(node.left) || isRed(node.right))) {  // RB rule: no two consecutive reds.  
                return new ValidateResult(false, 0);  // Report invalid consecutive red links.  
            }  // Close consecutive-red check.  

            ValidateResult left = validateSubtree(node.left, low, node.key);  // Validate left subtree with updated upper bound.  
            if (!left.ok) {  // Early return on left failure to keep logic simple.  
                return new ValidateResult(false, 0);  // Propagate failure.  
            }  // Close left-failure branch.  
            ValidateResult right = validateSubtree(node.right, node.key, high);  // Validate right subtree with updated lower bound.  
            if (!right.ok) {  // Early return on right failure to keep logic simple.  
                return new ValidateResult(false, 0);  // Propagate failure.  
            }  // Close right-failure branch.  

            if (left.blackHeight != right.blackHeight) {  // RB rule: all root-to-leaf paths have the same number of black nodes.  
                return new ValidateResult(false, 0);  // Report black-height mismatch.  
            }  // Close black-height check.  

            int blackHere = isRed(node) ? 0 : 1;  // Count black nodes; red nodes do not add to black-height.  
            return new ValidateResult(true, left.blackHeight + blackHere);  // Return ok with computed black-height.  
        }  // End validateSubtree().  

        public TreeSummary summarize() {  // Build a summary snapshot for demos/tests.  
            return new TreeSummary(  // Construct summary object.  
                size(),  // Compute node count.  
                height(),  // Compute height (edges).  
                min(),  // Compute minimum key (nullable).  
                max(),  // Compute maximum key (nullable).  
                inorder(),  // Compute inorder traversal.  
                validate()  // Validate red-black invariants.  
            );  // Close summary construction.  
        }  // End summarize().  
    }  // End RedBlackTree.  

    public static String formatDemo() {  // Format a small demo output using a classic insert sequence.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic textbook insert order.  
        RedBlackTree t = RedBlackTree.fromValues(values);  // Build tree by inserting in order.  
        TreeSummary s = t.summarize();  // Summarize properties and inorder traversal.  

        List<String> lines = new ArrayList<>();  // Accumulate printable lines.  
        lines.add("=== Red-Black Tree Demo (Java, LLRB) ===");  // Print header.  
        lines.add("insert order: " + Arrays.toString(values));  // Print insertion order.  
        lines.add("size=" + s.size + ", height=" + s.height + ", min=" + s.minValue + ", max=" + s.maxValue + ", valid=" + s.isValid);  // Print key properties.  
        lines.add("inorder(sorted)=" + Arrays.toString(s.inorder));  // Print inorder traversal (should be sorted).  
        lines.add("contains(6)=" + t.contains(6) + ", contains(99)=" + t.contains(99));  // Print search examples.  

        t.delete(7);  // Delete a leaf (7).  
        t.delete(14);  // Delete a one-child node (14 has left child 13 in this data set).  
        t.delete(3);  // Delete a two-child node (3) and rebalance.  
        lines.add("after deletes (7,14,3) inorder=" + Arrays.toString(t.inorder()) + ", valid=" + t.validate());  // Show results after deletions.  
        return String.join(System.lineSeparator(), lines);  // Join lines into one printable string.  
    }  // End formatDemo().  

    public static void main(String[] args) {  // Run demo output.  
        System.out.println(formatDemo());  // Print demo output.  
    }  // End main().  
}  // End RedBlackTreeDemo.  

