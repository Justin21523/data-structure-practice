# 實作說明（JavaScript）— 環狀佇列（Circular Queue）

## 目標

本單元用 Node.js 手刻 `CircularQueue`（固定容量 buffer + head + size），把 `dequeue` 的 shift 成本消除：

- `moved`：永遠為 0
- `copied`：只在擴容時出現

## 檔案

- `circularQueueDemo.js`：資料結構 + CLI（enqueue 成長表 + dequeue moved 表 + FIFO 小例子）
- `circularQueueDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### tailIndex 與 wrap-around

```js
const tailIndex = (this._head + this._size) % this._capacity;
```

### dequeue 不左移（moved=0）

只移動 head：

```js
this._head = (this._head + 1) % this._capacity;
this._size -= 1;
```

## 如何執行

在 `03-stacks-and-queues/03-circular-queue/javascript/`：

```bash
node circularQueueDemo.js
node --test
```

