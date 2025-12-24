/**
 * 雜湊表（Hash Table）- JavaScript 實作
 * 使用鏈結法（chaining）處理碰撞
 *
 * Hash Table implementation using chaining for collision resolution
 */

class HashTable {
    // ========== 常數 Constants ==========
    static DEFAULT_CAPACITY = 16;
    static MAX_LOAD_FACTOR = 0.75;

    /**
     * 建構子：初始化雜湊表
     * Constructor: Initialize hash table
     *
     * @param {number} capacity - 桶的數量（number of buckets）
     */
    constructor(capacity = HashTable.DEFAULT_CAPACITY) {
        if (capacity <= 0) {
            throw new Error('容量必須為正整數 / Capacity must be positive');
        }
        this._capacity = capacity;
        this._size = 0;
        // 使用陣列的陣列來實作鏈結法
        // Use array of arrays for chaining
        this._buckets = new Array(capacity).fill(null).map(() => []);
    }

    // ========== 私有方法 Private Methods ==========

    /**
     * 計算雜湊值
     * Compute hash value for the given key
     *
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 桶的索引（bucket index）
     */
    _hash(key) {
        // 將 key 轉為字串後計算雜湊值
        // Convert key to string and compute hash
        const strKey = String(key);
        let hash = 0;

        for (let i = 0; i < strKey.length; i++) {
            // 使用常見的雜湊算法
            // Using common hash algorithm (djb2 variant)
            hash = ((hash << 5) - hash + strKey.charCodeAt(i)) | 0;
        }

        // 確保回傳正數索引 - Ensure positive index
        return Math.abs(hash) % this._capacity;
    }

    /**
     * 擴容並重新雜湊
     * Resize and rehash
     */
    _rehash() {
        const oldBuckets = this._buckets;
        this._capacity *= 2;
        this._buckets = new Array(this._capacity).fill(null).map(() => []);
        this._size = 0;

        // 重新插入所有元素 - Reinsert all elements
        for (const bucket of oldBuckets) {
            for (const [key, value] of bucket) {
                this.insert(key, value);
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
     * @param {*} key - 鍵
     * @param {*} value - 值
     */
    insert(key, value) {
        const index = this._hash(key);
        const bucket = this._buckets[index];

        // 檢查 key 是否已存在 - Check if key exists
        for (let i = 0; i < bucket.length; i++) {
            if (bucket[i][0] === key) {
                bucket[i][1] = value; // 更新 - Update existing
                return;
            }
        }

        // 新增鍵值對 - Add new key-value pair
        bucket.push([key, value]);
        this._size++;

        // 檢查是否需要擴容 - Check if rehashing needed
        if (this.loadFactor > HashTable.MAX_LOAD_FACTOR) {
            this._rehash();
        }
    }

    /**
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *
     * @param {*} key - 要搜尋的鍵
     * @returns {*} 找到則回傳 value，否則回傳 undefined
     */
    search(key) {
        const index = this._hash(key);
        const bucket = this._buckets[index];

        for (const [k, v] of bucket) {
            if (k === key) {
                return v;
            }
        }
        return undefined;
    }

    /**
     * 刪除指定的鍵值對
     * Delete key-value pair
     *
     * @param {*} key - 要刪除的鍵
     * @returns {boolean} 刪除成功回傳 true，key 不存在回傳 false
     */
    delete(key) {
        const index = this._hash(key);
        const bucket = this._buckets[index];

        for (let i = 0; i < bucket.length; i++) {
            if (bucket[i][0] === key) {
                bucket.splice(i, 1);
                this._size--;
                return true;
            }
        }
        return false;
    }

    /**
     * 檢查 key 是否存在
     * Check if key exists
     *
     * @param {*} key - 要檢查的鍵
     * @returns {boolean}
     */
    contains(key) {
        return this.search(key) !== undefined;
    }

    /**
     * 使用 get 方法（等同於 search）
     * Get method (alias for search)
     */
    get(key) {
        return this.search(key);
    }

    /**
     * 使用 set 方法（等同於 insert）
     * Set method (alias for insert)
     */
    set(key, value) {
        this.insert(key, value);
        return this;
    }

    /**
     * 使用 has 方法（等同於 contains）
     * Has method (alias for contains)
     */
    has(key) {
        return this.contains(key);
    }

    // ========== 屬性 Properties ==========

    /**
     * 計算負載因子
     * Calculate load factor
     */
    get loadFactor() {
        return this._size / this._capacity;
    }

    /**
     * 回傳元素數量
     * Return number of elements
     */
    get size() {
        return this._size;
    }

    /**
     * 回傳桶的數量
     * Return number of buckets
     */
    get capacity() {
        return this._capacity;
    }

    /**
     * 檢查是否為空
     * Check if empty
     */
    isEmpty() {
        return this._size === 0;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    clear() {
        this._buckets = new Array(this._capacity).fill(null).map(() => []);
        this._size = 0;
    }

    // ========== 迭代器 Iterators ==========

    /**
     * 回傳所有鍵的陣列
     * Return array of all keys
     */
    keys() {
        const result = [];
        for (const bucket of this._buckets) {
            for (const [key] of bucket) {
                result.push(key);
            }
        }
        return result;
    }

    /**
     * 回傳所有值的陣列
     * Return array of all values
     */
    values() {
        const result = [];
        for (const bucket of this._buckets) {
            for (const [, value] of bucket) {
                result.push(value);
            }
        }
        return result;
    }

    /**
     * 回傳所有鍵值對的陣列
     * Return array of all key-value pairs
     */
    entries() {
        const result = [];
        for (const bucket of this._buckets) {
            for (const entry of bucket) {
                result.push([...entry]);
            }
        }
        return result;
    }

    /**
     * 對每個鍵值對執行回呼函數
     * Execute callback for each key-value pair
     */
    forEach(callback) {
        for (const bucket of this._buckets) {
            for (const [key, value] of bucket) {
                callback(value, key, this);
            }
        }
    }

    /**
     * 實作迭代器協定
     * Implement iterator protocol
     */
    *[Symbol.iterator]() {
        for (const bucket of this._buckets) {
            for (const entry of bucket) {
                yield entry;
            }
        }
    }

    /**
     * 字串表示
     * String representation
     */
    toString() {
        const items = this.entries()
            .map(([k, v]) => `${String(k)}: ${String(v)}`)
            .join(', ');
        return `HashTable{${items}}`;
    }
}

// ========== 匯出 Export ==========
// CommonJS 匯出 - CommonJS export
if (typeof module !== 'undefined' && module.exports) {
    module.exports = HashTable;
}

// ========== 範例執行 Example Usage ==========
if (typeof require !== 'undefined' && require.main === module) {
    console.log('=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n');

    // 建立雜湊表 - Create hash table
    const ht = new HashTable();

    // 插入操作 - Insert operations
    console.log('插入鍵值對 Inserting key-value pairs:');
    ht.insert('apple', 100);
    ht.insert('banana', 200);
    ht.insert('cherry', 300);
    ht.set('date', 400); // 使用 set 方法
    console.log(`  雜湊表: ${ht}`);
    console.log(`  大小: ${ht.size}, 負載因子: ${ht.loadFactor.toFixed(3)}\n`);

    // 搜尋操作 - Search operations
    console.log('搜尋操作 Search operations:');
    console.log(`  search('apple') = ${ht.search('apple')}`);
    console.log(`  get('banana') = ${ht.get('banana')}`);
    console.log(`  search('grape') = ${ht.search('grape')}`);
    console.log(`  has('cherry') = ${ht.has('cherry')}\n`);

    // 更新操作 - Update operation
    console.log('更新操作 Update operation:');
    ht.insert('apple', 150);
    console.log('  更新 apple 的值為 150');
    console.log(`  search('apple') = ${ht.search('apple')}\n`);

    // 刪除操作 - Delete operation
    console.log('刪除操作 Delete operations:');
    ht.delete('banana');
    console.log(`  刪除 'banana' 後: ${ht}`);
    console.log(`  大小: ${ht.size}\n`);

    // 迭代操作 - Iteration
    console.log('迭代操作 Iteration:');
    console.log(`  所有鍵 Keys: ${JSON.stringify(ht.keys())}`);
    console.log(`  所有值 Values: ${JSON.stringify(ht.values())}`);
    console.log(`  所有項目 Entries: ${JSON.stringify(ht.entries())}`);
}
