# 實作說明（Java）— 碰撞處理（Chaining / Open Addressing）

## 檔案與角色

- `src/main/java/ChainedHashTable.java`：鏈結法（桶陣列 + 鏈結節點）+ probe 統計。
- `src/main/java/OpenAddressingHashTable.java`：開放定址（含 `ProbeMethod`）+ tombstone。
- `src/test/java/CollisionTest.java`：JUnit 5 測試。
- `pom.xml`：Maven 設定（`mvn test`、exec 設定）。

## Chaining（鏈結法）

每個 bucket 是一條 `Entry<K,V>` 鏈；插入通常會把新節點放在鏈表頭，並統計：

- `totalProbes`：總探測次數（掃描節點/探測次數）
- `maxChainLength`：最長鏈長度
- `collisions`：碰撞次數

## Open Addressing（開放定址）

`OpenAddressingHashTable` 以 `Entry[] table` 存放資料，`Entry` 含 `isDeleted` tombstone 欄位。探測序列依 `ProbeMethod` 決定：

- Linear：`(h1 + i) % m`
- Quadratic：`(h1 + c1*i + c2*i*i) % m`
- Double Hash：`(h1 + i*h2) % m`

刪除會把 `isDeleted=true`，搜尋遇到 tombstone 必須繼續探測。

## 測試與執行

在 `04-hash-tables/02-collision-resolution/java/`：

```bash
mvn test
```

（如需執行主程式，請參考 `pom.xml` 中 `exec-maven-plugin` 的 `mainClass` 設定。）

## 注意事項

- open addressing 的負載因子需更保守；否則失敗搜尋成本會接近 `1/(1-α)` 急遽上升。
- tombstone 的存在會讓「有效元素數」與「已佔用槽位數（含墓碑）」不同，擴容條件應考慮這點。

