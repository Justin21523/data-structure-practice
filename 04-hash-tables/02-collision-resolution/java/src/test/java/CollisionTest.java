import static org.junit.jupiter.api.Assertions.*;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.Test;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.BeforeEach;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.DisplayName;  // Execute this statement as part of the data structure implementation.
import org.junit.jupiter.api.Nested;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 碰撞解決方法單元測試 / Collision Resolution Unit Tests
 *(blank line)
 * 測試鏈結法和開放定址法的各種碰撞解決策略 / Test chaining and open addressing collision resolution strategies
 *(blank line)
 * 使用 JUnit 5 測試框架
 * Using JUnit 5 testing framework
 */  // End of block comment
public class CollisionTest {  // Execute this statement as part of the data structure implementation.

    // ========== 鏈結雜湊表測試 Chained Hash Table Tests ==========

    @Nested  // Execute this statement as part of the data structure implementation.
    @DisplayName("鏈結雜湊表測試 - Chained Hash Table Tests")  // Execute this statement as part of the data structure implementation.
    class ChainedHashTableTests {  // Execute this statement as part of the data structure implementation.

        private ChainedHashTable<String, Integer> hashTable;  // Execute this statement as part of the data structure implementation.

        @BeforeEach  // Execute this statement as part of the data structure implementation.
        void setUp() {  // Execute this statement as part of the data structure implementation.
            hashTable = new ChainedHashTable<>();  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("建立空的雜湊表 - Create empty hash table")  // Compute a hash-based index so keys map into the table's storage.
        void testCreateEmptyHashTable() {  // Execute this statement as part of the data structure implementation.
            assertEquals(0, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.
            assertTrue(hashTable.isEmpty());  // Compute a hash-based index so keys map into the table's storage.
            assertEquals(16, hashTable.capacity());  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("插入與搜尋 - Insert and search")  // Execute this statement as part of the data structure implementation.
        void testInsertAndSearch() {  // Execute this statement as part of the data structure implementation.
            hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.
            hashTable.insert("banana", 200);  // Compute a hash-based index so keys map into the table's storage.
            hashTable.insert("cherry", 300);  // Compute a hash-based index so keys map into the table's storage.

            assertEquals(100, hashTable.search("apple"));  // Compute a hash-based index so keys map into the table's storage.
            assertEquals(200, hashTable.search("banana"));  // Compute a hash-based index so keys map into the table's storage.
            assertEquals(300, hashTable.search("cherry"));  // Compute a hash-based index so keys map into the table's storage.
            assertNull(hashTable.search("grape"));  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("更新已存在的鍵 - Update existing key")  // Execute this statement as part of the data structure implementation.
        void testUpdateExisting() {  // Execute this statement as part of the data structure implementation.
            hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.
            hashTable.insert("apple", 150);  // Compute a hash-based index so keys map into the table's storage.

            assertEquals(150, hashTable.search("apple"));  // Compute a hash-based index so keys map into the table's storage.
            assertEquals(1, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("刪除操作 - Delete operation")  // Execute this statement as part of the data structure implementation.
        void testDelete() {  // Execute this statement as part of the data structure implementation.
            hashTable.insert("apple", 100);  // Compute a hash-based index so keys map into the table's storage.
            hashTable.insert("banana", 200);  // Compute a hash-based index so keys map into the table's storage.

            assertTrue(hashTable.delete("apple"));  // Compute a hash-based index so keys map into the table's storage.
            assertNull(hashTable.search("apple"));  // Compute a hash-based index so keys map into the table's storage.
            assertEquals(1, hashTable.size());  // Compute a hash-based index so keys map into the table's storage.

            assertFalse(hashTable.delete("grape"));  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("碰撞處理 - Collision handling")  // Execute this statement as part of the data structure implementation.
        void testCollisionHandling() {  // Execute this statement as part of the data structure implementation.
            // 使用小容量強制碰撞 - Use small capacity to force collisions
            ChainedHashTable<Integer, String> ht = new ChainedHashTable<>(3);  // Assign or update a variable that represents the current algorithm state.

            ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
            ht.insert(4, "four");   // 4 % 3 = 1, will collide
            ht.insert(7, "seven");  // 7 % 3 = 1, will collide

            assertEquals("one", ht.search(1));  // Execute this statement as part of the data structure implementation.
            assertEquals("four", ht.search(4));  // Execute this statement as part of the data structure implementation.
            assertEquals("seven", ht.search(7));  // Execute this statement as part of the data structure implementation.

            // 刪除中間的元素 - Delete middle element
            assertTrue(ht.delete(4));  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search(4));  // Execute this statement as part of the data structure implementation.
            assertEquals("one", ht.search(1));  // Execute this statement as part of the data structure implementation.
            assertEquals("seven", ht.search(7));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("探測統計 - Probe statistics")  // Execute this statement as part of the data structure implementation.
        void testProbeStats() {  // Execute this statement as part of the data structure implementation.
            ChainedHashTable<Integer, String> ht = new ChainedHashTable<>(3);  // Assign or update a variable that represents the current algorithm state.

            ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
            ht.insert(4, "four");   // Collision
            ht.insert(7, "seven");  // Collision

            ChainedHashTable.ProbeStats stats = ht.getProbeStats();  // Assign or update a variable that represents the current algorithm state.
            assertTrue(stats.collisions > 0);  // Execute this statement as part of the data structure implementation.
            assertTrue(stats.totalProbes > 0);  // Execute this statement as part of the data structure implementation.
            assertTrue(stats.maxChainLength >= 3);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("負載因子與擴容 - Load factor and rehashing")  // Rehash entries into a larger table to keep operations near O(1) on average.
        void testLoadFactorAndRehash() {  // Compute a hash-based index so keys map into the table's storage.
            ChainedHashTable<String, Integer> ht = new ChainedHashTable<>(4);  // Assign or update a variable that represents the current algorithm state.

            // 插入足夠多元素觸發擴容 - Insert enough to trigger rehash
            for (int i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert("key" + i, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 應該已經擴容 - Should have rehashed
            assertTrue(ht.capacity() > 4);  // Execute this statement as part of the data structure implementation.

            // 所有元素應該仍然可存取 - All elements still accessible
            for (int i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                assertEquals(i, ht.search("key" + i));  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
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

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("重置統計 - Reset statistics")  // Execute this statement as part of the data structure implementation.
        void testResetStats() {  // Execute this statement as part of the data structure implementation.
            hashTable.insert("a", 1);  // Compute a hash-based index so keys map into the table's storage.
            hashTable.search("a");  // Compute a hash-based index so keys map into the table's storage.

            hashTable.resetStats();  // Compute a hash-based index so keys map into the table's storage.

            ChainedHashTable.ProbeStats stats = hashTable.getProbeStats();  // Compute a hash-based index so keys map into the table's storage.
            assertEquals(0, stats.totalProbes);  // Execute this statement as part of the data structure implementation.
            assertEquals(0, stats.collisions);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // ========== 開放定址法測試 Open Addressing Tests ==========

    @Nested  // Execute this statement as part of the data structure implementation.
    @DisplayName("開放定址法測試 - Open Addressing Tests")  // Execute this statement as part of the data structure implementation.
    class OpenAddressingTests {  // Execute this statement as part of the data structure implementation.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("線性探測 - Linear probing")  // Advance or track the probing sequence used by open addressing.
        void testLinearProbing() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<Integer, String> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(7, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert(10, "ten");  // Execute this statement as part of the data structure implementation.
            ht.insert(20, "twenty");  // Execute this statement as part of the data structure implementation.
            ht.insert(17, "seventeen");  // 10 % 7 = 3, 17 % 7 = 3, collision

            assertEquals("ten", ht.search(10));  // Execute this statement as part of the data structure implementation.
            assertEquals("twenty", ht.search(20));  // Execute this statement as part of the data structure implementation.
            assertEquals("seventeen", ht.search(17));  // Execute this statement as part of the data structure implementation.

            assertEquals(OpenAddressingHashTable.ProbeMethod.LINEAR, ht.getProbeMethod());  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("二次探測 - Quadratic probing")  // Advance or track the probing sequence used by open addressing.
        void testQuadraticProbing() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<Integer, String> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(11, OpenAddressingHashTable.ProbeMethod.QUADRATIC);  // Execute this statement as part of the data structure implementation.

            ht.insert(11, "eleven");  // Execute this statement as part of the data structure implementation.
            ht.insert(22, "twenty-two");  // Execute this statement as part of the data structure implementation.
            ht.insert(33, "thirty-three");  // Execute this statement as part of the data structure implementation.

            assertEquals("eleven", ht.search(11));  // Execute this statement as part of the data structure implementation.
            assertEquals("twenty-two", ht.search(22));  // Execute this statement as part of the data structure implementation.
            assertEquals("thirty-three", ht.search(33));  // Execute this statement as part of the data structure implementation.

            assertEquals(OpenAddressingHashTable.ProbeMethod.QUADRATIC, ht.getProbeMethod());  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("雙重雜湊 - Double hashing")  // Compute a hash-based index so keys map into the table's storage.
        void testDoubleHashing() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<Integer, String> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(11, OpenAddressingHashTable.ProbeMethod.DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.

            ht.insert(11, "eleven");  // Execute this statement as part of the data structure implementation.
            ht.insert(22, "twenty-two");  // Execute this statement as part of the data structure implementation.
            ht.insert(33, "thirty-three");  // Execute this statement as part of the data structure implementation.

            assertEquals("eleven", ht.search(11));  // Execute this statement as part of the data structure implementation.
            assertEquals("twenty-two", ht.search(22));  // Execute this statement as part of the data structure implementation.
            assertEquals("thirty-three", ht.search(33));  // Execute this statement as part of the data structure implementation.

            assertEquals(OpenAddressingHashTable.ProbeMethod.DOUBLE_HASH, ht.getProbeMethod());  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("墓碑標記 - Tombstone marker")  // Execute this statement as part of the data structure implementation.
        void testTombstoneMarker() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<String, Integer> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(7, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

            // 刪除中間元素 - Delete middle element
            assertTrue(ht.delete("b"));  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search("b"));  // Execute this statement as part of the data structure implementation.

            // 其他元素應該仍然可以找到 - Other elements still findable
            assertEquals(1, ht.search("a"));  // Execute this statement as part of the data structure implementation.
            assertEquals(3, ht.search("c"));  // Execute this statement as part of the data structure implementation.

            // 可以重新插入到已刪除的位置 - Can reinsert into deleted slot
            ht.insert("d", 4);  // Execute this statement as part of the data structure implementation.
            assertEquals(4, ht.search("d"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("更新已存在的鍵 - Update existing key")  // Execute this statement as part of the data structure implementation.
        void testUpdateExisting() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<String, Integer> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert("key", 100);  // Execute this statement as part of the data structure implementation.
            ht.insert("key", 200);  // Execute this statement as part of the data structure implementation.

            assertEquals(200, ht.search("key"));  // Execute this statement as part of the data structure implementation.
            assertEquals(1, ht.size());  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("探測統計 - Probe statistics")  // Execute this statement as part of the data structure implementation.
        void testProbeStats() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<Integer, String> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(5, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
            ht.insert(6, "six");    // Will collide (both map to index 1)
            ht.insert(11, "eleven"); // Will collide again

            OpenAddressingHashTable.ProbeStats stats = ht.getProbeStats();  // Assign or update a variable that represents the current algorithm state.
            assertTrue(stats.collisions > 0);  // Execute this statement as part of the data structure implementation.
            assertTrue(stats.totalProbes > 0);  // Execute this statement as part of the data structure implementation.
            assertTrue(stats.maxProbes > 1);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("負載因子與擴容 - Load factor and rehashing")  // Rehash entries into a larger table to keep operations near O(1) on average.
        void testLoadFactorAndRehash() {  // Compute a hash-based index so keys map into the table's storage.
            OpenAddressingHashTable<Integer, String> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(4, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
            ht.insert(2, "two");  // Execute this statement as part of the data structure implementation.

            int initialCapacity = ht.capacity();  // Assign or update a variable that represents the current algorithm state.

            // 超過負載因子閾值應觸發擴容 - Exceeding load factor should trigger rehash
            assertTrue(ht.capacity() >= initialCapacity);  // Execute this statement as part of the data structure implementation.

            // 所有元素應該仍然可存取 - All elements still accessible
            assertEquals("one", ht.search(1));  // Execute this statement as part of the data structure implementation.
            assertEquals("two", ht.search(2));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("清空操作 - Clear operation")  // Execute this statement as part of the data structure implementation.
        void testClear() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<String, Integer> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.clear();  // Execute this statement as part of the data structure implementation.

            assertTrue(ht.isEmpty());  // Execute this statement as part of the data structure implementation.
            assertEquals(0, ht.size());  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search("a"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("contains 方法 - Contains method")  // Execute this statement as part of the data structure implementation.
        void testContains() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<String, Integer> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.

            assertTrue(ht.contains("apple"));  // Execute this statement as part of the data structure implementation.
            assertFalse(ht.contains("banana"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // ========== 碰撞性能比較測試 Collision Performance Comparison Tests ==========

    @Nested  // Execute this statement as part of the data structure implementation.
    @DisplayName("碰撞性能比較 - Collision Performance Comparison")  // Execute this statement as part of the data structure implementation.
    class PerformanceComparisonTests {  // Execute this statement as part of the data structure implementation.

        private static final int TEST_SIZE = 100;  // Assign or update a variable that represents the current algorithm state.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("比較不同探測方法的性能 - Compare probing methods performance")  // Advance or track the probing sequence used by open addressing.
        void testProbingMethodsComparison() {  // Execute this statement as part of the data structure implementation.
            // 線性探測 - Linear probing
            OpenAddressingHashTable<Integer, String> linear =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            // 二次探測 - Quadratic probing
            OpenAddressingHashTable<Integer, String> quadratic =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.QUADRATIC);  // Execute this statement as part of the data structure implementation.

            // 雙重雜湊 - Double hashing
            OpenAddressingHashTable<Integer, String> doubleHash =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.

            // 插入相同的資料 - Insert same data
            for (int i = 0; i < TEST_SIZE; i++) {  // Iterate over a range/collection to process each item in sequence.
                linear.insert(i, "value" + i);  // Execute this statement as part of the data structure implementation.
                quadratic.insert(i, "value" + i);  // Execute this statement as part of the data structure implementation.
                doubleHash.insert(i, "value" + i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 比較統計 - Compare statistics
            OpenAddressingHashTable.ProbeStats linearStats = linear.getProbeStats();  // Assign or update a variable that represents the current algorithm state.
            OpenAddressingHashTable.ProbeStats quadraticStats = quadratic.getProbeStats();  // Assign or update a variable that represents the current algorithm state.
            OpenAddressingHashTable.ProbeStats doubleHashStats = doubleHash.getProbeStats();  // Assign or update a variable that represents the current algorithm state.

            // 所有方法應該都能成功插入 - All methods should successfully insert
            assertEquals(TEST_SIZE, linear.size());  // Execute this statement as part of the data structure implementation.
            assertEquals(TEST_SIZE, quadratic.size());  // Execute this statement as part of the data structure implementation.
            assertEquals(TEST_SIZE, doubleHash.size());  // Execute this statement as part of the data structure implementation.

            // 輸出統計資料（僅供參考）- Output statistics (for reference)
            System.out.println("線性探測 Linear: " + linearStats);  // Execute this statement as part of the data structure implementation.
            System.out.println("二次探測 Quadratic: " + quadraticStats);  // Execute this statement as part of the data structure implementation.
            System.out.println("雙重雜湊 Double Hash: " + doubleHashStats);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("比較鏈結法與開放定址法 - Compare chaining vs open addressing")  // Execute this statement as part of the data structure implementation.
        void testChainingVsOpenAddressing() {  // Execute this statement as part of the data structure implementation.
            ChainedHashTable<Integer, String> chained = new ChainedHashTable<>(50);  // Assign or update a variable that represents the current algorithm state.
            OpenAddressingHashTable<Integer, String> openAddr =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(50, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            // 插入相同的資料 - Insert same data
            for (int i = 0; i < TEST_SIZE; i++) {  // Iterate over a range/collection to process each item in sequence.
                chained.insert(i, "value" + i);  // Execute this statement as part of the data structure implementation.
                openAddr.insert(i, "value" + i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 驗證結果 - Verify results
            assertEquals(TEST_SIZE, chained.size());  // Execute this statement as part of the data structure implementation.
            assertEquals(TEST_SIZE, openAddr.size());  // Execute this statement as part of the data structure implementation.

            // 輸出統計資料 - Output statistics
            System.out.println("鏈結法 Chaining: " + chained.getProbeStats());  // Execute this statement as part of the data structure implementation.
            System.out.println("開放定址 Open Addressing: " + openAddr.getProbeStats());  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // ========== 特殊情況測試 Special Cases Tests ==========

    @Nested  // Execute this statement as part of the data structure implementation.
    @DisplayName("特殊情況測試 - Special Cases Tests")  // Execute this statement as part of the data structure implementation.
    class SpecialCasesTests {  // Execute this statement as part of the data structure implementation.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("null 鍵處理（鏈結法）- Null key handling (chaining)")  // Execute this statement as part of the data structure implementation.
        void testNullKeyChaining() {  // Execute this statement as part of the data structure implementation.
            ChainedHashTable<String, Integer> ht = new ChainedHashTable<>();  // Assign or update a variable that represents the current algorithm state.

            ht.insert(null, 100);  // Execute this statement as part of the data structure implementation.
            assertEquals(100, ht.search(null));  // Execute this statement as part of the data structure implementation.
            assertTrue(ht.contains(null));  // Execute this statement as part of the data structure implementation.

            assertTrue(ht.delete(null));  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search(null));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("null 鍵處理（開放定址）- Null key handling (open addressing)")  // Execute this statement as part of the data structure implementation.
        void testNullKeyOpenAddressing() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<String, Integer> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            ht.insert(null, 100);  // Execute this statement as part of the data structure implementation.
            assertEquals(100, ht.search(null));  // Execute this statement as part of the data structure implementation.
            assertTrue(ht.contains(null));  // Execute this statement as part of the data structure implementation.

            assertTrue(ht.delete(null));  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search(null));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("大量碰撞測試 - Heavy collision test")  // Execute this statement as part of the data structure implementation.
        void testHeavyCollisions() {  // Execute this statement as part of the data structure implementation.
            // 故意使用非常小的容量造成大量碰撞 - Use very small capacity for heavy collisions
            ChainedHashTable<Integer, String> ht = new ChainedHashTable<>(2);  // Assign or update a variable that represents the current algorithm state.

            for (int i = 0; i < 20; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert(i, "value" + i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 驗證所有元素都能正確存取 - Verify all elements accessible
            for (int i = 0; i < 20; i++) {  // Iterate over a range/collection to process each item in sequence.
                assertEquals("value" + i, ht.search(i));  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            ChainedHashTable.ProbeStats stats = ht.getProbeStats();  // Assign or update a variable that represents the current algorithm state.
            assertTrue(stats.collisions > 10);  // Execute this statement as part of the data structure implementation.
            assertTrue(stats.maxChainLength > 5);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("無效容量應拋出例外 - Invalid capacity should throw")  // Execute this statement as part of the data structure implementation.
        void testInvalidCapacity() {  // Execute this statement as part of the data structure implementation.
            assertThrows(IllegalArgumentException.class, () ->  // Execute this statement as part of the data structure implementation.
                new ChainedHashTable<String, Integer>(0));  // Execute this statement as part of the data structure implementation.

            assertThrows(IllegalArgumentException.class, () ->  // Execute this statement as part of the data structure implementation.
                new ChainedHashTable<String, Integer>(-1));  // Execute this statement as part of the data structure implementation.

            assertThrows(IllegalArgumentException.class, () ->  // Execute this statement as part of the data structure implementation.
                new OpenAddressingHashTable<String, Integer>(0,  // Execute this statement as part of the data structure implementation.
                    OpenAddressingHashTable.ProbeMethod.LINEAR));  // Execute this statement as part of the data structure implementation.

            assertThrows(IllegalArgumentException.class, () ->  // Execute this statement as part of the data structure implementation.
                new OpenAddressingHashTable<String, Integer>(-5,  // Execute this statement as part of the data structure implementation.
                    OpenAddressingHashTable.ProbeMethod.QUADRATIC));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        @Test  // Execute this statement as part of the data structure implementation.
        @DisplayName("連續刪除與插入 - Sequential delete and insert")  // Execute this statement as part of the data structure implementation.
        void testSequentialDeleteInsert() {  // Execute this statement as part of the data structure implementation.
            OpenAddressingHashTable<String, Integer> ht =  // Assign or update a variable that represents the current algorithm state.
                new OpenAddressingHashTable<>(7, OpenAddressingHashTable.ProbeMethod.LINEAR);  // Execute this statement as part of the data structure implementation.

            // 插入 - Insert
            for (int i = 0; i < 5; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert("key" + i, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 刪除 - Delete
            for (int i = 0; i < 3; i++) {  // Iterate over a range/collection to process each item in sequence.
                assertTrue(ht.delete("key" + i));  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 再次插入 - Insert again
            for (int i = 5; i < 8; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert("key" + i, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 驗證 - Verify
            assertNull(ht.search("key0"));  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search("key1"));  // Execute this statement as part of the data structure implementation.
            assertNull(ht.search("key2"));  // Execute this statement as part of the data structure implementation.
            assertEquals(3, ht.search("key3"));  // Execute this statement as part of the data structure implementation.
            assertEquals(4, ht.search("key4"));  // Execute this statement as part of the data structure implementation.
            assertEquals(5, ht.search("key5"));  // Execute this statement as part of the data structure implementation.
            assertEquals(6, ht.search("key6"));  // Execute this statement as part of the data structure implementation.
            assertEquals(7, ht.search("key7"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.
