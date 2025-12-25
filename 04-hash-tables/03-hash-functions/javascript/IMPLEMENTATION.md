# 實作說明（JavaScript）— 雜湊函數（Hash Functions）

## 檔案

- `hashFunctions.js`：整數/字串雜湊函數 + `analyzeDistribution`。
- `universalHashing.js`：通用雜湊（universal hashing）函數族 + `UniversalHashTable`（chaining）。
- `hashFunctionsDemo.js`：示範程式（印出幾個 hash 值與分布摘要）。
- `hashFunctions.test.js`：Node 內建 `node:test` 測試（無外部依賴）。

## 核心概念

### 1) 32-bit unsigned 的處理

JavaScript 的位元運算會落在 32-bit 整數空間；本單元把 DJB2 / FNV-1a / Jenkins 的結果都維持在 uint32：

- `>>> 0`：把值轉成 unsigned 32-bit
- `Math.imul(a, b)`：做 32-bit 乘法（FNV-1a 需要）

### 2) 通用雜湊需要「可重現的隨機」

Node 沒有內建 seeded random，因此 `universalHashing.js` 內用一個小型 LCG（`LcgRng`）：

- 測試可以用 `seed=t` 讓每次 trial 都挑到不同 `(a,b)`，但整體結果仍可重現

### 3) `UniversalHashTable` 的防禦性 rehash

插入後：

- load factor `> 0.75` 會 `resize`（capacity * 2）
- bucket chain 長度 `> 10` 會 `regenerate()` 並重新分配（避免對手刻意製造碰撞）

## 如何執行

在 `04-hash-tables/03-hash-functions/javascript/`：

```bash
node hashFunctionsDemo.js
node --test
```

