# 實作說明（C++）— 攤銷分析（Amortized Analysis）

## 目標

用 C++ 實作倍增擴容的動態陣列，並用「寫入 + 搬移」計數，把攤銷分析落到可執行的程式碼與測試。

## 成本模型

- 寫入新元素：`+1`
- 若擴容：搬移（copy）既有 `size` 個元素
- **actual cost** = `1 + copied`

程式用手動 copy（`for` 迴圈）而不是用 STL 容器的黑箱行為，確保成本完全可預測。

## 三種方法（與程式碼的對應點）

### 聚合法（Aggregate）

最終容量 `finalCapacity` 會是 2 的冪，且：

`totalCopies = finalCapacity - 1`

因此 `totalActualCost = m + totalCopies <= 3m`。

### 記帳法（Accounting）

每次 `append` 固定收 3，存在 `bank_`：

```cpp
bank_ += 3 - static_cast<long long>(actualCost);
```

測試會驗證 `bankAfter >= 0`。

### 位能法（Potential）

位能函數：

`Φ = 2·size - capacity + 1`

攤銷成本：

`amortized = actual + (Φ_after - Φ_before)`

在 doubling 規則下每步攤銷成本固定為常數（測試驗證 `amortizedCost == 3`）。

## 如何執行

在 `01-introduction-and-complexity/02-amortized-analysis/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/amortized_demo
ctest --test-dir build
```

