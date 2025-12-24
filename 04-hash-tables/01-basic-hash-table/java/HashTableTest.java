import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;

/**
 * 雜湊表單元測試
 * Hash Table Unit Tests
 *
 * 使用 JUnit 5 測試框架
 * Using JUnit 5 testing framework
 */
public class HashTableTest {

    private HashTable<String, Integer> hashTable;

    @BeforeEach
    void setUp() {
        hashTable = new HashTable<>();
    }

    // ========== 基本操作測試 Basic Operations Tests ==========

    @Test
    @DisplayName("建立空的雜湊表 - Create empty hash table")
    void testCreateEmptyHashTable() {
        assertEquals(0, hashTable.size());
        assertTrue(hashTable.isEmpty());
        assertEquals(16, hashTable.capacity()); // 預設容量
    }

    @Test
    @DisplayName("使用自訂容量 - Custom capacity")
    void testCreateWithCustomCapacity() {
        HashTable<String, Integer> ht = new HashTable<>(32);
        assertEquals(32, ht.capacity());
    }

    @Test
    @DisplayName("無效容量應拋出例外 - Invalid capacity should throw")
    void testInvalidCapacity() {
        assertThrows(IllegalArgumentException.class, () -> new HashTable<>(0));
        assertThrows(IllegalArgumentException.class, () -> new HashTable<>(-1));
    }

    @Test
    @DisplayName("插入與搜尋 - Insert and search")
    void testInsertAndSearch() {
        hashTable.insert("apple", 100);
        hashTable.insert("banana", 200);

        assertEquals(100, hashTable.search("apple"));
        assertEquals(200, hashTable.search("banana"));
        assertNull(hashTable.search("cherry"));
    }

    @Test
    @DisplayName("更新已存在的鍵 - Update existing key")
    void testInsertUpdateExisting() {
        hashTable.insert("apple", 100);
        hashTable.insert("apple", 150);

        assertEquals(150, hashTable.search("apple"));
        assertEquals(1, hashTable.size()); // 大小不應增加
    }

    @Test
    @DisplayName("刪除操作 - Delete operation")
    void testDelete() {
        hashTable.insert("apple", 100);
        hashTable.insert("banana", 200);

        assertTrue(hashTable.delete("apple"));
        assertNull(hashTable.search("apple"));
        assertEquals(1, hashTable.size());
    }

    @Test
    @DisplayName("刪除不存在的鍵 - Delete nonexistent key")
    void testDeleteNonexistent() {
        hashTable.insert("apple", 100);

        assertFalse(hashTable.delete("banana"));
        assertEquals(1, hashTable.size());
    }

    @Test
    @DisplayName("contains 方法 - Contains method")
    void testContains() {
        hashTable.insert("apple", 100);

        assertTrue(hashTable.contains("apple"));
        assertFalse(hashTable.contains("banana"));
    }

    // ========== 特殊情況測試 Special Cases Tests ==========

    @Test
    @DisplayName("插入 null 鍵 - Insert null key")
    void testInsertNullKey() {
        hashTable.insert(null, 100);

        assertEquals(100, hashTable.search(null));
        assertTrue(hashTable.contains(null));
    }

    @Test
    @DisplayName("插入 null 值 - Insert null value")
    void testInsertNullValue() {
        hashTable.insert("key", null);

        assertNull(hashTable.search("key"));
        // 注意：這裡 contains 的行為可能需要特別處理
    }

    @Test
    @DisplayName("清空操作 - Clear operation")
    void testClear() {
        hashTable.insert("a", 1);
        hashTable.insert("b", 2);
        hashTable.clear();

        assertTrue(hashTable.isEmpty());
        assertEquals(0, hashTable.size());
        assertNull(hashTable.search("a"));
    }

    // ========== 擴容測試 Rehashing Tests ==========

    @Test
    @DisplayName("負載因子觸發擴容 - Rehashing on load factor threshold")
    void testRehashOnLoadFactor() {
        // 使用小容量以快速觸發擴容
        HashTable<String, Integer> ht = new HashTable<>(4);

        // 插入足夠多的元素使負載因子超過 0.75
        for (int i = 0; i < 4; i++) {
            ht.insert("key" + i, i);
        }

        // 應該已經擴容
        assertTrue(ht.capacity() > 4);

        // 所有元素應該仍然可以存取
        for (int i = 0; i < 4; i++) {
            assertEquals(i, ht.search("key" + i));
        }
    }

    @Test
    @DisplayName("負載因子計算 - Load factor calculation")
    void testLoadFactor() {
        HashTable<String, Integer> ht = new HashTable<>(10);
        ht.insert("a", 1);
        ht.insert("b", 2);
        ht.insert("c", 3);

        assertEquals(0.3, ht.getLoadFactor(), 0.001);
    }

    // ========== 碰撞測試 Collision Tests ==========

    @Test
    @DisplayName("多個元素在同一桶 - Multiple items in same bucket")
    void testMultipleItemsSameBucket() {
        // 使用容量 1 強制所有元素碰撞
        HashTable<String, Integer> ht = new HashTable<>(1);

        ht.insert("a", 1);
        ht.insert("b", 2);
        ht.insert("c", 3);

        assertEquals(1, ht.search("a"));
        assertEquals(2, ht.search("b"));
        assertEquals(3, ht.search("c"));

        ht.delete("b");
        assertNull(ht.search("b"));
        assertEquals(1, ht.search("a"));
        assertEquals(3, ht.search("c"));
    }

    // ========== 不同鍵類型測試 Different Key Types Tests ==========

    @Test
    @DisplayName("整數作為鍵 - Integer as key")
    void testIntegerKey() {
        HashTable<Integer, String> ht = new HashTable<>();
        ht.insert(1, "one");
        ht.insert(2, "two");
        ht.insert(3, "three");

        assertEquals("one", ht.search(1));
        assertEquals("two", ht.search(2));
        assertEquals("three", ht.search(3));
    }
}
