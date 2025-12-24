# 實作說明（C）— 攤銷分析（Amortized Analysis）

## 目標

用 C 實作「動態陣列（dynamic array）」的倍增擴容，並以 **可計數的搬移次數** 做攤銷分析。

本單元刻意不用 `realloc` 的黑箱行為，而是「malloc 新陣列 + for 迴圈逐一 copy」，讓成本計數完全可重現：

- 寫入新元素：`+1`
- 擴容搬移：copy 1 個元素算 `+1`
- **actual cost** = `1 + copied`

## 三種方法與程式對應

### 聚合法（Aggregate）

容量從 1 開始倍增，最後落在 `final_capacity`（2 的冪）。  
因為每次擴容會 copy 舊容量個元素，所以：

`total_copies = 1 + 2 + 4 + ... + (final_capacity/2) = final_capacity - 1`

因此 `total_actual = m + total_copies <= 3m`（測試會驗證）。

### 記帳法（Accounting）

`table->bank` 表示信用，每次 append 固定收 3：

```c
table->bank += 3 - (long long)actual_cost;
```

測試逐步檢查 `bank_after >= 0`。

### 位能法（Potential）

使用位能函數：

`Φ = 2·size - capacity + 1`

攤銷成本：

`amortized = actual + (Φ_after - Φ_before)`

在 doubling 規則下每步攤銷成本為常數（測試驗證 `amortized_cost == 3`）。

## 檔案

- `amortized_demo.h` / `amortized_demo.c`：核心 table + 計數 + 模擬
- `amortized_demo_main.c`：CLI 印表格
- `test_amortized_demo.c`：最小化測試 runner
- `Makefile`：`make run` / `make test`

## 如何執行

在 `01-introduction-and-complexity/02-amortized-analysis/c/`：

```bash
make
make run
make test
```

