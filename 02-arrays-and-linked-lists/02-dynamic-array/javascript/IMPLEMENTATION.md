# 實作說明（JavaScript）— 動態陣列（Dynamic Array）

## 目標

JavaScript 的一般陣列（`Array`）本質上已經是可變長度容器；本單元則是用「教學版」`DynamicArray` 來明確呈現成本來源：

- `copied`：擴容時複製既有元素次數
- `moved`：插入/刪除時 shift 的搬移次數

容量策略採 **倍增（doubling）**：當 `size === capacity` 時改成 `capacity * 2`，使 `append` 呈現攤銷（amortized）O(1)。

## 檔案

- `dynamicArrayDemo.js`：`DynamicArray` + CLI（印表格）
- `dynamicArrayDemo.test.js`：Node 內建 `node:test` 測試

## 核心概念

### 擴容與 copied

擴容會配置新 buffer，並把 `0..size-1` 全部複製：

```js
for (let i = 0; i < this._size; i += 1) {
    newData[i] = this._data[i];
    copied += 1;
}
```

### insertAt / removeAt 與 moved

- `insertAt(i, v)`：右移 `[i, size-1]`，`moved = size - i`
- `removeAt(i)`：左移 `[i+1, size-1]`，`moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/02-dynamic-array/javascript/`：

```bash
node dynamicArrayDemo.js
node --test
```

