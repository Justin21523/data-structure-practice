# 實作說明（Java）— 二元搜尋樹（Binary Search Tree, BST）

## 目標

本單元用 Java 手刻 `BinarySearchTree`，重點練習 BST 的核心操作：

- `insert(key)`：插入（本 repo **不允許重複 key**；重複插入回傳 `false`）
- `contains(key)`：搜尋 key 是否存在
- `delete(key)`：刪除（0 子 / 1 子 / 2 子；2 子使用 **inorder successor**）
- `min()` / `max()`：最小/最大值（空樹回傳 `null`）
- `size()` / `height()`：節點數與高度（`height` 以邊數計：空樹 `-1`、葉 `0`）
- `inorder()`：回傳「值序列」，用來驗證 BST 排序性質

## 檔案

- `BstDemo.java`：`Node` / `BinarySearchTree` + demo 輸出
- `BstDemoTest.java`：不使用 JUnit 的最小測試 runner

## 關鍵設計

### 1) insert：一路往下找空位

```java
while (node != null) {
    parent = node;
    if (key < node.key) node = node.left;
    else if (key > node.key) node = node.right;
    else return false; // duplicate
}
```

### 2) delete：用 `DeleteResult` 回傳「新的子樹根」

Java 沒有 tuple，所以用小類別回傳：

```java
private static final class DeleteResult {
    public final Node newRoot;
    public final boolean removed;
}
```

### 3) 2 子刪除：用 inorder successor（右子樹最小）

```java
int successorKey = minKey(node.right);
node.key = successorKey;
node.right = deleteSubtree(node.right, successorKey).newRoot;
```

## 如何執行

在 `05-trees-basic/02-binary-search-tree/java/`：

```bash
javac BstDemo.java BstDemoTest.java
java BstDemo
java BstDemoTest
```

