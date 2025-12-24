# 實作說明（Java）— 單向鏈結串列（Singly Linked List）

## 目標

本單元用 `Node(value, next)` 手刻 `SinglyLinkedList`（只維護 `head`、不維護 `tail`），並用 `hops` 量化走訪成本：

- `hops`：執行 `current = current.next` 的次數

這能直接對應到時間複雜度直覺：走訪越多步，就越接近 O(n)。

## 檔案

- `SinglyLinkedListDemo.java`：`SinglyLinkedList` + CLI（印表格）
- `SinglyLinkedListDemoTest.java`：自製最小化測試 runner（無 JUnit）

## 核心概念（hops）

### `pushBack`（沒有 tail 指標）

```java
while (current.next != null) {
    current = current.next;
    hops += 1;
}
```

串列長度為 `n` 時：

- `n == 0` → `hops = 0`
- `n > 0` → `hops = n - 1`

### `getWithCost(i)`

走訪 `i` 次 `next` 才能到 index `i`，因此 `hops = i`。

## 如何執行

在 `02-arrays-and-linked-lists/03-singly-linked-list/java/`：

```bash
javac SinglyLinkedListDemo.java SinglyLinkedListDemoTest.java
java SinglyLinkedListDemo
java SinglyLinkedListDemoTest
```

