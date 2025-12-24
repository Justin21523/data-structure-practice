# 實作說明（C++）— 基本雜湊表

## 檔案與角色

- `HashTable.hpp`：header-only 的 `HashTable<K,V>` 模板類別（chaining）。
- `test_hash_table.cpp`：單元測試（搭配 CTest）。
- `example_word_count.cpp`：應用範例。
- `CMakeLists.txt`：CMake/CTest 設定。

## 核心資料結構

以 `std::vector<std::list<std::pair<K,V>>>` 表示 buckets：

- `vector`：固定長度的桶陣列（容量 `capacity_`）。
- `list`：每個桶的鏈（碰撞時同桶多元素）。

hash 以 `std::hash<K>` 取得雜湊值，再對 `capacity_` 取餘數決定桶索引。

## 主要操作

- `insert(key,value)`：在 bucket 的鏈上找 key；找到則更新，否則插入新 pair。必要時 rehash。
- `search(key)`：回傳 `std::optional<V>`（找不到回 `std::nullopt`）。
- `remove(key)`：刪除指定 key。
- `operator[]`：若 key 不存在會插入預設值並回傳參考（符合 C++ map 常見語意）。

範例片段（概念）：

```cpp
size_t index = hasher_(key) % capacity_;
auto& chain = buckets_[index];
```

## 建置與測試

在 `04-hash-tables/01-basic-hash-table/cpp/`：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## 注意事項

- chaining 版本刪除簡單（直接在鏈上移除），不需要 tombstone。
- rehash 時請確保所有元素重新映射到新 buckets（避免仍用舊索引）。

