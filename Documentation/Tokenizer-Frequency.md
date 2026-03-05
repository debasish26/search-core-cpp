# Word Frequency Benchmark (Hash Table)

## 1. Design Overview

### Module Structure

    search_engine/
    ├── include/
    │   ├── tokenizer.h
    │   └── frequency.h
    ├── src/
    │   ├── tokenizer.cpp
    │   ├── frequency.cpp
    │   └── main.cpp
    ├── data/
    │   ├── small.txt
    │   ├── medium.txt
    │   ├── large.txt
    │   └── huge.txt
    ├── README.md

### Module Responsibilities

  Module      Responsibility
  ----------- --------------------------------------------------
  tokenizer   Converts raw text into tokens
  frequency   Builds a word frequency table using a hash table
  main        Orchestrates execution and benchmarking

### Data Flow

    text file
       ↓
    tokenizer()
       ↓
    vector<string> tokens
       ↓
    frequency()
       ↓
    unordered_map<string,int>

The tokenizer extracts words from the input text, and the frequency
module counts occurrences using a hash table.

This modular design ensures **separation of responsibility**, improves
maintainability, and allows independent testing of each component.

------------------------------------------------------------------------

## 2. Algorithm

### Word Frequency Counting

The frequency module builds a hash table where:

    key   → word
    value → occurrence count

Algorithm:

    create empty unordered_map

    for each token in tokens:
        frequency[token]++

### Why This Works

Hash tables allow **direct access to elements by key**.

When a word appears: 1. The hash function maps the word to a bucket. 2.
If the word already exists, its count is incremented. 3. If it does not
exist, a new entry is created.

This enables efficient insertion and lookup operations.

------------------------------------------------------------------------

## 3. Complexity Analysis

### Average Case

For `n` tokens:

    n insert operations
    each insert ≈ O(1)

Total complexity:

    O(n)

This occurs because hash tables provide **constant-time average
insertion and lookup**.

### Worst Case

If all keys collide into the same bucket:

    lookup → O(n)
    insert → O(n)

Total complexity could degrade to:

    O(n²)

However, good hash functions make this scenario extremely unlikely.

### Amortized Analysis

When the table becomes too full, **rehashing** occurs:

1.  A larger bucket array is allocated.
2.  All elements are redistributed using the hash function.

Although rehashing is expensive, it happens **infrequently**, so the
amortized cost per insertion remains **O(1)**.

------------------------------------------------------------------------

## 4. Experimental Results

The program was tested using datasets with increasing input sizes.

  ------------------------------------------------------------------------
  File         Words       Unique      Buckets     Load Factor Time (ms)
               Inserted    Words                               
  ------------ ----------- ----------- ----------- ----------- -----------
  small.txt    100         35          47          0.7447      0

  medium.txt   1,000       36          47          0.7660      0

  large.txt    10,000      36          47          0.7660      8.15

  huge.txt     100,000     36          47          0.7660      62.40
  ------------------------------------------------------------------------

------------------------------------------------------------------------

## 5. Observations

### Load Factor Behavior

The load factor remained approximately:

    ~0.76

This indicates the hash table maintained a healthy balance between space
usage and collision probability.

Since the number of unique words was small, the bucket count remained
constant.

### Memory Trade-Off

Hash tables trade **additional memory** for faster lookups.

More buckets lead to: - fewer collisions - faster lookup performance -
increased memory usage

### Rehashing Behavior

Rehashing occurs when:

    load_factor > max_load_factor

During these experiments the load factor never exceeded the threshold,
so **no rehashing occurred** and the bucket count stayed constant at
**47**.

### Performance Scaling

Execution time increased approximately proportionally with input size:

    100 → ~0 ms
    1,000 → ~0 ms
    10,000 → 8 ms
    100,000 → 62 ms

This behavior supports the expected **linear O(n) performance** of
hash-table-based frequency counting.

------------------------------------------------------------------------

## Conclusion

The experiment demonstrates that `std::unordered_map` provides efficient
average-case performance for word frequency counting. By maintaining a
controlled load factor and using hashing for direct access, the
algorithm scales well as the dataset grows.
