# 實作說明（Python）— 雜湊函數（Hash Functions）

## 檔案與角色

- `hash_functions.py`：多種雜湊函數（整數/字串）示範與工具函式。
- `universal_hashing.py`：通用雜湊（universal hashing）與相關測試/示範。
- `test_hash_functions.py`：`unittest` 測試（範圍、確定性、分佈/碰撞機率等）。

## 目的與定位

這個單元的重點不是「雜湊表」本身，而是「設計/選擇雜湊函數」：

- **確定性（deterministic）**：同輸入同輸出。
- **分佈（distribution）**：輸出盡量均勻，降低碰撞。
- **效率**：計算成本要低（通常 O(1) 或 O(k)）。

## 代表性函數（概念）

1) 字串多項式雜湊（polynomial rolling hash）

```python
h = 0
for c in s:
    h = (h * a + ord(c)) % m
```

2) 通用雜湊（`h_{a,b}(k) = ((a*k + b) mod p) mod m`）

通用雜湊的核心是：從一個函數族中隨機選擇參數（a,b），可對抗「刻意製造碰撞」的輸入。

## 測試與執行

在 `04-hash-tables/03-hash-functions/python/`：

```bash
python3 -m unittest -v
```

## 建議延伸

- 嘗試用不同的 `m`（尤其是質數 vs 2 的冪次）比較分佈差異。
- 為你的雜湊表單元（例如 `01-basic-hash-table`）替換不同 hash function，觀察碰撞率與 rehash 次數。
