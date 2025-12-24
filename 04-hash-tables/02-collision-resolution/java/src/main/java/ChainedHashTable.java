/**
 * 鏈結雜湊表（Chained Hash Table）- 使用鏈結法處理碰撞
 * Chained Hash Table - Using chaining for collision resolution
 *
 * 當多個鍵映射到同一個桶時，使用鏈結串列來儲存所有碰撞的元素
 * When multiple keys map to the same bucket, use a linked list to store all colliding elements
 *
 * @param <K> 鍵的型別（key type）
 * @param <V> 值的型別（value type）
 */
public class ChainedHashTable<K, V> {

    // ========== 內部類別 Inner Classes ==========

    /**
     * 鍵值對節點
     * Key-value pair node for chaining
     */
    private static class Entry<K, V> {
        K key;
        V value;
        Entry<K, V> next;

        Entry(K key, V value) {
            this.key = key;
            this.value = value;
            this.next = null;
        }
    }

    /**
     * 探測統計資料
     * Probe statistics for performance analysis
     */
    public static class ProbeStats {
        public int totalProbes;      // 總探測次數 - Total number of probes
        public int maxChainLength;   // 最大鏈長度 - Maximum chain length
        public int collisions;       // 碰撞次數 - Number of collisions

        public ProbeStats() {
            this.totalProbes = 0;
            this.maxChainLength = 0;
            this.collisions = 0;
        }

        @Override
        public String toString() {
            return String.format(
                "ProbeStats{totalProbes=%d, maxChainLength=%d, collisions=%d}",
                totalProbes, maxChainLength, collisions
            );
        }
    }

    // ========== 常數 Constants ==========
    private static final int DEFAULT_CAPACITY = 16;
    private static final double MAX_LOAD_FACTOR = 0.75;

    // ========== 成員變數 Member Variables ==========
    private Entry<K, V>[] buckets;  // 桶陣列 - Array of buckets
    private int capacity;            // 桶的數量 - Number of buckets
    private int size;                // 元素數量 - Number of elements
    private ProbeStats stats;        // 探測統計 - Probe statistics

    // ========== 建構子 Constructors ==========

    /**
     * 使用預設容量建立雜湊表
     * Create hash table with default capacity
     */
    public ChainedHashTable() {
        this(DEFAULT_CAPACITY);
    }

    /**
     * 使用指定容量建立雜湊表
     * Create hash table with specified capacity
     *
     * @param capacity 桶的數量（number of buckets）
     */
    @SuppressWarnings("unchecked")
    public ChainedHashTable(int capacity) {
        if (capacity <= 0) {
            throw new IllegalArgumentException(
                "容量必須為正整數 / Capacity must be positive");
        }
        this.capacity = capacity;
        this.size = 0;
        this.buckets = (Entry<K, V>[]) new Entry[capacity];
        this.stats = new ProbeStats();
    }

    // ========== 私有方法 Private Methods ==========

    /**
     * 計算雜湊索引
     * Compute hash index
     *
     * @param key 要雜湊的鍵
     * @return 桶的索引
     */
    private int hash(K key) {
        if (key == null) {
            return 0;
        }
        // 確保雜湊值為正數 - Ensure positive hash value
        return Math.abs(key.hashCode()) % capacity;
    }

    /**
     * 擴容並重新雜湊
     * Resize and rehash
     */
    @SuppressWarnings("unchecked")
    private void rehash() {
        Entry<K, V>[] oldBuckets = buckets;
        capacity *= 2;
        buckets = (Entry<K, V>[]) new Entry[capacity];
        size = 0;

        // 保留舊的統計資料，但重置鏈長度
        // Keep old stats but reset chain length
        stats.maxChainLength = 0;

        // 重新插入所有元素 - Reinsert all elements
        for (Entry<K, V> head : oldBuckets) {
            Entry<K, V> current = head;
            while (current != null) {
                insert(current.key, current.value);
                current = current.next;
            }
        }
    }

    /**
     * 計算指定桶的鏈長度
     * Calculate chain length for a specific bucket
     *
     * @param index 桶的索引
     * @return 鏈的長度
     */
    private int getChainLength(int index) {
        int length = 0;
        Entry<K, V> current = buckets[index];
        while (current != null) {
            length++;
            current = current.next;
        }
        return length;
    }

    /**
     * 更新最大鏈長度統計
     * Update maximum chain length statistics
     */
    private void updateMaxChainLength() {
        int maxLength = 0;
        for (int i = 0; i < capacity; i++) {
            int length = getChainLength(i);
            if (length > maxLength) {
                maxLength = length;
            }
        }
        stats.maxChainLength = maxLength;
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
     */
    public void insert(K key, V value) {
        int index = hash(key);
        Entry<K, V> head = buckets[index];

        int probes = 0;

        // 檢查 key 是否已存在 - Check if key exists
        Entry<K, V> current = head;
        while (current != null) {
            probes++;
            if (current.key == null ? key == null : current.key.equals(key)) {
                current.value = value;  // 更新 - Update existing
                stats.totalProbes += probes;
                return;
            }
            current = current.next;
        }

        // 新增節點到鏈結串列頭部 - Add new node at head
        Entry<K, V> newEntry = new Entry<>(key, value);
        newEntry.next = head;
        buckets[index] = newEntry;
        size++;

        // 更新統計 - Update statistics
        if (head != null) {
            stats.collisions++;  // 發生碰撞 - Collision occurred
        }
        stats.totalProbes += probes + 1;  // +1 for the insertion

        int chainLength = getChainLength(index);
        if (chainLength > stats.maxChainLength) {
            stats.maxChainLength = chainLength;
        }

        // 檢查是否需要擴容 - Check if rehashing needed
        if (getLoadFactor() > MAX_LOAD_FACTOR) {
            rehash();
        }
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
        int index = hash(key);
        Entry<K, V> current = buckets[index];

        int probes = 0;

        while (current != null) {
            probes++;
            if (current.key == null ? key == null : current.key.equals(key)) {
                stats.totalProbes += probes;
                return current.value;
            }
            current = current.next;
        }

        stats.totalProbes += probes;
        return null;
    }

    /**
     * 刪除指定的鍵值對
     * Delete key-value pair
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     */
    public boolean delete(K key) {
        int index = hash(key);
        Entry<K, V> current = buckets[index];
        Entry<K, V> prev = null;

        int probes = 0;

        while (current != null) {
            probes++;
            if (current.key == null ? key == null : current.key.equals(key)) {
                if (prev == null) {
                    buckets[index] = current.next;
                } else {
                    prev.next = current.next;
                }
                size--;
                stats.totalProbes += probes;
                updateMaxChainLength();
                return true;
            }
            prev = current;
            current = current.next;
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
        return search(key) != null || (key == null && containsNullKey());
    }

    /**
     * 檢查是否包含 null 鍵
     * Check if contains null key
     */
    private boolean containsNullKey() {
        Entry<K, V> current = buckets[0];
        while (current != null) {
            if (current.key == null) {
                return true;
            }
            current = current.next;
        }
        return false;
    }

    /**
     * 回傳元素數量
     * Return number of elements
     */
    public int size() {
        return size;
    }

    /**
     * 回傳桶的數量
     * Return number of buckets
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
     * 負載因子 = 元素數量 / 桶數量
     * Load factor = number of elements / number of buckets
     *
     * @return 負載因子
     */
    public double getLoadFactor() {
        return (double) size / capacity;
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
        updateMaxChainLength();
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    @SuppressWarnings("unchecked")
    public void clear() {
        buckets = (Entry<K, V>[]) new Entry[capacity];
        size = 0;
        stats = new ProbeStats();
    }

    /**
     * 字串表示
     * String representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("ChainedHashTable{");
        boolean first = true;

        for (Entry<K, V> head : buckets) {
            Entry<K, V> current = head;
            while (current != null) {
                if (!first) {
                    sb.append(", ");
                }
                sb.append(current.key).append("=").append(current.value);
                first = false;
                current = current.next;
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
        for (int i = 0; i < capacity; i++) {
            System.out.printf("Bucket[%d]: ", i);
            Entry<K, V> current = buckets[i];
            if (current == null) {
                System.out.println("empty");
            } else {
                while (current != null) {
                    System.out.printf("%s->%s ", current.key, current.value);
                    current = current.next;
                }
                System.out.println();
            }
        }
        System.out.println("負載因子 Load Factor: " + getLoadFactor());
        System.out.println("統計資料 Statistics: " + stats);
    }

    // ========== 範例執行 Example Main ==========

    public static void main(String[] args) {
        System.out.println("=== 鏈結雜湊表示範 Chained Hash Table Demo ===\n");

        // 建立雜湊表 - Create hash table with small capacity to show collisions
        ChainedHashTable<String, Integer> ht = new ChainedHashTable<>(4);

        // 插入操作 - Insert operations
        System.out.println("插入鍵值對 Inserting key-value pairs:");
        ht.insert("apple", 100);
        ht.insert("banana", 200);
        ht.insert("cherry", 300);
        ht.insert("date", 400);
        ht.insert("elderberry", 500);
        System.out.println("  雜湊表: " + ht);
        System.out.printf("  大小: %d, 負載因子: %.3f%n", ht.size(), ht.getLoadFactor());
        System.out.println("  統計: " + ht.getProbeStats() + "\n");

        // 印出內部結構 - Print internal structure
        ht.printStructure();
        System.out.println();

        // 搜尋操作 - Search operations
        System.out.println("搜尋操作 Search operations:");
        System.out.println("  search('apple') = " + ht.search("apple"));
        System.out.println("  search('grape') = " + ht.search("grape"));
        System.out.println("  統計: " + ht.getProbeStats() + "\n");

        // 刪除操作 - Delete operation
        System.out.println("刪除操作 Delete operations:");
        ht.delete("banana");
        System.out.println("  刪除 'banana' 後: " + ht);
        System.out.println("  統計: " + ht.getProbeStats() + "\n");

        // 碰撞測試 - Collision test
        System.out.println("碰撞測試 Collision test:");
        ChainedHashTable<Integer, String> ht2 = new ChainedHashTable<>(3);
        ht2.insert(1, "one");
        ht2.insert(4, "four");   // 4 % 3 = 1, will collide with key 1
        ht2.insert(7, "seven");  // 7 % 3 = 1, will collide with keys 1 and 4
        ht2.printStructure();
    }
}
