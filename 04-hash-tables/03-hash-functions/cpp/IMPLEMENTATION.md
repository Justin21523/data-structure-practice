# 實作說明（C++）— 雜湊函數（Hash Functions）

## 檔案

- `HashFunctions.hpp`：整數/字串雜湊函數 + `analyzeDistribution`。
- `UniversalHashing.hpp`：通用雜湊（universal hashing）函數族 + `UniversalHashTable`（chaining）。
- `hash_functions_demo.cpp`：示範程式（印出 hash 值與分布摘要）。
- `test_hash_functions.cpp`：測試（範圍、確定性、anagram 碰撞、分布、通用雜湊、雜湊表操作）。
- `CMakeLists.txt`：CMake + CTest

## 核心概念

### 1) 32-bit string hash

字串 hash（DJB2 / FNV-1a / Jenkins）回傳 `std::uint32_t`，溢位行為就是「模 2^32」。

### 2) `analyzeDistribution`

接受一個 `(key, m) -> index` 的 callable（lambda），把 keys 打到 `m` 個桶後，計算：

- `nonEmptyBuckets`：有元素的桶數
- `stdDeviation`：桶大小標準差（越小通常代表越均勻）

### 3) 通用雜湊（Universal Hashing）

`UniversalHashFamily` 使用 `std::mt19937`，並允許指定 seed，讓測試可重現：

```
h_{a,b}(k) = ((a*k + b) mod p) mod m
```

### 4) `UniversalHashTable`

使用 `std::vector<std::vector<std::pair<int, std::string>>>` 做 chaining，插入後：

- load factor `> 0.75` → resize（capacity * 2）
- chain 長度 `> 10` → regenerate 並重新分配（防禦性 rehash）

## 如何執行

在 `04-hash-tables/03-hash-functions/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/hash_functions_demo
ctest --test-dir build --output-on-failure
```

