/**
 * 開放定址雜湊表（Open Addressing Hash Table）
 * Open Addressing Hash Table with multiple probing strategies
 *
 * 當發生碰撞時，使用探測序列在表中尋找下一個空位
 * When collision occurs, use a probe sequence to find the next empty slot
 *
 * 支援三種探測方法：
 * Supports three probing methods:
 * 1. 線性探測（Linear Probing）: h(k, i) = (h(k) + i) mod m
 * 2. 二次探測（Quadratic Probing）: h(k, i) = (h(k) + c1*i + c2*i^2) mod m
 * 3. 雙重雜湊（Double Hashing）: h(k, i) = (h1(k) + i*h2(k)) mod m
 *
 * @param <K> 鍵的型別（key type）
 * @param <V> 值的型別（value type）
 */
public class OpenAddressingHashTable<K, V> {

    // ========== 列舉型別 Enumerations ==========

    /**
     * 探測方法
     * Probe method for collision resolution
     */
    public enum ProbeMethod {
        LINEAR,       // 線性探測 - Linear probing
        QUADRATIC,    // 二次探測 - Quadratic probing
        DOUBLE_HASH   // 雙重雜湊 - Double hashing
    }

    // ========== 內部類別 Inner Classes ==========

    /**
     * 表格項目
     * Table entry storing key-value pair
     */
    private static class Entry<K, V> {
        K key;
        V value;
        boolean isDeleted;  // 墓碑標記 - Tombstone marker

        Entry(K key, V value) {
            this.key = key;
            this.value = value;
            this.isDeleted = false;
        }
    }

    /**
     * 探測統計資料
     * Probe statistics for performance analysis
     */
    public static class ProbeStats {
        public int totalProbes;      // 總探測次數 - Total number of probes
        public int maxProbes;        // 單次操作最大探測次數 - Max probes in single operation
        public int collisions;       // 碰撞次數 - Number of collisions

        public ProbeStats() {
            this.totalProbes = 0;
            this.maxProbes = 0;
            this.collisions = 0;
        }

        @Override
        public String toString() {
            return String.format(
                "ProbeStats{totalProbes=%d, maxProbes=%d, collisions=%d}",
                totalProbes, maxProbes, collisions
            );
        }
    }

    // ========== 常數 Constants ==========
    private static final int DEFAULT_CAPACITY = 16;
    private static final double MAX_LOAD_FACTOR = 0.5;  // 開放定址法建議較低的負載因子

    // 二次探測的常數 - Quadratic probing constants
    private static final int C1 = 1;
    private static final int C2 = 3;

    // ========== 成員變數 Member Variables ==========
    private Entry<K, V>[] table;     // 雜湊表陣列 - Hash table array
    private int capacity;             // 表格容量 - Table capacity
    private int size;                 // 元素數量 - Number of elements
    private int deletedCount;         // 已刪除項目數量 - Number of deleted entries
    private ProbeMethod probeMethod;  // 探測方法 - Probing method
    private ProbeStats stats;         // 探測統計 - Probe statistics

    // ========== 建構子 Constructors ==========

    /**
     * 使用預設容量和線性探測建立雜湊表
     * Create hash table with default capacity and linear probing
     */
    public OpenAddressingHashTable() {
        this(DEFAULT_CAPACITY, ProbeMethod.LINEAR);
    }

    /**
     * 使用指定探測方法建立雜湊表
     * Create hash table with specified probe method
     *
     * @param probeMethod 探測方法
     */
    public OpenAddressingHashTable(ProbeMethod probeMethod) {
        this(DEFAULT_CAPACITY, probeMethod);
    }

    /**
     * 使用指定容量和探測方法建立雜湊表
     * Create hash table with specified capacity and probe method
     *
     * @param capacity 表格容量
     * @param probeMethod 探測方法
     */
    @SuppressWarnings("unchecked")
    public OpenAddressingHashTable(int capacity, ProbeMethod probeMethod) {
        if (capacity <= 0) {
            throw new IllegalArgumentException(
                "容量必須為正整數 / Capacity must be positive");
        }
        this.capacity = capacity;
        this.size = 0;
        this.deletedCount = 0;
        this.probeMethod = probeMethod;
        this.table = (Entry<K, V>[]) new Entry[capacity];
        this.stats = new ProbeStats();
    }

    // ========== 私有方法 Private Methods ==========

    /**
     * 主雜湊函數
     * Primary hash function
     *
     * @param key 要雜湊的鍵
     * @return 雜湊索引
     */
    private int hash1(K key) {
        if (key == null) {
            return 0;
        }
        return Math.abs(key.hashCode()) % capacity;
    }

    /**
     * 次要雜湊函數（用於雙重雜湊）
     * Secondary hash function (for double hashing)
     *
     * @param key 要雜湊的鍵
     * @return 次要雜湊值
     */
    private int hash2(K key) {
        if (key == null) {
            return 1;
        }
        // 確保結果為正數且不為 0
        // Ensure result is positive and non-zero
        int h = Math.abs(key.hashCode());
        return 1 + (h % (capacity - 1));
    }

    /**
     * 計算探測序列的索引
     * Calculate probe sequence index
     *
     * @param key 鍵
     * @param i 探測次數（0, 1, 2, ...）
     * @return 探測索引
     */
    private int probe(K key, int i) {
        int h1 = hash1(key);

        switch (probeMethod) {
            case LINEAR:
                // 線性探測: h(k, i) = (h(k) + i) mod m
                // Linear probing: h(k, i) = (h(k) + i) mod m
                return (h1 + i) % capacity;

            case QUADRATIC:
                // 二次探測: h(k, i) = (h(k) + c1*i + c2*i^2) mod m
                // Quadratic probing: h(k, i) = (h(k) + c1*i + c2*i^2) mod m
                return (h1 + C1 * i + C2 * i * i) % capacity;

            case DOUBLE_HASH:
                // 雙重雜湊: h(k, i) = (h1(k) + i*h2(k)) mod m
                // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
                int h2 = hash2(key);
                return (h1 + i * h2) % capacity;

            default:
                return h1;
        }
    }

    /**
     * 擴容並重新雜湊
     * Resize and rehash
     */
    @SuppressWarnings("unchecked")
    private void rehash() {
        Entry<K, V>[] oldTable = table;
        capacity *= 2;
        table = (Entry<K, V>[]) new Entry[capacity];
        size = 0;
        deletedCount = 0;

        // 重新插入所有元素 - Reinsert all elements
        for (Entry<K, V> entry : oldTable) {
            if (entry != null && !entry.isDeleted) {
                insert(entry.key, entry.value);
            }
        }
    }

    /**
     * 檢查鍵是否相等
     * Check if keys are equal
     */
    private boolean keysEqual(K k1, K k2) {
        return k1 == null ? k2 == null : k1.equals(k2);
    }

    // ========== 公開方法 Public Methods ==========

    /**
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param key 鍵
     * @param value 值
     * @return 插入成功回傳 true，表格已滿回傳 false
     */
    public boolean insert(K key, V value) {
        // 檢查負載因子 - Check load factor
        if (getLoadFactor() >= MAX_LOAD_FACTOR) {
            rehash();
        }

        int probes = 0;
        int firstDeletedIndex = -1;  // 記錄第一個遇到的已刪除位置

        for (int i = 0; i < capacity; i++) {
            int index = probe(key, i);
            Entry<K, V> entry = table[index];

            probes++;

            if (entry == null) {
                // 找到空位 - Found empty slot
                int insertIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : index;
                table[insertIndex] = new Entry<>(key, value);

                if (firstDeletedIndex != -1) {
                    deletedCount--;  // 重用已刪除的位置
                }

                size++;
                stats.totalProbes += probes;
                if (probes > stats.maxProbes) {
                    stats.maxProbes = probes;
                }
                if (i > 0) {
                    stats.collisions++;  // 發生碰撞 - Collision occurred
                }
                return true;

            } else if (entry.isDeleted) {
                // 記錄第一個已刪除位置 - Record first deleted position
                if (firstDeletedIndex == -1) {
                    firstDeletedIndex = index;
                }

            } else if (keysEqual(entry.key, key)) {
                // 鍵已存在，更新值 - Key exists, update value
                entry.value = value;
                stats.totalProbes += probes;
                return true;
            }
        }

        // 表格已滿 - Table is full
        stats.totalProbes += probes;
        return false;
    }

    /**
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param key 要搜尋的鍵
     * @return 找到則回傳 value，否則回傳 null
     */
    public V search(K key) {
        int probes = 0;

        for (int i = 0; i < capacity; i++) {
            int index = probe(key, i);
            Entry<K, V> entry = table[index];

            probes++;

            if (entry == null) {
                // 遇到空位，表示 key 不存在 - Empty slot means key doesn't exist
                stats.totalProbes += probes;
                return null;

            } else if (!entry.isDeleted && keysEqual(entry.key, key)) {
                // 找到 key - Found key
                stats.totalProbes += probes;
                if (probes > stats.maxProbes) {
                    stats.maxProbes = probes;
                }
                return entry.value;
            }

            // 繼續探測（可能是已刪除項目或其他鍵）
            // Continue probing (could be deleted entry or other key)
        }

        stats.totalProbes += probes;
        return null;
    }

    /**
     * 刪除指定的鍵值對（使用墓碑標記）
     * Delete key-value pair (using tombstone marker)
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     */
    public boolean delete(K key) {
        int probes = 0;

        for (int i = 0; i < capacity; i++) {
            int index = probe(key, i);
            Entry<K, V> entry = table[index];

            probes++;

            if (entry == null) {
                // 遇到空位，表示 key 不存在 - Empty slot means key doesn't exist
                stats.totalProbes += probes;
                return false;

            } else if (!entry.isDeleted && keysEqual(entry.key, key)) {
                // 找到 key，標記為已刪除 - Found key, mark as deleted
                entry.isDeleted = true;
                size--;
                deletedCount++;
                stats.totalProbes += probes;
                if (probes > stats.maxProbes) {
                    stats.maxProbes = probes;
                }

                // 如果已刪除項目過多，考慮重新雜湊
                // Consider rehashing if too many deleted entries
                if (deletedCount > capacity / 4) {
                    rehash();
                }

                return true;
            }

            // 繼續探測 - Continue probing
        }

        stats.totalProbes += probes;
        return false;
    }

    /**
     * 檢查 key 是否存在
     * Check if key exists
     *
     * @param key 要檢查的鍵
     * @return 存在回傳 true，否則回傳 false
     */
    public boolean contains(K key) {
        return search(key) != null;
    }

    /**
     * 回傳元素數量
     * Return number of elements
     */
    public int size() {
        return size;
    }

    /**
     * 回傳表格容量
     * Return table capacity
     */
    public int capacity() {
        return capacity;
    }

    /**
     * 檢查是否為空
     * Check if empty
     */
    public boolean isEmpty() {
        return size == 0;
    }

    /**
     * 計算負載因子
     * Calculate load factor
     *
     * 負載因子 = (元素數量 + 已刪除項目數量) / 容量
     * Load factor = (number of elements + deleted entries) / capacity
     *
     * @return 負載因子
     */
    public double getLoadFactor() {
        return (double) (size + deletedCount) / capacity;
    }

    /**
     * 取得探測統計資料
     * Get probe statistics
     *
     * @return 探測統計資料
     */
    public ProbeStats getProbeStats() {
        return stats;
    }

    /**
     * 重置探測統計
     * Reset probe statistics
     */
    public void resetStats() {
        stats = new ProbeStats();
    }

    /**
     * 取得探測方法
     * Get probe method
     */
    public ProbeMethod getProbeMethod() {
        return probeMethod;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    @SuppressWarnings("unchecked")
    public void clear() {
        table = (Entry<K, V>[]) new Entry[capacity];
        size = 0;
        deletedCount = 0;
        stats = new ProbeStats();
    }

    /**
     * 字串表示
     * String representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("OpenAddressingHashTable{");
        boolean first = true;

        for (Entry<K, V> entry : table) {
            if (entry != null && !entry.isDeleted) {
                if (!first) {
                    sb.append(", ");
                }
                sb.append(entry.key).append("=").append(entry.value);
                first = false;
            }
        }

        sb.append("}");
        return sb.toString();
    }

    /**
     * 印出雜湊表內部結構（用於除錯）
     * Print internal structure (for debugging)
     */
    public void printStructure() {
        System.out.println("=== 雜湊表內部結構 Hash Table Structure ===");
        System.out.println("探測方法 Probe Method: " + probeMethod);

        for (int i = 0; i < capacity; i++) {
            Entry<K, V> entry = table[i];
            System.out.printf("Table[%d]: ", i);

            if (entry == null) {
                System.out.println("empty");
            } else if (entry.isDeleted) {
                System.out.println("DELETED (tombstone)");
            } else {
                System.out.printf("%s->%s%n", entry.key, entry.value);
            }
        }

        System.out.println("大小 Size: " + size);
        System.out.println("已刪除 Deleted: " + deletedCount);
        System.out.println("負載因子 Load Factor: " + getLoadFactor());
        System.out.println("統計資料 Statistics: " + stats);
    }

    // ========== 範例執行 Example Main ==========

    public static void main(String[] args) {
        System.out.println("=== 開放定址雜湊表示範 Open Addressing Hash Table Demo ===\n");

        // 測試線性探測 - Test linear probing
        System.out.println("--- 線性探測 Linear Probing ---");
        OpenAddressingHashTable<Integer, String> ht1 =
            new OpenAddressingHashTable<>(7, ProbeMethod.LINEAR);

        ht1.insert(10, "ten");
        ht1.insert(20, "twenty");
        ht1.insert(30, "thirty");
        ht1.insert(17, "seventeen");  // Will collide with 10 (10 % 7 = 3, 17 % 7 = 3)

        ht1.printStructure();
        System.out.println();

        // 測試二次探測 - Test quadratic probing
        System.out.println("--- 二次探測 Quadratic Probing ---");
        OpenAddressingHashTable<Integer, String> ht2 =
            new OpenAddressingHashTable<>(7, ProbeMethod.QUADRATIC);

        ht2.insert(10, "ten");
        ht2.insert(20, "twenty");
        ht2.insert(30, "thirty");
        ht2.insert(17, "seventeen");

        ht2.printStructure();
        System.out.println();

        // 測試雙重雜湊 - Test double hashing
        System.out.println("--- 雙重雜湊 Double Hashing ---");
        OpenAddressingHashTable<Integer, String> ht3 =
            new OpenAddressingHashTable<>(7, ProbeMethod.DOUBLE_HASH);

        ht3.insert(10, "ten");
        ht3.insert(20, "twenty");
        ht3.insert(30, "thirty");
        ht3.insert(17, "seventeen");

        ht3.printStructure();
        System.out.println();

        // 測試刪除與墓碑標記 - Test deletion with tombstones
        System.out.println("--- 刪除與墓碑標記 Deletion with Tombstones ---");
        OpenAddressingHashTable<String, Integer> ht4 =
            new OpenAddressingHashTable<>(5, ProbeMethod.LINEAR);

        ht4.insert("a", 1);
        ht4.insert("b", 2);
        ht4.insert("c", 3);
        System.out.println("插入後 After insertion:");
        ht4.printStructure();

        ht4.delete("b");
        System.out.println("\n刪除 'b' 後 After deleting 'b':");
        ht4.printStructure();

        System.out.println("\n搜尋 'c' Search 'c': " + ht4.search("c"));
        System.out.println("搜尋 'b' Search 'b': " + ht4.search("b"));
    }
}
