"""二元樹（Binary Tree）示範 / Binary tree demo.  # Bilingual module header.

本單元實作「二元樹（binary tree）」的基本結構與常見性質計算：  # Traditional Chinese overview.
- 我們用節點（node）儲存 value + left + right  # Explain node fields.
- 我們提供 size / height / count_leaves 等常用遞迴函式  # Explain typical tree properties.
- 我們也提供 preorder / inorder / postorder / level-order 走訪結果  # Explain traversals returned as lists.

We keep the implementation small and explicit so it reads like a study note set.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable demo records.  
from typing import List, Optional, Sequence  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make demo snapshots immutable and easy to print.  
class TreeSummary:  # Summarize key properties and traversals for printing/tests.  
    size: int  # Number of nodes in the tree.  
    height: int  # Height measured in edges (empty=-1, leaf=0).  
    leaves: int  # Number of leaf nodes.  
    preorder: List[int]  # Preorder traversal (root, left, right).  
    inorder: List[int]  # Inorder traversal (left, root, right).  
    postorder: List[int]  # Postorder traversal (left, right, root).  
    level_order: List[int]  # Level-order traversal (BFS).  


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
    def root(self) -> Optional[Node]:  # Expose the root for demos/tests (read-only semantics by convention).  
        return self._root  # Return root reference.  

    def is_empty(self) -> bool:  # Check whether the tree has no nodes.  
        return self._root is None  # Empty iff root is None.  

    @staticmethod
    def from_level_order(values: Sequence[Optional[int]]) -> BinaryTree:  # Build a tree from level-order array representation.  
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
            if node is None:  # Skip missing nodes (they have no children).  
                continue  # Move to next index.  
            left_i = 2 * i + 1  # Compute left child index in array representation.  
            right_i = 2 * i + 2  # Compute right child index in array representation.  
            if left_i < len(nodes):  # Link left child when index is in range.  
                node.left = nodes[left_i]  # Assign left child (may be None).  
            if right_i < len(nodes):  # Link right child when index is in range.  
                node.right = nodes[right_i]  # Assign right child (may be None).  

        return BinaryTree(root=nodes[0])  # Root is the node at index 0 (may be None).  

    def size(self) -> int:  # Return number of nodes in the tree (O(n)).  
        def _size(node: Optional[Node]) -> int:  # Recursive helper that counts nodes in a subtree.  
            if node is None:  # Empty subtree contributes 0 nodes.  
                return 0  # Return 0 for empty.  
            return 1 + _size(node.left) + _size(node.right)  # Count root + left + right.  

        return _size(self._root)  # Count nodes starting from root.  

    def height(self) -> int:  # Return tree height measured in edges (empty=-1, leaf=0).  
        def _height(node: Optional[Node]) -> int:  # Recursive helper that computes height for a subtree.  
            if node is None:  # Empty subtree has height -1 (so leaf children yield 0).  
                return -1  # Return -1 for empty.  
            return 1 + max(_height(node.left), _height(node.right))  # Height is 1 + max child height.  

        return _height(self._root)  # Compute height from root.  

    def count_leaves(self) -> int:  # Count leaf nodes (nodes with no children).  
        def _count(node: Optional[Node]) -> int:  # Recursive helper that counts leaves in a subtree.  
            if node is None:  # Empty subtree has 0 leaves.  
                return 0  # Return 0 for empty.  
            if node.left is None and node.right is None:  # Leaf node has no children.  
                return 1  # Count this leaf.  
            return _count(node.left) + _count(node.right)  # Sum leaves in left and right subtrees.  

        return _count(self._root)  # Count leaves from root.  

    def preorder(self) -> List[int]:  # Return preorder traversal as a list of values.  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for preorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            out.append(node.value)  # Visit root first.  
            _walk(node.left)  # Visit left subtree.  
            _walk(node.right)  # Visit right subtree.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def inorder(self) -> List[int]:  # Return inorder traversal as a list of values.  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for inorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            _walk(node.left)  # Visit left subtree first.  
            out.append(node.value)  # Visit root.  
            _walk(node.right)  # Visit right subtree.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def postorder(self) -> List[int]:  # Return postorder traversal as a list of values.  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for postorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            _walk(node.left)  # Visit left subtree first.  
            _walk(node.right)  # Visit right subtree next.  
            out.append(node.value)  # Visit root last.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def level_order(self) -> List[int]:  # Return level-order traversal (BFS) as a list of values.  
        if self._root is None:  # Empty tree has empty traversal.  
            return []  # Return empty list.  

        queue: List[Node] = [self._root]  # Use a list as a queue storage.  
        head = 0  # Maintain a head index so dequeue is O(1) without shifting.  
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

    def summarize(self) -> TreeSummary:  # Build a snapshot summary for demos/tests.  
        return TreeSummary(  # Construct a summary record.  
            size=self.size(),  # Compute node count.  
            height=self.height(),  # Compute height (edges).  
            leaves=self.count_leaves(),  # Compute leaf count.  
            preorder=self.preorder(),  # Compute preorder traversal.  
            inorder=self.inorder(),  # Compute inorder traversal.  
            postorder=self.postorder(),  # Compute postorder traversal.  
            level_order=self.level_order(),  # Compute level-order traversal.  
        )  # Close summary construction.  


def format_demo() -> str:  # Format a small demo output using a sample tree.  
    values: List[Optional[int]] = [1, 2, 3, 4, 5]  # Build sample tree: 1 with children 2 and 3; 2 has 4 and 5.  
    tree = BinaryTree.from_level_order(values)  # Build tree from level-order list.  
    s = tree.summarize()  # Summarize properties and traversals.  
    lines: List[str] = []  # Accumulate printable lines.  
    lines.append("=== Binary Tree Demo (Python) ===")  # Print header.  
    lines.append(f"level_order input: {values}")  # Print input list.  
    lines.append(f"size={s.size}, height={s.height}, leaves={s.leaves}")  # Print key properties.  
    lines.append(f"preorder={s.preorder}")  # Print preorder list.  
    lines.append(f"inorder={s.inorder}")  # Print inorder list.  
    lines.append(f"postorder={s.postorder}")  # Print postorder list.  
    lines.append(f"level_order={s.level_order}")  # Print BFS list.  
    return "\n".join(lines)  # Join lines into one printable string.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    print(format_demo())  # Print demo output.  

