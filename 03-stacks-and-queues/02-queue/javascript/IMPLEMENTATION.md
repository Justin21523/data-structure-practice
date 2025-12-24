# 實作說明（JavaScript）— 佇列（Queue）

## 目標

本單元用 Node.js 手刻 `ArrayQueue`（naive queue），把 `dequeue` 的 shift 成本量化：

- `copied`：擴容時複製元素次數
- `moved`：`dequeue` 後左移元素次數（通常 `size-1`）

## 檔案

- `queueDemo.js`：`ArrayQueue` + CLI（enqueue 成長表 + dequeue moved 表 + FIFO 小例子）
- `queueDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### dequeue 的 moved（左移）

```js
const moved = this._size - 1;
for (let i = 1; i < this._size; i += 1) {
    this._data[i - 1] = this._data[i];
}
```

> 下一單元 `03-circular-queue` 會用環狀索引讓 `moved = 0`。

## 如何執行

在 `03-stacks-and-queues/02-queue/javascript/`：

```bash
node queueDemo.js
node --test
```

