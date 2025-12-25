# 實作說明（C++）— 二元搜尋樹（Binary Search Tree, BST）

## 目標

本單元用 C++ 手刻 `BinarySearchTree`，重點練習：

- `insert(key)`：插入（本 repo **不允許重複 key**；重複插入回傳 `false`）
- `contains(key)`：搜尋 key 是否存在
- `remove(key)`：刪除（0 子 / 1 子 / 2 子；2 子使用 **inorder successor**）
- `min()` / `max()`：最小/最大值（空樹回傳 `std::nullopt`）
- `size()` / `height()`：節點數與高度（`height` 以邊數計：空樹 `-1`、葉 `0`）
- `inorder()`：回傳 `std::vector<int>`（BST 會得到遞增排序）

## 檔案

- `BinarySearchTree.hpp`：BST 實作（使用 `std::unique_ptr` 管理節點生命週期）
- `bst_demo.cpp`：demo（印出 summary 與 inorder）
- `test_bst.cpp`：最小測試（不使用 gtest）
- `CMakeLists.txt`：可用 `ctest` 跑測試

## 關鍵設計

### 1) ownership：節點用 `std::unique_ptr` 擁有左右子樹

`Node` 內含：

```cpp
std::unique_ptr<Node> left;
std::unique_ptr<Node> right;
```

因此不需要手動 `free/delete`，刪除子樹只要 move/reset 指標即可。

### 2) delete（remove）：用 `std::unique_ptr<Node>&` 直接改 link

核心 helper：

```cpp
static bool removeSubtree(std::unique_ptr<Node>& node, int key);
```

這樣才能在刪 root 或替換 child 時直接修改 parent 的指標。

### 3) 2 子刪除：用 inorder successor（右子樹最小）

```cpp
int successorKey = minKey(node->right.get());
node->key = successorKey;
removeSubtree(node->right, successorKey);
```

## 如何執行

在 `05-trees-basic/02-binary-search-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/bst_demo
ctest --test-dir build --output-on-failure
```

