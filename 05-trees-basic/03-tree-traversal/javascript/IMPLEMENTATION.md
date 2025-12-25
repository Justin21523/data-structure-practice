# 實作說明（JavaScript）— 樹走訪（Tree Traversal）

## 目標

本單元重點是把 DFS 三種走訪寫出「遞迴」與「迭代」兩種版本，並用測試確認結果一致：

- `preorder`：recursive + iterative（stack）
- `inorder`：recursive + iterative（stack）
- `postorder`：recursive + iterative（stack + visited flag）
- `levelOrder`：BFS（queue + head index）

## 檔案

- `treeTraversalDemo.js`：`Node` / `BinaryTree` + demo
- `treeTraversalDemo.test.js`：使用 `node --test` 的單元測試

## 關鍵做法

### 1) preorder iterative：先 push 右再 push 左

```js
const stack = [root];
while (stack.length > 0) {
    const node = stack.pop();
    out.push(node.value);
    if (node.right) stack.push(node.right);
    if (node.left) stack.push(node.left);
}
```

### 2) inorder iterative：一路往左 push，回來再往右

```js
while (node !== null || stack.length > 0) {
    while (node !== null) { stack.push(node); node = node.left; }
    node = stack.pop();
    out.push(node.value);
    node = node.right;
}
```

### 3) postorder iterative：用 `[node, visited]` 的 stack

```js
const stack = [[root, false]];
while (stack.length > 0) {
    const [node, visited] = stack.pop();
    if (visited) out.push(node.value);
    else {
        stack.push([node, true]);
        if (node.right) stack.push([node.right, false]);
        if (node.left) stack.push([node.left, false]);
    }
}
```

## 如何執行

在 `05-trees-basic/03-tree-traversal/javascript/`：

```bash
node treeTraversalDemo.js
node --test
```

