/** Doc block start
 * 雜湊表（Hash Table）- Java 實作
 * 使用鏈結法（chaining）處理碰撞
 *(blank line)
 * Hash Table implementation using chaining for collision resolution
 *(blank line)
 * @param <K> 鍵的型別（key type）
 * @param <V> 值的型別（value type）
 */  // End of block comment
public class HashTable<K, V> {  // Execute this statement as part of the data structure implementation.

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

    // ========== 常數 Constants ==========
    private static final int DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    private static final double MAX_LOAD_FACTOR = 0.75;  // Assign or update a variable that represents the current algorithm state.

    // ========== 成員變數 Member Variables ==========
    private Entry<K, V>[] buckets;  // 桶陣列 - Array of buckets
    private int capacity;            // 桶的數量 - Number of buckets
    private int size;                // 元素數量 - Number of elements

    // ========== 建構子 Constructors ==========

    /** Doc block start
     * 使用預設容量建立雜湊表 / Create hash table with default capacity
     */  // End of block comment
    public HashTable() {  // Execute this statement as part of the data structure implementation.
        this(DEFAULT_CAPACITY);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    /** Doc block start
     * 使用指定容量建立雜湊表 / Create hash table with specified capacity
     *(blank line)
     * @param capacity 桶的數量（number of buckets）
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    public HashTable(int capacity) {  // Execute this statement as part of the data structure implementation.
        if (capacity <= 0) {  // Evaluate the condition and branch into the appropriate code path.
            throw new IllegalArgumentException(  // Throw an exception to signal an invalid argument or operation.
                "容量必須為正整數 / Capacity must be positive");  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
        this.capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
        this.size = 0;  // Assign or update a variable that represents the current algorithm state.
        this.buckets = (Entry<K, V>[]) new Entry[capacity];  // Access or update the bucket storage used to hold entries or chains.
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

        // 重新插入所有元素 - Reinsert all elements
        for (Entry<K, V> head : oldBuckets) {  // Iterate over a range/collection to process each item in sequence.
            Entry<K, V> current = head;  // Assign or update a variable that represents the current algorithm state.
            while (current != null) {  // Repeat while the loop condition remains true.
                insert(current.key, current.value);  // Execute this statement as part of the data structure implementation.
                current = current.next;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        }  // Close the current block scope.
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

        // 檢查 key 是否已存在 - Check if key exists
        Entry<K, V> current = head;  // Assign or update a variable that represents the current algorithm state.
        while (current != null) {  // Repeat while the loop condition remains true.
            if (current.key == null ? key == null : current.key.equals(key)) {  // Evaluate the condition and branch into the appropriate code path.
                current.value = value;  // 更新 - Update existing
                return;  // Return the computed result to the caller.
            }  // Close the current block scope.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // 新增節點到鏈結串列頭部 - Add new node at head
        Entry<K, V> newEntry = new Entry<>(key, value);  // Assign or update a variable that represents the current algorithm state.
        newEntry.next = head;  // Assign or update a variable that represents the current algorithm state.
        buckets[index] = newEntry;  // Access or update the bucket storage used to hold entries or chains.
        size++;  // Execute this statement as part of the data structure implementation.

        // 檢查是否需要擴容 - Check if rehashing needed
        if (getLoadFactor() > MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
            rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *(blank line)
     * @param key 要搜尋的鍵
     * @return 找到則回傳 value，否則回傳 null
     */  // End of block comment
    public V search(K key) {  // Execute this statement as part of the data structure implementation.
        int index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
        Entry<K, V> current = buckets[index];  // Access or update the bucket storage used to hold entries or chains.

        while (current != null) {  // Repeat while the loop condition remains true.
            if (current.key == null ? key == null : current.key.equals(key)) {  // Evaluate the condition and branch into the appropriate code path.
                return current.value;  // Return the computed result to the caller.
            }  // Close the current block scope.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
        return null;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 刪除指定的鍵值對 / Delete key-value pair
     *(blank line)
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     */  // End of block comment
    public boolean delete(K key) {  // Execute this statement as part of the data structure implementation.
        int index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
        Entry<K, V> current = buckets[index];  // Access or update the bucket storage used to hold entries or chains.
        Entry<K, V> prev = null;  // Assign or update a variable that represents the current algorithm state.

        while (current != null) {  // Repeat while the loop condition remains true.
            if (current.key == null ? key == null : current.key.equals(key)) {  // Evaluate the condition and branch into the appropriate code path.
                if (prev == null) {  // Evaluate the condition and branch into the appropriate code path.
                    buckets[index] = current.next;  // Access or update the bucket storage used to hold entries or chains.
                } else {  // Handle the alternative branch when the condition is false.
                    prev.next = current.next;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.
                size--;  // Execute this statement as part of the data structure implementation.
                return true;  // Return the computed result to the caller.
            }  // Close the current block scope.
            prev = current;  // Assign or update a variable that represents the current algorithm state.
            current = current.next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
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
     */  // End of block comment
    public double getLoadFactor() {  // Execute this statement as part of the data structure implementation.
        return (double) size / capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    @SuppressWarnings("unchecked")  // Execute this statement as part of the data structure implementation.
    public void clear() {  // Execute this statement as part of the data structure implementation.
        buckets = (Entry<K, V>[]) new Entry[capacity];  // Access or update the bucket storage used to hold entries or chains.
        size = 0;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    /** Doc block start
     * 字串表示 / String representation
     */  // End of block comment
    @Override  // Execute this statement as part of the data structure implementation.
    public String toString() {  // Execute this statement as part of the data structure implementation.
        StringBuilder sb = new StringBuilder("HashTable{");  // Assign or update a variable that represents the current algorithm state.
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

    // ========== 範例執行 Example Main ==========

    public static void main(String[] args) {  // Execute this statement as part of the data structure implementation.
        System.out.println("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n");  // Execute this statement as part of the data structure implementation.

        // 建立雜湊表 - Create hash table
        HashTable<String, Integer> ht = new HashTable<>();  // Assign or update a variable that represents the current algorithm state.

        // 插入操作 - Insert operations
        System.out.println("插入鍵值對 Inserting key-value pairs:");  // Execute this statement as part of the data structure implementation.
        ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
        ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.
        ht.insert("cherry", 300);  // Execute this statement as part of the data structure implementation.
        System.out.println("  雜湊表: " + ht);  // Execute this statement as part of the data structure implementation.
        System.out.printf("  大小: %d, 負載因子: %.3f%n%n", ht.size(), ht.getLoadFactor());  // Execute this statement as part of the data structure implementation.

        // 搜尋操作 - Search operations
        System.out.println("搜尋操作 Search operations:");  // Execute this statement as part of the data structure implementation.
        System.out.println("  search('apple') = " + ht.search("apple"));  // Assign or update a variable that represents the current algorithm state.
        System.out.println("  search('grape') = " + ht.search("grape"));  // Assign or update a variable that represents the current algorithm state.
        System.out.println("  contains('cherry') = " + ht.contains("cherry") + "\n");  // Assign or update a variable that represents the current algorithm state.

        // 更新操作 - Update operation
        System.out.println("更新操作 Update operation:");  // Execute this statement as part of the data structure implementation.
        ht.insert("apple", 150);  // Execute this statement as part of the data structure implementation.
        System.out.println("  更新 apple 的值為 150");  // Execute this statement as part of the data structure implementation.
        System.out.println("  search('apple') = " + ht.search("apple") + "\n");  // Assign or update a variable that represents the current algorithm state.

        // 刪除操作 - Delete operation
        System.out.println("刪除操作 Delete operations:");  // Execute this statement as part of the data structure implementation.
        ht.delete("banana");  // Execute this statement as part of the data structure implementation.
        System.out.println("  刪除 'banana' 後: " + ht);  // Execute this statement as part of the data structure implementation.
        System.out.println("  大小: " + ht.size());  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
}  // Close the current block scope.
