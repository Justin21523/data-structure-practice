# 實作說明（Java）— 雙向鏈結串列（Doubly Linked List）

## 目標

本單元用 `Node(value, prev, next)` 手刻 `DoublyLinkedList`，並用 `hops` 量化走訪成本：

- `hops`：執行 `current = current.next` 或 `current = current.prev` 的次數

透過同時維護 `head` 與 `tail`，你會看到：

- 兩端操作（push/pop）皆是 O(1)（`hops=0`）
- `get/insertAt/removeAt` 仍需走訪，但可從較近端開始 → `hops = min(i, size-1-i)`

## 檔案

- `DoublyLinkedListDemo.java`：`DoublyLinkedList` + CLI（印出表格）
- `DoublyLinkedListDemoTest.java`：自製最小化測試 runner（無 JUnit）

## 核心概念（nearest-end traversal）

定位 index `i` 時，選擇 head/tail 擇一較近端：

```java
if (index < this.size / 2) {
    for (int i = 0; i < index; i++) {
        current = current.next;
        hops += 1;
    }
} else {
    int steps = this.size - 1 - index;
    for (int i = 0; i < steps; i++) {
        current = current.prev;
        hops += 1;
    }
}
```

因此 `hops = min(i, size-1-i)`。

## 如何執行

在 `02-arrays-and-linked-lists/04-doubly-linked-list/java/`：

```bash
javac DoublyLinkedListDemo.java DoublyLinkedListDemoTest.java
java DoublyLinkedListDemo
java DoublyLinkedListDemoTest
```

