// 03 通用雜湊示範（C#）/ Universal hashing demo (C#).  // Bilingual file header.

using System;  // Provide Math, Random, and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for bucket chains.  

namespace HashFunctionsUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class UniversalHashing  // Provide universal hash families and a small hash table that can rehash on attacks.  
    {  // Open class scope.  
        internal static bool IsPrime(int n)  // Check whether n is prime (trial division).  
        {  // Open method scope.  
            if (n < 2)  // Handle 0, 1, and negatives.  
            {  // Open branch.  
                return false;  // Not prime.  
            }  // Close branch.  
            if (n == 2)  // Handle the only even prime.  
            {  // Open branch.  
                return true;  // 2 is prime.  
            }  // Close branch.  
            if (n % 2 == 0)  // Reject other even numbers quickly.  
            {  // Open branch.  
                return false;  // Not prime.  
            }  // Close branch.  
            int limit = (int)Math.Floor(Math.Sqrt((double)n));  // Compute sqrt limit for trial division.  
            for (int i = 3; i <= limit; i += 2)  // Try odd divisors only.  
            {  // Open loop scope.  
                if (n % i == 0)  // Detect divisor.  
                {  // Open branch.  
                    return false;  // Not prime.  
                }  // Close branch.  
            }  // Close loop scope.  
            return true;  // No divisors found => prime.  
        }  // Close IsPrime.  

        internal static int NextPrime(int n)  // Find the smallest prime >= n.  
        {  // Open method scope.  
            int x = n < 2 ? 2 : n;  // Ensure we start at least from 2.  
            while (!IsPrime(x))  // Repeat until a prime is found.  
            {  // Open loop scope.  
                x += 1;  // Move to next candidate.  
            }  // Close loop scope.  
            return x;  // Return prime candidate.  
        }  // Close NextPrime.  

        internal sealed class UniversalHashFamily  // Represent h_{a,b}(k) = ((a*k + b) mod p) mod m.  
        {  // Open class scope.  
            private readonly int _m;  // Bucket count (table size).  
            private readonly int _p;  // Prime modulus.  
            private readonly Random _rng;  // RNG used to choose (a, b).  
            private int _a;  // Multiplier in [1, p-1].  
            private int _b;  // Offset in [0, p-1].  

            public UniversalHashFamily(int m, int? p = null, int? seed = null)  // Construct family with m and optional p/seed.  
            {  // Open constructor scope.  
                if (m <= 0)  // Validate bucket count.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
                }  // Close validation scope.  
                _m = m;  // Store bucket count.  
                _p = p ?? NextPrime(Math.Max(m * 10, 10007));  // Choose default p similar to Python reference.  
                _rng = seed.HasValue ? new Random(seed.Value) : new Random();  // Choose deterministic RNG when seed is provided.  
                Regenerate();  // Choose initial parameters.  
            }  // Close constructor scope.  

            public void Regenerate()  // Choose new random parameters (a, b).  
            {  // Open method scope.  
                _a = _rng.Next(1, _p);  // Pick a in [1, p-1] (upper bound exclusive).  
                _b = _rng.Next(0, _p);  // Pick b in [0, p-1].  
            }  // Close Regenerate.  

            public int Hash(int key)  // Compute hash value in [0, m).  
            {  // Open method scope.  
                long linear = (long)_a * (long)key + (long)_b;  // Compute a*k + b in 64-bit.  
                long modP = linear % (long)_p;  // Reduce mod p with signed remainder semantics.  
                if (modP < 0)  // Adjust negative remainder.  
                {  // Open branch.  
                    modP += _p;  // Shift into [0, p).  
                }  // Close branch.  
                return (int)(modP % (long)_m);  // Reduce mod m for bucket index.  
            }  // Close Hash.  
        }  // Close UniversalHashFamily.  

        internal sealed class UniversalStringHashFamily  // Represent a polynomial universal-ish hash family for strings.  
        {  // Open class scope.  
            private readonly int _m;  // Bucket count.  
            private readonly int _p;  // Prime modulus.  
            private readonly Random _rng;  // RNG used to choose base a.  
            private int _a;  // Base parameter in [1, p-1].  

            public UniversalStringHashFamily(int m, int? p = null, int? seed = null)  // Construct family with m and optional p/seed.  
            {  // Open constructor scope.  
                if (m <= 0)  // Validate bucket count.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
                }  // Close validation scope.  
                _m = m;  // Store bucket count.  
                _p = p ?? NextPrime(Math.Max(m * 10, 10007));  // Choose default p similar to Python reference.  
                _rng = seed.HasValue ? new Random(seed.Value) : new Random();  // Choose deterministic RNG when seed is provided.  
                Regenerate();  // Choose initial base parameter.  
            }  // Close constructor scope.  

            public void Regenerate()  // Choose new random base a.  
            {  // Open method scope.  
                _a = _rng.Next(1, _p);  // Pick a in [1, p-1].  
            }  // Close Regenerate.  

            public int Hash(string s)  // Compute string hash value in [0, m).  
            {  // Open method scope.  
                long h = 0;  // Initialize polynomial accumulator.  
                foreach (char c in s)  // Iterate UTF-16 code units.  
                {  // Open foreach scope.  
                    h = (h * (long)_a + (long)(int)c) % (long)_p;  // Horner step mod p.  
                }  // Close foreach scope.  
                return (int)(h % (long)_m);  // Reduce mod m for bucket index.  
            }  // Close Hash.  
        }  // Close UniversalStringHashFamily.  

        internal sealed class UniversalHashTable  // A simple chained hash table for int keys using universal hashing.  
        {  // Open class scope.  
            private const int DefaultCapacity = 16;  // Default bucket count.  
            private const double MaxLoadFactor = 0.75;  // Resize threshold.  
            private const int MaxChainLength = 10;  // Chain length threshold for defensive rehash.  

            private int _capacity;  // Number of buckets.  
            private int _size;  // Number of stored entries.  
            private List<List<Entry>> _buckets;  // Bucket array: each bucket is a chain (list) of entries.  
            private UniversalHashFamily _hashFamily;  // Hash family used to compute indices.  
            private int _rehashCount;  // How many times we regenerated parameters due to long chains.  
            private int _seed;  // Base seed used to create new hash families on resize.  

            public UniversalHashTable(int capacity = DefaultCapacity, int seed = 0)  // Construct table with capacity and base seed.  
            {  // Open constructor scope.  
                if (capacity <= 0)  // Validate capacity.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(capacity), "capacity must be >= 1");  // Signal invalid input.  
                }  // Close validation scope.  
                _capacity = capacity;  // Store capacity.  
                _size = 0;  // Start empty.  
                _seed = seed;  // Store base seed.  
                _buckets = AllocateBuckets(_capacity);  // Allocate empty buckets.  
                _hashFamily = new UniversalHashFamily(_capacity, seed: _seed);  // Create hash family for current capacity.  
                _rehashCount = 0;  // Start with no rehashes.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose current size.  
            public double LoadFactor => (double)_size / (double)_capacity;  // Expose current load factor.  
            public int RehashCount => _rehashCount;  // Expose defensive rehash count.  

            private static List<List<Entry>> AllocateBuckets(int capacity)  // Allocate empty bucket chains.  
            {  // Open method scope.  
                var buckets = new List<List<Entry>>(capacity: capacity);  // Allocate top-level list.  
                for (int i = 0; i < capacity; i++)  // Create one empty chain per bucket.  
                {  // Open loop scope.  
                    buckets.Add(new List<Entry>());  // Add empty chain.  
                }  // Close loop scope.  
                return buckets;  // Return buckets.  
            }  // Close AllocateBuckets.  

            private int Hash(int key)  // Compute bucket index for a key.  
            {  // Open method scope.  
                return _hashFamily.Hash(key);  // Delegate to current hash family.  
            }  // Close Hash.  

            public void Insert(int key, string value)  // Insert or update key->value.  
            {  // Open method scope.  
                int index = Hash(key);  // Compute bucket index.  
                List<Entry> bucket = _buckets[index];  // Get chain at bucket.  

                for (int i = 0; i < bucket.Count; i++)  // Search for existing key.  
                {  // Open loop scope.  
                    if (bucket[i].Key == key)  // Update existing key.  
                    {  // Open match scope.  
                        bucket[i] = new Entry(key, value);  // Overwrite value.  
                        return;  // Size does not change for updates.  
                    }  // Close match scope.  
                }  // Close loop scope.  

                bucket.Add(new Entry(key, value));  // Append new entry.  
                _size += 1;  // Increase size.  

                if (LoadFactor > MaxLoadFactor)  // Resize when load factor is too high.  
                {  // Open branch.  
                    Resize();  // Rehash into larger table.  
                }  // Close branch.  
                else if (bucket.Count > MaxChainLength)  // Regenerate when chain becomes suspiciously long.  
                {  // Open branch.  
                    RegenerateHash();  // Defensive rehash.  
                }  // Close branch.  
            }  // Close Insert.  

            public string? Search(int key)  // Search key and return value (null when missing).  
            {  // Open method scope.  
                int index = Hash(key);  // Compute bucket index.  
                foreach (Entry e in _buckets[index])  // Scan chain linearly.  
                {  // Open foreach scope.  
                    if (e.Key == key)  // Match found.  
                    {  // Open match scope.  
                        return e.Value;  // Return stored value.  
                    }  // Close match scope.  
                }  // Close foreach scope.  
                return null;  // Not found.  
            }  // Close Search.  

            public bool Delete(int key)  // Delete key; return true if removed.  
            {  // Open method scope.  
                int index = Hash(key);  // Compute bucket index.  
                List<Entry> bucket = _buckets[index];  // Get chain.  
                for (int i = 0; i < bucket.Count; i++)  // Scan chain for key.  
                {  // Open loop scope.  
                    if (bucket[i].Key == key)  // Match found.  
                    {  // Open match scope.  
                        bucket.RemoveAt(i);  // Remove entry.  
                        _size -= 1;  // Decrease size.  
                        return true;  // Report success.  
                    }  // Close match scope.  
                }  // Close loop scope.  
                return false;  // Not found.  
            }  // Close Delete.  

            public int GetMaxChainLength()  // Compute maximum chain length across buckets.  
            {  // Open method scope.  
                int max = 0;  // Track maximum chain length.  
                foreach (List<Entry> bucket in _buckets)  // Scan all buckets.  
                {  // Open foreach scope.  
                    if (bucket.Count > max)  // Update max when needed.  
                    {  // Open branch.  
                        max = bucket.Count;  // Store new maximum.  
                    }  // Close branch.  
                }  // Close foreach scope.  
                return max;  // Return maximum chain length.  
            }  // Close GetMaxChainLength.  

            private void Resize()  // Double capacity and reinsert all entries (rehash).  
            {  // Open method scope.  
                List<List<Entry>> oldBuckets = _buckets;  // Save old buckets.  
                _capacity *= 2;  // Double capacity.  
                _buckets = AllocateBuckets(_capacity);  // Allocate new buckets.  
                _seed += 1;  // Change seed so new hash family differs after resize.  
                _hashFamily = new UniversalHashFamily(_capacity, seed: _seed);  // Create new hash family for new capacity.  
                _size = 0;  // Reset size and reinsert.  

                foreach (List<Entry> bucket in oldBuckets)  // Reinsert entries into new buckets.  
                {  // Open foreach scope.  
                    foreach (Entry e in bucket)  // Reinsert one entry.  
                    {  // Open foreach scope.  
                        Insert(e.Key, e.Value);  // Use Insert to maintain invariants.  
                    }  // Close foreach scope.  
                }  // Close foreach scope.  
            }  // Close Resize.  

            private void RegenerateHash()  // Regenerate parameters and redistribute without changing capacity.  
            {  // Open method scope.  
                _rehashCount += 1;  // Count this defensive rehash.  
                List<List<Entry>> oldBuckets = _buckets;  // Save old buckets.  
                _buckets = AllocateBuckets(_capacity);  // Allocate new buckets.  
                _hashFamily.Regenerate();  // Choose new parameters.  
                _size = 0;  // Reset size and reinsert.  

                foreach (List<Entry> bucket in oldBuckets)  // Reinsert entries.  
                {  // Open foreach scope.  
                    foreach (Entry e in bucket)  // Reinsert one entry.  
                    {  // Open foreach scope.  
                        Insert(e.Key, e.Value);  // Use Insert to maintain invariants.  
                    }  // Close foreach scope.  
                }  // Close foreach scope.  
            }  // Close RegenerateHash.  

            private readonly struct Entry  // Represent one key-value pair stored in a chain.  
            {  // Open struct scope.  
                public Entry(int key, string value)  // Construct entry.  
                {  // Open constructor scope.  
                    Key = key;  // Store key.  
                    Value = value;  // Store value.  
                }  // Close constructor scope.  

                public int Key { get; }  // Stored key.  
                public string Value { get; }  // Stored value.  
            }  // Close struct scope.  
        }  // Close UniversalHashTable.  
    }  // Close class scope.  
}  // Close namespace scope.  

