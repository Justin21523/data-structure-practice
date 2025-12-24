# 實作說明（Python）— 基本雜湊表

## 檔案與角色

- `hash_table.py`：雜湊表主體實作（chaining 鏈結法）。
- `test_hash_table.py`：pytest 單元測試（插入/搜尋/刪除/擴容/碰撞等）。
- `example_word_count.py`：應用範例（單字計數）。

## 核心資料結構

`HashTable` 以「桶陣列（buckets）+ 每桶一條鏈（list of (key,value)）」實作碰撞處理。負載因子 `load_factor = size / capacity` 超過閾值會觸發 rehash（通常擴容為 2 倍）。

重點欄位：

- `_buckets`：`list[list[Tuple[key,value]]]`，每個 bucket 存放一串鍵值對。
- `_size` / `_capacity`：元素數量與桶數量。

## 主要操作流程

1) **雜湊索引**

```python
def _hash(self, key):
    return hash(key) % self._capacity
```

2) **插入（insert）**

- 先用 `_hash(key)` 找 bucket。
- 線性掃描 bucket：若 key 已存在就更新 value。
- 否則 append 新鍵值對並 `size += 1`。
- 若負載因子超過 `MAX_LOAD_FACTOR`，呼叫 `_rehash()` 將所有元素重新分配到新桶陣列。

3) **搜尋（search）/ 刪除（delete）**

- 都是先定位 bucket，再在鏈中線性掃描（平均 O(1)，最差 O(n)）。
- 刪除成功時會更新 `_size`。

## 測試與執行

在 `04-hash-tables/01-basic-hash-table/python/`：

```bash
pip install -r requirements.txt
python hash_table.py
python -m pytest
```

## 注意事項

- Python 的 `hash()` 可能因執行環境/啟動隨機化而不同，但這不影響正確性（只影響分桶位置）。
- chaining 的最差情況會退化成單一 bucket 很長的鏈；實作以 rehash + 負載因子控制來降低機率。

