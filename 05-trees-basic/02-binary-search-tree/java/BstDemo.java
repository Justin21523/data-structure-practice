// 02 二元搜尋樹示範（Java）/ Binary Search Tree (BST) demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to build traversals before converting to arrays.  
import java.util.Arrays;  // Use Arrays.toString for readable demo output.  
import java.util.List;  // Use List for type clarity when collecting values.  

public final class BstDemo {  // Provide a small, explicit BST implementation for study.  
    private BstDemo() {  // Prevent instantiation because this class is a static container.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class Node {  // BST node (key + left child + right child).  
        public int key;  // Store the node key (mutable so delete can replace with successor key).  
        public Node left;  // Store reference to left child (null means no child).  
        public Node right;  // Store reference to right child (null means no child).  

        public Node(int key) {  // Initialize node with a key and empty children.  
            this.key = key;  // Assign key.  
            this.left = null;  // Initialize left child as missing.  
            this.right = null;  // Initialize right child as missing.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class TreeSummary {  // Summarize key properties and inorder traversal for printing/tests.  
        public final int size;  // Number of nodes in the BST.  
        public final int height;  // Height measured in edges (empty=-1, leaf=0).  
        public final Integer minValue;  // Minimum key (null if empty).  
        public final Integer maxValue;  // Maximum key (null if empty).  
        public final int[] inorder;  // Inorder traversal (sorted keys for a valid BST).  

        public TreeSummary(int size, int height, Integer minValue, Integer maxValue, int[] inorder) {  // Construct immutable summary.  
            this.size = size;  // Store size.  
            this.height = height;  // Store height.  
            this.minValue = minValue;  // Store minimum value (nullable).  
            this.maxValue = maxValue;  // Store maximum value (nullable).  
            this.inorder = inorder;  // Store inorder traversal.  
        }  // Close constructor.  
    }  // End TreeSummary.  

    public static final class BinarySearchTree {  // A BST wrapper that holds a root node.  
        private Node root;  // Store root reference (null means empty tree).  

        public BinarySearchTree(Node root) {  // Initialize BST with an optional root.  
            this.root = root;  // Store root reference.  
        }  // Close constructor.  

        public static BinarySearchTree fromValues(int[] values) {  // Build a BST by inserting values in order.  
            BinarySearchTree t = new BinarySearchTree(null);  // Start from an empty tree.  
            for (int v : values) {  // Insert each value in the given order.  
                t.insert(v);  // Insert one key (duplicates are ignored by policy).  
            }  // Close loop.  
            return t;  // Return built BST.  
        }  // End fromValues().  

        public boolean isEmpty() {  // Check whether the BST has no nodes.  
            return this.root == null;  // Empty iff root is null.  
        }  // End isEmpty().  

        public boolean contains(int key) {  // Return true if key exists in the BST.  
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

        public boolean insert(int key) {  // Insert key into BST; return false when key already exists.  
            if (this.root == null) {  // Empty tree case: inserted node becomes the root.  
                this.root = new Node(key);  // Create root node.  
                return true;  // Report inserted.  
            }  // Close empty case.  

            Node parent = null;  // Track parent so we can attach the new node.  
            Node node = this.root;  // Start from root.  
            while (node != null) {  // Walk down until we find an empty spot.  
                parent = node;  // Update parent before moving.  
                if (key < node.key) {  // Go left when key is smaller.  
                    node = node.left;  // Move to left child.  
                } else if (key > node.key) {  // Go right when key is larger.  
                    node = node.right;  // Move to right child.  
                } else {  // key == node.key means duplicate key.  
                    return false;  // Reject duplicates by policy.  
                }  // Close compare branch.  
            }  // Close loop.  

            Node newNode = new Node(key);  // Create the new leaf node.  
            if (key < parent.key) {  // Attach to the left side when key is smaller.  
                parent.left = newNode;  // Link as left child.  
            } else {  // Otherwise attach to the right side.  
                parent.right = newNode;  // Link as right child.  
            }  // Close attach branch.  
            return true;  // Report inserted.  
        }  // End insert().  

        private static final class DeleteResult {  // Return (newSubtreeRoot, removed) for recursive deletes.  
            public final Node newRoot;  // The new subtree root after deletion.  
            public final boolean removed;  // Whether a node was removed.  

            public DeleteResult(Node newRoot, boolean removed) {  // Construct immutable result.  
                this.newRoot = newRoot;  // Store new root.  
                this.removed = removed;  // Store removed flag.  
            }  // Close constructor.  
        }  // End DeleteResult.  

        public boolean delete(int key) {  // Delete key from BST; return true when a node was removed.  
            DeleteResult r = deleteSubtree(this.root, key);  // Delete from root.  
            this.root = r.newRoot;  // Update root pointer (may change when deleting root).  
            return r.removed;  // Return whether deletion occurred.  
        }  // End delete().  

        private static DeleteResult deleteSubtree(Node node, int key) {  // Recursive delete that returns (newRoot, removed).  
            if (node == null) {  // Key not found in an empty subtree.  
                return new DeleteResult(null, false);  // Nothing removed.  
            }  // Close base case.  

            if (key < node.key) {  // Delete from left subtree when key is smaller.  
                DeleteResult r = deleteSubtree(node.left, key);  // Recurse into left child.  
                node.left = r.newRoot;  // Update left child pointer.  
                return new DeleteResult(node, r.removed);  // Return updated subtree.  
            }  // Close left branch.  
            if (key > node.key) {  // Delete from right subtree when key is larger.  
                DeleteResult r = deleteSubtree(node.right, key);  // Recurse into right child.  
                node.right = r.newRoot;  // Update right child pointer.  
                return new DeleteResult(node, r.removed);  // Return updated subtree.  
            }  // Close right branch.  

            // Now key == node.key, so we found the node to delete.  // Explain match branch.  
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
            int successorKey = minKey(node.right);  // Find successor key from right subtree.  
            node.key = successorKey;  // Copy successor key into current node.  
            DeleteResult r = deleteSubtree(node.right, successorKey);  // Delete successor node from right subtree.  
            node.right = r.newRoot;  // Update right child pointer after successor removal.  
            return new DeleteResult(node, true);  // Report successful deletion.  
        }  // End deleteSubtree().  

        public Integer min() {  // Return minimum key, or null if tree is empty.  
            if (this.root == null) {  // Empty tree has no minimum.  
                return null;  // Return null for empty.  
            }  // Close empty case.  
            return minKey(this.root);  // Return leftmost key.  
        }  // End min().  

        private static int minKey(Node node) {  // Return the minimum key in a non-empty subtree.  
            Node cur = node;  // Start at given node.  
            while (cur.left != null) {  // Keep going left until we cannot.  
                cur = cur.left;  // Move to left child.  
            }  // Close loop.  
            return cur.key;  // Leftmost node has the minimum key.  
        }  // End minKey().  

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

        public int size() {  // Return number of nodes in the BST (O(n)).  
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
            return toIntArray(out);  // Convert to primitive array for easy comparison.  
        }  // End inorder().  

        private static void inorderWalk(Node node, List<Integer> out) {  // Recursive inorder (left, root, right).  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            inorderWalk(node.left, out);  // Visit left subtree first.  
            out.add(node.key);  // Visit root.  
            inorderWalk(node.right, out);  // Visit right subtree.  
        }  // End inorderWalk().  

        private static int[] toIntArray(List<Integer> values) {  // Convert List<Integer> to int[] for compact results.  
            int[] out = new int[values.size()];  // Allocate exact-sized primitive array.  
            for (int i = 0; i < values.size(); i++) {  // Copy each element.  
                out[i] = values.get(i);  // Unbox Integer into int.  
            }  // Close loop.  
            return out;  // Return primitive array.  
        }  // End toIntArray().  

        public TreeSummary summarize() {  // Build a summary snapshot for demos/tests.  
            return new TreeSummary(  // Construct immutable summary record.  
                this.size(),  // Compute node count.  
                this.height(),  // Compute height (edges).  
                this.min(),  // Compute minimum key (nullable).  
                this.max(),  // Compute maximum key (nullable).  
                this.inorder()  // Compute inorder traversal.  
            );  // Close summary construction.  
        }  // End summarize().  
    }  // End BinarySearchTree.  

    public static String formatDemo() {  // Format a small demo output using a classic BST example.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic BST insert order used in many textbooks.  
        BinarySearchTree tree = BinarySearchTree.fromValues(values);  // Build BST by inserting in order.  
        TreeSummary s = tree.summarize();  // Summarize properties and inorder traversal.  
        StringBuilder sb = new StringBuilder();  // Accumulate output lines efficiently.  
        sb.append("=== Binary Search Tree Demo (Java) ===\n");  // Print header.  
        sb.append("insert order: ").append(Arrays.toString(values)).append("\n");  // Print insertion order.  
        sb.append("size=").append(s.size).append(", height=").append(s.height).append(", min=").append(s.minValue).append(", max=").append(s.maxValue).append("\n");  // Print key properties.  
        sb.append("inorder(sorted)=").append(Arrays.toString(s.inorder)).append("\n");  // Print inorder traversal.  
        sb.append("contains(6)=").append(tree.contains(6)).append(", contains(99)=").append(tree.contains(99)).append("\n");  // Print search examples.  
        tree.delete(7);  // Delete a leaf (7).  
        tree.delete(14);  // Delete a one-child node (14 has left child 13).  
        tree.delete(3);  // Delete a two-child node (3) using successor replacement.  
        sb.append("after deletes (7,14,3) inorder=").append(Arrays.toString(tree.inorder())).append("\n");  // Show inorder after deletions.  
        return sb.toString();  // Return formatted output.  
    }  // End formatDemo().  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        System.out.print(formatDemo());  // Print demo output.  
    }  // End main().  
}  // End BstDemo.  

