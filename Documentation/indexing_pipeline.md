# Indexing Pipeline Documentation

## Overview

This module implements the **core indexing pipeline of a search
engine**.\
Its purpose is to transform raw documents into a **structured,
searchable inverted index** that enables fast query processing.

The pipeline processes documents through several stages, where each
stage performs a single transformation. This follows the **pipeline
architecture pattern**, which is widely used in search engines,
compilers, and large‑scale data processing systems.

------------------------------------------------------------------------

# Indexing Pipeline Architecture

                            +---------------------+
                            |    fileNames.txt    |
                            |  list of documents  |
                            +----------+----------+
                                       |
                                       v
                            +---------------------+
                            |   Document Loader   |
                            | read file contents  |
                            +----------+----------+
                                       |
                                       v
                            +---------------------+
                            |      Tokenizer      |
                            | text → tokens       |
                            | O(N) character scan |
                            +----------+----------+
                                       |
                                       v
                            +---------------------+
                            |  Frequency Counter  |
                            | tokens → word freq  |
                            | unordered_map       |
                            +----------+----------+
                                       |
                                       v
                            +---------------------+
                            | Inverted Index      |
                            | Builder             |
                            | word → [(doc,freq)] |
                            +----------+----------+
                                       |
                    +------------------+------------------+
                    |                                     |
                    v                                     v
          +---------------------+               +---------------------- +
          | Document Norms      |               | Benchmark Module     |
          | √(freq² + freq²...) |               | measure performance  |
          | cosine similarity   |               | tokenizer / freq     |
          +----------+----------+               +----------+-----------+
                     |                                     |
                     +------------------+------------------+
                                        |
                                        v
                             +------------------------+
                             |   Serialization        |
                             | save index to disk     |
                             | binary format          |
                             | index.bin              |
                             +-----------+------------+
                                         |
                                         v
                             +------------------------+
                             |  Persistent Search     |
                             |       Index            |
                             | used by query engine   |
                             +------------------------+

------------------------------------------------------------------------

# Why Pipeline Architecture?

Pipeline architecture provides several advantages:

  Benefit                 Explanation
  ----------------------- ------------------------------------------
  Modularity              Each stage has a single responsibility
  Maintainability         Components can evolve independently
  Testability             Each stage can be unit tested
  Performance profiling   Benchmarks can measure individual stages
  Scalability             Future stages can be inserted easily

This architecture is commonly used in:

-   Search engines
-   Compilers
-   Machine learning preprocessing pipelines
-   ETL data systems
-   Large-scale distributed processing frameworks

------------------------------------------------------------------------

# Stage 1 --- Document Loading

The system first reads filenames from:

    fileNames.txt

Example:

    article1.txt
    article2.txt
    article3.txt

Documents are mapped to **document IDs**:

    doc_id → filename

Example:

    0 → article1.txt
    1 → article2.txt
    2 → article3.txt

Using `doc_id` instead of storing filenames in the index saves memory
and improves performance.

------------------------------------------------------------------------

# Stage 2 --- Tokenization

The tokenizer converts raw text into tokens (words).

Example:

Input text:

    "The cat sat on the mat."

Output tokens:

    ["the", "cat", "sat", "on", "the", "mat"]

Tokenizer properties:

  Property                  Description
  ------------------------- ---------------------------------
  Lowercase normalization   Ensures case‑insensitive search
  Sequential scan           Processes characters once
  ASCII token splitting     Separates words
  Time complexity           Θ(N) where N = characters

------------------------------------------------------------------------

# Stage 3 --- Frequency Counting

Tokens are converted into a **word frequency map**.

Example:

Tokens:

    ["the","cat","sat","on","the","mat"]

Frequency map:

    the → 2
    cat → 1
    sat → 1
    on  → 1
    mat → 1

Data structure used:

``` cpp
unordered_map<string,int>
```

Advantages:

-   Average **O(1)** insertion
-   Efficient counting
-   Automatic resizing

------------------------------------------------------------------------

# Stage 4 --- Inverted Index Construction

The inverted index maps **words to the documents containing them**.

Structure:

``` cpp
unordered_map<string, vector<pair<int,int>>> inverted_index;
```

Meaning:

    word → [(doc_id , frequency)]

Example:

    cat → [(0,1),(2,2)]
    dog → [(1,1)]
    sat → [(0,1)]

This enables fast search.

Instead of scanning all documents, the system retrieves the relevant
documents directly.

Example:

    Search: "cat"

Retrieve:

    cat → [(0,1),(2,2)]

Only documents **0 and 2** must be examined.

------------------------------------------------------------------------

# Stage 5 --- Document Vector Norms

Each document is represented as a vector of word frequencies.

Example document:

    cat dog dog

Vector:

    (cat,dog) → (1,2)

Vector magnitude:

    ||D|| = √(1² + 2²)

Stored as:

    doc_norms[doc_id]

Example:

    doc_norms[0] = 2.236
    doc_norms[1] = 1.414
    doc_norms[2] = 3.162

These values are required for **cosine similarity ranking**.

Similarity formula:

    score = (D · Q) / (||D|| × ||Q||)

Where:

-   `D` = document vector
-   `Q` = query vector

Normalization prevents long documents from unfairly dominating rankings.

------------------------------------------------------------------------

# Stage 6 --- Benchmarking

The benchmark module measures performance of key stages.

Metrics recorded:

  Metric                 Description
  ---------------------- ----------------------------------
  Tokenizer time         Time to tokenize text
  Frequency build time   Time to compute word frequencies
  Token count            Number of processed tokens
  Unique words           Vocabulary size
  Hash table stats       bucket count and load factor

This helps analyze system performance and identify bottlenecks.

------------------------------------------------------------------------

# Stage 7 --- Serialization (Binary Index)

After indexing, the inverted index is written to disk.

Purpose:

-   Avoid rebuilding the index each run
-   Faster system startup
-   Persistent storage

Binary layout:

    [word_length]
    [word]

    [posting_count]

    doc_id
    frequency
    doc_id
    frequency
    ...

Example entry:

    cat
    2
    0 1
    2 2

Advantages of binary storage:

  Benefit              Explanation
  -------------------- ------------------------
  Faster loading       No text parsing
  Smaller file size    Compact representation
  Efficient disk I/O   Direct memory reads

Output file:

    index.bin

------------------------------------------------------------------------

# Time Complexity of the Pipeline

  Stage                Complexity
  -------------------- ------------------
  File read            O(document size)
  Tokenization         O(N characters)
  Frequency counting   O(tokens)
  Index building       O(unique words)
  Norm computation     O(unique words)
  Serialization        O(index size)

Overall indexing complexity:

    O(total_tokens)

This is optimal for inverted index construction.

------------------------------------------------------------------------

# Current System Capabilities

The current system supports:

-   Document loading
-   Tokenization
-   Frequency analysis
-   Inverted index creation
-   Document vector normalization
-   Performance benchmarking
-   Binary serialization

Together these components form the **core backend of a search engine
indexing system**.

------------------------------------------------------------------------

# Future Improvements

### Information Retrieval Enhancements

-   TF‑IDF weighting
-   Stopword removal
-   Stemming / lemmatization
-   Phrase search
-   Boolean queries

### Index Optimization

-   Posting list sorting
-   Delta encoding
-   Compression
-   Memory‑mapped indexes

### Query Engine

Future pipeline:

    User Query
       ↓
    Tokenizer
       ↓
    Query Vector
       ↓
    Posting List Lookup
       ↓
    Cosine Similarity
       ↓
    Ranking
       ↓
    Top‑K Results

### Scalability

-   Parallel indexing
-   Streaming document processing
-   Distributed indexing

------------------------------------------------------------------------

# Summary

This indexing pipeline converts raw documents into a **persistent
inverted index** that supports fast information retrieval.

The modular pipeline architecture ensures:

-   Clean separation of responsibilities
-   Efficient indexing
-   Scalable design
-   Easy integration with a future query engine

This indexing stage forms the **foundation of the search engine**.
