# 實作說明（JavaScript）— 堆疊（Stack）

## 目標

JavaScript 內建陣列已經能 `push/pop`，但本單元刻意用「教學版」`ArrayStack` 自己管理 `size/capacity`，讓你能看到擴容成本：

- `copied`：擴容時複製既有元素次數

## 檔案

- `stackDemo.js`：`ArrayStack` + CLI（印出 push 成長表格與 LIFO 小例子）
- `stackDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### 倍增擴容（doubling）

當 `size === capacity` 時，把容量改成 `capacity * 2`，並複製 `0..size-1`：

```js
for (let i = 0; i < this._size; i += 1) {
    newData[i] = this._data[i];
    copied += 1;
}
```

因此 `push` 平均（攤銷）O(1)。

## 如何執行

在 `03-stacks-and-queues/01-stack/javascript/`：

```bash
node stackDemo.js
node --test
```

