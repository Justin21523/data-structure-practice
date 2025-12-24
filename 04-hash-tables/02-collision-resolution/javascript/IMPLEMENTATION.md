# 實作說明（JavaScript）— 碰撞處理（Chaining / Open Addressing）

## 檔案與角色

- `chaining.js`：鏈結法（separate chaining）的雜湊表與統計。
- `openAddressing.js`：開放定址（open addressing）雜湊表，含多種 probe 策略。
- `collision.test.js`：Jest 測試（碰撞、刪除、tombstone、擴容、探測統計等）。
- `package.json`：`npm test` / `npm run start` / `npm run start:open` 指令。

## Chaining

概念與基本雜湊表一致：bucket 裝一條鏈，碰撞時把元素掛同一 bucket。刪除是從鏈中移除，不需要墓碑。

## Open Addressing

開放定址把元素放在同一個表陣列中，碰撞時用 probe 序列找下一個可用槽位。刪除不能直接清空，必須用 tombstone 標記，否則搜尋會在遇到空位時提早停止而誤判不存在。

典型流程：

```js
// probeIndex = (h1 + i * step) % capacity
// if slot is DELETED -> keep probing
```

## 測試與執行

在 `04-hash-tables/02-collision-resolution/javascript/`：

```bash
npm install
npm test
npm run start
npm run start:open
```

## 常見檢查點

- tombstone 必須「可插入、不可中止搜尋」。
- 負載因子過高會使 open addressing 探測次數上升；擴容與合理閾值很重要。

