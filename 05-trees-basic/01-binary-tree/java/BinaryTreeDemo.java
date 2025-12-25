// 01 二元樹示範（Java）/ Binary tree demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to build traversals before converting to arrays.  
import java.util.Arrays;  // Use Arrays.toString for readable demo output.  
import java.util.List;  // Use List for type clarity when collecting values.  

public final class BinaryTreeDemo {  // Provide a small, explicit BinaryTree implementation for study.  
    private BinaryTreeDemo() {  // Prevent instantiation because this class is a static container.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class Node {  // Binary tree node (value + left child + right child).  
        public final int value;  // Store the node's value.  
        public Node left;  // Store reference to left child (null means no child).  
        public Node right;  // Store reference to right child (null means no child).  

        public Node(int value) {  // Initialize node with a value and empty children.  
            this.value = value;  // Assign value.  
            this.left = null;  // Initialize left child as missing.  
            this.right = null;  // Initialize right child as missing.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class TreeSummary {  // Summarize key properties and traversals (teaching-friendly).  
        public final int size;  // Number of nodes in the tree.  
        public final int height;  // Height measured in edges (empty=-1, leaf=0).  
        public final int leaves;  // Number of leaf nodes.  
        public final int[] preorder;  // Preorder traversal (root, left, right).  
        public final int[] inorder;  // Inorder traversal (left, root, right).  
        public final int[] postorder;  // Postorder traversal (left, right, root).  
        public final int[] levelOrder;  // Level-order traversal (BFS).  

        public TreeSummary(int size, int height, int leaves, int[] preorder, int[] inorder, int[] postorder, int[] levelOrder) {  // Construct immutable summary.  
            this.size = size;  // Store size.  
            this.height = height;  // Store height.  
            this.leaves = leaves;  // Store leaf count.  
            this.preorder = preorder;  // Store preorder traversal.  
            this.inorder = inorder;  // Store inorder traversal.  
            this.postorder = postorder;  // Store postorder traversal.  
            this.levelOrder = levelOrder;  // Store level-order traversal.  
        }  // Close constructor.  
    }  // End TreeSummary.  

    public static final class BinaryTree {  // A simple wrapper that holds a root node.  
        private final Node root;  // Store root reference (null means empty tree).  

        public BinaryTree(Node root) {  // Create a tree with the given root (may be null).  
            this.root = root;  // Store root.  
        }  // Close constructor.  

        public Node root() {  // Expose root reference for demos/tests (read-only by convention).  
            return this.root;  // Return root.  
        }  // End root().  

        public boolean isEmpty() {  // Check whether the tree has no nodes.  
            return this.root == null;  // Empty iff root is null.  
        }  // End isEmpty().  

        public static BinaryTree fromLevelOrder(Integer[] values) {  // Build a tree from a level-order array representation (null means missing node).  
            if (values == null || values.length == 0) {  // Treat null/empty array as an empty tree.  
                return new BinaryTree(null);  // Return empty tree.  
            }  // Close validation.  

            Node[] nodes = new Node[values.length];  // Allocate parallel node array (null for missing).  
            for (int i = 0; i < values.length; i++) {  // First pass: create Node for each non-null value.  
                Integer v = values[i];  // Read value at index i.  
                nodes[i] = (v == null) ? null : new Node(v);  // Create node or keep null.  
            }  // Close first pass.  

            for (int i = 0; i < values.length; i++) {  // Second pass: link children using array indices.  
                Node node = nodes[i];  // Read node at index i.  
                if (node == null) {  // Skip missing nodes because they have no children.  
                    continue;  // Move to next index.  
                }  // Close skip branch.  
                int leftIndex = 2 * i + 1;  // Compute left child index.  
                int rightIndex = 2 * i + 2;  // Compute right child index.  
                if (leftIndex < nodes.length) {  // Link left child when index is in range.  
                    node.left = nodes[leftIndex];  // Assign left child (may be null).  
                }  // Close left link.  
                if (rightIndex < nodes.length) {  // Link right child when index is in range.  
                    node.right = nodes[rightIndex];  // Assign right child (may be null).  
                }  // Close right link.  
            }  // Close second pass.  

            return new BinaryTree(nodes[0]);  // Root is at index 0 (may be null).  
        }  // End fromLevelOrder().  

        public int size() {  // Return number of nodes in the tree (O(n)).  
            return sizeOf(this.root);  // Count nodes starting from root.  
        }  // End size().  

        private static int sizeOf(Node node) {  // Recursive helper that counts nodes in a subtree.  
            if (node == null) {  // Empty subtree contributes 0 nodes.  
                return 0;  // Return 0 for empty.  
            }  // Close base case.  
            return 1 + sizeOf(node.left) + sizeOf(node.right);  // Count root + left + right.  
        }  // End sizeOf().  

        public int height() {  // Return tree height measured in edges (empty=-1, leaf=0).  
            return heightOf(this.root);  // Compute height from root.  
        }  // End height().  

        private static int heightOf(Node node) {  // Recursive helper that computes height in edges.  
            if (node == null) {  // Empty subtree has height -1 (so a leaf has height 0).  
                return -1;  // Return -1 for empty.  
            }  // Close base case.  
            return 1 + Math.max(heightOf(node.left), heightOf(node.right));  // Height is 1 + max(child heights).  
        }  // End heightOf().  

        public int countLeaves() {  // Return number of leaf nodes (O(n)).  
            return leavesOf(this.root);  // Count leaves from root.  
        }  // End countLeaves().  

        private static int leavesOf(Node node) {  // Recursive helper that counts leaf nodes in a subtree.  
            if (node == null) {  // Empty subtree contributes 0 leaves.  
                return 0;  // Return 0 for empty.  
            }  // Close base case.  
            if (node.left == null && node.right == null) {  // A node with no children is a leaf.  
                return 1;  // Count this leaf.  
            }  // Close leaf case.  
            return leavesOf(node.left) + leavesOf(node.right);  // Sum leaves from both subtrees.  
        }  // End leavesOf().  

        public int[] preorder() {  // Return preorder traversal as an int array.  
            List<Integer> out = new ArrayList<>();  // Accumulate values during traversal.  
            preorderWalk(this.root, out);  // Walk from root.  
            return toIntArray(out);  // Convert to primitive array for easy comparison.  
        }  // End preorder().  

        private static void preorderWalk(Node node, List<Integer> out) {  // Recursive preorder (root, left, right).  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            out.add(node.value);  // Visit root first.  
            preorderWalk(node.left, out);  // Visit left subtree.  
            preorderWalk(node.right, out);  // Visit right subtree.  
        }  // End preorderWalk().  

        public int[] inorder() {  // Return inorder traversal as an int array.  
            List<Integer> out = new ArrayList<>();  // Accumulate values during traversal.  
            inorderWalk(this.root, out);  // Walk from root.  
            return toIntArray(out);  // Convert to primitive array for easy comparison.  
        }  // End inorder().  

        private static void inorderWalk(Node node, List<Integer> out) {  // Recursive inorder (left, root, right).  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            inorderWalk(node.left, out);  // Visit left subtree first.  
            out.add(node.value);  // Visit root.  
            inorderWalk(node.right, out);  // Visit right subtree.  
        }  // End inorderWalk().  

        public int[] postorder() {  // Return postorder traversal as an int array.  
            List<Integer> out = new ArrayList<>();  // Accumulate values during traversal.  
            postorderWalk(this.root, out);  // Walk from root.  
            return toIntArray(out);  // Convert to primitive array for easy comparison.  
        }  // End postorder().  

        private static void postorderWalk(Node node, List<Integer> out) {  // Recursive postorder (left, right, root).  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            postorderWalk(node.left, out);  // Visit left subtree first.  
            postorderWalk(node.right, out);  // Visit right subtree next.  
            out.add(node.value);  // Visit root last.  
        }  // End postorderWalk().  

        public int[] levelOrder() {  // Return level-order traversal (BFS) as an int array.  
            if (this.root == null) {  // Empty tree has empty traversal.  
                return new int[0];  // Return empty array.  
            }  // Close empty case.  

            List<Node> queue = new ArrayList<>();  // Use ArrayList as a queue storage.  
            int head = 0;  // Maintain a head index so dequeue is O(1) without shifting.  
            List<Integer> out = new ArrayList<>();  // Accumulate visited values.  

            queue.add(this.root);  // Enqueue root node.  
            while (head < queue.size()) {  // Process queue until head reaches the end.  
                Node node = queue.get(head);  // Read next node to process.  
                head += 1;  // Advance head (logical dequeue).  
                out.add(node.value);  // Visit node in BFS order.  
                if (node.left != null) {  // Enqueue left child when present.  
                    queue.add(node.left);  // Append left child to queue tail.  
                }  // Close left enqueue.  
                if (node.right != null) {  // Enqueue right child when present.  
                    queue.add(node.right);  // Append right child to queue tail.  
                }  // Close right enqueue.  
            }  // Close BFS loop.  

            return toIntArray(out);  // Convert to primitive array for easy comparison.  
        }  // End levelOrder().  

        public TreeSummary summarize() {  // Build a summary snapshot for demos/tests.  
            return new TreeSummary(  // Construct immutable summary record.  
                this.size(),  // Compute node count.  
                this.height(),  // Compute height (edges).  
                this.countLeaves(),  // Compute leaf count.  
                this.preorder(),  // Compute preorder traversal.  
                this.inorder(),  // Compute inorder traversal.  
                this.postorder(),  // Compute postorder traversal.  
                this.levelOrder()  // Compute level-order traversal.  
            );  // Close summary construction.  
        }  // End summarize().  

        private static int[] toIntArray(List<Integer> values) {  // Convert List<Integer> to int[] for compact results.  
            int[] out = new int[values.size()];  // Allocate exact-sized primitive array.  
            for (int i = 0; i < values.size(); i++) {  // Copy each element.  
                out[i] = values.get(i);  // Unbox Integer into int.  
            }  // Close loop.  
            return out;  // Return primitive array.  
        }  // End toIntArray().  
    }  // End BinaryTree.  

    public static String formatDemo() {  // Format a small demo output using a sample tree.  
        Integer[] values = new Integer[] {1, 2, 3, 4, 5};  // Build sample tree: 1 with children 2 and 3; 2 has 4 and 5.  
        BinaryTree tree = BinaryTree.fromLevelOrder(values);  // Build tree from level-order list.  
        TreeSummary s = tree.summarize();  // Summarize properties and traversals.  
        StringBuilder sb = new StringBuilder();  // Accumulate output lines efficiently.  
        sb.append("=== Binary Tree Demo (Java) ===\n");  // Print header.  
        sb.append("level_order input: ").append(Arrays.toString(values)).append("\n");  // Print input array.  
        sb.append("size=").append(s.size).append(", height=").append(s.height).append(", leaves=").append(s.leaves).append("\n");  // Print key properties.  
        sb.append("preorder=").append(Arrays.toString(s.preorder)).append("\n");  // Print preorder.  
        sb.append("inorder=").append(Arrays.toString(s.inorder)).append("\n");  // Print inorder.  
        sb.append("postorder=").append(Arrays.toString(s.postorder)).append("\n");  // Print postorder.  
        sb.append("level_order=").append(Arrays.toString(s.levelOrder)).append("\n");  // Print BFS.  
        return sb.toString();  // Return formatted output.  
    }  // End formatDemo().  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        System.out.print(formatDemo());  // Print demo output.  
    }  // End main().  
}  // End BinaryTreeDemo.  

