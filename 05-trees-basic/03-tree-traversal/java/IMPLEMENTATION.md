# 實作說明（Java）— 樹走訪（Tree Traversal）

## 目標

本單元重點是「同一種走訪順序」可以用兩種方式實作：

- **遞迴（recursive）**：最直觀
- **迭代（iterative）**：用 stack/queue 模擬（面試/考試常考）

我們實作並驗證：

- `preorder`：recursive + iterative（stack）
- `inorder`：recursive + iterative（stack）
- `postorder`：recursive + iterative（stack + visited flag）
- `levelOrder`：BFS（queue + head index）

## 檔案

- `TreeTraversalDemo.java`：`Node` / `BinaryTree` + demo
- `TreeTraversalDemoTest.java`：不使用 JUnit 的最小測試 runner

## 關鍵做法

### 1) preorder iterative：先 push 右再 push 左

```java
stack.add(root);
while (!stack.isEmpty()) {
    Node node = stack.remove(stack.size() - 1);
    out.add(node.value);
    if (node.right != null) stack.add(node.right);
    if (node.left != null) stack.add(node.left);
}
```

### 2) inorder iterative：一路往左 push，回來再往右

```java
while (node != null || !stack.isEmpty()) {
    while (node != null) { stack.add(node); node = node.left; }
    node = stack.remove(stack.size() - 1);
    out.add(node.value);
    node = node.right;
}
```

### 3) postorder iterative：用 `(node, visited)` 的 stack

```java
stack.add(new Frame(root, false));
while (!stack.isEmpty()) {
    Frame f = stack.remove(stack.size() - 1);
    if (f.visited) out.add(f.node.value);
    else {
        stack.add(new Frame(f.node, true));
        if (f.node.right != null) stack.add(new Frame(f.node.right, false));
        if (f.node.left != null) stack.add(new Frame(f.node.left, false));
    }
}
```

## 如何執行

在 `05-trees-basic/03-tree-traversal/java/`：

```bash
javac TreeTraversalDemo.java TreeTraversalDemoTest.java
java TreeTraversalDemo
java TreeTraversalDemoTest
```

