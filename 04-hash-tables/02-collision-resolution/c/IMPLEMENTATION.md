# 實作說明（C）— 碰撞處理（Chaining / Open Addressing）

## 檔案與角色

- `chaining.h` / `chaining.c`：鏈結法雜湊表（separate chaining）。
- `open_addressing.h` / `open_addressing.c`：開放定址雜湊表（linear/quadratic/double hashing + tombstone）。
- `test_collision.c`：測試。

## Chaining

與基本雜湊表相同：每個 bucket 指向一條鏈，碰撞就插入該鏈。刪除直接從鏈移除，不需要墓碑。

## Open Addressing

開放定址的核心在於：

- **碰撞時探測下一個槽位**（依探測策略產生序列）。
- **刪除用 tombstone**：不能直接清空，否則會破壞後續元素的搜尋路徑。

建議閱讀 `open_addressing.h/.c` 中「探測方法」與「刪除（墓碑）」相關 API 與註解，並搭配 `test_collision.c` 的案例理解 tombstone 行為。

## 建置與測試

此單元的 C 版本沒有獨立 Makefile；可用你熟悉的方式編譯（或參考 `01-basic-hash-table/c/Makefile` 的寫法）。例如：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 test_collision.c chaining.c open_addressing.c -o test_collision
./test_collision
```

## 注意事項

- open addressing 的擴容條件通常要更嚴格（α 不宜太高）。
- C 版本要特別注意記憶體釋放與 key/value 的所有權規則（請依各 API 註解為準）。

