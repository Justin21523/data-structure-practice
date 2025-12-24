# 實作說明（Python）— 漸進符號示範（Asymptotic Notation）

## 目的

這個單元把「漸進符號」用可重現的方式示範出來：**不做實際計時**，改用「操作次數（operation count）」去觀察成長率，避免硬體、OS 排程、JIT 等因素干擾。

## 檔案

- `asymptotic_demo.py`：提供 5 個典型成長率的「操作次數」函式，並可印出表格。
- `test_asymptotic_demo.py`：使用 Python 內建 `unittest` 驗證計數是否符合定義。

## 核心設計（為什麼用操作次數）

時間複雜度分析的本質是「當 n 變大，成本如何成長」。在教學情境中，用操作次數可以直接對應到 Big-O 直覺：

- O(1)：不隨 n 增加（固定常數次）。
- O(log n)：每次把問題規模縮小一半（反覆除以 2）。
- O(n)：做 n 次。
- O(n log n)：外層 n 次、內層 log n 次。
- O(n²)：雙重迴圈。

## 代表性程式片段

### O(log n)：反覆除以 2

```python
while current > 1:
    current //= 2
    operations += 1
```

### O(n²)：雙重迴圈

```python
for _ in range(n):
    for _ in range(n):
        operations += 1
```

## 如何執行

在 `01-introduction-and-complexity/01-asymptotic-notation/python/`：

```bash
python3 asymptotic_demo.py
python3 asymptotic_demo.py 10 100 1000
python3 -m unittest -v
```

## 注意事項

- 這裡的「operations」是示範用的計數器，不是 CPU 指令數；重點在成長趨勢。
- `count_log2_ops(n)` 對 `n < 1` 會丟 `ValueError`，避免把不合理輸入帶進示範。

