# 實作說明（C++）— Red-Black Tree（LLRB 版本）

## 目標

本單元用 C++（不依賴外部套件）手刻 `rbtunit::RedBlackTree`，採用紅黑樹等價變體 **Left-Leaning Red-Black Tree（LLRB）**：

- LLRB 仍滿足紅黑樹性質，但額外要求「紅連結向左」，讓插入/刪除程式結構更一致
- 本 repo **不允許重複 key**：重複插入回傳 `false`
- `height` 以「邊數」定義：空樹 `-1`、葉 `0`（此單元高度用遞迴計算）

## 檔案

- `RedBlackTree.hpp`：資料結構與所有操作（header-only，便於閱讀）
- `rbt_demo.cpp`：demo 輸出
- `test_rbt.cpp`：自製測試（搭配 CTest）
- `CMakeLists.txt`：建置與註冊測試

## 關鍵設計

### 1) 使用 `std::unique_ptr` 管理記憶體

每個節點擁有左右子樹（RAII），避免手動 `new/delete`：

```cpp
std::unique_ptr<Node> left;
std::unique_ptr<Node> right;
```

### 2) 旋轉用 move 轉移擁有權並更新顏色

`rotateLeft/rotateRight` 除了移動指標，也會依 LLRB 規則設定顏色：新根拿舊根的顏色，舊根變紅。

### 3) delete：top-down 下降時先「借紅」，回溯再 `fixUp()`

刪除用 `moveRedLeft/moveRedRight`，避免往下走到 2-node 後無法修復；回溯時再 `fixUp()` 恢復 LLRB 形狀。

為了避免 key 不存在時破壞樹，本單元 `remove(key)` 會先 `contains(key)`，不存在直接回傳 `false`。

### 4) validate()：BST ordering + 紅黑樹不變量（含 LLRB）

`validateSubtree` 檢查：

- root 必須是黑  
- 無連續紅  
- 黑高一致  
- LLRB 規則：不允許右紅  
- BST ordering（嚴格遞增、無重複）

## 如何執行

在 `06-balanced-trees/02-red-black-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/rbt_demo
ctest --test-dir build --output-on-failure
```

