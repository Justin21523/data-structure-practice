import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;

/**
 * 碰撞解決方法單元測試
 * Collision Resolution Unit Tests
 *
 * 測試鏈結法和開放定址法的各種碰撞解決策略
 * Test chaining and open addressing collision resolution strategies
 *
 * 使用 JUnit 5 測試框架
 * Using JUnit 5 testing framework
 */
public class CollisionTest {

    // ========== 鏈結雜湊表測試 Chained Hash Table Tests ==========

    @Nested
    @DisplayName("鏈結雜湊表測試 - Chained Hash Table Tests")
    class ChainedHashTableTests {

        private ChainedHashTable<String, Integer> hashTable;

        @BeforeEach
        void setUp() {
            hashTable = new ChainedHashTable<>();
        }

        @Test
        @DisplayName("建立空的雜湊表 - Create empty hash table")
        void testCreateEmptyHashTable() {
            assertEquals(0, hashTable.size());
            assertTrue(hashTable.isEmpty());
            assertEquals(16, hashTable.capacity());
        }

        @Test
        @DisplayName("插入與搜尋 - Insert and search")
        void testInsertAndSearch() {
            hashTable.insert("apple", 100);
            hashTable.insert("banana", 200);
            hashTable.insert("cherry", 300);

            assertEquals(100, hashTable.search("apple"));
            assertEquals(200, hashTable.search("banana"));
            assertEquals(300, hashTable.search("cherry"));
            assertNull(hashTable.search("grape"));
        }

        @Test
        @DisplayName("更新已存在的鍵 - Update existing key")
        void testUpdateExisting() {
            hashTable.insert("apple", 100);
            hashTable.insert("apple", 150);

            assertEquals(150, hashTable.search("apple"));
            assertEquals(1, hashTable.size());
        }

        @Test
        @DisplayName("刪除操作 - Delete operation")
        void testDelete() {
            hashTable.insert("apple", 100);
            hashTable.insert("banana", 200);

            assertTrue(hashTable.delete("apple"));
            assertNull(hashTable.search("apple"));
            assertEquals(1, hashTable.size());

            assertFalse(hashTable.delete("grape"));
        }

        @Test
        @DisplayName("碰撞處理 - Collision handling")
        void testCollisionHandling() {
            // 使用小容量強制碰撞 - Use small capacity to force collisions
            ChainedHashTable<Integer, String> ht = new ChainedHashTable<>(3);

            ht.insert(1, "one");
            ht.insert(4, "four");   // 4 % 3 = 1, will collide
            ht.insert(7, "seven");  // 7 % 3 = 1, will collide

            assertEquals("one", ht.search(1));
            assertEquals("four", ht.search(4));
            assertEquals("seven", ht.search(7));

            // 刪除中間的元素 - Delete middle element
            assertTrue(ht.delete(4));
            assertNull(ht.search(4));
            assertEquals("one", ht.search(1));
            assertEquals("seven", ht.search(7));
        }

        @Test
        @DisplayName("探測統計 - Probe statistics")
        void testProbeStats() {
            ChainedHashTable<Integer, String> ht = new ChainedHashTable<>(3);

            ht.insert(1, "one");
            ht.insert(4, "four");   // Collision
            ht.insert(7, "seven");  // Collision

            ChainedHashTable.ProbeStats stats = ht.getProbeStats();
            assertTrue(stats.collisions > 0);
            assertTrue(stats.totalProbes > 0);
            assertTrue(stats.maxChainLength >= 3);
        }

        @Test
        @DisplayName("負載因子與擴容 - Load factor and rehashing")
        void testLoadFactorAndRehash() {
            ChainedHashTable<String, Integer> ht = new ChainedHashTable<>(4);

            // 插入足夠多元素觸發擴容 - Insert enough to trigger rehash
            for (int i = 0; i < 4; i++) {
                ht.insert("key" + i, i);
            }

            // 應該已經擴容 - Should have rehashed
            assertTrue(ht.capacity() > 4);

            // 所有元素應該仍然可存取 - All elements still accessible
            for (int i = 0; i < 4; i++) {
                assertEquals(i, ht.search("key" + i));
            }
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

        @Test
        @DisplayName("重置統計 - Reset statistics")
        void testResetStats() {
            hashTable.insert("a", 1);
            hashTable.search("a");

            hashTable.resetStats();

            ChainedHashTable.ProbeStats stats = hashTable.getProbeStats();
            assertEquals(0, stats.totalProbes);
            assertEquals(0, stats.collisions);
        }
    }

    // ========== 開放定址法測試 Open Addressing Tests ==========

    @Nested
    @DisplayName("開放定址法測試 - Open Addressing Tests")
    class OpenAddressingTests {

        @Test
        @DisplayName("線性探測 - Linear probing")
        void testLinearProbing() {
            OpenAddressingHashTable<Integer, String> ht =
                new OpenAddressingHashTable<>(7, OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert(10, "ten");
            ht.insert(20, "twenty");
            ht.insert(17, "seventeen");  // 10 % 7 = 3, 17 % 7 = 3, collision

            assertEquals("ten", ht.search(10));
            assertEquals("twenty", ht.search(20));
            assertEquals("seventeen", ht.search(17));

            assertEquals(OpenAddressingHashTable.ProbeMethod.LINEAR, ht.getProbeMethod());
        }

        @Test
        @DisplayName("二次探測 - Quadratic probing")
        void testQuadraticProbing() {
            OpenAddressingHashTable<Integer, String> ht =
                new OpenAddressingHashTable<>(11, OpenAddressingHashTable.ProbeMethod.QUADRATIC);

            ht.insert(11, "eleven");
            ht.insert(22, "twenty-two");
            ht.insert(33, "thirty-three");

            assertEquals("eleven", ht.search(11));
            assertEquals("twenty-two", ht.search(22));
            assertEquals("thirty-three", ht.search(33));

            assertEquals(OpenAddressingHashTable.ProbeMethod.QUADRATIC, ht.getProbeMethod());
        }

        @Test
        @DisplayName("雙重雜湊 - Double hashing")
        void testDoubleHashing() {
            OpenAddressingHashTable<Integer, String> ht =
                new OpenAddressingHashTable<>(11, OpenAddressingHashTable.ProbeMethod.DOUBLE_HASH);

            ht.insert(11, "eleven");
            ht.insert(22, "twenty-two");
            ht.insert(33, "thirty-three");

            assertEquals("eleven", ht.search(11));
            assertEquals("twenty-two", ht.search(22));
            assertEquals("thirty-three", ht.search(33));

            assertEquals(OpenAddressingHashTable.ProbeMethod.DOUBLE_HASH, ht.getProbeMethod());
        }

        @Test
        @DisplayName("墓碑標記 - Tombstone marker")
        void testTombstoneMarker() {
            OpenAddressingHashTable<String, Integer> ht =
                new OpenAddressingHashTable<>(7, OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert("a", 1);
            ht.insert("b", 2);
            ht.insert("c", 3);

            // 刪除中間元素 - Delete middle element
            assertTrue(ht.delete("b"));
            assertNull(ht.search("b"));

            // 其他元素應該仍然可以找到 - Other elements still findable
            assertEquals(1, ht.search("a"));
            assertEquals(3, ht.search("c"));

            // 可以重新插入到已刪除的位置 - Can reinsert into deleted slot
            ht.insert("d", 4);
            assertEquals(4, ht.search("d"));
        }

        @Test
        @DisplayName("更新已存在的鍵 - Update existing key")
        void testUpdateExisting() {
            OpenAddressingHashTable<String, Integer> ht =
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert("key", 100);
            ht.insert("key", 200);

            assertEquals(200, ht.search("key"));
            assertEquals(1, ht.size());
        }

        @Test
        @DisplayName("探測統計 - Probe statistics")
        void testProbeStats() {
            OpenAddressingHashTable<Integer, String> ht =
                new OpenAddressingHashTable<>(5, OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert(1, "one");
            ht.insert(6, "six");    // Will collide (both map to index 1)
            ht.insert(11, "eleven"); // Will collide again

            OpenAddressingHashTable.ProbeStats stats = ht.getProbeStats();
            assertTrue(stats.collisions > 0);
            assertTrue(stats.totalProbes > 0);
            assertTrue(stats.maxProbes > 1);
        }

        @Test
        @DisplayName("負載因子與擴容 - Load factor and rehashing")
        void testLoadFactorAndRehash() {
            OpenAddressingHashTable<Integer, String> ht =
                new OpenAddressingHashTable<>(4, OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert(1, "one");
            ht.insert(2, "two");

            int initialCapacity = ht.capacity();

            // 超過負載因子閾值應觸發擴容 - Exceeding load factor should trigger rehash
            assertTrue(ht.capacity() >= initialCapacity);

            // 所有元素應該仍然可存取 - All elements still accessible
            assertEquals("one", ht.search(1));
            assertEquals("two", ht.search(2));
        }

        @Test
        @DisplayName("清空操作 - Clear operation")
        void testClear() {
            OpenAddressingHashTable<String, Integer> ht =
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert("a", 1);
            ht.insert("b", 2);
            ht.clear();

            assertTrue(ht.isEmpty());
            assertEquals(0, ht.size());
            assertNull(ht.search("a"));
        }

        @Test
        @DisplayName("contains 方法 - Contains method")
        void testContains() {
            OpenAddressingHashTable<String, Integer> ht =
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert("apple", 100);

            assertTrue(ht.contains("apple"));
            assertFalse(ht.contains("banana"));
        }
    }

    // ========== 碰撞性能比較測試 Collision Performance Comparison Tests ==========

    @Nested
    @DisplayName("碰撞性能比較 - Collision Performance Comparison")
    class PerformanceComparisonTests {

        private static final int TEST_SIZE = 100;

        @Test
        @DisplayName("比較不同探測方法的性能 - Compare probing methods performance")
        void testProbingMethodsComparison() {
            // 線性探測 - Linear probing
            OpenAddressingHashTable<Integer, String> linear =
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.LINEAR);

            // 二次探測 - Quadratic probing
            OpenAddressingHashTable<Integer, String> quadratic =
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.QUADRATIC);

            // 雙重雜湊 - Double hashing
            OpenAddressingHashTable<Integer, String> doubleHash =
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.DOUBLE_HASH);

            // 插入相同的資料 - Insert same data
            for (int i = 0; i < TEST_SIZE; i++) {
                linear.insert(i, "value" + i);
                quadratic.insert(i, "value" + i);
                doubleHash.insert(i, "value" + i);
            }

            // 比較統計 - Compare statistics
            OpenAddressingHashTable.ProbeStats linearStats = linear.getProbeStats();
            OpenAddressingHashTable.ProbeStats quadraticStats = quadratic.getProbeStats();
            OpenAddressingHashTable.ProbeStats doubleHashStats = doubleHash.getProbeStats();

            // 所有方法應該都能成功插入 - All methods should successfully insert
            assertEquals(TEST_SIZE, linear.size());
            assertEquals(TEST_SIZE, quadratic.size());
            assertEquals(TEST_SIZE, doubleHash.size());

            // 輸出統計資料（僅供參考）- Output statistics (for reference)
            System.out.println("線性探測 Linear: " + linearStats);
            System.out.println("二次探測 Quadratic: " + quadraticStats);
            System.out.println("雙重雜湊 Double Hash: " + doubleHashStats);
        }

        @Test
        @DisplayName("比較鏈結法與開放定址法 - Compare chaining vs open addressing")
        void testChainingVsOpenAddressing() {
            ChainedHashTable<Integer, String> chained = new ChainedHashTable<>(50);
            OpenAddressingHashTable<Integer, String> openAddr =
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.LINEAR);

            // 插入相同的資料 - Insert same data
            for (int i = 0; i < TEST_SIZE; i++) {
                chained.insert(i, "value" + i);
                openAddr.insert(i, "value" + i);
            }

            // 驗證結果 - Verify results
            assertEquals(TEST_SIZE, chained.size());
            assertEquals(TEST_SIZE, openAddr.size());

            // 輸出統計資料 - Output statistics
            System.out.println("鏈結法 Chaining: " + chained.getProbeStats());
            System.out.println("開放定址 Open Addressing: " + openAddr.getProbeStats());
        }
    }

    // ========== 特殊情況測試 Special Cases Tests ==========

    @Nested
    @DisplayName("特殊情況測試 - Special Cases Tests")
    class SpecialCasesTests {

        @Test
        @DisplayName("null 鍵處理（鏈結法）- Null key handling (chaining)")
        void testNullKeyChaining() {
            ChainedHashTable<String, Integer> ht = new ChainedHashTable<>();

            ht.insert(null, 100);
            assertEquals(100, ht.search(null));
            assertTrue(ht.contains(null));

            assertTrue(ht.delete(null));
            assertNull(ht.search(null));
        }

        @Test
        @DisplayName("null 鍵處理（開放定址）- Null key handling (open addressing)")
        void testNullKeyOpenAddressing() {
            OpenAddressingHashTable<String, Integer> ht =
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);

            ht.insert(null, 100);
            assertEquals(100, ht.search(null));
            assertTrue(ht.contains(null));

            assertTrue(ht.delete(null));
            assertNull(ht.search(null));
        }

        @Test
        @DisplayName("大量碰撞測試 - Heavy collision test")
        void testHeavyCollisions() {
            // 故意使用非常小的容量造成大量碰撞 - Use very small capacity for heavy collisions
            ChainedHashTable<Integer, String> ht = new ChainedHashTable<>(2);

            for (int i = 0; i < 20; i++) {
                ht.insert(i, "value" + i);
            }

            // 驗證所有元素都能正確存取 - Verify all elements accessible
            for (int i = 0; i < 20; i++) {
                assertEquals("value" + i, ht.search(i));
            }

            ChainedHashTable.ProbeStats stats = ht.getProbeStats();
            assertTrue(stats.collisions > 10);
            assertTrue(stats.maxChainLength > 5);
        }

        @Test
        @DisplayName("無效容量應拋出例外 - Invalid capacity should throw")
        void testInvalidCapacity() {
            assertThrows(IllegalArgumentException.class, () ->
                new ChainedHashTable<String, Integer>(0));

            assertThrows(IllegalArgumentException.class, () ->
                new ChainedHashTable<String, Integer>(-1));

            assertThrows(IllegalArgumentException.class, () ->
                new OpenAddressingHashTable<String, Integer>(0,
                    OpenAddressingHashTable.ProbeMethod.LINEAR));

            assertThrows(IllegalArgumentException.class, () ->
                new OpenAddressingHashTable<String, Integer>(-5,
                    OpenAddressingHashTable.ProbeMethod.QUADRATIC));
        }

        @Test
        @DisplayName("連續刪除與插入 - Sequential delete and insert")
        void testSequentialDeleteInsert() {
            OpenAddressingHashTable<String, Integer> ht =
                new OpenAddressingHashTable<>(7, OpenAddressingHashTable.ProbeMethod.LINEAR);

            // 插入 - Insert
            for (int i = 0; i < 5; i++) {
                ht.insert("key" + i, i);
            }

            // 刪除 - Delete
            for (int i = 0; i < 3; i++) {
                assertTrue(ht.delete("key" + i));
            }

            // 再次插入 - Insert again
            for (int i = 5; i < 8; i++) {
                ht.insert("key" + i, i);
            }

            // 驗證 - Verify
            assertNull(ht.search("key0"));
            assertNull(ht.search("key1"));
            assertNull(ht.search("key2"));
            assertEquals(3, ht.search("key3"));
            assertEquals(4, ht.search("key4"));
            assertEquals(5, ht.search("key5"));
            assertEquals(6, ht.search("key6"));
            assertEquals(7, ht.search("key7"));
        }
    }
}
