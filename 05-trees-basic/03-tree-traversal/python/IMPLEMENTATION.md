# 實作說明（Python）— 樹走訪（Tree Traversal）

## 目標

本單元重點是「同一種走訪順序」可以用兩種思維寫出來：

- **遞迴（recursive）**：最直觀
- **迭代（iterative）**：用 `stack/queue` 模擬遞迴（面試/考試常考）

我們實作並驗證：

- `preorder`（root→left→right）：recursive + iterative
- `inorder`（left→root→right）：recursive + iterative
- `postorder`（left→right→root）：recursive + iterative
- `level_order`（BFS）：用 queue（本 repo 用 `head` index 避免 `pop(0)`）

## 檔案

- `tree_traversal_demo.py`：`Node` / `BinaryTree` + 六種走訪 + demo
- `test_tree_traversal_demo.py`：使用 `unittest` 的測試

## 關鍵做法

### 1) preorder iterative：stack（先 push 右再 push 左）

```python
stack = [root]
while stack:
    node = stack.pop()
    out.append(node.value)
    if node.right: stack.append(node.right)
    if node.left: stack.append(node.left)
```

### 2) inorder iterative：一路往左 push，回來再往右

```python
while node is not None or stack:
    while node is not None:
        stack.append(node)
        node = node.left
    node = stack.pop()
    out.append(node.value)
    node = node.right
```

### 3) postorder iterative：用 `(node, visited)` 的 stack

用「visited 標記」確保 node 在左右子樹之後才輸出：

```python
stack = [(root, False)]
while stack:
    node, visited = stack.pop()
    if visited:
        out.append(node.value)
    else:
        stack.append((node, True))
        if node.right: stack.append((node.right, False))
        if node.left: stack.append((node.left, False))
```

## 如何執行

在 `05-trees-basic/03-tree-traversal/python/`：

```bash
python3 tree_traversal_demo.py
python3 -m unittest -v
```

