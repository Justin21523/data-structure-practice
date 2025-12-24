# 實作說明（JavaScript）— 雙向鏈結串列（Doubly Linked List）

## 目標

本單元用 `Node` + `DoublyLinkedList` 自己做一個「head+tail 的雙向鏈結串列」，並回傳可測試的走訪成本：

- `hops`：每次沿著 `next` 或 `prev` 前進都算 1 hop

## 檔案

- `doublyLinkedListDemo.js`：實作 + CLI（印出 hops 表格）
- `doublyLinkedListDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### 兩端操作（push/pop）為 O(1)

因為我們維護 `tail`，所以尾端操作不必走訪（`hops=0`）。

### get/insert/remove 的 hops（從較近端開始）

```js
if (index < Math.floor(this._size / 2)) {
    for (let i = 0; i < index; i += 1) {
        current = current.next;
        hops += 1;
    }
} else {
    const steps = this._size - 1 - index;
    for (let i = 0; i < steps; i += 1) {
        current = current.prev;
        hops += 1;
    }
}
```

因此 `hops = min(i, size-1-i)`。

## 如何執行

在 `02-arrays-and-linked-lists/04-doubly-linked-list/javascript/`：

```bash
node doublyLinkedListDemo.js
node --test
```

