# 實作說明（Java）— 二元樹（Binary Tree）

## 目標

本單元用最小結構手刻 `BinaryTree`，練習「樹的遞迴」與四種常見走訪：

- `size()`：節點數（遞迴累加）
- `height()`：高度**以邊數計**（空樹 `-1`、葉 `0`）
- `countLeaves()`：葉節點數
- `preorder / inorder / postorder / levelOrder`：回傳「值序列」方便對照答案

## 檔案

- `BinaryTreeDemo.java`：`Node` / `BinaryTree` + demo 輸出
- `BinaryTreeDemoTest.java`：不使用 JUnit 的最小測試 runner

## 關鍵設計

### 1) 用 level-order 陣列建樹（支援 `null` 缺洞）

索引規則：`left = 2*i+1`、`right = 2*i+2`。先建立 `nodes[]`，再把 children link 起來：

```java
int leftIndex = 2 * i + 1;
int rightIndex = 2 * i + 2;
node.left = (leftIndex < nodes.length) ? nodes[leftIndex] : null;
node.right = (rightIndex < nodes.length) ? nodes[rightIndex] : null;
```

### 2) height 以「邊數」定義

讓空樹回傳 `-1`，就能讓「葉節點」自然回到 `0`：

```java
if (node == null) return -1;
return 1 + Math.max(heightOf(node.left), heightOf(node.right));
```

### 3) level-order 走訪不用 shift（O(1) dequeue）

用 `head` 指標代表 queue 的前端位置：

```java
int head = 0;
while (head < queue.size()) {
    Node node = queue.get(head);
    head += 1;
}
```

## 如何執行

在 `05-trees-basic/01-binary-tree/java/`：

```bash
javac BinaryTreeDemo.java BinaryTreeDemoTest.java
java BinaryTreeDemo
java BinaryTreeDemoTest
```

