/**
 * 雜湊表單元測試
 * Hash Table Unit Tests
 *
 * 使用 Jest 測試框架
 * Using Jest testing framework
 */

const HashTable = require('./hashTable');

describe('HashTable', () => {
    // ========== 基本操作測試 Basic Operations Tests ==========

    describe('建構子 Constructor', () => {
        test('建立空的雜湊表 - Create empty hash table', () => {
            const ht = new HashTable();
            expect(ht.size).toBe(0);
            expect(ht.isEmpty()).toBe(true);
            expect(ht.capacity).toBe(16); // 預設容量
        });

        test('使用自訂容量 - Custom capacity', () => {
            const ht = new HashTable(32);
            expect(ht.capacity).toBe(32);
        });

        test('無效容量應拋出錯誤 - Invalid capacity should throw', () => {
            expect(() => new HashTable(0)).toThrow();
            expect(() => new HashTable(-1)).toThrow();
        });
    });

    describe('插入與搜尋 Insert and Search', () => {
        let ht;

        beforeEach(() => {
            ht = new HashTable();
        });

        test('插入與搜尋基本操作 - Basic insert and search', () => {
            ht.insert('apple', 100);
            ht.insert('banana', 200);

            expect(ht.search('apple')).toBe(100);
            expect(ht.search('banana')).toBe(200);
            expect(ht.search('cherry')).toBeUndefined();
        });

        test('更新已存在的鍵 - Update existing key', () => {
            ht.insert('apple', 100);
            ht.insert('apple', 150);

            expect(ht.search('apple')).toBe(150);
            expect(ht.size).toBe(1);
        });

        test('使用 get/set 方法 - Using get/set methods', () => {
            ht.set('apple', 100);
            expect(ht.get('apple')).toBe(100);
        });
    });

    describe('刪除操作 Delete', () => {
        let ht;

        beforeEach(() => {
            ht = new HashTable();
            ht.insert('apple', 100);
            ht.insert('banana', 200);
        });

        test('刪除存在的鍵 - Delete existing key', () => {
            expect(ht.delete('apple')).toBe(true);
            expect(ht.search('apple')).toBeUndefined();
            expect(ht.size).toBe(1);
        });

        test('刪除不存在的鍵 - Delete nonexistent key', () => {
            expect(ht.delete('cherry')).toBe(false);
            expect(ht.size).toBe(2);
        });
    });

    describe('contains/has 方法', () => {
        test('檢查鍵是否存在 - Check if key exists', () => {
            const ht = new HashTable();
            ht.insert('apple', 100);

            expect(ht.contains('apple')).toBe(true);
            expect(ht.contains('banana')).toBe(false);
            expect(ht.has('apple')).toBe(true);
            expect(ht.has('banana')).toBe(false);
        });
    });

    // ========== 特殊情況測試 Special Cases Tests ==========

    describe('特殊情況 Special Cases', () => {
        test('插入 undefined 值 - Insert undefined value', () => {
            const ht = new HashTable();
            ht.insert('key', undefined);

            // 注意：undefined 值會導致 contains 誤判
            expect(ht.search('key')).toBeUndefined();
        });

        test('各種鍵類型 - Various key types', () => {
            const ht = new HashTable();
            ht.insert('string', 1);
            ht.insert(123, 2);
            ht.insert(true, 3);

            expect(ht.search('string')).toBe(1);
            expect(ht.search(123)).toBe(2);
            expect(ht.search(true)).toBe(3);
        });

        test('清空操作 - Clear operation', () => {
            const ht = new HashTable();
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.clear();

            expect(ht.isEmpty()).toBe(true);
            expect(ht.size).toBe(0);
            expect(ht.search('a')).toBeUndefined();
        });
    });

    // ========== 擴容測試 Rehashing Tests ==========

    describe('擴容 Rehashing', () => {
        test('負載因子觸發擴容 - Rehashing on load factor threshold', () => {
            const ht = new HashTable(4);

            for (let i = 0; i < 4; i++) {
                ht.insert(`key${i}`, i);
            }

            expect(ht.capacity).toBeGreaterThan(4);

            // 所有元素應該仍然可以存取
            for (let i = 0; i < 4; i++) {
                expect(ht.search(`key${i}`)).toBe(i);
            }
        });

        test('負載因子計算 - Load factor calculation', () => {
            const ht = new HashTable(10);
            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);

            expect(ht.loadFactor).toBeCloseTo(0.3);
        });
    });

    // ========== 迭代器測試 Iterator Tests ==========

    describe('迭代器 Iterators', () => {
        let ht;

        beforeEach(() => {
            ht = new HashTable();
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

        test('forEach() 方法', () => {
            const result = [];
            ht.forEach((value, key) => {
                result.push([key, value]);
            });
            expect(result.length).toBe(3);
        });

        test('for...of 迭代 - for...of iteration', () => {
            let count = 0;
            for (const [key, value] of ht) {
                expect(typeof key).toBe('string');
                expect(typeof value).toBe('number');
                count++;
            }
            expect(count).toBe(3);
        });
    });

    // ========== 碰撞測試 Collision Tests ==========

    describe('碰撞處理 Collision Handling', () => {
        test('多個元素在同一桶 - Multiple items in same bucket', () => {
            const ht = new HashTable(1); // 強制所有元素碰撞

            ht.insert('a', 1);
            ht.insert('b', 2);
            ht.insert('c', 3);

            expect(ht.search('a')).toBe(1);
            expect(ht.search('b')).toBe(2);
            expect(ht.search('c')).toBe(3);

            ht.delete('b');
            expect(ht.search('b')).toBeUndefined();
            expect(ht.search('a')).toBe(1);
            expect(ht.search('c')).toBe(3);
        });
    });
});
