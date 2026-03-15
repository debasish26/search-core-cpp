// System Headers
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>

// User defines Headers
#include "../include/tokenizer.h"
#include "../include/frequency.h"
#include "../include/serialization.h"
#include "../include/tfIdfcalc.h"
#include "../include/docmagnitude.h"
#include "../include/benchmarks.h"
#include "../include/query.h"

using namespace std;
#define nl '\n'


/* Global Variables */

// Inverted Index is a data structure use to find which files containes that specific word used in search engine,dbms
unordered_map<string,vector<pair<int,int>>>inverted_index;
vector<string>documents; // stores document name ans assign docId as the index
vector<double>doc_magnitude;
unordered_map<int, unordered_map<string, int>>frequencies;
unordered_map<string,double>idf;
unordered_map<int, unordered_map<string, double>>tfIdf;
int NoOfFiles;

/*-------------------------------------------------------------------------*/
int main() {
    vector<string>fileNames;
    string x;

    ifstream txtFiles("../data/fileNames.txt");
    while(getline(txtFiles,x)){
        fileNames.push_back(x);
    }

    for(auto x: fileNames){
        cout<<x<<" ";
    }
    cout<<nl;
    NoOfFiles = (int)fileNames.size();

    // mapping each document with its doc_id which is in my case simply its array index
    for(int i =0;i<(int)fileNames.size();i++){
        documents.push_back(fileNames[i]);
    }

    //reading all the files one by one and perfrom operations
    for(int i =0;i<(int)fileNames.size();i++){
        int doc_id = i;
        const string filename = "../data/"+fileNames[i];
        ifstream file(filename);
        string s = "";
        string line;
        while(getline(file,line)){
            s += line;
            s += nl;
        }

        // tokenized the document split it in word array or document vector
        auto start = chrono::high_resolution_clock::now();
        vector<string> tokens = tokenizer(s);
        auto end = chrono::high_resolution_clock::now();

        // for the word array i created i calculated each word frequency
        auto freqstart = chrono::high_resolution_clock::now();
        unordered_map<string,int> frequencie = frequency(tokens);
        auto freqend = chrono::high_resolution_clock::now();
        frequencies[doc_id] = frequencie;

        // calculated invert index (its a global vector) which store which word appear in which documents with there frequency
        // need to calculate inverted_index
        // which will store text_word = {file,frequency}
        // the = {doc0,2},{doc1,3}
        // inverted_index[word].push_back({file,frequency})
        // loop over frequencies vector
        // store doc_id a number not filenames cause string takes more storage

        for(auto &word:frequencie){
            inverted_index[word.first].push_back({i,word.second});
        }

        chrono::duration<double,milli> tokenizer_time = end - start;
        chrono::duration<double,milli> frequency_time = freqend - freqstart;

        //benchmarks(filename, tokens.size(), frequencie, tokenizer_time, frequency_time);

    }

    // making binary files of inverted_index for faster retreival
    serialize(inverted_index);

    // calculating IDF so for each string i need 2 things
    // n - number of documents (easy)
    // documents containing that word
    // for the - size of the vactor
    // inverted index =
    // the - {doc1,0},{doc2,1}
    // is - {doc1,2},{doc3,5}
    // need to make the documet vector per document which have the weight of
    // every word per document then we will compare the query vector
    // how do we compute weight of a word in a documet simple use TF - IDF

    idf = idfCalc(inverted_index,NoOfFiles);

    // cout << "\n====== IDF VALUES ======" << nl;

    // for(auto &term : idf){
    //     cout << term.first << " -> " << term.second << nl;
    // }

    //cout << "========================\n" << nl;

    tfIdf = tfIdfCalc(frequencies);

    //cout << "\n====== TF-IDF VECTORS ======" << nl;

    // for(auto &doc : tfIdf){

    //     int doc_id = doc.first;

    //     cout << "Doc " << doc_id << " (" << documents[doc_id] << ")" << nl;

    //     for(auto &term : doc.second){
    //         cout << "   " << term.first << " -> " << term.second << nl;
    //     }

    //     cout << nl;
    // }

    // cout << "============================\n" << nl;

    // similarity between 2 vectors - dot product/resulatnt of 2 vectors
    // resultant of 2 vectors = sqrt((ai)^2 + (a2)^2 + ..... (an)^2) - this is called doc_norms or the normalized form for the vector so that cosine similarity calculation will be easy
    // storing document lengths
    doc_magnitude = doc_magn_calc(tfIdf);

    // cout << "\n====== DOCUMENT NORMS ======" << nl;

    // for(int i = 0; i < doc_magnitude.size(); i++){
    //     cout << "Doc " << i << " (" << documents[i] << ") -> "
    //          << doc_magnitude[i] << nl;
    // }

    // cout << "============================\n" << nl;

    query();

    return 0;
}

/*
 * dog
 * tokens = dog
 * frequency = dog : 1
 * frequencies = 0 : {dog : 1}
 * inverted_index = dog : {0,1}
 * calculate weight hold for each document and created a document vector
 * calculated document vector for each document
 * make query.cpp and calculated its vector magnitude
 * now need to calculate dot product
 * but dimension of q vector and documents are different then
 *
 *
 *
 *
 *
 */
