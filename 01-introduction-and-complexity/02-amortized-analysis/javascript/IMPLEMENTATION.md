# 實作說明（JavaScript）— 攤銷分析（Amortized Analysis）

## 目標

用 Node.js（純 JS、無外部套件）示範攤銷分析：  
自己做一個「固定容量陣列 + 滿了就倍增」的動態陣列，並用可計數的成本（writes/copies）證明 `append` 的攤銷成本為常數。

## 成本模型（deterministic）

- 寫入新元素：`+1`
- 擴容時搬移元素：每搬 1 個算 `+1`
- **actual cost** = `1 + copied`

## 三種方法（程式對應）

### 聚合法（Aggregate）

容量最終會落在某個 `finalCapacity`（2 的冪），總搬移次數必為：

`totalCopies = finalCapacity - 1`

因此總成本 `totalActualCost = m + totalCopies <= 3m`。

### 記帳法（Accounting）

每次 `append` 固定收 3，放到 `bank`：

```js
this._bank += 3 - actualCost;
```

測試會保證整段序列中 `bankAfter >= 0`。

### 位能法（Potential）

使用：

`Φ = 2·size - capacity + 1`

並以：

`amortized = actual + (Φ_after - Φ_before)`

讓每次 `append` 的攤銷成本成為常數（測試驗證 `amortizedCost === 3`）。

## 代表性片段（擴容）

```js
if (this._size === this._capacity) {
  copied = this._resize(this._capacity * 2);
  actualCost += copied;
}
```

## 如何執行

在 `01-introduction-and-complexity/02-amortized-analysis/javascript/`：

```bash
node amortizedDemo.js
node --test
```

