# 實作說明（C）— 基本雜湊表

## 檔案與角色

- `hash_table.h`：資料結構與 API 宣告。
- `hash_table.c`：主體實作（chaining + rehash）。
- `test_hash_table.c`：單元測試。
- `example_word_count.c`：應用範例。
- `Makefile`：建置/測試指令。

## 核心資料結構

- `HashTableEntry`：鏈結節點，包含 `char* key`、`void* value`、`next`。
- `HashTable`：`HashTableEntry** buckets`（桶陣列）、`capacity`、`size`。

碰撞時把元素放到同一 bucket 的鏈上（separate chaining）。

## 雜湊與插入

`hash()` 使用 djb2（`hash * 33 + c`）處理字串 key，再對 `capacity` 取餘數得到 bucket index。`ht_insert`：

1) 定位 bucket。
2) 走訪鏈表：若 key 相同就更新 value。
3) 否則建立新節點並插入（本實作會複製 key：`strdup`）。

## 擴容（rehash）與記憶體責任

當負載因子過高會 rehash：

- 配置更大的 buckets（通常 2 倍）
- 逐一把舊節點重插到新 buckets

記憶體責任要點：

- **key 由雜湊表複製並持有**（節點釋放時會 `free(key)`）。
- **value 不由雜湊表釋放**（`void*` 由呼叫端管理生命週期）。

## 測試與執行

在 `04-hash-tables/01-basic-hash-table/c/`：

```bash
make
make test
make run
```

## 常見陷阱

- rehash 時搬移節點要避免 memory leak（舊 buckets 與舊節點需正確釋放）。
- 字串 key 一定要複製（否則呼叫端釋放/改寫後會造成野指標或錯誤比對）。

