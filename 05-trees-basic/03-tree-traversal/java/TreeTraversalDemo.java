// 03 樹走訪示範（Java）/ Tree traversal demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to build traversal outputs and to implement stacks/queues.  
import java.util.Arrays;  // Use Arrays.toString for readable demo output.  
import java.util.List;  // Use List for type clarity when collecting values.  

public final class TreeTraversalDemo {  // Provide traversal implementations (recursive vs iterative) for study.  
    private TreeTraversalDemo() {  // Prevent instantiation because this class is a static container.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class Node {  // Binary tree node (value + left child + right child).  
        public final int value;  // Store node value.  
        public Node left;  // Store left child pointer (null means no child).  
        public Node right;  // Store right child pointer (null means no child).  

        public Node(int value) {  // Initialize node with a value and empty children.  
            this.value = value;  // Assign value.  
            this.left = null;  // Initialize left child as missing.  
            this.right = null;  // Initialize right child as missing.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class TraversalSummary {  // Summarize recursive/iterative traversal outputs for printing/tests.  
        public final int[] preorderRecursive;  // Preorder traversal via recursion.  
        public final int[] preorderIterative;  // Preorder traversal via explicit stack.  
        public final int[] inorderRecursive;  // Inorder traversal via recursion.  
        public final int[] inorderIterative;  // Inorder traversal via explicit stack.  
        public final int[] postorderRecursive;  // Postorder traversal via recursion.  
        public final int[] postorderIterative;  // Postorder traversal via explicit stack.  
        public final int[] levelOrder;  // Level-order traversal via BFS queue.  

        public TraversalSummary(int[] preorderRecursive, int[] preorderIterative, int[] inorderRecursive, int[] inorderIterative, int[] postorderRecursive, int[] postorderIterative, int[] levelOrder) {  // Construct immutable summary.  
            this.preorderRecursive = preorderRecursive;  // Store recursive preorder.  
            this.preorderIterative = preorderIterative;  // Store iterative preorder.  
            this.inorderRecursive = inorderRecursive;  // Store recursive inorder.  
            this.inorderIterative = inorderIterative;  // Store iterative inorder.  
            this.postorderRecursive = postorderRecursive;  // Store recursive postorder.  
            this.postorderIterative = postorderIterative;  // Store iterative postorder.  
            this.levelOrder = levelOrder;  // Store level-order traversal.  
        }  // Close constructor.  
    }  // End TraversalSummary.  

    public static final class BinaryTree {  // A binary tree wrapper that holds a root node.  
        private final Node root;  // Store root reference (null means empty tree).  

        public BinaryTree(Node root) {  // Initialize tree with an optional root.  
            this.root = root;  // Store root reference.  
        }  // Close constructor.  

        public static BinaryTree fromLevelOrder(Integer[] values) {  // Build a tree from level-order array representation (null means missing node).  
            if (values == null || values.length == 0) {  // Treat null/empty array as an empty tree.  
                return new BinaryTree(null);  // Return empty tree.  
            }  // Close validation.  

            Node[] nodes = new Node[values.length];  // Allocate parallel node array (null for missing).  
            for (int i = 0; i < values.length; i++) {  // First pass: create nodes for non-null values.  
                Integer v = values[i];  // Read value at index i.  
                nodes[i] = (v == null) ? null : new Node(v);  // Create node or keep null.  
            }  // Close first pass.  

            for (int i = 0; i < values.length; i++) {  // Second pass: link children using array indices.  
                Node node = nodes[i];  // Read node at index i.  
                if (node == null) {  // Skip missing nodes.  
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
            }  // Close linking pass.  

            return new BinaryTree(nodes[0]);  // Root is at index 0 (may be null).  
        }  // End fromLevelOrder().  

        public int[] preorderRecursive() {  // Return preorder traversal via recursion (root, left, right).  
            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            preorderWalk(this.root, out);  // Walk from root.  
            return toIntArray(out);  // Convert to primitive array.  
        }  // End preorderRecursive().  

        private static void preorderWalk(Node node, List<Integer> out) {  // Recursive preorder helper.  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            out.add(node.value);  // Visit root first.  
            preorderWalk(node.left, out);  // Visit left subtree.  
            preorderWalk(node.right, out);  // Visit right subtree.  
        }  // End preorderWalk().  

        public int[] inorderRecursive() {  // Return inorder traversal via recursion (left, root, right).  
            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            inorderWalk(this.root, out);  // Walk from root.  
            return toIntArray(out);  // Convert to primitive array.  
        }  // End inorderRecursive().  

        private static void inorderWalk(Node node, List<Integer> out) {  // Recursive inorder helper.  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            inorderWalk(node.left, out);  // Visit left subtree first.  
            out.add(node.value);  // Visit root.  
            inorderWalk(node.right, out);  // Visit right subtree.  
        }  // End inorderWalk().  

        public int[] postorderRecursive() {  // Return postorder traversal via recursion (left, right, root).  
            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            postorderWalk(this.root, out);  // Walk from root.  
            return toIntArray(out);  // Convert to primitive array.  
        }  // End postorderRecursive().  

        private static void postorderWalk(Node node, List<Integer> out) {  // Recursive postorder helper.  
            if (node == null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            postorderWalk(node.left, out);  // Visit left subtree first.  
            postorderWalk(node.right, out);  // Visit right subtree next.  
            out.add(node.value);  // Visit root last.  
        }  // End postorderWalk().  

        public int[] preorderIterative() {  // Return preorder traversal via explicit stack (root, left, right).  
            if (this.root == null) {  // Empty tree has empty traversal.  
                return new int[0];  // Return empty array.  
            }  // Close empty case.  

            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            List<Node> stack = new ArrayList<>();  // Use ArrayList as a stack.  
            stack.add(this.root);  // Push root onto stack.  

            while (!stack.isEmpty()) {  // Process until stack is empty.  
                Node node = stack.remove(stack.size() - 1);  // Pop top node (LIFO).  
                out.add(node.value);  // Visit node when popped (preorder).  
                if (node.right != null) {  // Push right child first so left is processed first.  
                    stack.add(node.right);  // Push right child.  
                }  // Close right push.  
                if (node.left != null) {  // Push left child last.  
                    stack.add(node.left);  // Push left child.  
                }  // Close left push.  
            }  // Close loop.  

            return toIntArray(out);  // Convert to primitive array.  
        }  // End preorderIterative().  

        public int[] inorderIterative() {  // Return inorder traversal via explicit stack (left, root, right).  
            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            List<Node> stack = new ArrayList<>();  // Use ArrayList as a stack.  
            Node node = this.root;  // Start from root.  

            while (node != null || !stack.isEmpty()) {  // Continue while there is a node to visit or stack is non-empty.  
                while (node != null) {  // Walk left as far as possible.  
                    stack.add(node);  // Push current node so we can come back later.  
                    node = node.left;  // Move to left child.  
                }  // Close left-walk loop.  
                node = stack.remove(stack.size() - 1);  // Pop the next node whose left subtree is done.  
                out.add(node.value);  // Visit node (inorder).  
                node = node.right;  // Move to right subtree next.  
            }  // Close main loop.  

            return toIntArray(out);  // Convert to primitive array.  
        }  // End inorderIterative().  

        private static final class Frame {  // Store a node plus a visited flag for iterative postorder.  
            public final Node node;  // Node pointer for this frame.  
            public final boolean visited;  // True means children were already processed.  

            public Frame(Node node, boolean visited) {  // Construct immutable frame.  
                this.node = node;  // Store node.  
                this.visited = visited;  // Store visited flag.  
            }  // Close constructor.  
        }  // End Frame.  

        public int[] postorderIterative() {  // Return postorder traversal via explicit stack (left, right, root).  
            if (this.root == null) {  // Empty tree has empty traversal.  
                return new int[0];  // Return empty array.  
            }  // Close empty case.  

            List<Integer> out = new ArrayList<>();  // Accumulate traversal result.  
            List<Frame> stack = new ArrayList<>();  // Use ArrayList as a stack of frames.  
            stack.add(new Frame(this.root, false));  // Push root frame (not visited).  

            while (!stack.isEmpty()) {  // Process until stack is empty.  
                Frame f = stack.remove(stack.size() - 1);  // Pop top frame.  
                if (f.visited) {  // When visited==true, children were already processed.  
                    out.add(f.node.value);  // Visit node last (postorder).  
                    continue;  // Move to next frame.  
                }  // Close visited branch.  
                stack.add(new Frame(f.node, true));  // Re-push node marked as visited (to output after children).  
                if (f.node.right != null) {  // Push right child first so left is processed first.  
                    stack.add(new Frame(f.node.right, false));  // Process right subtree.  
                }  // Close right push.  
                if (f.node.left != null) {  // Push left child last.  
                    stack.add(new Frame(f.node.left, false));  // Process left subtree first.  
                }  // Close left push.  
            }  // Close loop.  

            return toIntArray(out);  // Convert to primitive array.  
        }  // End postorderIterative().  

        public int[] levelOrder() {  // Return level-order traversal (BFS) as an int array.  
            if (this.root == null) {  // Empty tree has empty traversal.  
                return new int[0];  // Return empty array.  
            }  // Close empty case.  

            List<Node> queue = new ArrayList<>();  // Use ArrayList as a queue storage.  
            int head = 0;  // Maintain a head index so dequeue is O(1) without shifting.  
            List<Integer> out = new ArrayList<>();  // Accumulate visited values.  

            queue.add(this.root);  // Enqueue root.  
            while (head < queue.size()) {  // Process queue until head reaches the end.  
                Node node = queue.get(head);  // Read next node to process.  
                head += 1;  // Advance head (logical dequeue).  
                out.add(node.value);  // Visit node in BFS order.  
                if (node.left != null) {  // Enqueue left child when present.  
                    queue.add(node.left);  // Append to queue tail.  
                }  // Close left enqueue.  
                if (node.right != null) {  // Enqueue right child when present.  
                    queue.add(node.right);  // Append to queue tail.  
                }  // Close right enqueue.  
            }  // Close BFS loop.  

            return toIntArray(out);  // Convert to primitive array.  
        }  // End levelOrder().  

        public TraversalSummary summarize() {  // Build a summary snapshot for demos/tests.  
            return new TraversalSummary(  // Construct immutable summary record.  
                preorderRecursive(),  // Compute recursive preorder.  
                preorderIterative(),  // Compute iterative preorder.  
                inorderRecursive(),  // Compute recursive inorder.  
                inorderIterative(),  // Compute iterative inorder.  
                postorderRecursive(),  // Compute recursive postorder.  
                postorderIterative(),  // Compute iterative postorder.  
                levelOrder()  // Compute BFS traversal.  
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
        Integer[] values = new Integer[] {1, 2, 3, 4, 5};  // Sample tree used across this repo for traversal examples.  
        BinaryTree tree = BinaryTree.fromLevelOrder(values);  // Build tree from level-order list.  
        TraversalSummary s = tree.summarize();  // Summarize all traversal variants.  
        StringBuilder sb = new StringBuilder();  // Accumulate output lines efficiently.  
        sb.append("=== Tree Traversal Demo (Java) ===\n");  // Print header.  
        sb.append("level_order input: ").append(Arrays.toString(values)).append("\n");  // Print input array.  
        sb.append("preorder_recursive=").append(Arrays.toString(s.preorderRecursive)).append("\n");  // Print recursive preorder.  
        sb.append("preorder_iterative=").append(Arrays.toString(s.preorderIterative)).append("\n");  // Print iterative preorder.  
        sb.append("inorder_recursive=").append(Arrays.toString(s.inorderRecursive)).append("\n");  // Print recursive inorder.  
        sb.append("inorder_iterative=").append(Arrays.toString(s.inorderIterative)).append("\n");  // Print iterative inorder.  
        sb.append("postorder_recursive=").append(Arrays.toString(s.postorderRecursive)).append("\n");  // Print recursive postorder.  
        sb.append("postorder_iterative=").append(Arrays.toString(s.postorderIterative)).append("\n");  // Print iterative postorder.  
        sb.append("level_order=").append(Arrays.toString(s.levelOrder)).append("\n");  // Print level-order BFS.  
        return sb.toString();  // Return formatted output.  
    }  // End formatDemo().  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        System.out.print(formatDemo());  // Print demo output.  
    }  // End main().  
}  // End TreeTraversalDemo.  

