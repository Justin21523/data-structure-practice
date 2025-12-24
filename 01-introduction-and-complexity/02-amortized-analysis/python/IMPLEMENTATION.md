# 實作說明（Python）— 攤銷分析（Amortized Analysis）

## 目標

用「動態陣列（dynamic array）」的 **擴容（doubling resize）** 來示範攤銷分析：  
單次 `append` 有時很貴（需要搬移很多元素），但一長串 `append` 的平均成本仍是 **O(1) 攤銷時間**。

本單元刻意不用實際計時，而是用可重現的 **操作次數（operation count）**：

- **write**：每次 `append` 寫入新元素算 `1`
- **copy**：擴容時搬移（copy）每個既有元素算 `1`
- **actual cost** = `1 + copies`

## 對應三種方法（與程式怎麼連起來）

### 1) 聚合法（Aggregate）

容量從 1 開始，每次滿了就變成 2 倍：`1, 2, 4, 8, ...`  
每次擴容會搬移「舊容量」個元素，因此總搬移次數是：

`1 + 2 + 4 + ... + (final_capacity/2) = final_capacity - 1`

所以：

`total_actual = m (writes) + (final_capacity - 1) <= 3m`

（因為 `final_capacity < 2m`，所以 `m + (2m-1) < 3m`）

### 2) 記帳法（Accounting）

程式用 `bank` 表示信用：每次 `append` **固定收 3**，再扣掉實際成本：

```python
self._bank += 3 - actual_cost
```

測試會驗證整串操作中 `bank` 永遠不會變成負數。

### 3) 位能法（Potential）

本單元使用：

`Φ = 2·size - capacity + 1`

並用：

`amortized = actual + (Φ_after - Φ_before)`

在這個 doubling 規則下，你會看到每一步的 `amortized_cost` 都是常數（測試驗證為 `3`）。

## 代表性片段（擴容 + 成本）

```python
if self._size == self._capacity:
    copied = self._resize(self._capacity * 2)
    actual_cost += copied
```

## 如何執行

在 `01-introduction-and-complexity/02-amortized-analysis/python/`：

```bash
python3 amortized_demo.py
python3 -m unittest -v
```

