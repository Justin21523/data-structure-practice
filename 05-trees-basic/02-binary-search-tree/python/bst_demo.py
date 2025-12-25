"""二元搜尋樹（BST）示範 / Binary Search Tree (BST) demo.  # Bilingual module header.

本單元實作「二元搜尋樹（binary search tree, BST）」的基本操作：  # Traditional Chinese overview.
- insert / contains / delete（含 0 子、1 子、2 子）  # Explain core operations.
- inorder 走訪會得到遞增排序（用來驗證 BST 性質）  # Explain inorder property.
- size / height / min / max 等基本性質  # Explain common properties.

We keep the implementation small and explicit so it reads like a study note set.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for simple immutable demo records.  
from typing import List, Optional, Sequence, Tuple  # Type hints for readability and intent.  


@dataclass(frozen=True)  # Make demo snapshots immutable and easy to print.  
class BstSummary:  # Summarize key properties and inorder traversal for printing/tests.  
    size: int  # Number of nodes in the BST.  
    height: int  # Height measured in edges (empty=-1, leaf=0).  
    min_value: Optional[int]  # Minimum key (None if empty).  
    max_value: Optional[int]  # Maximum key (None if empty).  
    inorder: List[int]  # Inorder traversal (sorted keys for a valid BST).  


class Node:  # BST node (key + left child + right child).  
    __slots__ = ("key", "left", "right")  # Keep the node minimal and explicit.  

    def __init__(self, key: int) -> None:  # Initialize node with a key and empty children.  
        self.key = key  # Store node key.  
        self.left: Optional[Node] = None  # Store left child pointer (None means no child).  
        self.right: Optional[Node] = None  # Store right child pointer (None means no child).  


class BinarySearchTree:  # A BST wrapper that holds a root node.  
    def __init__(self, root: Optional[Node] = None) -> None:  # Initialize BST with an optional root.  
        self._root = root  # Store root reference (None means empty tree).  

    @property
    def root(self) -> Optional[Node]:  # Expose root for demos/tests (read-only semantics by convention).  
        return self._root  # Return root reference.  

    def is_empty(self) -> bool:  # Check whether the BST has no nodes.  
        return self._root is None  # Empty iff root is None.  

    @staticmethod
    def from_values(values: Sequence[int]) -> BinarySearchTree:  # Build a BST by inserting values in order.  
        tree = BinarySearchTree(root=None)  # Start from an empty tree.  
        for v in values:  # Insert each value in the given order.  
            tree.insert(int(v))  # Insert one key (duplicates are ignored by policy).  
        return tree  # Return the built BST.  

    def contains(self, key: int) -> bool:  # Return True if key exists in the BST.  
        node = self._root  # Start searching from the root.  
        while node is not None:  # Walk down until we hit a missing child.  
            if key < node.key:  # Go left when key is smaller.  
                node = node.left  # Move to left child.  
            elif key > node.key:  # Go right when key is larger.  
                node = node.right  # Move to right child.  
            else:  # key == node.key means we found it.  
                return True  # Report found.  
        return False  # Report not found when search falls off the tree.  

    def insert(self, key: int) -> bool:  # Insert key into BST; return False when key already exists.  
        if self._root is None:  # Empty tree case: inserted node becomes the root.  
            self._root = Node(key)  # Create root node.  
            return True  # Report inserted.  

        parent: Optional[Node] = None  # Track parent so we can attach the new node.  
        node = self._root  # Start from root.  
        while node is not None:  # Walk down until we find an empty spot.  
            parent = node  # Update parent before moving.  
            if key < node.key:  # Go left when key is smaller.  
                node = node.left  # Move to left child.  
            elif key > node.key:  # Go right when key is larger.  
                node = node.right  # Move to right child.  
            else:  # key == node.key means duplicate key.  
                return False  # Reject duplicates by policy.  

        new_node = Node(key)  # Create the new leaf node.  
        assert parent is not None  # Parent must be non-None because root was non-empty.  
        if key < parent.key:  # Attach to the left side when key is smaller.  
            parent.left = new_node  # Link as left child.  
        else:  # Otherwise attach to the right side.  
            parent.right = new_node  # Link as right child.  
        return True  # Report inserted.  

    def delete(self, key: int) -> bool:  # Delete key from BST; return True when a node was removed.  
        self._root, removed = self._delete_subtree(self._root, key)  # Delete from root and update root pointer.  
        return removed  # Return whether deletion occurred.  

    def _delete_subtree(self, node: Optional[Node], key: int) -> Tuple[Optional[Node], bool]:  # Recursive delete that returns (new_subtree_root, removed).  
        if node is None:  # Key not found in an empty subtree.  
            return None, False  # Nothing removed.  

        if key < node.key:  # Delete from left subtree when key is smaller.  
            node.left, removed = self._delete_subtree(node.left, key)  # Recurse into left child.  
            return node, removed  # Return updated subtree root.  
        if key > node.key:  # Delete from right subtree when key is larger.  
            node.right, removed = self._delete_subtree(node.right, key)  # Recurse into right child.  
            return node, removed  # Return updated subtree root.  

        # Now key == node.key, so we found the node to delete.  # Explain match branch.  
        if node.left is None and node.right is None:  # Case 1: leaf node (0 children).  
            return None, True  # Remove leaf by returning None.  
        if node.left is None:  # Case 2a: only right child exists.  
            return node.right, True  # Replace node by its right child.  
        if node.right is None:  # Case 2b: only left child exists.  
            return node.left, True  # Replace node by its left child.  

        # Case 3: two children => replace with inorder successor (min of right subtree).  # Explain 2-child strategy.  
        successor_key = self._min_key(node.right)  # Find successor key from right subtree.  
        node.key = successor_key  # Copy successor key into current node (keeps tree shape).  
        node.right, _ = self._delete_subtree(node.right, successor_key)  # Delete successor node from right subtree.  
        return node, True  # Report successful deletion.  

    def min(self) -> Optional[int]:  # Return minimum key, or None if tree is empty.  
        if self._root is None:  # Empty tree has no minimum.  
            return None  # Return None for empty.  
        return self._min_key(self._root)  # Return leftmost key.  

    def _min_key(self, node: Node) -> int:  # Return the minimum key in a non-empty subtree.  
        while node.left is not None:  # Keep going left until we cannot.  
            node = node.left  # Move to left child.  
        return node.key  # Leftmost node has the minimum key.  

    def max(self) -> Optional[int]:  # Return maximum key, or None if tree is empty.  
        if self._root is None:  # Empty tree has no maximum.  
            return None  # Return None for empty.  
        node = self._root  # Start at root.  
        while node.right is not None:  # Keep going right until we cannot.  
            node = node.right  # Move to right child.  
        return node.key  # Rightmost node has the maximum key.  

    def size(self) -> int:  # Return number of nodes in the BST (O(n)).  
        def _size(node: Optional[Node]) -> int:  # Recursive helper that counts nodes in a subtree.  
            if node is None:  # Empty subtree contributes 0 nodes.  
                return 0  # Return 0 for empty.  
            return 1 + _size(node.left) + _size(node.right)  # Count root + left + right.  

        return _size(self._root)  # Count nodes starting from root.  

    def height(self) -> int:  # Return height measured in edges (empty=-1, leaf=0).  
        def _height(node: Optional[Node]) -> int:  # Recursive helper that computes height in edges.  
            if node is None:  # Empty subtree has height -1.  
                return -1  # Return -1 for empty.  
            return 1 + max(_height(node.left), _height(node.right))  # Height is 1 + max(child heights).  

        return _height(self._root)  # Compute height from root.  

    def inorder(self) -> List[int]:  # Return inorder traversal (sorted for a valid BST).  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive helper for inorder traversal.  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            _walk(node.left)  # Visit left subtree first.  
            out.append(node.key)  # Visit root.  
            _walk(node.right)  # Visit right subtree.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def summarize(self) -> BstSummary:  # Build a snapshot summary for demos/tests.  
        return BstSummary(  # Construct a summary record.  
            size=self.size(),  # Compute node count.  
            height=self.height(),  # Compute height (edges).  
            min_value=self.min(),  # Compute minimum key (or None).  
            max_value=self.max(),  # Compute maximum key (or None).  
            inorder=self.inorder(),  # Compute inorder traversal.  
        )  # Close summary construction.  


def format_demo() -> str:  # Format a small demo output using a classic BST example.  
    values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic BST insert order used in many textbooks.  
    tree = BinarySearchTree.from_values(values)  # Build BST by inserting in order.  
    s = tree.summarize()  # Summarize properties and inorder traversal.  
    lines: List[str] = []  # Accumulate printable lines.  
    lines.append("=== Binary Search Tree Demo (Python) ===")  # Print header.  
    lines.append(f"insert order: {values}")  # Print insertion order.  
    lines.append(f"size={s.size}, height={s.height}, min={s.min_value}, max={s.max_value}")  # Print key properties.  
    lines.append(f"inorder(sorted)={s.inorder}")  # Print inorder traversal (should be sorted).  
    lines.append(f"contains(6)={tree.contains(6)}, contains(99)={tree.contains(99)}")  # Print search examples.  

    tree.delete(7)  # Delete a leaf (7) to show the simplest delete case.  
    tree.delete(14)  # Delete a one-child node (14 has left child 13) to show 1-child delete.  
    tree.delete(3)  # Delete a two-child node (3) to show successor replacement.  
    lines.append(f"after deletes (7,14,3) inorder={tree.inorder()}")  # Show inorder after deletions.  
    return "\n".join(lines)  # Join lines into one printable string.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    print(format_demo())  # Print demo output.  

