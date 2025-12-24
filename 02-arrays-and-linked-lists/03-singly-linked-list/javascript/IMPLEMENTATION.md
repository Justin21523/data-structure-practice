# 實作說明（JavaScript）— 單向鏈結串列（Singly Linked List）

## 目標

本單元用 `Node` + `SinglyLinkedList` 自己做一個單向鏈結串列（只存 `head`，不存 `tail`），並回傳可測試的走訪成本：

- `hops`：`current = current.next` 的次數

## 檔案

- `singlyLinkedListDemo.js`：實作 + CLI（印出 hops 表格）
- `singlyLinkedListDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### `pushBack` 為何是 O(n)

沒有 tail 時，必須一路走到最後：

```js
while (current.next !== null) {
    current = current.next;
    hops += 1;
}
```

### `getWithCost(i)` 為何是 O(n)

要到 index `i` 必須從 head 走 `i` 步，所以 `hops = i`。

## 如何執行

在 `02-arrays-and-linked-lists/03-singly-linked-list/javascript/`：

```bash
node singlyLinkedListDemo.js
node --test
```

