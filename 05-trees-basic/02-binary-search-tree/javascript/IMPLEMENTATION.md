# 實作說明（JavaScript）— 二元搜尋樹（Binary Search Tree, BST）

## 目標

本單元用 JavaScript 手刻 `BinarySearchTree`，重點練習：

- `insert(key)`：插入（本 repo **不允許重複 key**；重複插入回傳 `false`）
- `contains(key)`：搜尋 key 是否存在
- `delete(key)`：刪除（0 子 / 1 子 / 2 子；2 子使用 **inorder successor**）
- `min()` / `max()`：最小/最大值（空樹回傳 `null`）
- `size()` / `height()`：節點數與高度（`height` 以邊數計：空樹 `-1`、葉 `0`）
- `inorder()`：回傳「值序列」，用來驗證 BST 排序性質

## 檔案

- `bstDemo.js`：`Node` / `BinarySearchTree` + demo 輸出
- `bstDemo.test.js`：使用 `node --test` 的單元測試

## 關鍵設計

### 1) insert：一路往下找空位

```js
while (node !== null) {
    parent = node;
    if (key < node.key) node = node.left;
    else if (key > node.key) node = node.right;
    else return false;
}
```

### 2) delete：遞迴回傳 `{ node, removed }`

JavaScript 沒有 tuple，但可以回傳 object：

```js
const r = this._deleteSubtree(node.left, key);
node.left = r.node;
return { node, removed: r.removed };
```

### 3) 2 子刪除：用 inorder successor（右子樹最小）

```js
const successorKey = this._minKey(node.right);
node.key = successorKey;
node.right = this._deleteSubtree(node.right, successorKey).node;
```

## 如何執行

在 `05-trees-basic/02-binary-search-tree/javascript/`：

```bash
node bstDemo.js
node --test
```

