# 實作說明（JavaScript / Node.js）— 漸進符號示範（Asymptotic Notation）

## 目標

用 **Node.js** 以「操作次數（operation count）」示範不同成長率（O(1)、O(log n)、O(n)、O(n log n)、O(n²)），避免用實際時間造成結果不穩定。

## 檔案

- `asymptoticDemo.js`：示範程式（可印出表格），並 `module.exports` 匯出函式供測試使用。
- `asymptoticDemo.test.js`：使用 Node 內建測試框架 `node:test`（不需 Jest）。

## 核心片段

O(log n) 透過反覆除以 2 計數：

```js
while (current > 1) {
  current = Math.floor(current / 2);
  operations += 1;
}
```

## 如何執行

在 `01-introduction-and-complexity/01-asymptotic-notation/javascript/`：

```bash
node asymptoticDemo.js
node --test
```

## 注意事項

- demo 會要求表格輸入的 n 皆為整數且 `>= 1`，避免 log 欄位遇到不合理值。
- 測試使用內建框架，不需要安裝任何套件。

