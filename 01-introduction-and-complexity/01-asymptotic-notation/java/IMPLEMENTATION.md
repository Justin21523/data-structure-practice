# 實作說明（Java）— 漸進符號示範（Asymptotic Notation）

## 目標

本單元用 **Java** 示範不同時間複雜度的典型成長率，採用「操作次數（operation count）」取代實際計時，讓結果可重現且更貼近 Big-O 的分析精神。

## 檔案

- `AsymptoticDemo.java`：提供五種操作次數函式（O(1)、O(log n)、O(n)、O(n log n)、O(n²)）並印出表格。
- `AsymptoticDemoTest.java`：不依賴 JUnit，使用 `AssertionError` 實作最小化測試 runner。

## 設計要點

- O(log n) 透過「反覆除以 2」的迴圈計數：

```java
while (current > 1) {
  current /= 2;
  operations += 1;
}
```

- O(n log n) 透過「外層 n 次 + 內層 halving」得到 `n * floor(log2(n))` 的操作次數。
- 所有函式對無效輸入（例如 `n < 0` 或 `n < 1`）會丟 `IllegalArgumentException`，讓錯誤更早暴露。

## 如何執行

在 `01-introduction-and-complexity/01-asymptotic-notation/java/`：

```bash
javac AsymptoticDemo.java AsymptoticDemoTest.java
java AsymptoticDemo
java AsymptoticDemoTest
```

