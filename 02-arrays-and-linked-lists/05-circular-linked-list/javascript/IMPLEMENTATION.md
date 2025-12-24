# 實作說明（JavaScript）— 環狀鏈結串列（Circular Linked List）

## 目標

本單元用 Node.js 手刻 `CircularLinkedList`（單向環狀鏈結串列），並回傳可測試的 `hops`：

- `hops`：每做一次 `current = current.next` 就 +1

## 檔案

- `circularLinkedListDemo.js`：資料結構 + CLI 示範（CommonJS）
- `circularLinkedListDemo.test.js`：使用 `node:test` 的測試（無需任何套件）

## 核心概念

### 1) tail-only：`head = tail.next`

```js
head() {
    if (this._tail === null) return null;
    return this._tail.next;
}
```

### 2) 走訪必須用 size 限制

因為沒有 `null` 結尾，`toArray()` / 搜尋一律跑 `for (i < this._size)`，避免無限迴圈。

### 3) 成本公式（hops）

- `pushFront / pushBack / popFront`：`hops = 0`
- `popBack`：找 predecessor → `hops = max(0, n-2)`
- `rotate(k)`：`hops = k % size`，且 `k < 0` 會丟 `RangeError`

## 如何執行

在 `02-arrays-and-linked-lists/05-circular-linked-list/javascript/`：

```bash
node circularLinkedListDemo.js
node --test
```

