# 實作說明（Java）— 攤銷分析（Amortized Analysis）

## 目標

用 Java 實作一個「會擴容的動態陣列（dynamic array）」並計算成本，讓攤銷分析變成可執行、可驗證的結果。

本單元的成本模型是 **操作次數（operation count）**：

- 每次 `append` 寫入新元素：`+1`
- 若擴容（容量滿了）：搬移既有元素 `copied = size` 次
- **actual cost** = `1 + copied`

## 三種方法如何在程式中呈現

### 聚合法（Aggregate）

容量從 1 開始倍增：`1, 2, 4, ...`  
總搬移次數恆等於：

`totalCopies = finalCapacity - 1`

因此 `totalActualCost = m + totalCopies <= 3m`（測試會驗證這個界線）。

### 記帳法（Accounting）

每次操作固定收 3，信用存在 `bank`：

```java
this.bank += 3L - (long) actualCost;
```

測試會逐步檢查 `bankAfter >= 0`。

### 位能法（Potential）

使用位能函數：

`Φ = 2·size - capacity + 1`

並計算：

`amortized = actual + (Φ_after - Φ_before)`

在 doubling 規則下每步攤銷成本會是常數（測試驗證 `amortizedCost == 3`）。

## 代表性片段（擴容）

```java
if (this.size == this.capacity) {
    copied = resize(this.capacity * 2);
    actualCost += copied;
}
```

## 如何執行

在 `01-introduction-and-complexity/02-amortized-analysis/java/`：

```bash
javac AmortizedDemo.java AmortizedDemoTest.java
java AmortizedDemo
java AmortizedDemoTest
```

