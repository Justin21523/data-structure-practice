# 實作說明（JavaScript）— 基本雜湊表

## 檔案與角色

- `hashTable.js`：雜湊表主體實作（chaining）。
- `hashTable.test.js`：Jest 單元測試。
- `exampleWordCount.js`：應用範例（單字計數）。
- `package.json`：測試/執行指令（`npm test`、`npm run example` 等）。

## 核心資料結構

`HashTable` 以「桶陣列 `_buckets`」儲存多條鏈：每個 bucket 是一個 `Array<[key,value]>`。碰撞時把元素放在同一 bucket 的鏈中，插入/搜尋/刪除都在該鏈線性掃描。

## 雜湊函數與索引

`_hash(key)` 會先把 `key` 轉成字串，然後用簡化的 djb2 變體累加出 32-bit 整數，再 `Math.abs(hash) % capacity` 映射到 bucket 範圍：

```js
_hash(key) {
  const strKey = String(key);
  let hash = 0;
  for (let i = 0; i < strKey.length; i++) {
    hash = ((hash << 5) - hash + strKey.charCodeAt(i)) | 0;
  }
  return Math.abs(hash) % this._capacity;
}
```

## 擴容（rehash）

當 `loadFactor > MAX_LOAD_FACTOR`：

- `_capacity *= 2`
- 重新配置 `_buckets`
- 將舊 buckets 的所有元素重新 `insert` 進新表（確保 bucket 分佈符合新容量）

## 測試與執行

在 `04-hash-tables/01-basic-hash-table/javascript/`：

```bash
npm install
npm test
npm run example
```

## 常見檢查點

- 插入同 key 應覆寫 value（而不是增加 size）。
- rehash 後所有 key 仍可查到正確值。
- chaining 版本刪除不需要 tombstone（和 open addressing 不同）。

