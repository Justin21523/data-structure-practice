# 實作說明（C++）— AVL Tree（自平衡二元搜尋樹）

## 目標

本單元用 C++（不依賴外部套件）手刻 `avlunit::AvlTree`：

- 節點快取 `height`（本 repo 以**邊數**定義：空樹 `-1`、葉 `0`）
- 插入/刪除回溯時：`updateHeight()` + `rebalance()`（LL/RR/LR/RL 旋轉）
- 提供 `validate()`：檢查 BST ordering、height 正確、以及 AVL 平衡條件

## 檔案

- `AvlTree.hpp`：資料結構與所有操作（header-only，便於閱讀）
- `avl_demo.cpp`：demo 輸出
- `test_avl.cpp`：自製測試（搭配 CTest）
- `CMakeLists.txt`：建置與註冊測試

## 關鍵設計

### 1) 使用 `std::unique_ptr` 管理記憶體

每個節點擁有左右子樹（RAII），避免手動 `new/delete`：

```cpp
std::unique_ptr<Node> left;
std::unique_ptr<Node> right;
```

### 2) 旋轉用 move 轉移擁有權

以右旋為例：把 `y->left` move 出來成為新的根 `x`，再把 `x->right` 接回 `y->left`。

### 3) insert/remove 走遞迴回傳新的子樹根

`insertSubtree` / `removeSubtree` 都回傳 `std::unique_ptr<Node>`（新的子樹根），因此上層只要接回 `node->left/node->right`，再做：

1) `updateHeight(node.get())`  
2) `rebalance(std::move(node))`

### 4) validate()：把不變量寫成可測的函式

`validateSubtree(node, low, high, outHeight)` 同時檢查：

- BST ordering（嚴格遞增、無重複）
- `node->height` 等於由子樹計算出的高度
- `abs(height(left) - height(right)) <= 1`

## 如何執行

在 `06-balanced-trees/01-avl-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/avl_demo
ctest --test-dir build --output-on-failure
```

