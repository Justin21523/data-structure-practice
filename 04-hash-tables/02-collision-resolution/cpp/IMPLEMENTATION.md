# 實作說明（C++）— 碰撞處理（Chaining / Open Addressing）

## 檔案與角色

- `Chaining.hpp`：鏈結法雜湊表（header-only）。
- `OpenAddressing.hpp`：開放定址雜湊表（含探測策略與 tombstone）。
- `test_collision.cpp`：測試（搭配 CTest）。
- `CMakeLists.txt`：建置與 CTest 設定。

## Chaining

chaining 版本的核心是：`buckets_[index]` 是一條鏈（常見是 `std::list<pair>` 或等價容器），碰撞時插入同一鏈中；刪除只需在鏈上移除，語意直覺。

## Open Addressing

open addressing 版本會把元素放在單一陣列中，碰撞時用 probe 序列尋找可用位置。刪除要使用 tombstone（保留搜尋路徑），因此「表面空位」與「真正從未用過的空位」不同，擴容與搜尋必須分別處理。

## 建置與測試

在 `04-hash-tables/02-collision-resolution/cpp/`：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## 建議閱讀順序

1) 先看 `Chaining.hpp`（理解基礎碰撞處理）。
2) 再看 `OpenAddressing.hpp`（理解 probe 與 tombstone）。
3) 最後用 `test_collision.cpp` 對照每個 edge case。

