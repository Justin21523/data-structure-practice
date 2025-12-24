import static org.junit.jupiter.api.Assertions.*;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.Test;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.BeforeEach;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.DisplayName;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 雜湊表單元測試 / Hash Table Unit Tests
 *(blank line)
 * 使用 JUnit 5 測試框架
 * Using JUnit 5 testing framework
 */  // End of block comment
public class HashTableTest {  // Execute this statement as part of the data structure implementation.

    private HashTable<String, Integer> hashTable;  // Execute this statement as part of the data structure implementation.

    @BeforeEach  // Execute this statement as part of the data structure implementation.
    void setUp() {  // Execute this statement as part of the data structure implementation.
        hashTable = new HashTable<>();  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    // ========== 基本操作測試 Basic Operations Tests ==========

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("建立空的雜湊表 - Create empty hash table")  // Compute a hash-based index so keys map into the table's storage.
    void testCreateEmptyHashTable() {  // Execute this statement as part of the data structure implementation.
        assertEquals(0, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.
        assertTrue(hashTable.isEmpty());  // Compute a hash-based index so keys map into the table's storage.
        assertEquals(16, hashTable.capacity()); // 預設容量
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("使用自訂容量 - Custom capacity")  // Execute this statement as part of the data structure implementation.
    void testCreateWithCustomCapacity() {  // Execute this statement as part of the data structure implementation.
        HashTable<String, Integer> ht = new HashTable<>(32);  // Assign or update a variable that represents the current algorithm state.
        assertEquals(32, ht.capacity());  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("無效容量應拋出例外 - Invalid capacity should throw")  // Execute this statement as part of the data structure implementation.
    void testInvalidCapacity() {  // Execute this statement as part of the data structure implementation.
        assertThrows(IllegalArgumentException.class, () -> new HashTable<>(0));  // Execute this statement as part of the data structure implementation.
        assertThrows(IllegalArgumentException.class, () -> new HashTable<>(-1));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("插入與搜尋 - Insert and search")  // Execute this statement as part of the data structure implementation.
    void testInsertAndSearch() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.
        hashTable.insert("banana", 200);  // Compute a hash-based index so keys map into the table's storage.

        assertEquals(100, hashTable.search("apple"));  // Compute a hash-based index so keys map into the table's storage.
        assertEquals(200, hashTable.search("banana"));  // Compute a hash-based index so keys map into the table's storage.
        assertNull(hashTable.search("cherry"));  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("更新已存在的鍵 - Update existing key")  // Execute this statement as part of the data structure implementation.
    void testInsertUpdateExisting() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.
        hashTable.insert("apple", 150);  // Compute a hash-based index so keys map into the table's storage.

        assertEquals(150, hashTable.search("apple"));  // Compute a hash-based index so keys map into the table's storage.
        assertEquals(1, hashTable.size()); // 大小不應增加
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("刪除操作 - Delete operation")  // Execute this statement as part of the data structure implementation.
    void testDelete() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.
        hashTable.insert("banana", 200);  // Compute a hash-based index so keys map into the table's storage.

        assertTrue(hashTable.delete("apple"));  // Compute a hash-based index so keys map into the table's storage.
        assertNull(hashTable.search("apple"));  // Compute a hash-based index so keys map into the table's storage.
        assertEquals(1, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("刪除不存在的鍵 - Delete nonexistent key")  // Execute this statement as part of the data structure implementation.
    void testDeleteNonexistent() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.

        assertFalse(hashTable.delete("banana"));  // Compute a hash-based index so keys map into the table's storage.
        assertEquals(1, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("contains 方法 - Contains method")  // Execute this statement as part of the data structure implementation.
    void testContains() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.

        assertTrue(hashTable.contains("apple"));  // Compute a hash-based index so keys map into the table's storage.
        assertFalse(hashTable.contains("banana"));  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    // ========== 特殊情況測試 Special Cases Tests ==========

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("插入 null 鍵 - Insert null key")  // Execute this statement as part of the data structure implementation.
    void testInsertNullKey() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert(null, 100);  // Compute a hash-based index so keys map into the table's storage.

        assertEquals(100, hashTable.search(null));  // Compute a hash-based index so keys map into the table's storage.
        assertTrue(hashTable.contains(null));  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("插入 null 值 - Insert null value")  // Execute this statement as part of the data structure implementation.
    void testInsertNullValue() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("key", null);  // Compute a hash-based index so keys map into the table's storage.

        assertNull(hashTable.search("key"));  // Compute a hash-based index so keys map into the table's storage.
        // 注意：這裡 contains 的行為可能需要特別處理
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("清空操作 - Clear operation")  // Execute this statement as part of the data structure implementation.
    void testClear() {  // Execute this statement as part of the data structure implementation.
        hashTable.insert("a", 1);  // Compute a hash-based index so keys map into the table's storage.
        hashTable.insert("b", 2);  // Compute a hash-based index so keys map into the table's storage.
        hashTable.clear();  // Compute a hash-based index so keys map into the table's storage.

        assertTrue(hashTable.isEmpty());  // Compute a hash-based index so keys map into the table's storage.
        assertEquals(0, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.
        assertNull(hashTable.search("a"));  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    // ========== 擴容測試 Rehashing Tests ==========

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("負載因子觸發擴容 - Rehashing on load factor threshold")  // Compute a hash-based index so keys map into the table's storage.
    void testRehashOnLoadFactor() {  // Compute a hash-based index so keys map into the table's storage.
        // 使用小容量以快速觸發擴容 / Use 小 capacity 以快速觸發 resize
        HashTable<String, Integer> ht = new HashTable<>(4);  // Assign or update a variable that represents the current algorithm state.

        // 插入足夠多的元素使負載因子超過 0.75 / Insert 足夠多的 elements 使 load factor 超過 0.75
        for (int i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
            ht.insert("key" + i, i);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // 應該已經擴容 / Should 已經 resize
        assertTrue(ht.capacity() > 4);  // Execute this statement as part of the data structure implementation.

        // 所有元素應該仍然可以存取 / 所有 elements should still 然 can 存取
        for (int i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
            assertEquals(i, ht.search("key" + i));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("負載因子計算 - Load factor calculation")  // Execute this statement as part of the data structure implementation.
    void testLoadFactor() {  // Execute this statement as part of the data structure implementation.
        HashTable<String, Integer> ht = new HashTable<>(10);  // Assign or update a variable that represents the current algorithm state.
        ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
        ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
        ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

        assertEquals(0.3, ht.getLoadFactor(), 0.001);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // ========== 碰撞測試 Collision Tests ==========

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("多個元素在同一桶 - Multiple items in same bucket")  // Access or update the bucket storage used to hold entries or chains.
    void testMultipleItemsSameBucket() {  // Execute this statement as part of the data structure implementation.
        // 使用容量 1 強制所有元素碰撞 / Use capacity 1 強制所有 elements collision
        HashTable<String, Integer> ht = new HashTable<>(1);  // Assign or update a variable that represents the current algorithm state.

        ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
        ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
        ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

        assertEquals(1, ht.search("a"));  // Execute this statement as part of the data structure implementation.
        assertEquals(2, ht.search("b"));  // Execute this statement as part of the data structure implementation.
        assertEquals(3, ht.search("c"));  // Execute this statement as part of the data structure implementation.

        ht.delete("b");  // Execute this statement as part of the data structure implementation.
        assertNull(ht.search("b"));  // Execute this statement as part of the data structure implementation.
        assertEquals(1, ht.search("a"));  // Execute this statement as part of the data structure implementation.
        assertEquals(3, ht.search("c"));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // ========== 不同鍵類型測試 Different Key Types Tests ==========

    @Test  // Execute this statement as part of the data structure implementation.
    @DisplayName("整數作為鍵 - Integer as key")  // Execute this statement as part of the data structure implementation.
    void testIntegerKey() {  // Execute this statement as part of the data structure implementation.
        HashTable<Integer, String> ht = new HashTable<>();  // Assign or update a variable that represents the current algorithm state.
        ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
        ht.insert(2, "two");  // Execute this statement as part of the data structure implementation.
        ht.insert(3, "three");  // Execute this statement as part of the data structure implementation.

        assertEquals("one", ht.search(1));  // Execute this statement as part of the data structure implementation.
        assertEquals("two", ht.search(2));  // Execute this statement as part of the data structure implementation.
        assertEquals("three", ht.search(3));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
}  // Close the current block scope.
