# 實作說明（C）— 雜湊函數（Hash Functions）

## 檔案

- `hash_functions.h` / `hash_functions.c`：整數/字串雜湊函數 + 分布分析 `analyze_distribution`。
- `universal_hashing.h` / `universal_hashing.c`：通用雜湊（universal hashing）函數族 + `universal_hash_table`（chaining）。
- `hash_functions_main.c`：示範程式（印出 hash 值與分布摘要）。
- `test_hash_functions.c`：測試（範圍、確定性、anagram 碰撞、分布、通用雜湊、雜湊表操作）。
- `Makefile`：`make run` / `make test` / `make clean`

## 核心概念

### 1) 32-bit hash 值

字串 hash（DJB2 / FNV-1a / Jenkins）用 `uint32_t` 表示 32-bit unsigned，溢位行為就是「模 2^32」。

### 2) 正餘數（positive modulo）

C 的 `%` 對負數會保留負號，因此 `division_hash` / `folding_hash` 會先把 remainder 調整到 `[0, m)`。

### 3) 通用雜湊的隨機參數（可重現）

為了讓測試可重現、避免依賴 `rand()`，`universal_hashing.c` 使用自製 LCG：

- `seed=t` → 每個 trial 都會產生不同的 `(a,b)`，但整體結果仍可重現

### 4) `universal_hash_table` 的 rehash

插入後：

- load factor `> 0.75` 會擴容（capacity * 2）並重新分配所有 node
- bucket chain 長度 `> 10` 會 `regenerate (a,b)` 並重新分配（防禦性 rehash）

## 如何執行

在 `04-hash-tables/03-hash-functions/c/`：

```bash
make
make run
make test
make clean
```

