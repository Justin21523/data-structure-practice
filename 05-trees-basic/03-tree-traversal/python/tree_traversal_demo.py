"""樹走訪（Tree Traversal）示範 / Tree traversal demo.  # Bilingual module header.

本單元的重點是「走訪順序」與「遞迴 vs 迭代（stack）」：  # Traditional Chinese overview.
- preorder / inorder / postorder（DFS）  # Explain DFS variants.
- level-order（BFS）  # Explain BFS.
- DFS 同時提供 recursive 與 iterative 兩套版本  # Explain both implementations.

We keep the implementation small and explicit so it reads like a study note set.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable demo records.  
from typing import List, Optional, Sequence, Tuple  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make demo snapshots immutable and easy to print.  
class TraversalSummary:  # Summarize recursive/iterative traversal outputs for printing/tests.  
    preorder_recursive: List[int]  # Preorder traversal via recursion.  
    preorder_iterative: List[int]  # Preorder traversal via explicit stack.  
    inorder_recursive: List[int]  # Inorder traversal via recursion.  
    inorder_iterative: List[int]  # Inorder traversal via explicit stack.  
    postorder_recursive: List[int]  # Postorder traversal via recursion.  
    postorder_iterative: List[int]  # Postorder traversal via explicit stack.  
    level_order: List[int]  # Level-order traversal via BFS queue.  


class Node:  # Binary tree node (value + left child + right child).  
    __slots__ = ("value", "left", "right")  # Keep the node minimal and explicit.  

    def __init__(self, value: int) -> None:  # Initialize a node with a value and empty children.  
        self.value = value  # Store node value.  
        self.left: Optional[Node] = None  # Store left child pointer (None means no child).  
        self.right: Optional[Node] = None  # Store right child pointer (None means no child).  


class BinaryTree:  # A binary tree wrapper that holds a root node.  
    def __init__(self, root: Optional[Node] = None) -> None:  # Initialize tree with an optional root.  
        self._root = root  # Store root reference (None means empty tree).  

    @property
    def root(self) -> Optional[Node]:  # Expose root for demos/tests (read-only semantics by convention).  
        return self._root  # Return root reference.  

    @staticmethod
    def from_level_order(values: Sequence[Optional[int]]) -> BinaryTree:  # Build a tree from level-order array representation (None means missing node).  
        if not values:  # Empty list builds an empty tree.  
            return BinaryTree(root=None)  # Return empty tree.  

        nodes: List[Optional[Node]] = [None] * len(values)  # Allocate parallel node array (None for missing).  
        for i, v in enumerate(values):  # First pass: create nodes for non-None values.  
            if v is None:  # Keep missing nodes as None.  
                nodes[i] = None  # Store None placeholder.  
                continue  # Move to next index.  
            nodes[i] = Node(int(v))  # Create node for this value.  

        for i in range(len(values)):  # Second pass: link children using array indices.  
            node = nodes[i]  # Read node at index i.  
            if node is None:  # Skip missing nodes.  
                continue  # Move to next index.  
            left_i = 2 * i + 1  # Compute left child index.  
            right_i = 2 * i + 2  # Compute right child index.  
            if left_i < len(nodes):  # Link left child when index is in range.  
                node.left = nodes[left_i]  # Assign left child (may be None).  
            if right_i < len(nodes):  # Link right child when index is in range.  
                node.right = nodes[right_i]  # Assign right child (may be None).  

        return BinaryTree(root=nodes[0])  # Root is at index 0 (may be None).  

    # =========================  # Visual separator for traversal APIs.  
    # Recursive DFS traversals  # Section title.  
    # =========================  # End separator.  

    def preorder_recursive(self) -> List[int]:  # Return preorder traversal via recursion (root, left, right).  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for preorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            out.append(node.value)  # Visit root first.  
            _walk(node.left)  # Visit left subtree.  
            _walk(node.right)  # Visit right subtree.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def inorder_recursive(self) -> List[int]:  # Return inorder traversal via recursion (left, root, right).  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for inorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            _walk(node.left)  # Visit left subtree first.  
            out.append(node.value)  # Visit root.  
            _walk(node.right)  # Visit right subtree.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def postorder_recursive(self) -> List[int]:  # Return postorder traversal via recursion (left, right, root).  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for postorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            _walk(node.left)  # Visit left subtree first.  
            _walk(node.right)  # Visit right subtree next.  
            out.append(node.value)  # Visit root last.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    # =========================  # Visual separator for traversal APIs.  
    # Iterative DFS traversals  # Section title.  
    # =========================  # End separator.  

    def preorder_iterative(self) -> List[int]:  # Return preorder traversal via explicit stack (root, left, right).  
        if self._root is None:  # Empty tree has empty traversal.  
            return []  # Return empty list.  

        out: List[int] = []  # Accumulate traversal result.  
        stack: List[Node] = [self._root]  # Use list as stack; start with root.  
        while stack:  # Process until stack is empty.  
            node = stack.pop()  # Pop the top node (LIFO).  
            out.append(node.value)  # Visit node when popped (preorder).  
            if node.right is not None:  # Push right child first so left is processed first.  
                stack.append(node.right)  # Push right child.  
            if node.left is not None:  # Push left child last.  
                stack.append(node.left)  # Push left child.  

        return out  # Return traversal list.  

    def inorder_iterative(self) -> List[int]:  # Return inorder traversal via explicit stack (left, root, right).  
        out: List[int] = []  # Accumulate traversal result.  
        stack: List[Node] = []  # Use list as stack to store the path to current node.  
        node = self._root  # Start from root.  

        while node is not None or stack:  # Continue while there is a node to visit or stack is non-empty.  
            while node is not None:  # Walk left as far as possible.  
                stack.append(node)  # Push current node so we can come back later.  
                node = node.left  # Move to left child.  
            node = stack.pop()  # Pop the next node whose left subtree is done.  
            out.append(node.value)  # Visit node (inorder).  
            node = node.right  # Move to right subtree next.  

        return out  # Return traversal list.  

    def postorder_iterative(self) -> List[int]:  # Return postorder traversal via explicit stack (left, right, root).  
        if self._root is None:  # Empty tree has empty traversal.  
            return []  # Return empty list.  

        out: List[int] = []  # Accumulate traversal result.  
        stack: List[Tuple[Node, bool]] = [(self._root, False)]  # Store (node, visited) so we can output after children.  
        while stack:  # Process until stack is empty.  
            node, visited = stack.pop()  # Pop one frame.  
            if visited:  # When visited==True, children were already processed.  
                out.append(node.value)  # Visit node last (postorder).  
                continue  # Move to next stack frame.  
            stack.append((node, True))  # Re-push node marked as visited (to output after children).  
            if node.right is not None:  # Push right child first so left is processed first.  
                stack.append((node.right, False))  # Process right subtree before returning to node.  
            if node.left is not None:  # Push left child last.  
                stack.append((node.left, False))  # Process left subtree first.  

        return out  # Return traversal list.  

    # =========================  # Visual separator for BFS traversal.  
    # BFS traversal (queue)     # Section title.  
    # =========================  # End separator.  

    def level_order(self) -> List[int]:  # Return level-order traversal (BFS) as a list of values.  
        if self._root is None:  # Empty tree has empty traversal.  
            return []  # Return empty list.  

        queue: List[Node] = [self._root]  # Use a list as queue storage.  
        head = 0  # Maintain head index so dequeue is O(1) without shifting.  
        out: List[int] = []  # Accumulate traversal result.  

        while head < len(queue):  # Process queue until head reaches the end.  
            node = queue[head]  # Read next node to process.  
            head += 1  # Advance head (logical dequeue).  
            out.append(node.value)  # Visit node in BFS order.  
            if node.left is not None:  # Enqueue left child when present.  
                queue.append(node.left)  # Append to queue tail.  
            if node.right is not None:  # Enqueue right child when present.  
                queue.append(node.right)  # Append to queue tail.  

        return out  # Return traversal list.  

    def summarize(self) -> TraversalSummary:  # Build a summary snapshot for demos/tests.  
        return TraversalSummary(  # Construct summary record.  
            preorder_recursive=self.preorder_recursive(),  # Compute recursive preorder.  
            preorder_iterative=self.preorder_iterative(),  # Compute iterative preorder.  
            inorder_recursive=self.inorder_recursive(),  # Compute recursive inorder.  
            inorder_iterative=self.inorder_iterative(),  # Compute iterative inorder.  
            postorder_recursive=self.postorder_recursive(),  # Compute recursive postorder.  
            postorder_iterative=self.postorder_iterative(),  # Compute iterative postorder.  
            level_order=self.level_order(),  # Compute BFS traversal.  
        )  # Close summary construction.  


def format_demo() -> str:  # Format a small demo output using a sample tree.  
    values: List[Optional[int]] = [1, 2, 3, 4, 5]  # Sample tree used across this repo for traversal examples.  
    tree = BinaryTree.from_level_order(values)  # Build tree from level-order list.  
    s = tree.summarize()  # Summarize all traversal variants.  
    lines: List[str] = []  # Accumulate printable lines.  
    lines.append("=== Tree Traversal Demo (Python) ===")  # Print header.  
    lines.append(f"level_order input: {values}")  # Print input list.  
    lines.append(f"preorder_recursive={s.preorder_recursive}")  # Print recursive preorder.  
    lines.append(f"preorder_iterative={s.preorder_iterative}")  # Print iterative preorder.  
    lines.append(f"inorder_recursive={s.inorder_recursive}")  # Print recursive inorder.  
    lines.append(f"inorder_iterative={s.inorder_iterative}")  # Print iterative inorder.  
    lines.append(f"postorder_recursive={s.postorder_recursive}")  # Print recursive postorder.  
    lines.append(f"postorder_iterative={s.postorder_iterative}")  # Print iterative postorder.  
    lines.append(f"level_order={s.level_order}")  # Print level-order BFS.  
    return "\n".join(lines)  # Join lines into one printable string.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    print(format_demo())  # Print demo output.  

