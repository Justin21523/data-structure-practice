# 實作說明（Java）— AVL Tree（自平衡二元搜尋樹）

## 目標

本單元用 Java 手刻 `AvlTree`，練習「BST + 平衡維護」：

- 每個節點維護 `height`（本 repo 以**邊數**定義：空樹 `-1`、葉 `0`）
- 透過 `balanceFactor = height(left) - height(right)` 判斷是否需要旋轉
- 支援 `insert/contains/delete`，並提供 `validate()` 在測試中快速檢查不變量

## 檔案

- `AvlTreeDemo.java`：資料結構 + demo 輸出
- `AvlTreeDemoTest.java`：不依賴 JUnit 的自製測試（離線可跑）

## 關鍵設計

### 1) updateHeight：插入/刪除回溯時更新快取高度

```java
node.height = 1 + Math.max(h(node.left), h(node.right));
```

其中 `h(null) = -1`，確保葉節點高度會變成 `0`。

### 2) rebalance：用 LL/RR/LR/RL 四種情況做旋轉

- `balance > 1`：左重（LL 或 LR）
- `balance < -1`：右重（RR 或 RL）

LR/RL 需要「先旋子樹、再旋自己」：

```java
node.left = rotateLeft(node.left);
return rotateRight(node);
```

### 3) insert/delete 都用遞迴回傳「新的子樹根」

為了避免漏接 parent 指標，`insertSubtree` / `deleteSubtree` 都回傳 `(newRoot, inserted/removed)`，回到上一層再做：

1) 接回 `node.left/node.right`  
2) `updateHeight(node)`  
3) `rebalance(node)`

### 4) validate()：BST ordering + AVL 平衡 + height 正確性

`validateSubtree(node, low, high)` 用開區間界線檢查 BST ordering（嚴格遞增、無重複），並同時驗證：

- `node.height` 等於由子樹計算出的高度
- `abs(height(left) - height(right)) <= 1`

## 如何執行

在 `06-balanced-trees/01-avl-tree/java/`：

```bash
javac AvlTreeDemo.java AvlTreeDemoTest.java
java AvlTreeDemo
java AvlTreeDemoTest
```

