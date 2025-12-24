# 實作說明（Java）— 環狀鏈結串列（Circular Linked List）

## 目標

本單元手刻 `CircularLinkedList`（單向環狀鏈結串列），並以 `hops` 量化走訪成本：

- `hops`：每做一次 `current = current.next` 就 +1

## 檔案

- `CircularLinkedListDemo.java`：資料結構 + CLI 示範
- `CircularLinkedListDemoTest.java`：不使用 JUnit 的最小測試器（直接 `main` 跑）

## 核心概念

### 1) tail-only 表示法

只存 `tail`，`head` 用 `tail.next` 表示：

```java
private Node head() {
    if (this.tail == null) return null;
    return this.tail.next;
}
```

### 2) 走訪要 bounded（用 size）

因為環狀結構沒有終點，`toArray()` / `indexOfWithCost()` 都用 `for (i < size)` 限制次數。

### 3) 成本公式（hops）

- `pushFront / pushBack / popFront`：`hops = 0`
- `popBack`：找 predecessor → `hops = max(0, n-2)`
- `rotate(k)`：`hops = k % size`，且 `k < 0` 會丟 `IllegalArgumentException`

## 如何執行

在 `02-arrays-and-linked-lists/05-circular-linked-list/java/`：

```bash
javac CircularLinkedListDemo.java CircularLinkedListDemoTest.java
java CircularLinkedListDemo
java CircularLinkedListDemoTest
```

