# 實作說明（C#）— 基本雜湊表

## 檔案與角色

- `HashTable.cs`：`HashTable<TKey,TValue>`（chaining）。
- `HashTableTest.cs`：xUnit 單元測試。
- `ExampleWordCount.cs`：應用範例（單字計數）。
- `HashTable.csproj`：.NET 專案與測試套件設定。

## 核心資料結構

每個 bucket 是一條單向鏈，節點 `Entry` 包含：

- `Key` / `Value`
- `Next`：下一個節點

表本體用 `_buckets`（`Entry[]`）保存各 bucket 的鏈表頭，並追蹤 `_capacity`、`_size`。

## 主要操作

- `Insert(key,value)`：若鏈中 key 已存在則更新；否則插入新節點（通常插入鏈表頭）。
- `TryGetValue` / `Search`：定位 bucket 後沿鏈掃描。
- `Delete(key)`：沿鏈找 key，找到就移除並更新 `_size`。
- `Rehash()`：容量加倍後重插所有元素，維持負載因子在閾值下。
- Indexer `this[key]`：get 會在缺 key 時丟 `KeyNotFoundException`，set 會等同 `Insert`。

## 測試與執行

在 `04-hash-tables/01-basic-hash-table/csharp/`：

```bash
dotnet test
dotnet run
```

## 注意事項

- `GetHashCode()` 可能回傳負數；實作以 `Math.Abs(hash) % _capacity` 映射到 bucket 範圍。
- chaining 版本平均 O(1)，但在極端碰撞下會退化到 O(n)。

