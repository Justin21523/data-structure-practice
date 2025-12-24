/** Doc block start
 * 碰撞解決雜湊表單元測試 / Collision Resolution Hash Table Unit Tests
 *(blank line)
 * 使用 Jest 測試框架
 * Using Jest testing framework
 */  // End of block comment

const ChainedHashTable = require('./chaining');  // Assign or update a variable that represents the current algorithm state.
const { OpenAddressingHashTable, ProbeMethod, DELETED } = require('./openAddressing');  // Handle tombstones so deletions do not break the probing/search sequence.

// ========== 鏈結法測試 Chained Hash Table Tests ==========

describe('ChainedHashTable', () => {  // Assign or update a variable that represents the current algorithm state.
    describe('建構子 Constructor', () => {  // Assign or update a variable that represents the current algorithm state.
        test('建立空的鏈結雜湊表 - Create empty chained hash table', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new ChainedHashTable();  // Assign or update a variable that represents the current algorithm state.
            expect(ht.size).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.isEmpty()).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.capacity).toBe(16); // 預設容量
        });  // Execute this statement as part of the data structure implementation.

        test('使用自訂容量 - Custom capacity', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new ChainedHashTable(32);  // Assign or update a variable that represents the current algorithm state.
            expect(ht.capacity).toBe(32);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('無效容量應拋出錯誤 - Invalid capacity should throw', () => {  // Assign or update a variable that represents the current algorithm state.
            expect(() => new ChainedHashTable(0)).toThrow();  // Assign or update a variable that represents the current algorithm state.
            expect(() => new ChainedHashTable(-1)).toThrow();  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('插入與搜尋 Insert and Search', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.

        test('插入並搜尋鍵值對 - Insert and search key-value pairs', () => {  // Assign or update a variable that represents the current algorithm state.
            const probes1 = ht.insert('apple', 100);  // Advance or track the probing sequence used by open addressing.
            const probes2 = ht.insert('banana', 200);  // Advance or track the probing sequence used by open addressing.
            const probes3 = ht.insert('cherry', 300);  // Advance or track the probing sequence used by open addressing.

            expect(probes1).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(probes2).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(probes3).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.

            const result1 = ht.search('apple');  // Assign or update a variable that represents the current algorithm state.
            expect(result1.value).toBe(100);  // Execute this statement as part of the data structure implementation.
            expect(result1.probes).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.

            const result2 = ht.search('banana');  // Assign or update a variable that represents the current algorithm state.
            expect(result2.value).toBe(200);  // Execute this statement as part of the data structure implementation.

            const result3 = ht.search('cherry');  // Assign or update a variable that represents the current algorithm state.
            expect(result3.value).toBe(300);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('搜尋不存在的鍵 - Search nonexistent key', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            const result = ht.search('grape');  // Assign or update a variable that represents the current algorithm state.
            expect(result).toBeNull();  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('更新已存在的鍵 - Update existing key', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            const probes = ht.insert('apple', 150);  // Advance or track the probing sequence used by open addressing.

            expect(probes).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            const result = ht.search('apple');  // Assign or update a variable that represents the current algorithm state.
            expect(result.value).toBe(150);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(1);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('contains 方法 - contains method', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            expect(ht.contains('apple')).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.contains('banana')).toBe(false);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('刪除操作 Delete', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('刪除存在的鍵 - Delete existing key', () => {  // Assign or update a variable that represents the current algorithm state.
            const result = ht.delete('banana');  // Assign or update a variable that represents the current algorithm state.
            expect(result.success).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(result.probes).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(ht.search('banana')).toBeNull();  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(2);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('刪除不存在的鍵 - Delete nonexistent key', () => {  // Assign or update a variable that represents the current algorithm state.
            const result = ht.delete('grape');  // Assign or update a variable that represents the current algorithm state.
            expect(result.success).toBe(false);  // Execute this statement as part of the data structure implementation.
            expect(result.probes).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(ht.size).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('統計功能 Statistics', () => {  // Assign or update a variable that represents the current algorithm state.
        test('負載因子計算 - Load factor calculation', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new ChainedHashTable(10);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.

            expect(ht.loadFactor).toBeCloseTo(0.3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('平均探測次數 - Average probes', () => {  // Advance or track the probing sequence used by open addressing.
            const ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.search('apple');  // Execute this statement as part of the data structure implementation.

            const avgProbes = ht.getAverageProbes();  // Assign or update a variable that represents the current algorithm state.
            expect(avgProbes).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('最長鏈結長度 - Max chain length', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new ChainedHashTable(2); // 小容量強制碰撞
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.

            const maxChain = ht.getMaxChainLength();  // Assign or update a variable that represents the current algorithm state.
            expect(maxChain).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('完整統計資訊 - Complete statistics', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.search('apple');  // Execute this statement as part of the data structure implementation.

            const stats = ht.getStatistics();  // Assign or update a variable that represents the current algorithm state.
            expect(stats.size).toBe(2);  // Execute this statement as part of the data structure implementation.
            expect(stats.capacity).toBe(8);  // Execute this statement as part of the data structure implementation.
            expect(stats.loadFactor).toBeCloseTo(0.25);  // Execute this statement as part of the data structure implementation.
            expect(stats.totalProbes).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
            expect(stats.operations).toBe(3);  // Execute this statement as part of the data structure implementation.
            expect(stats.averageProbes).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
            expect(stats.maxChainLength).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('碰撞處理 Collision Handling', () => {  // Assign or update a variable that represents the current algorithm state.
        test('多個元素在同一桶 - Multiple items in same bucket', () => {  // Access or update the bucket storage used to hold entries or chains.
            const ht = new ChainedHashTable(1); // 強制所有元素碰撞

            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('a').value).toBe(1);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('b').value).toBe(2);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('c').value).toBe(3);  // Execute this statement as part of the data structure implementation.
            expect(ht.getMaxChainLength()).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('擴容 Rehashing', () => {  // Compute a hash-based index so keys map into the table's storage.
        test('負載因子觸發擴容 - Rehashing on load factor threshold', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new ChainedHashTable(4);  // Assign or update a variable that represents the current algorithm state.

            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert(`key${i}`, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            expect(ht.capacity).toBeGreaterThan(4);  // Execute this statement as part of the data structure implementation.

            // 所有元素應該仍然可以存取 / 所有 elements should still 然 can 存取
            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                const result = ht.search(`key${i}`);  // Assign or update a variable that represents the current algorithm state.
                expect(result.value).toBe(i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('迭代器 Iterators', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('keys() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const keys = ht.keys();  // Assign or update a variable that represents the current algorithm state.
            expect(keys.sort()).toEqual(['a', 'b', 'c']);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('values() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const values = ht.values();  // Assign or update a variable that represents the current algorithm state.
            expect(values.sort()).toEqual([1, 2, 3]);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('entries() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const entries = ht.entries();  // Assign or update a variable that represents the current algorithm state.
            expect(entries.length).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('清空操作 Clear', () => {  // Assign or update a variable that represents the current algorithm state.
        test('清空雜湊表 - Clear hash table', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.clear();  // Execute this statement as part of the data structure implementation.

            expect(ht.isEmpty()).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('a')).toBeNull();  // Execute this statement as part of the data structure implementation.
            expect(ht.getStatistics().totalProbes).toBe(0);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.
});  // Execute this statement as part of the data structure implementation.

// ========== 開放定址法測試 Open Addressing Hash Table Tests ==========

describe('OpenAddressingHashTable', () => {  // Assign or update a variable that represents the current algorithm state.
    describe('建構子 Constructor', () => {  // Assign or update a variable that represents the current algorithm state.
        test('建立空的開放定址雜湊表 - Create empty open addressing hash table', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new OpenAddressingHashTable();  // Assign or update a variable that represents the current algorithm state.
            expect(ht.size).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.isEmpty()).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.capacity).toBe(16); // 預設容量
            expect(ht.probeMethod).toBe(ProbeMethod.LINEAR);  // Advance or track the probing sequence used by open addressing.
        });  // Execute this statement as part of the data structure implementation.

        test('使用自訂容量和探測方法 - Custom capacity and probe method', () => {  // Advance or track the probing sequence used by open addressing.
            const ht = new OpenAddressingHashTable(32, ProbeMethod.QUADRATIC);  // Assign or update a variable that represents the current algorithm state.
            expect(ht.capacity).toBe(32);  // Execute this statement as part of the data structure implementation.
            expect(ht.probeMethod).toBe(ProbeMethod.QUADRATIC);  // Advance or track the probing sequence used by open addressing.
        });  // Execute this statement as part of the data structure implementation.

        test('無效容量應拋出錯誤 - Invalid capacity should throw', () => {  // Assign or update a variable that represents the current algorithm state.
            expect(() => new OpenAddressingHashTable(0)).toThrow();  // Assign or update a variable that represents the current algorithm state.
            expect(() => new OpenAddressingHashTable(-1)).toThrow();  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.

        test('無效探測方法應拋出錯誤 - Invalid probe method should throw', () => {  // Advance or track the probing sequence used by open addressing.
            expect(() => new OpenAddressingHashTable(16, 'INVALID')).toThrow();  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('線性探測 Linear Probing', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.

        test('插入並搜尋鍵值對 - Insert and search key-value pairs', () => {  // Assign or update a variable that represents the current algorithm state.
            const probes1 = ht.insert('apple', 100);  // Advance or track the probing sequence used by open addressing.
            const probes2 = ht.insert('banana', 200);  // Advance or track the probing sequence used by open addressing.
            const probes3 = ht.insert('cherry', 300);  // Advance or track the probing sequence used by open addressing.

            expect(probes1).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(probes2).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(probes3).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.

            const result1 = ht.search('apple');  // Assign or update a variable that represents the current algorithm state.
            expect(result1.value).toBe(100);  // Execute this statement as part of the data structure implementation.
            expect(result1.probes).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.

            const result2 = ht.search('banana');  // Assign or update a variable that represents the current algorithm state.
            expect(result2.value).toBe(200);  // Execute this statement as part of the data structure implementation.

            const result3 = ht.search('cherry');  // Assign or update a variable that represents the current algorithm state.
            expect(result3.value).toBe(300);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('搜尋不存在的鍵 - Search nonexistent key', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            const result = ht.search('grape');  // Assign or update a variable that represents the current algorithm state.
            expect(result).toBeNull();  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('更新已存在的鍵 - Update existing key', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('apple', 150);  // Execute this statement as part of the data structure implementation.

            const result = ht.search('apple');  // Assign or update a variable that represents the current algorithm state.
            expect(result.value).toBe(150);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(1);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('刪除操作 - Delete operation', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.

            const delResult = ht.delete('banana');  // Assign or update a variable that represents the current algorithm state.
            expect(delResult.success).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(delResult.probes).toBeGreaterThan(0);  // Advance or track the probing sequence used by open addressing.
            expect(ht.search('banana')).toBeNull();  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(2);  // Execute this statement as part of the data structure implementation.

            // 刪除後仍能找到其他元素 / Delete 後 still 能 be found 其他 elements
            expect(ht.search('apple').value).toBe(100);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('cherry').value).toBe(300);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('墓碑機制 - Tombstone mechanism', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.delete('apple');  // Execute this statement as part of the data structure implementation.

            // 刪除後可在相同位置插入新元素 / Delete 後可在相同位置 insert new elements
            ht.insert('date', 400);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('date').value).toBe(400);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('banana').value).toBe(200);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('二次探測 Quadratic Probing', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new OpenAddressingHashTable(8, ProbeMethod.QUADRATIC);  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.

        test('插入並搜尋 - Insert and search', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('apple').value).toBe(100);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('banana').value).toBe(200);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('cherry').value).toBe(300);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('碰撞處理 - Collision handling', () => {  // Assign or update a variable that represents the current algorithm state.
            // 插入多個元素測試二次探測 / Insert 多個 elements test quadratic probing
            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert(`key${i}`, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                expect(ht.search(`key${i}`).value).toBe(i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('雙重雜湊 Double Hashing', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new OpenAddressingHashTable(8, ProbeMethod.DOUBLE_HASH);  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.

        test('插入並搜尋 - Insert and search', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('apple').value).toBe(100);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('banana').value).toBe(200);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('cherry').value).toBe(300);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('碰撞處理 - Collision handling', () => {  // Assign or update a variable that represents the current algorithm state.
            // 插入多個元素測試雙重雜湊 / Insert 多個 elements test double hashing
            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert(`key${i}`, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                expect(ht.search(`key${i}`).value).toBe(i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('統計功能 Statistics', () => {  // Assign or update a variable that represents the current algorithm state.
        test('負載因子計算 - Load factor calculation', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new OpenAddressingHashTable(10, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.

            expect(ht.loadFactor).toBeCloseTo(0.2);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('平均探測次數 - Average probes', () => {  // Advance or track the probing sequence used by open addressing.
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.search('apple');  // Execute this statement as part of the data structure implementation.

            const avgProbes = ht.getAverageProbes();  // Assign or update a variable that represents the current algorithm state.
            expect(avgProbes).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('完整統計資訊 - Complete statistics', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
            ht.delete('apple');  // Execute this statement as part of the data structure implementation.

            const stats = ht.getStatistics();  // Assign or update a variable that represents the current algorithm state.
            expect(stats.size).toBe(1);  // Execute this statement as part of the data structure implementation.
            expect(stats.capacity).toBe(8);  // Execute this statement as part of the data structure implementation.
            expect(stats.probeMethod).toBe(ProbeMethod.LINEAR);  // Advance or track the probing sequence used by open addressing.
            expect(stats.totalProbes).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
            expect(stats.operations).toBe(3);  // Execute this statement as part of the data structure implementation.
            expect(stats.averageProbes).toBeGreaterThan(0);  // Execute this statement as part of the data structure implementation.
            expect(stats.deletedSlots).toBeGreaterThan(0);  // Handle tombstones so deletions do not break the probing/search sequence.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('擴容 Rehashing', () => {  // Compute a hash-based index so keys map into the table's storage.
        test('負載因子觸發擴容 - Rehashing on load factor threshold', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new OpenAddressingHashTable(4, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.

            // 插入足夠元素觸發擴容 (負載因子 0.5) / Insert 足夠 elements 觸發 resize ( load factor 0.5)
            ht.insert('key0', 0);  // Execute this statement as part of the data structure implementation.
            ht.insert('key1', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('key2', 2);  // Execute this statement as part of the data structure implementation.

            expect(ht.capacity).toBeGreaterThan(4);  // Execute this statement as part of the data structure implementation.

            // 所有元素應該仍然可以存取 / 所有 elements should still 然 can 存取
            expect(ht.search('key0').value).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('key1').value).toBe(1);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('key2').value).toBe(2);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('迭代器 Iterators', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('keys() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const keys = ht.keys();  // Assign or update a variable that represents the current algorithm state.
            expect(keys.sort()).toEqual(['a', 'b', 'c']);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('values() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const values = ht.values();  // Assign or update a variable that represents the current algorithm state.
            expect(values.sort()).toEqual([1, 2, 3]);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('entries() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const entries = ht.entries();  // Assign or update a variable that represents the current algorithm state.
            expect(entries.length).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('刪除後的迭代 - Iteration after deletion', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.delete('b');  // Execute this statement as part of the data structure implementation.
            const keys = ht.keys();  // Assign or update a variable that represents the current algorithm state.
            expect(keys.sort()).toEqual(['a', 'c']);  // Execute this statement as part of the data structure implementation.
            expect(keys.length).toBe(2);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('清空操作 Clear', () => {  // Assign or update a variable that represents the current algorithm state.
        test('清空雜湊表 - Clear hash table', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.clear();  // Execute this statement as part of the data structure implementation.

            expect(ht.isEmpty()).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('a')).toBeNull();  // Execute this statement as part of the data structure implementation.
            expect(ht.getStatistics().totalProbes).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.getStatistics().deletedSlots).toBe(0);  // Handle tombstones so deletions do not break the probing/search sequence.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('contains 方法', () => {  // Assign or update a variable that represents the current algorithm state.
        test('檢查鍵是否存在 - Check if key exists', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.

            expect(ht.contains('apple')).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.contains('banana')).toBe(false);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.
});  // Execute this statement as part of the data structure implementation.

// ========== 探測方法比較測試 Probe Method Comparison Tests ==========

describe('探測方法比較 Probe Method Comparison', () => {  // Assign or update a variable that represents the current algorithm state.
    const testData = [  // Assign or update a variable that represents the current algorithm state.
        ['apple', 100],  // Execute this statement as part of the data structure implementation.
        ['banana', 200],  // Execute this statement as part of the data structure implementation.
        ['cherry', 300],  // Execute this statement as part of the data structure implementation.
        ['date', 400],  // Execute this statement as part of the data structure implementation.
        ['elderberry', 500]  // Execute this statement as part of the data structure implementation.
    ];  // Execute this statement as part of the data structure implementation.

    test('比較不同探測方法的效能 - Compare performance of different probe methods', () => {  // Advance or track the probing sequence used by open addressing.
        const linearHT = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.
        const quadraticHT = new OpenAddressingHashTable(8, ProbeMethod.QUADRATIC);  // Assign or update a variable that represents the current algorithm state.
        const doubleHT = new OpenAddressingHashTable(8, ProbeMethod.DOUBLE_HASH);  // Assign or update a variable that represents the current algorithm state.

        // 插入相同的資料 / Insert 相同的資料
        for (const [key, value] of testData) {  // Iterate over a range/collection to process each item in sequence.
            linearHT.insert(key, value);  // Execute this statement as part of the data structure implementation.
            quadraticHT.insert(key, value);  // Execute this statement as part of the data structure implementation.
            doubleHT.insert(key, value);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // 取得統計資訊 / Get count 資訊
        const linearStats = linearHT.getStatistics();  // Assign or update a variable that represents the current algorithm state.
        const quadraticStats = quadraticHT.getStatistics();  // Assign or update a variable that represents the current algorithm state.
        const doubleStats = doubleHT.getStatistics();  // Assign or update a variable that represents the current algorithm state.

        // 所有方法都應該成功插入 / 所有方法都 should 成功 insert
        expect(linearStats.size).toBe(testData.length);  // Execute this statement as part of the data structure implementation.
        expect(quadraticStats.size).toBe(testData.length);  // Execute this statement as part of the data structure implementation.
        expect(doubleStats.size).toBe(testData.length);  // Execute this statement as part of the data structure implementation.

        // 驗證資料完整性 / English translation
        for (const [key, value] of testData) {  // Iterate over a range/collection to process each item in sequence.
            expect(linearHT.search(key).value).toBe(value);  // Execute this statement as part of the data structure implementation.
            expect(quadraticHT.search(key).value).toBe(value);  // Execute this statement as part of the data structure implementation.
            expect(doubleHT.search(key).value).toBe(value);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    });  // Execute this statement as part of the data structure implementation.
});  // Execute this statement as part of the data structure implementation.
