/**
 * 碰撞解決雜湊表單元測試
 * Collision Resolution Hash Table Unit Tests
 *
 * 使用 Jest 測試框架
 * Using Jest testing framework
 */

const ChainedHashTable = require('./chaining');
const { OpenAddressingHashTable, ProbeMethod, DELETED } = require('./openAddressing');

// ========== 鏈結法測試 Chained Hash Table Tests ==========

describe('ChainedHashTable', () => {
    describe('建構子 Constructor', () => {
        test('建立空的鏈結雜湊表 - Create empty chained hash table', () => {
            const ht = new ChainedHashTable();
            expect(ht.size).toBe(0);
            expect(ht.isEmpty()).toBe(true);
            expect(ht.capacity).toBe(16); // 預設容量
        });

        test('使用自訂容量 - Custom capacity', () => {
            const ht = new ChainedHashTable(32);
            expect(ht.capacity).toBe(32);
        });

        test('無效容量應拋出錯誤 - Invalid capacity should throw', () => {
            expect(() => new ChainedHashTable(0)).toThrow();
            expect(() => new ChainedHashTable(-1)).toThrow();
        });
    });

    describe('插入與搜尋 Insert and Search', () => {
        let ht;

        beforeEach(() => {
            ht = new ChainedHashTable(8);
        });

        test('插入並搜尋鍵值對 - Insert and search key-value pairs', () => {
            const probes1 = ht.insert('apple', 100);
            const probes2 = ht.insert('banana', 200);
            const probes3 = ht.insert('cherry', 300);

            expect(probes1).toBeGreaterThan(0);
            expect(probes2).toBeGreaterThan(0);
            expect(probes3).toBeGreaterThan(0);

            const result1 = ht.search('apple');
            expect(result1.value).toBe(100);
            expect(result1.probes).toBeGreaterThan(0);

            const result2 = ht.search('banana');
            expect(result2.value).toBe(200);

            const result3 = ht.search('cherry');
            expect(result3.value).toBe(300);
        });

        test('搜尋不存在的鍵 - Search nonexistent key', () => {
            ht.insert('apple', 100);
            const result = ht.search('grape');
            expect(result).toBeNull();
        });

        test('更新已存在的鍵 - Update existing key', () => {
            ht.insert('apple', 100);
            const probes = ht.insert('apple', 150);

            expect(probes).toBeGreaterThan(0);
            const result = ht.search('apple');
            expect(result.value).toBe(150);
            expect(ht.size).toBe(1);
        });

        test('contains 方法 - contains method', () => {
            ht.insert('apple', 100);
            expect(ht.contains('apple')).toBe(true);
            expect(ht.contains('banana')).toBe(false);
        });
    });

    describe('刪除操作 Delete', () => {
        let ht;

        beforeEach(() => {
            ht = new ChainedHashTable(8);
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.insert('cherry', 300);
        });

        test('刪除存在的鍵 - Delete existing key', () => {
            const result = ht.delete('banana');
            expect(result.success).toBe(true);
            expect(result.probes).toBeGreaterThan(0);
            expect(ht.search('banana')).toBeNull();
            expect(ht.size).toBe(2);
        });

        test('刪除不存在的鍵 - Delete nonexistent key', () => {
            const result = ht.delete('grape');
            expect(result.success).toBe(false);
            expect(result.probes).toBeGreaterThan(0);
            expect(ht.size).toBe(3);
        });
    });

    describe('統計功能 Statistics', () => {
        test('負載因子計算 - Load factor calculation', () => {
            const ht = new ChainedHashTable(10);
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);

            expect(ht.loadFactor).toBeCloseTo(0.3);
        });

        test('平均探測次數 - Average probes', () => {
            const ht = new ChainedHashTable(8);
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.search('apple');

            const avgProbes = ht.getAverageProbes();
            expect(avgProbes).toBeGreaterThan(0);
        });

        test('最長鏈結長度 - Max chain length', () => {
            const ht = new ChainedHashTable(2); // 小容量強制碰撞
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);

            const maxChain = ht.getMaxChainLength();
            expect(maxChain).toBeGreaterThan(0);
        });

        test('完整統計資訊 - Complete statistics', () => {
            const ht = new ChainedHashTable(8);
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.search('apple');

            const stats = ht.getStatistics();
            expect(stats.size).toBe(2);
            expect(stats.capacity).toBe(8);
            expect(stats.loadFactor).toBeCloseTo(0.25);
            expect(stats.totalProbes).toBeGreaterThan(0);
            expect(stats.operations).toBe(3);
            expect(stats.averageProbes).toBeGreaterThan(0);
            expect(stats.maxChainLength).toBeGreaterThan(0);
        });
    });

    describe('碰撞處理 Collision Handling', () => {
        test('多個元素在同一桶 - Multiple items in same bucket', () => {
            const ht = new ChainedHashTable(1); // 強制所有元素碰撞

            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);

            expect(ht.search('a').value).toBe(1);
            expect(ht.search('b').value).toBe(2);
            expect(ht.search('c').value).toBe(3);
            expect(ht.getMaxChainLength()).toBe(3);
        });
    });

    describe('擴容 Rehashing', () => {
        test('負載因子觸發擴容 - Rehashing on load factor threshold', () => {
            const ht = new ChainedHashTable(4);

            for (let i = 0; i < 4; i++) {
                ht.insert(`key${i}`, i);
            }

            expect(ht.capacity).toBeGreaterThan(4);

            // 所有元素應該仍然可以存取
            for (let i = 0; i < 4; i++) {
                const result = ht.search(`key${i}`);
                expect(result.value).toBe(i);
            }
        });
    });

    describe('迭代器 Iterators', () => {
        let ht;

        beforeEach(() => {
            ht = new ChainedHashTable(8);
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);
        });

        test('keys() 方法', () => {
            const keys = ht.keys();
            expect(keys.sort()).toEqual(['a', 'b', 'c']);
        });

        test('values() 方法', () => {
            const values = ht.values();
            expect(values.sort()).toEqual([1, 2, 3]);
        });

        test('entries() 方法', () => {
            const entries = ht.entries();
            expect(entries.length).toBe(3);
        });
    });

    describe('清空操作 Clear', () => {
        test('清空雜湊表 - Clear hash table', () => {
            const ht = new ChainedHashTable(8);
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.clear();

            expect(ht.isEmpty()).toBe(true);
            expect(ht.size).toBe(0);
            expect(ht.search('a')).toBeNull();
            expect(ht.getStatistics().totalProbes).toBe(0);
        });
    });
});

// ========== 開放定址法測試 Open Addressing Hash Table Tests ==========

describe('OpenAddressingHashTable', () => {
    describe('建構子 Constructor', () => {
        test('建立空的開放定址雜湊表 - Create empty open addressing hash table', () => {
            const ht = new OpenAddressingHashTable();
            expect(ht.size).toBe(0);
            expect(ht.isEmpty()).toBe(true);
            expect(ht.capacity).toBe(16); // 預設容量
            expect(ht.probeMethod).toBe(ProbeMethod.LINEAR);
        });

        test('使用自訂容量和探測方法 - Custom capacity and probe method', () => {
            const ht = new OpenAddressingHashTable(32, ProbeMethod.QUADRATIC);
            expect(ht.capacity).toBe(32);
            expect(ht.probeMethod).toBe(ProbeMethod.QUADRATIC);
        });

        test('無效容量應拋出錯誤 - Invalid capacity should throw', () => {
            expect(() => new OpenAddressingHashTable(0)).toThrow();
            expect(() => new OpenAddressingHashTable(-1)).toThrow();
        });

        test('無效探測方法應拋出錯誤 - Invalid probe method should throw', () => {
            expect(() => new OpenAddressingHashTable(16, 'INVALID')).toThrow();
        });
    });

    describe('線性探測 Linear Probing', () => {
        let ht;

        beforeEach(() => {
            ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
        });

        test('插入並搜尋鍵值對 - Insert and search key-value pairs', () => {
            const probes1 = ht.insert('apple', 100);
            const probes2 = ht.insert('banana', 200);
            const probes3 = ht.insert('cherry', 300);

            expect(probes1).toBeGreaterThan(0);
            expect(probes2).toBeGreaterThan(0);
            expect(probes3).toBeGreaterThan(0);

            const result1 = ht.search('apple');
            expect(result1.value).toBe(100);
            expect(result1.probes).toBeGreaterThan(0);

            const result2 = ht.search('banana');
            expect(result2.value).toBe(200);

            const result3 = ht.search('cherry');
            expect(result3.value).toBe(300);
        });

        test('搜尋不存在的鍵 - Search nonexistent key', () => {
            ht.insert('apple', 100);
            const result = ht.search('grape');
            expect(result).toBeNull();
        });

        test('更新已存在的鍵 - Update existing key', () => {
            ht.insert('apple', 100);
            ht.insert('apple', 150);

            const result = ht.search('apple');
            expect(result.value).toBe(150);
            expect(ht.size).toBe(1);
        });

        test('刪除操作 - Delete operation', () => {
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.insert('cherry', 300);

            const delResult = ht.delete('banana');
            expect(delResult.success).toBe(true);
            expect(delResult.probes).toBeGreaterThan(0);
            expect(ht.search('banana')).toBeNull();
            expect(ht.size).toBe(2);

            // 刪除後仍能找到其他元素
            expect(ht.search('apple').value).toBe(100);
            expect(ht.search('cherry').value).toBe(300);
        });

        test('墓碑機制 - Tombstone mechanism', () => {
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.delete('apple');

            // 刪除後可在相同位置插入新元素
            ht.insert('date', 400);
            expect(ht.search('date').value).toBe(400);
            expect(ht.search('banana').value).toBe(200);
        });
    });

    describe('二次探測 Quadratic Probing', () => {
        let ht;

        beforeEach(() => {
            ht = new OpenAddressingHashTable(8, ProbeMethod.QUADRATIC);
        });

        test('插入並搜尋 - Insert and search', () => {
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.insert('cherry', 300);

            expect(ht.search('apple').value).toBe(100);
            expect(ht.search('banana').value).toBe(200);
            expect(ht.search('cherry').value).toBe(300);
        });

        test('碰撞處理 - Collision handling', () => {
            // 插入多個元素測試二次探測
            for (let i = 0; i < 4; i++) {
                ht.insert(`key${i}`, i);
            }

            for (let i = 0; i < 4; i++) {
                expect(ht.search(`key${i}`).value).toBe(i);
            }
        });
    });

    describe('雙重雜湊 Double Hashing', () => {
        let ht;

        beforeEach(() => {
            ht = new OpenAddressingHashTable(8, ProbeMethod.DOUBLE_HASH);
        });

        test('插入並搜尋 - Insert and search', () => {
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.insert('cherry', 300);

            expect(ht.search('apple').value).toBe(100);
            expect(ht.search('banana').value).toBe(200);
            expect(ht.search('cherry').value).toBe(300);
        });

        test('碰撞處理 - Collision handling', () => {
            // 插入多個元素測試雙重雜湊
            for (let i = 0; i < 4; i++) {
                ht.insert(`key${i}`, i);
            }

            for (let i = 0; i < 4; i++) {
                expect(ht.search(`key${i}`).value).toBe(i);
            }
        });
    });

    describe('統計功能 Statistics', () => {
        test('負載因子計算 - Load factor calculation', () => {
            const ht = new OpenAddressingHashTable(10, ProbeMethod.LINEAR);
            ht.insert('a', 1);
            ht.insert('b', 2);

            expect(ht.loadFactor).toBeCloseTo(0.2);
        });

        test('平均探測次數 - Average probes', () => {
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.search('apple');

            const avgProbes = ht.getAverageProbes();
            expect(avgProbes).toBeGreaterThan(0);
        });

        test('完整統計資訊 - Complete statistics', () => {
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
            ht.insert('apple', 100);
            ht.insert('banana', 200);
            ht.delete('apple');

            const stats = ht.getStatistics();
            expect(stats.size).toBe(1);
            expect(stats.capacity).toBe(8);
            expect(stats.probeMethod).toBe(ProbeMethod.LINEAR);
            expect(stats.totalProbes).toBeGreaterThan(0);
            expect(stats.operations).toBe(3);
            expect(stats.averageProbes).toBeGreaterThan(0);
            expect(stats.deletedSlots).toBeGreaterThan(0);
        });
    });

    describe('擴容 Rehashing', () => {
        test('負載因子觸發擴容 - Rehashing on load factor threshold', () => {
            const ht = new OpenAddressingHashTable(4, ProbeMethod.LINEAR);

            // 插入足夠元素觸發擴容 (負載因子 0.5)
            ht.insert('key0', 0);
            ht.insert('key1', 1);
            ht.insert('key2', 2);

            expect(ht.capacity).toBeGreaterThan(4);

            // 所有元素應該仍然可以存取
            expect(ht.search('key0').value).toBe(0);
            expect(ht.search('key1').value).toBe(1);
            expect(ht.search('key2').value).toBe(2);
        });
    });

    describe('迭代器 Iterators', () => {
        let ht;

        beforeEach(() => {
            ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);
        });

        test('keys() 方法', () => {
            const keys = ht.keys();
            expect(keys.sort()).toEqual(['a', 'b', 'c']);
        });

        test('values() 方法', () => {
            const values = ht.values();
            expect(values.sort()).toEqual([1, 2, 3]);
        });

        test('entries() 方法', () => {
            const entries = ht.entries();
            expect(entries.length).toBe(3);
        });

        test('刪除後的迭代 - Iteration after deletion', () => {
            ht.delete('b');
            const keys = ht.keys();
            expect(keys.sort()).toEqual(['a', 'c']);
            expect(keys.length).toBe(2);
        });
    });

    describe('清空操作 Clear', () => {
        test('清空雜湊表 - Clear hash table', () => {
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.clear();

            expect(ht.isEmpty()).toBe(true);
            expect(ht.size).toBe(0);
            expect(ht.search('a')).toBeNull();
            expect(ht.getStatistics().totalProbes).toBe(0);
            expect(ht.getStatistics().deletedSlots).toBe(0);
        });
    });

    describe('contains 方法', () => {
        test('檢查鍵是否存在 - Check if key exists', () => {
            const ht = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
            ht.insert('apple', 100);

            expect(ht.contains('apple')).toBe(true);
            expect(ht.contains('banana')).toBe(false);
        });
    });
});

// ========== 探測方法比較測試 Probe Method Comparison Tests ==========

describe('探測方法比較 Probe Method Comparison', () => {
    const testData = [
        ['apple', 100],
        ['banana', 200],
        ['cherry', 300],
        ['date', 400],
        ['elderberry', 500]
    ];

    test('比較不同探測方法的效能 - Compare performance of different probe methods', () => {
        const linearHT = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);
        const quadraticHT = new OpenAddressingHashTable(8, ProbeMethod.QUADRATIC);
        const doubleHT = new OpenAddressingHashTable(8, ProbeMethod.DOUBLE_HASH);

        // 插入相同的資料
        for (const [key, value] of testData) {
            linearHT.insert(key, value);
            quadraticHT.insert(key, value);
            doubleHT.insert(key, value);
        }

        // 取得統計資訊
        const linearStats = linearHT.getStatistics();
        const quadraticStats = quadraticHT.getStatistics();
        const doubleStats = doubleHT.getStatistics();

        // 所有方法都應該成功插入
        expect(linearStats.size).toBe(testData.length);
        expect(quadraticStats.size).toBe(testData.length);
        expect(doubleStats.size).toBe(testData.length);

        // 驗證資料完整性
        for (const [key, value] of testData) {
            expect(linearHT.search(key).value).toBe(value);
            expect(quadraticHT.search(key).value).toBe(value);
            expect(doubleHT.search(key).value).toBe(value);
        }
    });
});
