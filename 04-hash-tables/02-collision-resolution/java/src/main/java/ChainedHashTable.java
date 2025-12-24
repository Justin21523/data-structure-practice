/** Doc block start
 * 鏈結雜湊表（Chained Hash Table）- 使用鏈結法處理碰撞
 * Chained Hash Table - Using chaining for collision resolution
 *(blank line)
 * 當多個鍵映射到同一個桶時，使用鏈結串列來儲存所有碰撞的元素 / When multiple keys map to the same bucket, use a linked list to store all colliding elements
 *(blank line)
 * @param <K> 鍵的型別（key type）
 * @param <V> 值的型別（value type）
 */  // End of block comment
public class ChainedHashTable<K, V> {  // Execute this statement as part of the data structure implementation.

    // ========== 內部類別 Inner Classes ==========

    /** Doc block start
     * 鍵值對節點 / Key-value pair node for chaining
     */  // End of block comment
    private static class Entry<K, V> {  // Execute this statement as part of the data structure implementation.
        K key;  // Execute this statement as part of the data structure implementation.
        V value;  // Execute this statement as part of the data structure implementation.
        Entry<K, V> next;  // Execute this statement as part of the data structure implementation.

        Entry(K key, V value) {  // Execute this statement as part of the data structure implementation.
            this.key = key;  // Assign or update a variable that represents the current algorithm state.
            this.value = value;  // Assign or update a variable that represents the current algorithm state.
            this.next = null;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 探測統計資料 / Probe statistics for performance analysis
     */  // End of block comment
    public static class ProbeStats {  // Execute this statement as part of the data structure implementation.
        public int totalProbes;      // 總探測次數 - Total number of probes
        public int maxChainLength;   // 最大鏈長度 - Maximum chain length
        public int collisions;       // 碰撞次數 - Number of collisions

        public ProbeStats() {  // Execute this statement as part of the data structure implementation.
            this.totalProbes = 0;  // Assign or update a variable that represents the current algorithm state.
            this.maxChainLength = 0;  // Assign or update a variable that represents the current algorithm state.
            this.collisions = 0;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        @Override  // Execute this statement as part of the data structure implementation.
        public String toString() {  // Execute this statement as part of the data structure implementation.
            return String.format(  // Return the computed result to the caller.
                "ProbeStats{totalProbes=%d, maxChainLength=%d, collisions=%d}",  // Assign or update a variable that represents the current algorithm state.
                totalProbes, maxChainLength, collisions  // Execute this statement as part of the data structure implementation.
            );  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // ========== 常數 Constants ==========
    private static final int DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    private static final double MAX_LOAD_FACTOR = 0.75;  // Assign or update a variable that represents the current algorithm state.

    // ========== 成員變數 Member Variables ==========
    private Entry<K, V>[] buckets;  // 桶陣列 - Array of buckets
    private int capacity;            // 桶的數量 - Number of buckets
    private int size;                // 元素數量 - Number of elements
    private ProbeStats stats;        // 探測統計 - Probe statistics

    // ========== 建構子 Constructors ==========

    /** Doc block start
     * 使用預設容量建立雜湊表 / Create hash table with default capacity
     */  // End of block comment
    public ChainedHashTable() {  // Execute this statement as part of the data structure implementation.
        this(DEFAULT_CAPACITY);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    /** Doc block start
     * 使用指定容量建立雜湊表 / Create hash table with specified capacity
     *(blank line)
     * @param capacity 桶的數量（number of buckets）
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    public ChainedHashTable(int capacity) {  // Execute this statement as part of the data structure implementation.
        if (capacity <= 0) {  // Evaluate the condition and branch into the appropriate code path.
            throw new IllegalArgumentException(  // Throw an exception to signal an invalid argument or operation.
                "容量必須為正整數 / Capacity must be positive");  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
        this.capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
        this.size = 0;  // Assign or update a variable that represents the current algorithm state.
        this.buckets = (Entry<K, V>[]) new Entry[capacity];  // Access or update the bucket storage used to hold entries or chains.
        this.stats = new ProbeStats();  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    // ========== 私有方法 Private Methods ==========

    /** Doc block start
     * 計算雜湊索引 / Compute hash index
     *(blank line)
     * @param key 要雜湊的鍵
     * @return 桶的索引
     */  // End of block comment
    private int hash(K key) {  // Compute a hash-based index so keys map into the table's storage.
        if (key == null) {  // Evaluate the condition and branch into the appropriate code path.
            return 0;  // Return the computed result to the caller.
        }  // Close the current block scope.
        // 確保雜湊值為正數 - Ensure positive hash value
        return Math.abs(key.hashCode()) % capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 擴容並重新雜湊 / Resize and rehash
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    private void rehash() {  // Rehash entries into a larger table to keep operations near O(1) on average.
        Entry<K, V>[] oldBuckets = buckets;  // Access or update the bucket storage used to hold entries or chains.
        capacity *= 2;  // Assign or update a variable that represents the current algorithm state.
        buckets = (Entry<K, V>[]) new Entry[capacity];  // Access or update the bucket storage used to hold entries or chains.
        size = 0;  // Assign or update a variable that represents the current algorithm state.

        // 保留舊的統計資料，但重置鏈長度 / Keep old stats but reset chain length
        stats.maxChainLength = 0;  // Assign or update a variable that represents the current algorithm state.

        // 重新插入所有元素 - Reinsert all elements
        for (Entry<K, V> head : oldBuckets) {  // Iterate over a range/collection to process each item in sequence.
            Entry<K, V> current = head;  // Assign or update a variable that represents the current algorithm state.
            while (current != null) {  // Repeat while the loop condition remains true.
                insert(current.key, current.value);  // Execute this statement as part of the data structure implementation.
                current = current.next;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 計算指定桶的鏈長度 / Calculate chain length for a specific bucket
     *(blank line)
     * @param index 桶的索引
     * @return 鏈的長度
     */  // End of block comment
    private int getChainLength(int index) {  // Execute this statement as part of the data structure implementation.
        int length = 0;  // Assign or update a variable that represents the current algorithm state.
        Entry<K, V> current = buckets[index];  // Access or update the bucket storage used to hold entries or chains.
        while (current != null) {  // Repeat while the loop condition remains true.
            length++;  // Execute this statement as part of the data structure implementation.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
        return length;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 更新最大鏈長度統計 / Update maximum chain length statistics
     */  // End of block comment
    private void updateMaxChainLength() {  // Execute this statement as part of the data structure implementation.
        int maxLength = 0;  // Assign or update a variable that represents the current algorithm state.
        for (int i = 0; i < capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            int length = getChainLength(i);  // Assign or update a variable that represents the current algorithm state.
            if (length > maxLength) {  // Evaluate the condition and branch into the appropriate code path.
                maxLength = length;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        }  // Close the current block scope.
        stats.maxChainLength = maxLength;  // Assign or update a variable that represents the current algorithm state.
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
     */  // End of block comment
    public void insert(K key, V value) {  // Execute this statement as part of the data structure implementation.
        int index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
        Entry<K, V> head = buckets[index];  // Access or update the bucket storage used to hold entries or chains.

        int probes = 0;  // Advance or track the probing sequence used by open addressing.

        // 檢查 key 是否已存在 - Check if key exists
        Entry<K, V> current = head;  // Assign or update a variable that represents the current algorithm state.
        while (current != null) {  // Repeat while the loop condition remains true.
            probes++;  // Advance or track the probing sequence used by open addressing.
            if (current.key == null ? key == null : current.key.equals(key)) {  // Evaluate the condition and branch into the appropriate code path.
                current.value = value;  // 更新 - Update existing
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                return;  // Return the computed result to the caller.
            }  // Close the current block scope.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // 新增節點到鏈結串列頭部 - Add new node at head
        Entry<K, V> newEntry = new Entry<>(key, value);  // Assign or update a variable that represents the current algorithm state.
        newEntry.next = head;  // Assign or update a variable that represents the current algorithm state.
        buckets[index] = newEntry;  // Access or update the bucket storage used to hold entries or chains.
        size++;  // Execute this statement as part of the data structure implementation.

        // 更新統計 - Update statistics
        if (head != null) {  // Evaluate the condition and branch into the appropriate code path.
            stats.collisions++;  // 發生碰撞 - Collision occurred
        }  // Close the current block scope.
        stats.totalProbes += probes + 1;  // +1 for the insertion

        int chainLength = getChainLength(index);  // Assign or update a variable that represents the current algorithm state.
        if (chainLength > stats.maxChainLength) {  // Evaluate the condition and branch into the appropriate code path.
            stats.maxChainLength = chainLength;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // 檢查是否需要擴容 - Check if rehashing needed
        if (getLoadFactor() > MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
            rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
        }  // Close the current block scope.
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
        int index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
        Entry<K, V> current = buckets[index];  // Access or update the bucket storage used to hold entries or chains.

        int probes = 0;  // Advance or track the probing sequence used by open addressing.

        while (current != null) {  // Repeat while the loop condition remains true.
            probes++;  // Advance or track the probing sequence used by open addressing.
            if (current.key == null ? key == null : current.key.equals(key)) {  // Evaluate the condition and branch into the appropriate code path.
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                return current.value;  // Return the computed result to the caller.
            }  // Close the current block scope.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        return null;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 刪除指定的鍵值對 / Delete key-value pair
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     */  // End of block comment
    public boolean delete(K key) {  // Execute this statement as part of the data structure implementation.
        int index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
        Entry<K, V> current = buckets[index];  // Access or update the bucket storage used to hold entries or chains.
        Entry<K, V> prev = null;  // Assign or update a variable that represents the current algorithm state.

        int probes = 0;  // Advance or track the probing sequence used by open addressing.

        while (current != null) {  // Repeat while the loop condition remains true.
            probes++;  // Advance or track the probing sequence used by open addressing.
            if (current.key == null ? key == null : current.key.equals(key)) {  // Evaluate the condition and branch into the appropriate code path.
                if (prev == null) {  // Evaluate the condition and branch into the appropriate code path.
                    buckets[index] = current.next;  // Access or update the bucket storage used to hold entries or chains.
                } else {  // Handle the alternative branch when the condition is false.
                    prev.next = current.next;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.
                size--;  // Execute this statement as part of the data structure implementation.
                stats.totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                updateMaxChainLength();  // Execute this statement as part of the data structure implementation.
                return true;  // Return the computed result to the caller.
            }  // Close the current block scope.
            prev = current;  // Assign or update a variable that represents the current algorithm state.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
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
        return search(key) != null || (key == null && containsNullKey());  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查是否包含 null 鍵
     * Check if contains null key
     */  // End of block comment
    private boolean containsNullKey() {  // Execute this statement as part of the data structure implementation.
        Entry<K, V> current = buckets[0];  // Access or update the bucket storage used to hold entries or chains.
        while (current != null) {  // Repeat while the loop condition remains true.
            if (current.key == null) {  // Evaluate the condition and branch into the appropriate code path.
                return true;  // Return the computed result to the caller.
            }  // Close the current block scope.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
        return false;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳元素數量 / Return number of elements
     */  // End of block comment
    public int size() {  // Execute this statement as part of the data structure implementation.
        return size;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳桶的數量 / Return number of buckets
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
     * 負載因子 = 元素數量 / 桶數量 / Load factor = number of elements / number of buckets
     *(blank line)
     * @return 負載因子
     */  // End of block comment
    public double getLoadFactor() {  // Execute this statement as part of the data structure implementation.
        return (double) size / capacity;  // Return the computed result to the caller.
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
        updateMaxChainLength();  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    public void clear() {  // Execute this statement as part of the data structure implementation.
        buckets = (Entry<K, V>[]) new Entry[capacity];  // Access or update the bucket storage used to hold entries or chains.
        size = 0;  // Assign or update a variable that represents the current algorithm state.
        stats = new ProbeStats();  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    /** Doc block start
     * 字串表示 / String representation
     */  // End of block comment
    @Override  // Execute this statement as part of the data structure implementation.
    public String toString() {  // Execute this statement as part of the data structure implementation.
        StringBuilder sb = new StringBuilder("ChainedHashTable{");  // Assign or update a variable that represents the current algorithm state.
        boolean first = true;  // Assign or update a variable that represents the current algorithm state.

        for (Entry<K, V> head : buckets) {  // Iterate over a range/collection to process each item in sequence.
            Entry<K, V> current = head;  // Assign or update a variable that represents the current algorithm state.
            while (current != null) {  // Repeat while the loop condition remains true.
                if (!first) {  // Evaluate the condition and branch into the appropriate code path.
                    sb.append(", ");  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
                sb.append(current.key).append("=").append(current.value);  // Assign or update a variable that represents the current algorithm state.
                first = false;  // Assign or update a variable that represents the current algorithm state.
                current = current.next;  // Assign or update a variable that represents the current algorithm state.
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
        for (int i = 0; i < capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            System.out.printf("Bucket[%d]: ", i);  // Execute this statement as part of the data structure implementation.
            Entry<K, V> current = buckets[i];  // Access or update the bucket storage used to hold entries or chains.
            if (current == null) {  // Evaluate the condition and branch into the appropriate code path.
                System.out.println("empty");  // Execute this statement as part of the data structure implementation.
            } else {  // Handle the alternative branch when the condition is false.
                while (current != null) {  // Repeat while the loop condition remains true.
                    System.out.printf("%s->%s ", current.key, current.value);  // Execute this statement as part of the data structure implementation.
                    current = current.next;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.
                System.out.println();  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
        System.out.println("負載因子 Load Factor: " + getLoadFactor());  // Execute this statement as part of the data structure implementation.
        System.out.println("統計資料 Statistics: " + stats);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // ========== 範例執行 Example Main ==========

    public static void main(String[] args) {  // Execute this statement as part of the data structure implementation.
        System.out.println("=== 鏈結雜湊表示範 Chained Hash Table Demo ===\n");  // Execute this statement as part of the data structure implementation.

        // 建立雜湊表 - Create hash table with small capacity to show collisions
        ChainedHashTable<String, Integer> ht = new ChainedHashTable<>(4);  // Assign or update a variable that represents the current algorithm state.

        // 插入操作 - Insert operations
        System.out.println("插入鍵值對 Inserting key-value pairs:");  // Execute this statement as part of the data structure implementation.
        ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
        ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.
        ht.insert("cherry", 300);  // Execute this statement as part of the data structure implementation.
        ht.insert("date", 400);  // Execute this statement as part of the data structure implementation.
        ht.insert("elderberry", 500);  // Execute this statement as part of the data structure implementation.
        System.out.println("  雜湊表: " + ht);  // Execute this statement as part of the data structure implementation.
        System.out.printf("  大小: %d, 負載因子: %.3f%n", ht.size(), ht.getLoadFactor());  // Execute this statement as part of the data structure implementation.
        System.out.println("  統計: " + ht.getProbeStats() + "\n");  // Execute this statement as part of the data structure implementation.

        // 印出內部結構 - Print internal structure
        ht.printStructure();  // Execute this statement as part of the data structure implementation.
        System.out.println();  // Execute this statement as part of the data structure implementation.

        // 搜尋操作 - Search operations
        System.out.println("搜尋操作 Search operations:");  // Execute this statement as part of the data structure implementation.
        System.out.println("  search('apple') = " + ht.search("apple"));  // Assign or update a variable that represents the current algorithm state.
        System.out.println("  search('grape') = " + ht.search("grape"));  // Assign or update a variable that represents the current algorithm state.
        System.out.println("  統計: " + ht.getProbeStats() + "\n");  // Execute this statement as part of the data structure implementation.

        // 刪除操作 - Delete operation
        System.out.println("刪除操作 Delete operations:");  // Execute this statement as part of the data structure implementation.
        ht.delete("banana");  // Execute this statement as part of the data structure implementation.
        System.out.println("  刪除 'banana' 後: " + ht);  // Execute this statement as part of the data structure implementation.
        System.out.println("  統計: " + ht.getProbeStats() + "\n");  // Execute this statement as part of the data structure implementation.

        // 碰撞測試 - Collision test
        System.out.println("碰撞測試 Collision test:");  // Execute this statement as part of the data structure implementation.
        ChainedHashTable<Integer, String> ht2 = new ChainedHashTable<>(3);  // Assign or update a variable that represents the current algorithm state.
        ht2.insert(1, "one");  // Execute this statement as part of the data structure implementation.
        ht2.insert(4, "four");   // 4 % 3 = 1, will collide with key 1
        ht2.insert(7, "seven");  // 7 % 3 = 1, will collide with keys 1 and 4
        ht2.printStructure();  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
}  // Close the current block scope.
