# 實作說明（Python）— 碰撞處理（Chaining / Open Addressing）

## 檔案與角色

- `chaining.py`：`ChainedHashTable`（鏈結法）+ 探測/鏈長統計。
- `open_addressing.py`：`OpenAddressingHashTable`（開放定址）+ `ProbeMethod`（線性/二次/雙重雜湊）。
- `test_collision.py`：pytest 測試（包含 tombstone、擴容與分佈/探測統計）。

## Chaining（鏈結法）

`ChainedHashTable` 結構與基本雜湊表相同：`_buckets[index]` 是一條 list chain。額外差異是此檔案會回傳/累計 **probes**（每次操作掃描了多少節點），方便用來比較效能。

核心概念片段：

```python
index = self._hash(key)
bucket = self._buckets[index]
```

## Open Addressing（開放定址）

`OpenAddressingHashTable` 用兩個陣列 `_keys` / `_values` 存放資料，並用兩個特殊 marker：

- `EMPTY`：從未使用過的槽位
- `DELETED`：墓碑（tombstone），表示曾經有元素但已刪除

插入/搜尋的關鍵是 `_probe(key, i)`：依照 `ProbeMethod` 產生第 i 次探測位置。遇到 `DELETED` 不能直接停止（否則會錯誤判定 key 不存在），必須繼續探測；插入時可以優先回收墓碑位置。

```python
if self._keys[index] is self.DELETED:
    continue  # Skip tombstone and keep probing
```

## 測試與執行

在 `04-hash-tables/02-collision-resolution/python/`：

```bash
python -m pytest
```

## 常見陷阱

- open addressing 的負載因子通常要更低（本實作採較低門檻），否則探測次數會急遽上升。
- 刪除一定要用墓碑（tombstone），不能直接設回 `EMPTY`（會破壞探測序列）。

