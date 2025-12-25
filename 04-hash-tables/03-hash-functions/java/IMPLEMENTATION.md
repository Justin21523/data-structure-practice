# 實作說明（Java）— 雜湊函數（Hash Functions）

## 檔案

- `HashFunctions.java`：整數/字串雜湊函數 + 分布分析工具（`analyzeDistribution`），並內建 `main()` demo。
- `UniversalHashing.java`：通用雜湊（universal hashing）函數族 + 以通用雜湊驅動的 chaining hash table。
- `HashFunctionsTest.java`：不依賴 JUnit 的小型測試執行器（避免受限環境下載套件）。

## 核心概念

### 1) 整數雜湊（Division / Multiplication）

- 除法法：`h(k) = k mod m`（用 `Math.floorMod` 讓結果永遠在 `[0, m)`）。
- 乘法法：`h(k) = floor(m * frac(k*A))`，`A` 預設用黃金比例倒數 `((sqrt(5)-1)/2)`。

### 2) 字串雜湊（DJB2 / FNV-1a / Jenkins）

這些函數都輸出「**32-bit unsigned**」概念；Java 沒有 unsigned int，所以用 `long` 存放並用 `& 0xFFFFFFFFL` 保留低 32 位元。

### 3) 分布分析（Distribution）

`analyzeDistribution` 會把 keys 打到 `m` 個桶後，計算：

- `nonEmptyBuckets`：有元素的桶數
- `stdDeviation`：桶大小標準差（越小通常代表越均勻）

### 4) 通用雜湊（Universal Hashing）

`UniversalHashFamily` 使用：

```
h_{a,b}(k) = ((a*k + b) mod p) mod m
```

並提供 `seed` 建構子，讓測試可重現（每次 trial 用不同 seed 模擬「隨機挑函數」）。

## 如何執行

在 `04-hash-tables/03-hash-functions/java/`：

```bash
# 跑測試
javac HashFunctions.java UniversalHashing.java HashFunctionsTest.java
java HashFunctionsTest

# 跑 demo
java HashFunctions
```

