# 實作說明（JavaScript）— 靜態陣列（Fixed Capacity Array）

## 目標

JavaScript 的陣列 `Array` 本身是動態的，但本單元要學的是「**固定容量**」的陣列行為：  
我們用 `FixedArray` 自己維護 `capacity/size`，並在插入/刪除時手動 shift，並回傳 **moved elements**（搬移次數）來對應 O(n)。

## 檔案

- `fixedArrayDemo.js`：`FixedArray` + CLI（印出不同位置 insert/remove 的搬移次數）
- `fixedArrayDemo.test.js`：使用 Node 內建 `node:test` 的測試

## 核心概念（搬移次數）

### 插入 `insertAt(i, v)`

需要把 `[i, size-1]` 右移一格：

- `moved = size - i`

```js
for (let i = this._size; i > index; i -= 1) {
    this._data[i] = this._data[i - 1];
}
```

### 刪除 `removeAt(i)`

需要把 `[i+1, size-1]` 左移一格：

- `moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/01-array/javascript/`：

```bash
node fixedArrayDemo.js
node --test
```

