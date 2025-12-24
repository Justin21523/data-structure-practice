/** Doc block start
 * 開放定址雜湊表（Open Addressing Hash Table）
 * Open Addressing Hash Table with multiple probing strategies
 *(blank line)
 * 當發生碰撞時，使用探測序列在表中尋找下一個空位 / When collision occurs, use a probe sequence to find the next empty slot
 *(blank line)
 * 支援三種探測方法： / Supports three probing methods:
 * 1. 線性探測（Linear Probing）: h(k, i) = (h(k) + i) mod m
 * 2. 二次探測（Quadratic Probing）: h(k, i) = (h(k) + c1*i + c2*i^2) mod m
 * 3. 雙重雜湊（Double Hashing）: h(k, i) = (h1(k) + i*h2(k)) mod m
 *(blank line)
 * @param <K> 鍵的型別（key type）
 * @param <V> 值的型別（value type）
 */  // End of block comment
public class OpenAddressingHashTable<K, V> {  // Execute this statement as part of the data structure implementation.

    // ========== 列舉型別 Enumerations ==========

    /** Doc block start
     * 探測方法 / Probe method for collision resolution
     */  // End of block comment
    public enum ProbeMethod {  // Execute this statement as part of the data structure implementation.
        LINEAR,       // 線性探測 - Linear probing
        QUADRATIC,    // 二次探測 - Quadratic probing
        DOUBLE_HASH   // 雙重雜湊 - Double hashing
    }  // Close the current block scope.

    // ========== 內部類別 Inner Classes ==========

    /** Doc block start
     * 表格項目 / Table entry storing key-value pair
     */  // End of block comment
    private static class Entry<K, V> {  // Execute this statement as part of the data structure implementation.
        K key;  // Execute this statement as part of the data structure implementation.
        V value;  // Execute this statement as part of the data structure implementation.
        boolean isDeleted;  // 墓碑標記 - Tombstone marker

        Entry(K key, V value) {  // Execute this statement as part of the data structure implementation.
            this.key = key;  // Assign or update a variable that represents the current algorithm state.
            this.value = value;  // Assign or update a variable that represents the current algorithm state.
            this.isDeleted = false;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 探測統計資料 / Probe statistics for performance analysis
     */  // End of block comment
    public static class ProbeStats {  // Execute this statement as part of the data structure implementation.
        public int totalProbes;      // 總探測次數 - Total number of probes
        public int maxProbes;        // 單次操作最大探測次數 - Max probes in single operation
        public int collisions;       // 碰撞次數 - Number of collisions

        public ProbeStats() {  // Execute this statement as part of the data structure implementation.
            this.totalProbes = 0;  // Assign or update a variable that represents the current algorithm state.
            this.maxProbes = 0;  // Assign or update a variable that represents the current algorithm state.
            this.collisions = 0;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        @Override  // Execute this statement as part of the data structure implementation.
        public String toString() {  // Execute this statement as part of the data structure implementation.
            return String.format(  // Return the computed result to the caller.
                "ProbeStats{totalProbes=%d, maxProbes=%d, collisions=%d}",  // Assign or update a variable that represents the current algorithm state.
                totalProbes, maxProbes, collisions  // Execute this statement as part of the data structure implementation.
            );  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // ========== 常數 Constants ==========
    private static final int DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    private static final double MAX_LOAD_FACTOR = 0.5;  // 開放定址法建議較低的負載因子

    // 二次探測的常數 - Quadratic probing constants
    private static final int C1 = 1;  // Assign or update a variable that represents the current algorithm state.
    private static final int C2 = 3;  // Assign or update a variable that represents the current algorithm state.

    // ========== 成員變數 Member Variables ==========
    private Entry<K, V>[] table;     // 雜湊表陣列 - Hash table array
    private int capacity;             // 表格容量 - Table capacity
    private int size;                 // 元素數量 - Number of elements
    private int deletedCount;         // 已刪除項目數量 - Number of deleted entries
    private ProbeMethod probeMethod;  // 探測方法 - Probing method
    private ProbeStats stats;         // 探測統計 - Probe statistics

    // ========== 建構子 Constructors ==========

    /** Doc block start
     * 使用預設容量和線性探測建立雜湊表 / Create hash table with default capacity and linear probing
     */  // End of block comment
    public OpenAddressingHashTable() {  // Execute this statement as part of the data structure implementation.
        this(DEFAULT_CAPACITY, ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    /** Doc block start
     * 使用指定探測方法建立雜湊表 / Create hash table with specified probe method
     *(blank line)
     * @param probeMethod 探測方法
     */  // End of block comment
    public OpenAddressingHashTable(ProbeMethod probeMethod) {  // Advance or track the probing sequence used by open addressing.
        this(DEFAULT_CAPACITY, probeMethod);  // Advance or track the probing sequence used by open addressing.
    }  // Close the current block scope.

    /** Doc block start
     * 使用指定容量和探測方法建立雜湊表 / Create hash table with specified capacity and probe method
     *(blank line)
     * @param capacity 表格容量
     * @param probeMethod 探測方法
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    public OpenAddressingHashTable(int capacity, ProbeMethod probeMethod) {  // Advance or track the probing sequence used by open addressing.
        if (capacity <= 0) {  // Evaluate the condition and branch into the appropriate code path.
            throw new IllegalArgumentException(  // Throw an exception to signal an invalid argument or operation.
                "容量必須為正整數 / Capacity must be positive");  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
        this.capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
        this.size = 0;  // Assign or update a variable that represents the current algorithm state.
        this.deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
        this.probeMethod = probeMethod;  // Advance or track the probing sequence used by open addressing.
        this.table = (Entry<K, V>[]) new Entry[capacity];  // Assign or update a variable that represents the current algorithm state.
        this.stats = new ProbeStats();  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    // ========== 私有方法 Private Methods ==========

    /** Doc block start
     * 主雜湊函數 / Primary hash function
     *(blank line)
     * @param key 要雜湊的鍵
     * @return 雜湊索引
     */  // End of block comment
    private int hash1(K key) {  // Compute a hash-based index so keys map into the table's storage.
        if (key == null) {  // Evaluate the condition and branch into the appropriate code path.
            return 0;  // Return the computed result to the caller.
        }  // Close the current block scope.
        return Math.abs(key.hashCode()) % capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 次要雜湊函數（用於雙重雜湊） / Secondary hash function (for double hashing)
     *(blank line)
     * @param key 要雜湊的鍵
     * @return 次要雜湊值
     */  // End of block comment
    private int hash2(K key) {  // Compute a hash-based index so keys map into the table's storage.
        if (key == null) {  // Evaluate the condition and branch into the appropriate code path.
            return 1;  // Return the computed result to the caller.
        }  // Close the current block scope.
        // 確保結果為正數且不為 0 / Ensure result is positive and non-zero
        int h = Math.abs(key.hashCode());  // Compute a stable hashCode-based index and map it into the bucket range.
        return 1 + (h % (capacity - 1));  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 計算探測序列的索引 / Calculate probe sequence index
     *(blank line)
     * @param key 鍵
     * @param i 探測次數（0, 1, 2, ...）
     * @return 探測索引
     */  // End of block comment
    private int probe(K key, int i) {  // Advance or track the probing sequence used by open addressing.
        int h1 = hash1(key);  // Compute a hash-based index so keys map into the table's storage.

        switch (probeMethod) {  // Advance or track the probing sequence used by open addressing.
            case LINEAR:  // Execute this statement as part of the data structure implementation.
                // 線性探測: h(k, i) = (h(k) + i) mod m
                // Linear probing: h(k, i) = (h(k) + i) mod m
                return (h1 + i) % capacity;  // Return the computed result to the caller.

            case QUADRATIC:  // Execute this statement as part of the data structure implementation.
                // 二次探測: h(k, i) = (h(k) + c1*i + c2*i^2) mod m
                // Quadratic probing: h(k, i) = (h(k) + c1*i + c2*i^2) mod m
                return (h1 + C1 * i + C2 * i * i) % capacity;  // Return the computed result to the caller.

            case DOUBLE_HASH:  // Execute this statement as part of the data structure implementation.
                // 雙重雜湊: h(k, i) = (h1(k) + i*h2(k)) mod m
                // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
                int h2 = hash2(key);  // Compute a hash-based index so keys map into the table's storage.
                return (h1 + i * h2) % capacity;  // Return the computed result to the caller.

            default:  // Execute this statement as part of the data structure implementation.
                return h1;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 擴容並重新雜湊 / Resize and rehash
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    private void rehash() {  // Rehash entries into a larger table to keep operations near O(1) on average.
        Entry<K, V>[] oldTable = table;  // Assign or update a variable that represents the current algorithm state.
        capacity *= 2;  // Assign or update a variable that represents the current algorithm state.
        table = (Entry<K, V>[]) new Entry[capacity];  // Assign or update a variable that represents the current algorithm state.
        size = 0;  // Assign or update a variable that represents the current algorithm state.
        deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.

        // 重新插入所有元素 - Reinsert all elements
        for (Entry<K, V> entry : oldTable) {  // Iterate over a range/collection to process each item in sequence.
            if (entry != null && !entry.isDeleted) {  // Evaluate the condition and branch into the appropriate code path.
                insert(entry.key, entry.value);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查鍵是否相等 / Check if keys are equal
     */  // End of block comment
    private boolean keysEqual(K k1, K k2) {  // Execute this statement as part of the data structure implementation.
        return k1 == null ? k2 == null : k1.equals(k2);  // Return the computed result to the caller.
    }  // Close the current block scope.

    // ========== 公開方法 Public Methods ==========

    /** Doc block start
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param key 鍵
     * @param value 值
     * @return 插入成功回傳 true，表格已滿回傳 false
     */  // End of block comment
    public boolean insert(K key, V value) {  // Execute this statement as part of the data structure implementation.
        // 檢查負載因子 - Check load factor
        if (getLoadFactor() >= MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
            rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
        }  // Close the current block scope.

        int probes = 0;  // Advance or track the probing sequence used by open addressing.
        int firstDeletedIndex = -1;  // 記錄第一個遇到的已刪除位置

        for (int i = 0; i < capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            int index = probe(key, i);  // Advance or track the probing sequence used by open addressing.
            Entry<K, V> entry = table[index];  // Assign or update a variable that represents the current algorithm state.

            probes++;  // Advance or track the probing sequence used by open addressing.

            if (entry == null) {  // Evaluate the condition and branch into the appropriate code path.
                // 找到空位 - Found empty slot
                int insertIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : index;  // Execute this statement as part of the data structure implementation.
                table[insertIndex] = new Entry<>(key, value);  // Assign or update a variable that represents the current algorithm state.

                if (firstDeletedIndex != -1) {  // Evaluate the condition and branch into the appropriate code path.
                    deletedCount--;  // 重用已刪除的位置
                }  // Close the current block scope.

                size++;  // Execute this statement as part of the data structure implementation.
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                if (probes > stats.maxProbes) {  // Evaluate the condition and branch into the appropriate code path.
                    stats.maxProbes = probes;  // Advance or track the probing sequence used by open addressing.
                }  // Close the current block scope.
                if (i > 0) {  // Evaluate the condition and branch into the appropriate code path.
                    stats.collisions++;  // 發生碰撞 - Collision occurred
                }  // Close the current block scope.
                return true;  // Return the computed result to the caller.

            } else if (entry.isDeleted) {  // Execute this statement as part of the data structure implementation.
                // 記錄第一個已刪除位置 - Record first deleted position
                if (firstDeletedIndex == -1) {  // Evaluate the condition and branch into the appropriate code path.
                    firstDeletedIndex = index;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.

            } else if (keysEqual(entry.key, key)) {  // Execute this statement as part of the data structure implementation.
                // 鍵已存在，更新值 - Key exists, update value
                entry.value = value;  // Assign or update a variable that represents the current algorithm state.
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                return true;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 表格已滿 - Table is full
        stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        return false;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param key 要搜尋的鍵
     * @return 找到則回傳 value，否則回傳 null
     */  // End of block comment
    public V search(K key) {  // Execute this statement as part of the data structure implementation.
        int probes = 0;  // Advance or track the probing sequence used by open addressing.

        for (int i = 0; i < capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            int index = probe(key, i);  // Advance or track the probing sequence used by open addressing.
            Entry<K, V> entry = table[index];  // Assign or update a variable that represents the current algorithm state.

            probes++;  // Advance or track the probing sequence used by open addressing.

            if (entry == null) {  // Evaluate the condition and branch into the appropriate code path.
                // 遇到空位，表示 key 不存在 - Empty slot means key doesn't exist
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                return null;  // Return the computed result to the caller.

            } else if (!entry.isDeleted && keysEqual(entry.key, key)) {  // Execute this statement as part of the data structure implementation.
                // 找到 key - Found key
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                if (probes > stats.maxProbes) {  // Evaluate the condition and branch into the appropriate code path.
                    stats.maxProbes = probes;  // Advance or track the probing sequence used by open addressing.
                }  // Close the current block scope.
                return entry.value;  // Return the computed result to the caller.
            }  // Close the current block scope.

            // 繼續探測（可能是已刪除項目或其他鍵） / Continue probing (could be deleted entry or other key)
        }  // Close the current block scope.

        stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        return null;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 刪除指定的鍵值對（使用墓碑標記） / Delete key-value pair (using tombstone marker)
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     */  // End of block comment
    public boolean delete(K key) {  // Execute this statement as part of the data structure implementation.
        int probes = 0;  // Advance or track the probing sequence used by open addressing.

        for (int i = 0; i < capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            int index = probe(key, i);  // Advance or track the probing sequence used by open addressing.
            Entry<K, V> entry = table[index];  // Assign or update a variable that represents the current algorithm state.

            probes++;  // Advance or track the probing sequence used by open addressing.

            if (entry == null) {  // Evaluate the condition and branch into the appropriate code path.
                // 遇到空位，表示 key 不存在 - Empty slot means key doesn't exist
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                return false;  // Return the computed result to the caller.

            } else if (!entry.isDeleted && keysEqual(entry.key, key)) {  // Execute this statement as part of the data structure implementation.
                // 找到 key，標記為已刪除 - Found key, mark as deleted
                entry.isDeleted = true;  // Assign or update a variable that represents the current algorithm state.
                size--;  // Execute this statement as part of the data structure implementation.
                deletedCount++;  // Handle tombstones so deletions do not break the probing/search sequence.
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                if (probes > stats.maxProbes) {  // Evaluate the condition and branch into the appropriate code path.
                    stats.maxProbes = probes;  // Advance or track the probing sequence used by open addressing.
                }  // Close the current block scope.

                // 如果已刪除項目過多，考慮重新雜湊 / Consider rehashing if too many deleted entries
                if (deletedCount > capacity / 4) {  // Evaluate the condition and branch into the appropriate code path.
                    rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
                }  // Close the current block scope.

                return true;  // Return the computed result to the caller.
            }  // Close the current block scope.

            // 繼續探測 - Continue probing
        }  // Close the current block scope.

        stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        return false;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查 key 是否存在
     * Check if key exists
     *(blank line)
     * @param key 要檢查的鍵
     * @return 存在回傳 true，否則回傳 false
     */  // End of block comment
    public boolean contains(K key) {  // Execute this statement as part of the data structure implementation.
        return search(key) != null;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳元素數量 / Return number of elements
     */  // End of block comment
    public int size() {  // Execute this statement as part of the data structure implementation.
        return size;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳表格容量 / Return table capacity
     */  // End of block comment
    public int capacity() {  // Execute this statement as part of the data structure implementation.
        return capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查是否為空 / Check if empty
     */  // End of block comment
    public boolean isEmpty() {  // Execute this statement as part of the data structure implementation.
        return size == 0;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 計算負載因子 / Calculate load factor
     *(blank line)
     * 負載因子 = (元素數量 + 已刪除項目數量) / 容量 / Load factor = (number of elements + deleted entries) / capacity
     *(blank line)
     * @return 負載因子
     */  // End of block comment
    public double getLoadFactor() {  // Execute this statement as part of the data structure implementation.
        return (double) (size + deletedCount) / capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 取得探測統計資料 / Get probe statistics
     *(blank line)
     * @return 探測統計資料
     */  // End of block comment
    public ProbeStats getProbeStats() {  // Execute this statement as part of the data structure implementation.
        return stats;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 重置探測統計 / Reset probe statistics
     */  // End of block comment
    public void resetStats() {  // Execute this statement as part of the data structure implementation.
        stats = new ProbeStats();  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    /** Doc block start
     * 取得探測方法 / Get probe method
     */  // End of block comment
    public ProbeMethod getProbeMethod() {  // Execute this statement as part of the data structure implementation.
        return probeMethod;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    public void clear() {  // Execute this statement as part of the data structure implementation.
        table = (Entry<K, V>[]) new Entry[capacity];  // Assign or update a variable that represents the current algorithm state.
        size = 0;  // Assign or update a variable that represents the current algorithm state.
        deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
        stats = new ProbeStats();  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    /** Doc block start
     * 字串表示 / String representation
     */  // End of block comment
    @Override  // Execute this statement as part of the data structure implementation.
    public String toString() {  // Execute this statement as part of the data structure implementation.
        StringBuilder sb = new StringBuilder("OpenAddressingHashTable{");  // Assign or update a variable that represents the current algorithm state.
        boolean first = true;  // Assign or update a variable that represents the current algorithm state.

        for (Entry<K, V> entry : table) {  // Iterate over a range/collection to process each item in sequence.
            if (entry != null && !entry.isDeleted) {  // Evaluate the condition and branch into the appropriate code path.
                if (!first) {  // Evaluate the condition and branch into the appropriate code path.
                    sb.append(", ");  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
                sb.append(entry.key).append("=").append(entry.value);  // Assign or update a variable that represents the current algorithm state.
                first = false;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        }  // Close the current block scope.

        sb.append("}");  // Execute this statement as part of the data structure implementation.
        return sb.toString();  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 印出雜湊表內部結構（用於除錯） / Print internal structure (for debugging)
     */  // End of block comment
    public void printStructure() {  // Execute this statement as part of the data structure implementation.
        System.out.println("=== 雜湊表內部結構 Hash Table Structure ===");  // Execute this statement as part of the data structure implementation.
        System.out.println("探測方法 Probe Method: " + probeMethod);  // Advance or track the probing sequence used by open addressing.

        for (int i = 0; i < capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            Entry<K, V> entry = table[i];  // Assign or update a variable that represents the current algorithm state.
            System.out.printf("Table[%d]: ", i);  // Execute this statement as part of the data structure implementation.

            if (entry == null) {  // Evaluate the condition and branch into the appropriate code path.
                System.out.println("empty");  // Execute this statement as part of the data structure implementation.
            } else if (entry.isDeleted) {  // Execute this statement as part of the data structure implementation.
                System.out.println("DELETED (tombstone)");  // Handle tombstones so deletions do not break the probing/search sequence.
            } else {  // Handle the alternative branch when the condition is false.
                System.out.printf("%s->%s%n", entry.key, entry.value);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.

        System.out.println("大小 Size: " + size);  // Execute this statement as part of the data structure implementation.
        System.out.println("已刪除 Deleted: " + deletedCount);  // Handle tombstones so deletions do not break the probing/search sequence.
        System.out.println("負載因子 Load Factor: " + getLoadFactor());  // Execute this statement as part of the data structure implementation.
        System.out.println("統計資料 Statistics: " + stats);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // ========== 範例執行 Example Main ==========

    public static void main(String[] args) {  // Execute this statement as part of the data structure implementation.
        System.out.println("=== 開放定址雜湊表示範 Open Addressing Hash Table Demo ===\n");  // Execute this statement as part of the data structure implementation.

        // 測試線性探測 - Test linear probing
        System.out.println("--- 線性探測 Linear Probing ---");  // Execute this statement as part of the data structure implementation.
        OpenAddressingHashTable<Integer, String> ht1 =  // Assign or update a variable that represents the current algorithm state.
            new OpenAddressingHashTable<>(7, ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

        ht1.insert(10, "ten");  // Execute this statement as part of the data structure implementation.
        ht1.insert(20, "twenty");  // Execute this statement as part of the data structure implementation.
        ht1.insert(30, "thirty");  // Execute this statement as part of the data structure implementation.
        ht1.insert(17, "seventeen");  // Will collide with 10 (10 % 7 = 3, 17 % 7 = 3)

        ht1.printStructure();  // Execute this statement as part of the data structure implementation.
        System.out.println();  // Execute this statement as part of the data structure implementation.

        // 測試二次探測 - Test quadratic probing
        System.out.println("--- 二次探測 Quadratic Probing ---");  // Execute this statement as part of the data structure implementation.
        OpenAddressingHashTable<Integer, String> ht2 =  // Assign or update a variable that represents the current algorithm state.
            new OpenAddressingHashTable<>(7, ProbeMethod.QUADRATIC);  // Execute this statement as part of the data structure implementation.

        ht2.insert(10, "ten");  // Execute this statement as part of the data structure implementation.
        ht2.insert(20, "twenty");  // Execute this statement as part of the data structure implementation.
        ht2.insert(30, "thirty");  // Execute this statement as part of the data structure implementation.
        ht2.insert(17, "seventeen");  // Execute this statement as part of the data structure implementation.

        ht2.printStructure();  // Execute this statement as part of the data structure implementation.
        System.out.println();  // Execute this statement as part of the data structure implementation.

        // 測試雙重雜湊 - Test double hashing
        System.out.println("--- 雙重雜湊 Double Hashing ---");  // Execute this statement as part of the data structure implementation.
        OpenAddressingHashTable<Integer, String> ht3 =  // Assign or update a variable that represents the current algorithm state.
            new OpenAddressingHashTable<>(7, ProbeMethod.DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.

        ht3.insert(10, "ten");  // Execute this statement as part of the data structure implementation.
        ht3.insert(20, "twenty");  // Execute this statement as part of the data structure implementation.
        ht3.insert(30, "thirty");  // Execute this statement as part of the data structure implementation.
        ht3.insert(17, "seventeen");  // Execute this statement as part of the data structure implementation.

        ht3.printStructure();  // Execute this statement as part of the data structure implementation.
        System.out.println();  // Execute this statement as part of the data structure implementation.

        // 測試刪除與墓碑標記 - Test deletion with tombstones
        System.out.println("--- 刪除與墓碑標記 Deletion with Tombstones ---");  // Execute this statement as part of the data structure implementation.
        OpenAddressingHashTable<String, Integer> ht4 =  // Assign or update a variable that represents the current algorithm state.
            new OpenAddressingHashTable<>(5, ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

        ht4.insert("a", 1);  // Execute this statement as part of the data structure implementation.
        ht4.insert("b", 2);  // Execute this statement as part of the data structure implementation.
        ht4.insert("c", 3);  // Execute this statement as part of the data structure implementation.
        System.out.println("插入後 After insertion:");  // Execute this statement as part of the data structure implementation.
        ht4.printStructure();  // Execute this statement as part of the data structure implementation.

        ht4.delete("b");  // Execute this statement as part of the data structure implementation.
        System.out.println("\n刪除 'b' 後 After deleting 'b':");  // Execute this statement as part of the data structure implementation.
        ht4.printStructure();  // Execute this statement as part of the data structure implementation.

        System.out.println("\n搜尋 'c' Search 'c': " + ht4.search("c"));  // Execute this statement as part of the data structure implementation.
        System.out.println("搜尋 'b' Search 'b': " + ht4.search("b"));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
}  // Close the current block scope.
