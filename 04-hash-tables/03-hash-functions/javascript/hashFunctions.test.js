// 04 雜湊函數測試（JavaScript）/ Hash functions tests (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import integer/string hash functions and analysis helpers.  
    divisionHash,  // Division method hash.  
    multiplicationHash,  // Multiplication method hash.  
    midSquareHash,  // Mid-square hash.  
    foldingHash,  // Folding hash.  
    simpleSumHash,  // Simple sum hash.  
    polynomialHash,  // Polynomial rolling hash.  
    djb2Hash,  // DJB2 hash.  
    fnv1aHash,  // FNV-1a hash.  
    jenkinsOneAtATime,  // Jenkins one-at-a-time hash.  
    analyzeDistribution,  // Distribution analyzer.  
} = require("./hashFunctions.js");  // Load module relative to this file.  

const {  // Import universal hashing families and universal-hashing-backed table.  
    UniversalHashFamily,  // Universal hash family for ints.  
    UniversalStringHashFamily,  // Universal hash family for strings.  
    UniversalHashTable,  // Hash table using universal hashing.  
} = require("./universalHashing.js");  // Load module relative to this file.  

test("divisionHash range and determinism", () => {  // Division hashing should stay in [0, m) and be deterministic.  
    const m = 97;  // Use a prime modulus for division hashing.  
    for (let key = 0; key < 1000; key += 1) {  // Check a range of keys.  
        const h = divisionHash(key, m);  // Compute hash value.  
        assert.ok(0 <= h && h < m);  // Validate range.  
    }  // Close loop.  
    const key = 12345;  // Pick one representative key.  
    assert.equal(divisionHash(key, m), divisionHash(key, m));  // Validate determinism.  
});  // End test.  

test("multiplicationHash range and determinism", () => {  // Multiplication hashing should stay in [0, m) and be deterministic.  
    const m = 128;  // Use a power-of-two bucket count (common for multiplication hashing).  
    for (let key = 0; key < 1000; key += 1) {  // Check a range of keys.  
        const h = multiplicationHash(key, m);  // Compute hash value.  
        assert.ok(0 <= h && h < m);  // Validate range.  
    }  // Close loop.  
    const key = 12345;  // Pick one representative key.  
    assert.equal(multiplicationHash(key, m), multiplicationHash(key, m));  // Validate determinism.  
});  // End test.  

test("midSquareHash example", () => {  // Mid-square hashing should match the reference extraction for a known example.  
    const h = midSquareHash(1234, 4);  // Compute mid-square hash.  
    assert.equal(h, 5227);  // Validate expected digits from 1234^2 = 1522756 -> middle 4 digits "5227".  
});  // End test.  

test("foldingHash range", () => {  // Folding hash should stay in [0, m).  
    const m = 100;  // Choose a small bucket count for range testing.  
    const h = foldingHash(123456789, 3, m);  // Hash with chunk size 3.  
    assert.ok(0 <= h && h < m);  // Validate range.  
});  // End test.  

test("string hash functions basics", () => {  // String hashes should be deterministic and stay within expected ranges.  
    const m = 100;  // Bucket count for modulo-based hashes.  
    for (const s of ["hello", "world", "test", "hash"]) {  // Test a small set of strings.  
        const sum = simpleSumHash(s, m);  // Compute simple sum hash.  
        assert.ok(0 <= sum && sum < m);  // Validate range.  
        assert.equal(polynomialHash(s, 31, m), polynomialHash(s, 31, m));  // Validate determinism.  
        const djb2 = djb2Hash(s);  // Compute DJB2 hash.  
        const fnv = fnv1aHash(s);  // Compute FNV-1a hash.  
        const jen = jenkinsOneAtATime(s);  // Compute Jenkins hash.  
        assert.ok(djb2 >= 0 && djb2 <= 0xFFFFFFFF);  // Validate 32-bit unsigned range.  
        assert.ok(fnv >= 0 && fnv <= 0xFFFFFFFF);  // Validate 32-bit unsigned range.  
        assert.ok(jen >= 0 && jen <= 0xFFFFFFFF);  // Validate 32-bit unsigned range.  
    }  // Close loop.  
});  // End test.  

test("analyzeDistribution returns consistent stats", () => {  // Distribution analyzer should return self-consistent fields.  
    const keys = [];  // Build deterministic key list.  
    for (let i = 0; i < 1000; i += 1) {  // Generate 1000 keys.  
        keys.push(`key_${i}`);  // Add one key string.  
    }  // Close loop.  
    const m = 128;  // Choose bucket count.  
    const r = analyzeDistribution((k, buckets) => (djb2Hash(k) % buckets), keys, m);  // Analyze DJB2 reduced mod m.  
    assert.equal(r.totalKeys, 1000);  // Validate total key count.  
    assert.equal(r.buckets, m);  // Validate bucket count.  
    assert.equal(r.distribution.length, m);  // Validate distribution length.  
    assert.ok(r.nonEmptyBuckets <= m);  // Validate bounds.  
    assert.ok(r.maxBucketSize >= r.minBucketSize);  // Validate ordering.  
    assert.ok(r.stdDeviation >= 0);  // Validate std deviation.  
});  // End test.  

test("UniversalHashFamily range and determinism", () => {  // Universal hashing should stay in range and be deterministic per instance.  
    const m = 100;  // Bucket count.  
    const uh = new UniversalHashFamily(m, null, 123);  // Use deterministic seed for stable tests.  
    for (let key = 0; key < 1000; key += 1) {  // Check a range of keys.  
        const h = uh.hash(key);  // Compute hash value.  
        assert.ok(0 <= h && h < m);  // Validate range.  
    }  // Close loop.  
    assert.equal(uh.hash(12345), uh.hash(12345));  // Validate determinism within the instance.  
});  // End test.  

test("UniversalHashFamily collision probability is small", () => {  // Empirically verify collision probability is bounded (deterministic simulation).  
    const m = 100;  // Bucket count for probability bound 1/m.  
    const trials = 5000;  // Use a moderate number of trials to keep tests fast.  
    let collisions = 0;  // Count collisions across trials.  
    const x = 42;  // Fixed key x.  
    const y = 137;  // Fixed key y (x != y).  
    for (let t = 0; t < trials; t += 1) {  // Repeat trials with different deterministic seeds.  
        const uh = new UniversalHashFamily(m, null, t);  // Use seed=t to vary parameters.  
        if (uh.hash(x) === uh.hash(y)) {  // Count collisions.  
            collisions += 1;  // Increment collision counter.  
        }  // Close collision branch.  
    }  // Close trial loop.  
    const probability = collisions / trials;  // Compute empirical collision probability.  
    assert.ok(probability < (2 / m));  // Allow variance like the Python test.  
});  // End test.  

test("UniversalStringHashFamily range and determinism", () => {  // Universal string hashing should stay in range and be deterministic per instance.  
    const m = 100;  // Bucket count.  
    const ush = new UniversalStringHashFamily(m, null, 999);  // Use deterministic seed for stable tests.  
    for (const s of ["hello", "world", "test", "hash"]) {  // Test a small set of strings.  
        const h = ush.hash(s);  // Compute hash value.  
        assert.ok(0 <= h && h < m);  // Validate range.  
        assert.equal(ush.hash(s), ush.hash(s));  // Validate determinism within the instance.  
    }  // Close loop.  
});  // End test.  

test("UniversalHashTable basic operations", () => {  // Universal hash table should support insert/search/delete/update.  
    const ht = new UniversalHashTable();  // Create empty table.  
    ht.insert(1, "one");  // Insert 1 -> "one".  
    ht.insert(2, "two");  // Insert 2 -> "two".  
    assert.equal(ht.search(1), "one");  // Validate search.  
    assert.equal(ht.search(2), "two");  // Validate search.  
    assert.equal(ht.search(3), null);  // Validate missing.  
    ht.insert(1, "ONE");  // Update existing key.  
    assert.equal(ht.search(1), "ONE");  // Validate update.  
    assert.equal(ht.size, 2);  // Size should not increase on update.  
    assert.equal(ht.delete(1), true);  // Delete existing key.  
    assert.equal(ht.search(1), null);  // Validate removal.  
    assert.equal(ht.delete(1), false);  // Delete missing key.  
});  // End test.  

test("UniversalHashTable many insertions", () => {  // Table should remain correct after many inserts (and possible resizes).  
    const ht = new UniversalHashTable();  // Create empty table.  
    for (let i = 0; i < 100; i += 1) {  // Insert 100 items.  
        ht.insert(i, `value_${i}`);  // Insert i -> "value_i".  
    }  // Close loop.  
    assert.equal(ht.size, 100);  // Validate size.  
    for (let i = 0; i < 100; i += 1) {  // Validate lookups.  
        assert.equal(ht.search(i), `value_${i}`);  // Validate stored value.  
    }  // Close loop.  
});  // End test.  

