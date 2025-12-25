# 實作說明（JavaScript）— 二元樹（Binary Tree）

## 目標

本單元用 JavaScript 手刻 `BinaryTree`，重點練習：

- 遞迴計算：`size()` / `height()` / `countLeaves()`
- DFS 走訪：`preorder / inorder / postorder`
- BFS 走訪：`levelOrder()`（用 `head` 指標避免 `shift()`）
- `fromLevelOrder([...])`：用含 `null` 的 level-order 陣列快速建樹

## 檔案

- `binaryTreeDemo.js`：`Node` / `BinaryTree` + demo 輸出
- `binaryTreeDemo.test.js`：使用 `node --test` 的單元測試

## 關鍵設計

### 1) level-order 建樹（支援 `null` 缺洞）

索引規則：`left = 2*i+1`、`right = 2*i+2`：

```js
const leftIndex = 2 * i + 1;
const rightIndex = 2 * i + 2;
node.left = leftIndex < nodes.length ? nodes[leftIndex] : null;
node.right = rightIndex < nodes.length ? nodes[rightIndex] : null;
```

### 2) height 以「邊數」定義

空樹回傳 `-1`，葉節點自然是 `0`：

```js
if (node === null) return -1;
return 1 + Math.max(heightOf(node.left), heightOf(node.right));
```

### 3) BFS 不用 `shift()`（O(1) dequeue）

```js
let head = 0;
while (head < queue.length) {
    const node = queue[head];
    head += 1;
}
```

## 如何執行

在 `05-trees-basic/01-binary-tree/javascript/`：

```bash
node binaryTreeDemo.js
node --test
```

