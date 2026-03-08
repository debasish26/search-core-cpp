
# TF-IDF in Our Search Engine

## Overview

TF-IDF (Term Frequency – Inverse Document Frequency) is a **term weighting technique** used in information retrieval systems.
It assigns a **numerical importance score** to each word in a document.

The idea:

> A word is important if it appears **frequently in a document** but **rarely across all documents**.

TF-IDF converts raw text into **weighted vectors**, which can later be compared using **cosine similarity** to rank documents.

---

# Why TF-IDF Is Needed

Consider two documents:

Doc1:
the cat sat on the mat

Doc2:
the dog sat on the sofa

Words like:

the  
on  
sat  

appear everywhere.

They are **not useful for distinguishing documents**.

But words like:

cat  
dog  
sofa  

help identify documents.

TF-IDF reduces the weight of common words and increases the weight of rare words.

---

# Components of TF-IDF

TF-IDF consists of two parts:

TF  → Term Frequency  
IDF → Inverse Document Frequency  

Final weight:

TF-IDF = TF × IDF

---

# Term Frequency (TF)

## Definition

TF measures how often a term appears in a document.

### Formula

TF(term, doc) = count(term in doc)

Sometimes normalized:

TF = count(term) / total_terms_in_doc

Example:

Document:
cat cat dog

TF values:

cat → 2  
dog → 1  

---

# Inverse Document Frequency (IDF)

## Definition

IDF measures how rare a word is across the entire document collection.

Words appearing in many documents are **less informative**.

### Formula

IDF(term) = log ( N / df )

Where:

N  = total number of documents  
df = number of documents containing the term

Example:

N = 100 documents  
df(cat) = 10  

IDF(cat) = log(100 / 10)

Rare terms receive **higher IDF scores**.

---

# TF-IDF Formula

Combine TF and IDF:

TF-IDF(term, doc) = TF(term, doc) × IDF(term)

Interpretation:

A term has high TF-IDF if:

1. It appears many times in the document  
2. It appears in few documents overall  

---

# TF-IDF in Our Search Engine Pipeline

Our system processes documents through multiple stages:

Documents
│
▼
Tokenizer
│
▼
Tokens
│
▼
Word Frequency
│
▼
Inverted Index
│
▼
TF-IDF Computation
│
▼
Vector Representation
│
▼
Cosine Similarity
│
▼
Ranked Search Results

TF-IDF acts as the **bridge between indexing and ranking**.

---

# Interaction With System Components

## 1. Tokens

Produced by the tokenizer.

Example:

"The cat sat"

Tokens:

["the", "cat", "sat"]

These tokens are the **input to the frequency module**.

---

## 2. Frequencies

The frequency module computes word counts.

Structure:

unordered_map<string, int> frequencies

Example:

cat → 2  
dog → 1  

These counts are used to compute **Term Frequency (TF)**.

---

## 3. Inverted Index

The inverted index maps words to documents.

Example:

cat → [doc1, doc5, doc8]  
dog → [doc2, doc5]

This allows us to compute:

df(term) = number of documents containing the term

Which is required for **IDF calculation**.

---

## 4. IDF Table

We compute IDF for each term and store it.

Example structure:

unordered_map<string, double> idf

Computation:

idf[term] = log(N / df)

---

## 5. TF-IDF Vectors

Each document becomes a vector of TF-IDF weights.

Structure:

unordered_map<int, unordered_map<string,double>> doc_vectors

Example:

doc1:

cat → 1.4  
dog → 0.3  
apple → 0.8  

This is a **sparse vector representation**.

---

## 6. Document Norms

To compute cosine similarity efficiently we store vector magnitudes.

Structure:

unordered_map<int,double> doc_norms

Computation:

|D| = sqrt( Σ (tfidf²) )

Example:

doc_norms[doc1] = sqrt(cat² + dog² + apple²)

These norms allow **fast cosine similarity computation** during search.

---

# How We Compute TF-IDF in Our System

## Step 1 — Tokenization

document → tokenizer → tokens

Example:

["cat","dog","cat"]

---

## Step 2 — Compute Frequencies

tokens → frequency map

Example:

cat → 2  
dog → 1  

---

## Step 3 — Build Inverted Index

term → list of documents

Example:

cat → [doc1, doc4]  
dog → [doc1, doc2]

This lets us compute document frequency.

---

## Step 4 — Compute IDF

idf(term) = log(N / df)

Store results in:

unordered_map<string,double> idf

---

## Step 5 — Compute TF-IDF

For each document:

tfidf(term) = tf(term, doc) × idf(term)

Store in:

doc_vectors[doc_id][term]

---

## Step 6 — Compute Document Norms

For each document vector:

norm = sqrt(sum(tfidf²))

Store in:

doc_norms[doc_id]

---

# How TF-IDF Enables Search Ranking

When a query arrives:

Example query:

"machine learning"

Process:

query
│
▼
tokenize
│
▼
compute query TF-IDF
│
▼
compare with document vectors
│
▼
compute cosine similarity
│
▼
rank documents

Similarity formula:

similarity(Q, D) = (Q · D) / (|Q| × |D|)

Documents with higher scores are **more relevant**.

---

# Advantages of TF-IDF

✔ Simple and fast  
✔ Highlights important words  
✔ Reduces effect of common words  
✔ Works well with inverted indexes  

---

# Limitations

TF-IDF does not understand semantics.

Example:

car  
automobile  

TF-IDF treats them as different words.

Modern systems may combine TF-IDF with:

BM25  
Word embeddings  
Neural retrieval models  

---

# Summary

TF-IDF is a **term weighting method** used in search engines.

It works by:

1. Measuring term frequency within documents  
2. Penalizing words that appear in many documents  
3. Producing weighted vectors for documents  
4. Enabling ranking through cosine similarity  

In our system it interacts with:

tokens  
frequencies  
inverted_index  
idf_table  
doc_vectors  
doc_norms  

Together these components allow the search engine to **rank documents based on relevance to a query**.
