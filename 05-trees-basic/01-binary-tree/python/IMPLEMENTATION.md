# 實作說明（Python）— 二元樹（Binary Tree）

## 檔案

- `binary_tree_demo.py`：`Node` / `BinaryTree` + demo（列印 size/height/leaves 與走訪結果）
- `test_binary_tree_demo.py`：`unittest` 測試（建樹、性質、走訪）

## 核心概念

### 1) level-order 陣列建樹（fromLevelOrder）

用陣列索引關係建立子節點：

- `left = 2*i + 1`
- `right = 2*i + 2`

並允許 `None` 代表缺洞：

```python
left_i = 2 * i + 1
right_i = 2 * i + 2
node.left = nodes[left_i]
node.right = nodes[right_i]
```

### 2) height 的定義（邊數）

本單元 `height()` 定義為「從節點到最深葉節點的邊數」：

- 空樹：`-1`
- 葉節點：`0`

核心遞迴：

```python
if node is None:
    return -1
return 1 + max(height(node.left), height(node.right))
```

### 3) DFS 走訪（pre/in/post order）

以遞迴產生結果 list（只回傳值序列，方便比對考題答案）：

```python
out.append(node.value)        # preorder
walk(node.left); walk(node.right)
```

## 如何執行

在 `05-trees-basic/01-binary-tree/python/`：

```bash
python3 binary_tree_demo.py
python3 -m unittest -v
```

