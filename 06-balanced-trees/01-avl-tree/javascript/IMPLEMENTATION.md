# 實作說明（JavaScript）— AVL Tree（自平衡二元搜尋樹）

## 目標

本單元用 Node.js（不依賴 npm 套件）實作 `AvlTree`：

- 節點快取 `height`（本 repo 以**邊數**定義：空樹 `-1`、葉 `0`）
- 插入/刪除回溯時用 `rebalance()` 做 LL/RR/LR/RL 旋轉
- 提供 `validate()` 讓測試能直接檢查「仍然是 AVL」

## 檔案

- `avlTreeDemo.js`：資料結構 + demo 輸出（CommonJS module）
- `avlTreeDemo.test.js`：使用 `node --test` 的測試（離線可跑）

## 關鍵設計

### 1) 高度定義與 balance factor

空子樹高度定義為 `-1`，因此葉節點高度為 `0`：

```js
function h(node) { return node === null ? -1 : node.height; }
function balanceFactor(node) { return h(node.left) - h(node.right); }
```

### 2) 旋轉（rotation）+ updateHeight

旋轉後要重新計算 height（先更新被下移的節點，再更新新的根）：

```js
updateHeight(y);
updateHeight(x);
```

### 3) insert/delete：遞迴回傳新的子樹根

為了避免漏接 parent 指標，`insertSubtree` / `deleteSubtree` 都回傳：

- `insertSubtree` => `{ node: newRoot, inserted: boolean }`
- `deleteSubtree` => `{ node: newRoot, removed: boolean }`

回到上一層再做 `updateHeight(node)` + `rebalance(node)`。

### 4) validate()：把不變量寫成可測的函式

`validateSubtree(node, low, high)` 同時檢查：

- BST ordering（嚴格遞增、無重複）
- `node.height` 是否正確
- `abs(height(left) - height(right)) <= 1`

## 如何執行

在 `06-balanced-trees/01-avl-tree/javascript/`：

```bash
node avlTreeDemo.js
node --test
```

