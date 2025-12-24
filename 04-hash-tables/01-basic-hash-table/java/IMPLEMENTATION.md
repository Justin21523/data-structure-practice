# 實作說明（Java）— 基本雜湊表

## 檔案與角色

- `HashTable.java`：泛型雜湊表 `HashTable<K,V>`（chaining）。
- `HashTableTest.java`：JUnit 5 測試。
- `ExampleWordCount.java`：應用範例（單字計數）。
- `pom.xml`：Maven 設定（編譯/測試/執行）。

## 核心資料結構

每個 bucket 是一條單向鏈結串列，節點 `Entry<K,V>` 透過 `next` 串起來；`buckets` 是 `Entry<K,V>[]`。

關鍵點：

- `hash(K key)`：`null` key 固定落在 bucket 0，其餘用 `Math.abs(key.hashCode()) % capacity`。
- `insert`：若鏈中已存在 key 則更新；否則插入新節點到鏈表頭（O(1) 插入）。
- `rehash`：容量加倍後重插所有元素。

## 主要流程片段

```java
private int hash(K key) {
  if (key == null) return 0;
  return Math.abs(key.hashCode()) % capacity;
}
```

rehash 會把舊 buckets 全部走訪並 `insert(current.key, current.value)` 到新 buckets 中，確保新容量下的分桶正確。

## 測試與執行

在 `04-hash-tables/01-basic-hash-table/java/`：

```bash
mvn test
mvn exec:java
```

## 注意事項

- `Math.abs(Integer.MIN_VALUE)` 仍是負數（Java 的 int 溢位特性）；若要更嚴謹，可改用 `hash & 0x7fffffff` 做非負化。此 repo 以教學清楚為主，先用 `Math.abs()` 示意。
- chaining 版本平均 O(1)，最差會退化到 O(n)（所有 key 落同 bucket）。

