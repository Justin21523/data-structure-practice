# 實作說明（C++）— 樹走訪（Tree Traversal）

## 目標

本單元把 DFS 三種走訪寫出「遞迴」與「迭代」兩種版本，並用測試確認結果一致：

- `preorder`：recursive + iterative（stack）
- `inorder`：recursive + iterative（stack）
- `postorder`：recursive + iterative（stack + visited flag）
- `levelOrder`：BFS（queue + head index）

## 檔案

- `TreeTraversal.hpp`：`BinaryTree` + traversal 實作
- `tree_traversal_demo.cpp`：demo（印出各走訪序列）
- `test_tree_traversal.cpp`：最小測試（不使用 gtest）
- `CMakeLists.txt`：可用 `ctest` 跑測試

## 關鍵做法

### 1) postorder iterative：`(node, visited)` stack

用 `std::vector<std::pair<const Node*, bool>>` 存 frame，確保 node 在左右子樹之後才輸出。

### 2) inorder iterative：一路往左 push

用 `stack` 存 path，走到最左後 pop/visit，再切到 right。

## 如何執行

在 `05-trees-basic/03-tree-traversal/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/tree_traversal_demo
ctest --test-dir build --output-on-failure
```

