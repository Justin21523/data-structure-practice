# 實作說明（C）— 漸進符號示範（Asymptotic Notation）

## 目標

用 **C** 以「操作次數（operation count）」示範成長率，讓你把 Big-O 直接連結到「迴圈跑幾次」這個最直觀的模型。

## 檔案

- `asymptotic_demo.h` / `asymptotic_demo.c`：五種成長率的操作次數函式（可被測試與 CLI 共用）。
- `asymptotic_demo_main.c`：CLI 入口（`main`）+ 參數解析 + 印出表格。
- `test_asymptotic_demo.c`：自製最小化測試 runner（不依賴外部框架）。
- `Makefile`：`make run` / `make test`。

> 為什麼要拆檔？因為測試也會連結 `asymptotic_demo.c`，若 `main` 也在同一個檔案就會出現「multiple definition of main」的 linker 錯誤。

## 錯誤處理策略

C 沒有例外（exceptions），本單元用 **回傳 -1** 作為簡單錯誤訊號：

- `count_log2_ops(n)`：`n < 1` 回傳 `-1`
- 其他需要 `n >= 0` 的函式：`n < 0` 回傳 `-1`

## 代表性片段

O(log n) 的核心是「反覆除以 2」：

```c
while (current > 1) {
  current /= 2;
  operations += 1;
}
```

## 如何執行

在 `01-introduction-and-complexity/01-asymptotic-notation/c/`：

```bash
make
make run
make test
```
