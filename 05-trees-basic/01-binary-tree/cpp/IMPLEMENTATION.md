# 實作說明（C++）— 二元樹（Binary Tree）

## 目標

本單元用 C++ 手刻 `BinaryTree`，重點是「遞迴」與「走訪結果可比對」：

- `size()`：節點數
- `height()`：高度**以邊數計**（空樹 `-1`、葉 `0`）
- `countLeaves()`：葉節點數
- `preorder / inorder / postorder / levelOrder`：回傳 `std::vector<int>`
- `fromLevelOrder(std::vector<std::optional<int>>)`：支援 `null` 缺洞

## 檔案

- `BinaryTree.hpp`：`Node` / `BinaryTree` + traversal/summary
- `binary_tree_demo.cpp`：demo（印出 summary 與四種走訪）
- `test_binary_tree.cpp`：最小測試（不使用 gtest）
- `CMakeLists.txt`：可用 `ctest` 跑測試

## 關鍵設計

### 1) `fromLevelOrder` 用索引建樹

索引規則：`left=2*i+1`、`right=2*i+2`。先建 `refs[]` 再 link：

```cpp
size_t leftIndex = 2 * i + 1;
size_t rightIndex = 2 * i + 2;
node->left = leftIndex < refs.size() ? refs[leftIndex] : nullptr;
node->right = rightIndex < refs.size() ? refs[rightIndex] : nullptr;
```

### 2) nodes 的 ownership

`BinaryTree` 用 `std::vector<std::unique_ptr<Node>> nodes_` 擁有所有節點；`left/right/root_` 只存 raw pointer，讓「連結」直觀但不會漏記憶體。

### 3) BFS 用 head index（不 shift）

```cpp
size_t head = 0;
while (head < queue.size()) {
    const Node* node = queue[head];
    head += 1;
}
```

## 如何執行

在 `05-trees-basic/01-binary-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/binary_tree_demo
ctest --test-dir build --output-on-failure
```

