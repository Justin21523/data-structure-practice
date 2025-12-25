// 03 雜湊函數示範（C#）/ Hash functions demo (C#).  // Bilingual file header.

using System;  // Provide Math functions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for distribution arrays and key lists.  

namespace HashFunctionsUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class HashFunctions  // Provide hash function implementations plus a small distribution analyzer.  
    {  // Open class scope.  
        // ============================================================  // Section banner: integer hash functions.  
        // Integer Hash Functions  // Section title.  
        // ============================================================  // Section banner end.  

        internal static int DivisionHash(int key, int m)  // Compute division-method hash: h(k) = k mod m.  
        {  // Open method scope.  
            if (m <= 0)  // Validate bucket count to avoid divide-by-zero and invalid ranges.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  
            int r = key % m;  // Compute remainder using C# remainder semantics.  
            return r < 0 ? r + m : r;  // Convert remainder to [0, m) for negative keys.  
        }  // Close DivisionHash.  

        internal static int MultiplicationHash(int key, int m, double? A = null)  // Compute multiplication-method hash: floor(m * frac(k*A)).  
        {  // Open method scope.  
            if (m <= 0)  // Validate bucket count so we return a valid index range.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  

            double aConst = A ?? ((Math.Sqrt(5.0) - 1.0) / 2.0);  // Use golden ratio conjugate when A is not provided.  
            double product = (double)key * aConst;  // Multiply key by A in floating-point.  
            double fractional = product - Math.Floor(product);  // Extract fractional part in [0, 1) (works for negative too).  
            int index = (int)Math.Floor((double)m * fractional);  // Scale by m and take floor.  
            if (index >= m)  // Clamp for rare rounding edge cases (fractional could be ~1.0).  
            {  // Open clamp scope.  
                index = m - 1;  // Clamp to last valid bucket.  
            }  // Close clamp scope.  
            if (index < 0)  // Defensive clamp for negative rounding edge cases.  
            {  // Open clamp scope.  
                index = 0;  // Clamp to first bucket.  
            }  // Close clamp scope.  
            return index;  // Return bucket index.  
        }  // Close MultiplicationHash.  

        internal static long MidSquareHash(int key, int r)  // Compute mid-square hash by squaring and extracting middle r digits.  
        {  // Open method scope.  
            if (r <= 0)  // Validate digit count so substring extraction is meaningful.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(r), "r must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  

            long squared = (long)key * (long)key;  // Square as 64-bit integer.  
            string s = squared.ToString();  // Convert squared value to decimal string.  
            if (s.Length < r)  // If there are not enough digits, return squared directly (Python reference behavior).  
            {  // Open short-length scope.  
                return squared;  // Return fallback result.  
            }  // Close short-length scope.  

            int mid = s.Length / 2;  // Find the middle position.  
            int start = mid - (r / 2);  // Choose start so we take r digits around the middle.  
            string slice = s.Substring(start, r);  // Extract the middle digits.  
            return long.Parse(slice);  // Parse extracted digits and return.  
        }  // Close MidSquareHash.  

        internal static int FoldingHash(int key, int chunkSize, int m)  // Compute folding hash by chunking digits and summing.  
        {  // Open method scope.  
            if (chunkSize <= 0)  // Validate chunk size to avoid invalid loops.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(chunkSize), "chunkSize must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  
            if (m <= 0)  // Validate bucket count so we return a valid range.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  

            string digits = Math.Abs((long)key).ToString();  // Convert abs(key) to digits string.  
            long total = 0;  // Accumulate chunk sums.  
            for (int i = 0; i < digits.Length; i += chunkSize)  // Walk the string in chunkSize steps.  
            {  // Open loop scope.  
                int end = Math.Min(i + chunkSize, digits.Length);  // Clamp end for final chunk.  
                string chunk = digits.Substring(i, end - i);  // Extract one digit chunk.  
                total += long.Parse(chunk);  // Add chunk value.  
            }  // Close loop scope.  
            return (int)(total % (long)m);  // Reduce mod m for bucket index.  
        }  // Close FoldingHash.  

        // ============================================================  // Section banner: string hash functions.  
        // String Hash Functions  // Section title.  
        // ============================================================  // Section banner end.  

        internal static int SimpleSumHash(string s, int m)  // Compute simple sum-of-chars hash reduced mod m.  
        {  // Open method scope.  
            if (m <= 0)  // Validate bucket count so we return a valid range.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  
            long sum = 0;  // Accumulate UTF-16 code units (consistent with char iteration).  
            foreach (char c in s)  // Iterate code units.  
            {  // Open foreach scope.  
                sum += (int)c;  // Add one code unit value.  
            }  // Close foreach scope.  
            return (int)(sum % (long)m);  // Reduce mod m for bucket index.  
        }  // Close SimpleSumHash.  

        internal static uint PolynomialHash(string s, uint a, uint m)  // Compute polynomial rolling hash reduced mod m.  
        {  // Open method scope.  
            if (m == 0u)  // Validate modulo so division is defined.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  
            ulong h = 0u;  // Use 64-bit accumulator to reduce overflow risk between mods.  
            foreach (char c in s)  // Iterate UTF-16 code units.  
            {  // Open foreach scope.  
                h = (h * (ulong)a + (ulong)(uint)c) % (ulong)m;  // Horner step: h = h*a + c (mod m).  
            }  // Close foreach scope.  
            return (uint)h;  // Return reduced value.  
        }  // Close PolynomialHash.  

        internal static uint Djb2Hash(string s)  // Compute DJB2 hash (32-bit unsigned).  
        {  // Open method scope.  
            uint h = 5381u;  // Initialize with DJB2 offset basis.  
            foreach (char c in s)  // Iterate UTF-16 code units.  
            {  // Open foreach scope.  
                h = (uint)(((h << 5) + h) + (uint)c);  // Update: h = h*33 + c (wrap-around is intentional).  
            }  // Close foreach scope.  
            return h;  // Return 32-bit hash.  
        }  // Close Djb2Hash.  

        internal static uint Fnv1aHash(string s)  // Compute FNV-1a hash (32-bit unsigned).  
        {  // Open method scope.  
            const uint FNV_OFFSET_BASIS = 2166136261u;  // Define FNV offset basis constant.  
            const uint FNV_PRIME = 16777619u;  // Define FNV prime constant.  

            uint h = FNV_OFFSET_BASIS;  // Initialize accumulator.  
            foreach (char c in s)  // Iterate UTF-16 code units.  
            {  // Open foreach scope.  
                h ^= (uint)c;  // XOR in the next code unit.  
                h *= FNV_PRIME;  // Multiply by FNV prime (mod 2^32 via overflow).  
            }  // Close foreach scope.  
            return h;  // Return 32-bit hash.  
        }  // Close Fnv1aHash.  

        internal static uint JenkinsOneAtATime(string s)  // Compute Jenkins one-at-a-time hash (32-bit unsigned).  
        {  // Open method scope.  
            uint h = 0u;  // Initialize accumulator.  
            foreach (char c in s)  // Iterate UTF-16 code units.  
            {  // Open foreach scope.  
                h += (uint)c;  // Add code unit value.  
                h += (h << 10);  // Mix with left shift.  
                h ^= (h >> 6);  // Mix with right shift.  
            }  // Close foreach scope.  
            h += (h << 3);  // Final avalanche step 1.  
            h ^= (h >> 11);  // Final avalanche step 2.  
            h += (h << 15);  // Final avalanche step 3.  
            return h;  // Return 32-bit hash.  
        }  // Close JenkinsOneAtATime.  

        // ============================================================  // Section banner: distribution analysis.  
        // Hash Function Analysis Tools  // Section title.  
        // ============================================================  // Section banner end.  

        internal readonly struct DistributionReport  // Report distribution statistics for a set of keys.  
        {  // Open struct scope.  
            public DistributionReport(int totalKeys, int buckets, int nonEmptyBuckets, int maxBucketSize, int minBucketSize, double avgBucketSize, double stdDeviation, int[] distribution)  // Construct report.  
            {  // Open constructor scope.  
                TotalKeys = totalKeys;  // Store total key count.  
                Buckets = buckets;  // Store bucket count.  
                NonEmptyBuckets = nonEmptyBuckets;  // Store non-empty bucket count.  
                MaxBucketSize = maxBucketSize;  // Store max bucket size.  
                MinBucketSize = minBucketSize;  // Store min bucket size.  
                AvgBucketSize = avgBucketSize;  // Store average bucket size.  
                StdDeviation = stdDeviation;  // Store standard deviation.  
                Distribution = distribution;  // Store distribution array.  
            }  // Close constructor scope.  

            public int TotalKeys { get; }  // Total number of keys analyzed.  
            public int Buckets { get; }  // Number of buckets (m).  
            public int NonEmptyBuckets { get; }  // Count of buckets with at least one key.  
            public int MaxBucketSize { get; }  // Maximum bucket occupancy.  
            public int MinBucketSize { get; }  // Minimum bucket occupancy (often 0).  
            public double AvgBucketSize { get; }  // Average bucket occupancy (n/m).  
            public double StdDeviation { get; }  // Standard deviation of bucket occupancies.  
            public int[] Distribution { get; }  // Raw bucket counts.  
        }  // Close struct scope.  

        internal static DistributionReport AnalyzeDistribution(Func<string, int, int> hashFunc, IReadOnlyList<string> keys, int m)  // Analyze bucket distribution for a string hash function.  
        {  // Open method scope.  
            if (m <= 0)  // Validate bucket count so arrays have a valid size.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 1");  // Signal invalid input.  
            }  // Close validation scope.  

            int[] buckets = new int[m];  // Allocate bucket counters (all start at 0).  
            for (int i = 0; i < keys.Count; i++)  // Hash each key into a bucket.  
            {  // Open loop scope.  
                int h = hashFunc(keys[i], m);  // Compute bucket index for this key.  
                buckets[h] += 1;  // Increment bucket occupancy.  
            }  // Close loop scope.  

            int nonEmpty = 0;  // Count non-empty buckets.  
            int maxCount = 0;  // Track maximum bucket size.  
            int minCount = buckets[0];  // Track minimum bucket size.  
            for (int i = 0; i < buckets.Length; i++)  // Scan bucket counts.  
            {  // Open loop scope.  
                int b = buckets[i];  // Read one bucket count.  
                if (b > 0)  // Identify non-empty buckets.  
                {  // Open branch.  
                    nonEmpty += 1;  // Increment non-empty counter.  
                }  // Close branch.  
                if (b > maxCount)  // Update max when needed.  
                {  // Open branch.  
                    maxCount = b;  // Store new maximum.  
                }  // Close branch.  
                if (b < minCount)  // Update min when needed.  
                {  // Open branch.  
                    minCount = b;  // Store new minimum.  
                }  // Close branch.  
            }  // Close loop scope.  

            double avg = (double)keys.Count / (double)m;  // Compute average bucket size (n/m).  
            double variance = 0.0;  // Accumulate variance over buckets.  
            for (int i = 0; i < buckets.Length; i++)  // Compute squared deviations.  
            {  // Open loop scope.  
                double diff = (double)buckets[i] - avg;  // Compute deviation.  
                variance += diff * diff;  // Accumulate squared deviation.  
            }  // Close loop scope.  
            variance /= (double)m;  // Convert sum to average.  
            double stdDev = Math.Sqrt(variance);  // Compute standard deviation.  

            return new DistributionReport(  // Return report.  
                totalKeys: keys.Count,  // Store total key count.  
                buckets: m,  // Store bucket count.  
                nonEmptyBuckets: nonEmpty,  // Store non-empty bucket count.  
                maxBucketSize: maxCount,  // Store max bucket size.  
                minBucketSize: minCount,  // Store min bucket size.  
                avgBucketSize: avg,  // Store average bucket size.  
                stdDeviation: stdDev,  // Store std deviation.  
                distribution: buckets  // Store distribution array.  
            );  // Close constructor call.  
        }  // Close AnalyzeDistribution.  
    }  // Close class scope.  
}  // Close namespace scope.  

