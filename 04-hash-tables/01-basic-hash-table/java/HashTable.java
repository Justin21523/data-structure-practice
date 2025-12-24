/**
 * 雜湊表（Hash Table）- Java 實作
 * 使用鏈結法（chaining）處理碰撞
 *
 * Hash Table implementation using chaining for collision resolution
 *
 * @param <K> 鍵的型別（key type）
 * @param <V> 值的型別（value type）
 */
public class HashTable<K, V> {

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

    // ========== 常數 Constants ==========
    private static final int DEFAULT_CAPACITY = 16;
    private static final double MAX_LOAD_FACTOR = 0.75;

    // ========== 成員變數 Member Variables ==========
    private Entry<K, V>[] buckets;  // 桶陣列 - Array of buckets
    private int capacity;            // 桶的數量 - Number of buckets
    private int size;                // 元素數量 - Number of elements

    // ========== 建構子 Constructors ==========

    /**
     * 使用預設容量建立雜湊表
     * Create hash table with default capacity
     */
    public HashTable() {
        this(DEFAULT_CAPACITY);
    }

    /**
     * 使用指定容量建立雜湊表
     * Create hash table with specified capacity
     *
     * @param capacity 桶的數量（number of buckets）
     */
    @SuppressWarnings("unchecked")
    public HashTable(int capacity) {
        if (capacity <= 0) {
            throw new IllegalArgumentException(
                "容量必須為正整數 / Capacity must be positive");
        }
        this.capacity = capacity;
        this.size = 0;
        this.buckets = (Entry<K, V>[]) new Entry[capacity];
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

        // 重新插入所有元素 - Reinsert all elements
        for (Entry<K, V> head : oldBuckets) {
            Entry<K, V> current = head;
            while (current != null) {
                insert(current.key, current.value);
                current = current.next;
            }
        }
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

        // 檢查 key 是否已存在 - Check if key exists
        Entry<K, V> current = head;
        while (current != null) {
            if (current.key == null ? key == null : current.key.equals(key)) {
                current.value = value;  // 更新 - Update existing
                return;
            }
            current = current.next;
        }

        // 新增節點到鏈結串列頭部 - Add new node at head
        Entry<K, V> newEntry = new Entry<>(key, value);
        newEntry.next = head;
        buckets[index] = newEntry;
        size++;

        // 檢查是否需要擴容 - Check if rehashing needed
        if (getLoadFactor() > MAX_LOAD_FACTOR) {
            rehash();
        }
    }

    /**
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *
     * @param key 要搜尋的鍵
     * @return 找到則回傳 value，否則回傳 null
     */
    public V search(K key) {
        int index = hash(key);
        Entry<K, V> current = buckets[index];

        while (current != null) {
            if (current.key == null ? key == null : current.key.equals(key)) {
                return current.value;
            }
            current = current.next;
        }
        return null;
    }

    /**
     * 刪除指定的鍵值對
     * Delete key-value pair
     *
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     */
    public boolean delete(K key) {
        int index = hash(key);
        Entry<K, V> current = buckets[index];
        Entry<K, V> prev = null;

        while (current != null) {
            if (current.key == null ? key == null : current.key.equals(key)) {
                if (prev == null) {
                    buckets[index] = current.next;
                } else {
                    prev.next = current.next;
                }
                size--;
                return true;
            }
            prev = current;
            current = current.next;
        }
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
     */
    public double getLoadFactor() {
        return (double) size / capacity;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    @SuppressWarnings("unchecked")
    public void clear() {
        buckets = (Entry<K, V>[]) new Entry[capacity];
        size = 0;
    }

    /**
     * 字串表示
     * String representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("HashTable{");
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

    // ========== 範例執行 Example Main ==========

    public static void main(String[] args) {
        System.out.println("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n");

        // 建立雜湊表 - Create hash table
        HashTable<String, Integer> ht = new HashTable<>();

        // 插入操作 - Insert operations
        System.out.println("插入鍵值對 Inserting key-value pairs:");
        ht.insert("apple", 100);
        ht.insert("banana", 200);
        ht.insert("cherry", 300);
        System.out.println("  雜湊表: " + ht);
        System.out.printf("  大小: %d, 負載因子: %.3f%n%n", ht.size(), ht.getLoadFactor());

        // 搜尋操作 - Search operations
        System.out.println("搜尋操作 Search operations:");
        System.out.println("  search('apple') = " + ht.search("apple"));
        System.out.println("  search('grape') = " + ht.search("grape"));
        System.out.println("  contains('cherry') = " + ht.contains("cherry") + "\n");

        // 更新操作 - Update operation
        System.out.println("更新操作 Update operation:");
        ht.insert("apple", 150);
        System.out.println("  更新 apple 的值為 150");
        System.out.println("  search('apple') = " + ht.search("apple") + "\n");

        // 刪除操作 - Delete operation
        System.out.println("刪除操作 Delete operations:");
        ht.delete("banana");
        System.out.println("  刪除 'banana' 後: " + ht);
        System.out.println("  大小: " + ht.size());
    }
}
