/** Doc block start
 * 雜湊表單元測試 / Hash Table Unit Tests
 *(blank line)
 * 使用 Jest 測試框架
 * Using Jest testing framework
 */  // End of block comment

const HashTable = require('./hashTable');  // Compute a hash-based index so keys map into the table's storage.

describe('HashTable', () => {  // Assign or update a variable that represents the current algorithm state.
    // ========== 基本操作測試 Basic Operations Tests ==========

    describe('建構子 Constructor', () => {  // Assign or update a variable that represents the current algorithm state.
        test('建立空的雜湊表 - Create empty hash table', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
            expect(ht.size).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.isEmpty()).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.capacity).toBe(16); // 預設容量
        });  // Execute this statement as part of the data structure implementation.

        test('使用自訂容量 - Custom capacity', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new HashTable(32);  // Assign or update a variable that represents the current algorithm state.
            expect(ht.capacity).toBe(32);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('無效容量應拋出錯誤 - Invalid capacity should throw', () => {  // Assign or update a variable that represents the current algorithm state.
            expect(() => new HashTable(0)).toThrow();  // Assign or update a variable that represents the current algorithm state.
            expect(() => new HashTable(-1)).toThrow();  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('插入與搜尋 Insert and Search', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
        });  // Execute this statement as part of the data structure implementation.

        test('插入與搜尋基本操作 - Basic insert and search', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('apple')).toBe(100);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('banana')).toBe(200);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('cherry')).toBeUndefined();  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('更新已存在的鍵 - Update existing key', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('apple', 150);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('apple')).toBe(150);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(1);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('使用 get/set 方法 - Using get/set methods', () => {  // Assign or update a variable that represents the current algorithm state.
            ht.set('apple', 100);  // Execute this statement as part of the data structure implementation.
            expect(ht.get('apple')).toBe(100);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('刪除操作 Delete', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
            ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('刪除存在的鍵 - Delete existing key', () => {  // Assign or update a variable that represents the current algorithm state.
            expect(ht.delete('apple')).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('apple')).toBeUndefined();  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(1);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('刪除不存在的鍵 - Delete nonexistent key', () => {  // Assign or update a variable that represents the current algorithm state.
            expect(ht.delete('cherry')).toBe(false);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(2);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    describe('contains/has 方法', () => {  // Assign or update a variable that represents the current algorithm state.
        test('檢查鍵是否存在 - Check if key exists', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
            ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.

            expect(ht.contains('apple')).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.contains('banana')).toBe(false);  // Execute this statement as part of the data structure implementation.
            expect(ht.has('apple')).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.has('banana')).toBe(false);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    // ========== 特殊情況測試 Special Cases Tests ==========

    describe('特殊情況 Special Cases', () => {  // Assign or update a variable that represents the current algorithm state.
        test('插入 undefined 值 - Insert undefined value', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
            ht.insert('key', undefined);  // Execute this statement as part of the data structure implementation.

            // 注意：undefined 值會導致 contains 誤判
            expect(ht.search('key')).toBeUndefined();  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('各種鍵類型 - Various key types', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
            ht.insert('string', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert(123, 2);  // Execute this statement as part of the data structure implementation.
            ht.insert(true, 3);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('string')).toBe(1);  // Execute this statement as part of the data structure implementation.
            expect(ht.search(123)).toBe(2);  // Execute this statement as part of the data structure implementation.
            expect(ht.search(true)).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('清空操作 - Clear operation', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.clear();  // Execute this statement as part of the data structure implementation.

            expect(ht.isEmpty()).toBe(true);  // Execute this statement as part of the data structure implementation.
            expect(ht.size).toBe(0);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('a')).toBeUndefined();  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    // ========== 擴容測試 Rehashing Tests ==========

    describe('擴容 Rehashing', () => {  // Compute a hash-based index so keys map into the table's storage.
        test('負載因子觸發擴容 - Rehashing on load factor threshold', () => {  // Compute a hash-based index so keys map into the table's storage.
            const ht = new HashTable(4);  // Assign or update a variable that represents the current algorithm state.

            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                ht.insert(`key${i}`, i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            expect(ht.capacity).toBeGreaterThan(4);  // Execute this statement as part of the data structure implementation.

            // 所有元素應該仍然可以存取 / 所有 elements should still 然 can 存取
            for (let i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
                expect(ht.search(`key${i}`)).toBe(i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        });  // Execute this statement as part of the data structure implementation.

        test('負載因子計算 - Load factor calculation', () => {  // Assign or update a variable that represents the current algorithm state.
            const ht = new HashTable(10);  // Assign or update a variable that represents the current algorithm state.
            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.

            expect(ht.loadFactor).toBeCloseTo(0.3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    // ========== 迭代器測試 Iterator Tests ==========

    describe('迭代器 Iterators', () => {  // Assign or update a variable that represents the current algorithm state.
        let ht;  // Execute this statement as part of the data structure implementation.

        beforeEach(() => {  // Assign or update a variable that represents the current algorithm state.
            ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.
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

        test('forEach() 方法', () => {  // Assign or update a variable that represents the current algorithm state.
            const result = [];  // Assign or update a variable that represents the current algorithm state.
            ht.forEach((value, key) => {  // Assign or update a variable that represents the current algorithm state.
                result.push([key, value]);  // Execute this statement as part of the data structure implementation.
            });  // Execute this statement as part of the data structure implementation.
            expect(result.length).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.

        test('for...of 迭代 - for...of iteration', () => {  // Assign or update a variable that represents the current algorithm state.
            let count = 0;  // Assign or update a variable that represents the current algorithm state.
            for (const [key, value] of ht) {  // Iterate over a range/collection to process each item in sequence.
                expect(typeof key).toBe('string');  // Execute this statement as part of the data structure implementation.
                expect(typeof value).toBe('number');  // Execute this statement as part of the data structure implementation.
                count++;  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
            expect(count).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.

    // ========== 碰撞測試 Collision Tests ==========

    describe('碰撞處理 Collision Handling', () => {  // Assign or update a variable that represents the current algorithm state.
        test('多個元素在同一桶 - Multiple items in same bucket', () => {  // Access or update the bucket storage used to hold entries or chains.
            const ht = new HashTable(1); // 強制所有元素碰撞

            ht.insert('a', 1);  // Execute this statement as part of the data structure implementation.
            ht.insert('b', 2);  // Execute this statement as part of the data structure implementation.
            ht.insert('c', 3);  // Execute this statement as part of the data structure implementation.

            expect(ht.search('a')).toBe(1);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('b')).toBe(2);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('c')).toBe(3);  // Execute this statement as part of the data structure implementation.

            ht.delete('b');  // Execute this statement as part of the data structure implementation.
            expect(ht.search('b')).toBeUndefined();  // Execute this statement as part of the data structure implementation.
            expect(ht.search('a')).toBe(1);  // Execute this statement as part of the data structure implementation.
            expect(ht.search('c')).toBe(3);  // Execute this statement as part of the data structure implementation.
        });  // Execute this statement as part of the data structure implementation.
    });  // Execute this statement as part of the data structure implementation.
});  // Execute this statement as part of the data structure implementation.
