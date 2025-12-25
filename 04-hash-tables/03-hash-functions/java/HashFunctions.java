// 04 雜湊函數（Java）/ Hash functions (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList for convenient dynamic lists in demo/analysis.  
import java.util.List;  // Use List for clearer APIs and test readability.  

public final class HashFunctions {  // Provide static hash function implementations plus a small distribution analyzer.  
    private HashFunctions() {  // Prevent instantiation because this is a utility class.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    // ============================================================  // Section banner: integer hash functions.  
    // Integer Hash Functions  // Section title.  
    // ============================================================  // Section banner end.  

    public static int divisionHash(int key, int m) {  // Compute division-method hash: h(k) = k mod m.  
        if (m <= 0) {  // Validate bucket count to avoid divide-by-zero and invalid ranges.  
            throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
        }  // Close validation.  
        return Math.floorMod(key, m);  // Use floorMod so the result is always in [0, m).  
    }  // End divisionHash().  

    public static int multiplicationHash(int key, int m) {  // Compute multiplication-method hash with default A.  
        return multiplicationHash(key, m, null);  // Delegate to the 3-arg overload for shared logic.  
    }  // End multiplicationHash(key, m).  

    public static int multiplicationHash(int key, int m, Double A) {  // Compute multiplication-method hash: floor(m * frac(k*A)).  
        if (m <= 0) {  // Validate bucket count so we return a valid index range.  
            throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
        }  // Close validation.  

        double aConst = (A != null) ? A : ((Math.sqrt(5.0) - 1.0) / 2.0);  // Use golden ratio conjugate when A is not provided.  
        double product = ((double)key) * aConst;  // Multiply key by A in floating-point.  
        double fractional = product - Math.floor(product);  // Extract fractional part in [0, 1) (works for negative too).  
        int index = (int)Math.floor(((double)m) * fractional);  // Scale by m and take floor to get an index.  
        return Math.min(index, m - 1);  // Clamp for rare rounding edge cases (fractional could be ~1.0).  
    }  // End multiplicationHash(key, m, A).  

    public static long midSquareHash(int key, int r) {  // Compute mid-square hash by squaring and extracting middle r digits.  
        if (r <= 0) {  // Validate digit count so substring extraction is meaningful.  
            throw new IllegalArgumentException("r must be >= 1");  // Signal invalid input.  
        }  // Close validation.  

        long squared = (long)key * (long)key;  // Square as long to reduce overflow risk for moderate keys.  
        String s = Long.toString(squared);  // Convert to decimal string for digit extraction.  
        if (s.length() < r) {  // If there are not enough digits, return the squared value directly (same as the Python reference).  
            return squared;  // Return fallback result.  
        }  // Close short-length branch.  

        int mid = s.length() / 2;  // Find the middle position of the digit string.  
        int start = mid - (r / 2);  // Choose the start index so we take r digits around the middle.  
        int end = start + r;  // Compute exclusive end index.  
        String slice = s.substring(start, end);  // Extract the middle digits.  
        return Long.parseLong(slice);  // Convert back to a number and return.  
    }  // End midSquareHash().  

    public static int foldingHash(int key, int chunkSize, int m) {  // Compute folding hash by chunking digits and summing.  
        if (chunkSize <= 0) {  // Validate chunk size to avoid infinite loops.  
            throw new IllegalArgumentException("chunkSize must be >= 1");  // Signal invalid input.  
        }  // Close validation.  
        if (m <= 0) {  // Validate bucket count so we return a valid range.  
            throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
        }  // Close validation.  

        String digits = Long.toString(Math.abs((long)key));  // Use absolute value so we only process digits.  
        long total = 0L;  // Accumulate the sum of chunks.  
        for (int i = 0; i < digits.length(); i += chunkSize) {  // Walk the string in chunkSize steps.  
            int end = Math.min(i + chunkSize, digits.length());  // Clamp the final chunk end.  
            String chunk = digits.substring(i, end);  // Extract one digit chunk.  
            total += Long.parseLong(chunk);  // Add this chunk value to the running total.  
        }  // Close chunk loop.  
        return (int)(total % (long)m);  // Reduce mod m to get a bucket index.  
    }  // End foldingHash().  

    // ============================================================  // Section banner: string hash functions.  
    // String Hash Functions  // Section title.  
    // ============================================================  // Section banner end.  

    public static int simpleSumHash(String s, int m) {  // Compute simple sum-of-chars hash (anagrams collide).  
        if (m <= 0) {  // Validate bucket count so we return a valid range.  
            throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
        }  // Close validation.  
        long sum = 0L;  // Accumulate character codes.  
        for (int i = 0; i < s.length(); i++) {  // Iterate code units (consistent with Python's per-character ord).  
            sum += (int)s.charAt(i);  // Add the UTF-16 code unit value.  
        }  // Close loop.  
        return (int)(sum % (long)m);  // Reduce mod m to get a bucket index.  
    }  // End simpleSumHash().  

    public static long polynomialHash(String s, int a, Integer m) {  // Compute polynomial rolling hash (Horner's method).  
        long h = 0L;  // Track rolling hash accumulator.  
        for (int i = 0; i < s.length(); i++) {  // Iterate code units.  
            h = h * (long)a + (long)((int)s.charAt(i));  // Apply Horner step: h = h*a + c.  
            if (m != null) {  // Apply modulo when m is provided (keeps numbers small).  
                h = Math.floorMod(h, (long)m.intValue());  // Use floorMod to keep result non-negative.  
            }  // Close modulo branch.  
        }  // Close loop.  
        return h;  // Return computed hash value (possibly not reduced mod m when m is null).  
    }  // End polynomialHash().  

    public static long djb2Hash(String s) {  // Compute DJB2 hash (unsigned 32-bit output).  
        long h = 5381L;  // Initialize with DJB2 offset basis.  
        for (int i = 0; i < s.length(); i++) {  // Iterate code units.  
            h = ((h << 5) + h) + (long)((int)s.charAt(i));  // Update: h = h*33 + c.  
        }  // Close loop.  
        return h & 0xFFFFFFFFL;  // Mask to 32-bit unsigned range (0..2^32-1).  
    }  // End djb2Hash().  

    public static long fnv1aHash(String s) {  // Compute FNV-1a 32-bit hash (unsigned 32-bit output).  
        final long FNV_OFFSET_BASIS = 2166136261L;  // Define FNV offset basis constant.  
        final long FNV_PRIME = 16777619L;  // Define FNV prime constant.  

        long h = FNV_OFFSET_BASIS;  // Initialize accumulator.  
        for (int i = 0; i < s.length(); i++) {  // Iterate code units.  
            h ^= (long)((int)s.charAt(i));  // XOR in the next byte/code unit.  
            h *= FNV_PRIME;  // Multiply by FNV prime (mod 2^32 via masking).  
            h &= 0xFFFFFFFFL;  // Keep only the lower 32 bits.  
        }  // Close loop.  
        return h;  // Return unsigned 32-bit result stored in long.  
    }  // End fnv1aHash().  

    public static long jenkinsOneAtATime(String s) {  // Compute Jenkins one-at-a-time hash (unsigned 32-bit output).  
        long h = 0L;  // Initialize accumulator.  
        for (int i = 0; i < s.length(); i++) {  // Iterate code units.  
            h += (long)((int)s.charAt(i));  // Add character code.  
            h += (h << 10);  // Mix with a left shift.  
            h ^= (h >> 6);  // Mix with a right shift.  
            h &= 0xFFFFFFFFL;  // Keep only the lower 32 bits.  
        }  // Close loop.  
        h += (h << 3);  // Final avalanche mixing step 1.  
        h ^= (h >> 11);  // Final avalanche mixing step 2.  
        h += (h << 15);  // Final avalanche mixing step 3.  
        return h & 0xFFFFFFFFL;  // Return masked 32-bit result.  
    }  // End jenkinsOneAtATime().  

    // ============================================================  // Section banner: analysis helpers.  
    // Hash Function Analysis Tools  // Section title.  
    // ============================================================  // Section banner end.  

    @FunctionalInterface  // Mark this interface as a functional interface for lambdas.  
    public interface HashFunction<T> {  // Represent a function that maps (key, m) -> bucket index.  
        int hash(T key, int m);  // Compute a bucket index in [0, m).  
    }  // End HashFunction.  

    public static final class DistributionReport {  // Hold distribution statistics in a typed record-like object.  
        public final int totalKeys;  // Total number of keys analyzed.  
        public final int buckets;  // Number of buckets (m).  
        public final int nonEmptyBuckets;  // Count of buckets with at least one key.  
        public final int maxBucketSize;  // Maximum bucket occupancy.  
        public final int minBucketSize;  // Minimum bucket occupancy (often 0).  
        public final double avgBucketSize;  // Average bucket occupancy (n/m).  
        public final double stdDeviation;  // Standard deviation of bucket occupancies.  
        public final int[] distribution;  // Raw bucket counts.  

        public DistributionReport(int totalKeys, int buckets, int nonEmptyBuckets, int maxBucketSize, int minBucketSize, double avgBucketSize, double stdDeviation, int[] distribution) {  // Construct report.  
            this.totalKeys = totalKeys;  // Store total key count.  
            this.buckets = buckets;  // Store bucket count.  
            this.nonEmptyBuckets = nonEmptyBuckets;  // Store non-empty bucket count.  
            this.maxBucketSize = maxBucketSize;  // Store max bucket size.  
            this.minBucketSize = minBucketSize;  // Store min bucket size.  
            this.avgBucketSize = avgBucketSize;  // Store average bucket size.  
            this.stdDeviation = stdDeviation;  // Store standard deviation.  
            this.distribution = distribution;  // Store distribution array.  
        }  // Close constructor.  
    }  // End DistributionReport.  

    public static <T> DistributionReport analyzeDistribution(HashFunction<T> hashFunc, List<T> keys, int m) {  // Analyze bucket distribution for a given hash function.  
        if (m <= 0) {  // Validate bucket count so arrays have a valid size.  
            throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
        }  // Close validation.  

        int[] buckets = new int[m];  // Allocate bucket counters (all start at 0).  
        for (T key : keys) {  // Hash each key into a bucket.  
            int h = hashFunc.hash(key, m);  // Compute bucket index for this key.  
            buckets[h] += 1;  // Increment bucket occupancy.  
        }  // Close loop.  

        int nonEmpty = 0;  // Count non-empty buckets.  
        int maxCount = 0;  // Track maximum bucket size.  
        int minCount = buckets[0];  // Track minimum bucket size (initialize from first bucket).  
        for (int b : buckets) {  // Scan bucket counts to compute summary stats.  
            if (b > 0) {  // Identify non-empty buckets.  
                nonEmpty += 1;  // Increment non-empty counter.  
            }  // Close non-empty branch.  
            if (b > maxCount) {  // Update max when needed.  
                maxCount = b;  // Store new maximum.  
            }  // Close max update.  
            if (b < minCount) {  // Update min when needed.  
                minCount = b;  // Store new minimum.  
            }  // Close min update.  
        }  // Close scan loop.  

        double avg = ((double)keys.size()) / ((double)m);  // Compute average bucket size (n/m).  
        double variance = 0.0;  // Accumulate variance over buckets.  
        for (int b : buckets) {  // Compute squared deviations from the mean.  
            double diff = ((double)b) - avg;  // Compute deviation.  
            variance += diff * diff;  // Accumulate squared deviation.  
        }  // Close variance loop.  
        variance /= (double)m;  // Convert sum to average.  
        double stdDev = Math.sqrt(variance);  // Compute standard deviation.  

        return new DistributionReport(keys.size(), m, nonEmpty, maxCount, minCount, avg, stdDev, buckets);  // Return typed report.  
    }  // End analyzeDistribution().  

    public static void main(String[] args) {  // Provide a small console demo similar to the Python reference.  
        int testKey = 123456;  // Pick a deterministic integer key for the demo output.  
        int mPrime = 97;  // Use a prime bucket count for division hashing.  
        System.out.println("=== Hash Functions Demo (Java) ===");  // Print header.  
        System.out.println("Key: " + testKey + ", m: " + mPrime);  // Print key summary.  
        System.out.println("  Division:       " + divisionHash(testKey, mPrime));  // Print division hash.  
        System.out.println("  Multiplication: " + multiplicationHash(testKey, mPrime));  // Print multiplication hash.  
        System.out.println("  Mid-Square (4): " + midSquareHash(testKey, 4));  // Print mid-square hash.  
        System.out.println("  Folding (3):    " + foldingHash(testKey, 3, mPrime));  // Print folding hash.  

        List<String> strings = new ArrayList<>();  // Build a small list of strings for the demo.  
        strings.add("hello");  // Add one sample string.  
        strings.add("world");  // Add one sample string.  
        strings.add("hash");  // Add one sample string.  
        strings.add("table");  // Add one sample string.  
        strings.add("java");  // Add one sample string.  
        System.out.println();  // Print blank line.  
        System.out.println("--- String hashes ---");  // Print section title.  
        for (String s : strings) {  // Print a few hash values for each string.  
            long djb2 = djb2Hash(s);  // Compute DJB2 hash.  
            long fnv = fnv1aHash(s);  // Compute FNV-1a hash.  
            long poly = polynomialHash(s, 31, 1000003);  // Compute polynomial hash with a modulus.  
            System.out.println(s + " => djb2=" + djb2 + ", fnv1a=" + fnv + ", poly=" + poly);  // Print one line.  
        }  // Close loop.  
    }  // Close main().  
}  // End HashFunctions.  

