# 實作說明（JavaScript）— Red-Black Tree（LLRB 版本）

## 目標

本單元用 Node.js（不依賴 npm 套件）實作紅黑樹的等價變體 **Left-Leaning Red-Black Tree（LLRB）**：

- LLRB 仍滿足紅黑樹性質，但額外要求「紅連結向左」，讓插入/刪除的程式結構更一致
- 本 repo **不允許重複 key**：重複插入回傳 `false`
- `height` 以「邊數」定義：空樹 `-1`、葉 `0`（此單元高度用遞迴計算）

## 檔案

- `redBlackTreeDemo.js`：資料結構 + demo 輸出（CommonJS module）
- `redBlackTreeDemo.test.js`：使用 `node --test` 的測試（離線可跑）

## 關鍵設計

### 1) `isRed(null) === false`

空指標視為黑色，讓判斷更乾淨：

```js
function isRed(node) { return node !== null && node.red === true; }
```

### 2) insert：BST 插入 + `fixUp()` 修正 LLRB 形狀

`fixUp()` 依序做：

1) 右紅就左旋（rotateLeft）  
2) 連續左紅就右旋（rotateRight）  
3) 左右都紅就 flipColors（分裂 4-node）

### 3) delete：top-down 下降時先「借紅」，回溯再 `fixUp()`

刪除用 `moveRedLeft/moveRedRight`，先把紅色往要走的方向「借」過去，避免下去後無法修復。

為了避免 key 不存在時破壞樹，本單元 `delete(key)` 會先 `contains(key)`，不存在直接回傳 `false`。

### 4) validate()：BST ordering + 紅黑樹不變量（含 LLRB）

`validate()` 檢查：

- root 必須是黑  
- 無連續紅（red node 的 children 不能是 red）  
- 黑高一致（所有 root->leaf 路徑黑節點數相同）  
- LLRB 規則：不允許右紅（red right link）  
- BST ordering（嚴格遞增、無重複）

## 如何執行

在 `06-balanced-trees/02-red-black-tree/javascript/`：

```bash
node redBlackTreeDemo.js
node --test
```

