# 實作說明（JavaScript）— 雙端佇列（Deque）

## 目標

本單元用 Node.js 手刻 `Deque`（環狀陣列 + head + size），讓兩端操作不需要搬移元素：

- `moved`：永遠為 `0`
- `copied`：只有 resize（doubling）才會出現，且一次 resize 會複製 `size` 個元素

## 檔案

- `dequeDemo.js`：資料結構 + CLI（pushBack 成長表 + 雙端操作小例子）
- `dequeDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### 1) indexAt：邏輯 offset → 實體索引

```js
_indexAt(offset) {
    return (this._head + offset) % this._capacity;
}
```

### 2) pushFront / pushBack 都是 O(1)（moved=0）

pushBack：

```js
const tailIndex = this._indexAt(this._size);
this._data[tailIndex] = value;
this._size += 1;
```

pushFront（head 往左繞回去）：

```js
this._head = (this._head - 1 + this._capacity) % this._capacity;
this._data[this._head] = value;
this._size += 1;
```

### 3) resize：依 deque 順序複製並 head=0

```js
for (let i = 0; i < this._size; i += 1) {
    newData[i] = this._data[this._indexAt(i)];
}
this._head = 0;
```

## 如何執行

在 `03-stacks-and-queues/04-deque/javascript/`：

```bash
node dequeDemo.js
node --test
```

