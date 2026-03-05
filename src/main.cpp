// System Headers
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>
#include <cmath>

// User defines Headers
#include "../include/tokenizer.h"
#include "../include/frequency.h"
#include "../include/serialization.h"
#include "../include/benchmarks.h"

using namespace std;
#define nl '\n'


/* Global Variables */

// Inverted Index is a data structure use to find which files containes that specific word used in search engine,dbms
unordered_map<string,vector<pair<int,int>>>inverted_index;
vector<string>documents;
vector<double>doc_norms;

/*-------------------------------------------------------------------------*/
int main() {
    vector<string>fileNames;
    string x;

    ifstream txtFiles("../data/fileNames.txt");
    while(getline(txtFiles,x)){
        fileNames.push_back(x);
    }

    for(int i =0;i<(int)fileNames.size();i++){
        documents.push_back(fileNames[i]);
    }

    for(int i =0;i<(int)fileNames.size();i++){
        const string filename = "../data/"+fileNames[i];
        ifstream file(filename);
        string s = "";
        string line;
        while(getline(file,line)){
            s += line;
            s += nl;
        }

        auto start = chrono::high_resolution_clock::now();
        vector<string> tokens = tokenizer(s);
        auto end = chrono::high_resolution_clock::now();
        

        auto freqstart = chrono::high_resolution_clock::now();
        unordered_map<string,int> frequencies = frequency(tokens);
        auto freqend = chrono::high_resolution_clock::now();

        // need to calculate inverted_index
        // which will store text_word = {file,frequency}
        // the = {doc0,2},{doc1,3}
        // inverted_index[word].push_back({file,frequency})
        // loop over frequencies vector
        // store doc_id a number not filenames cause string takes more storage

        for(auto &word:frequencies){
            inverted_index[word.first].push_back({i,word.second});
        }

        // similarity between 2 vectors - dot product/resultant of 2 vactor
        // storing document lengths
        double sum = 0;
        for(auto &chunks: frequencies){
            sum += (chunks.second * chunks.second);
        }
        doc_norms.push_back(sqrt(sum));
        
        chrono::duration<double,milli> tokenizer_time = end - start;
        chrono::duration<double,milli> frequency_time = freqend - freqstart;
        
        benchmarks(filename, tokens.size(), frequencies, tokenizer_time, frequency_time);

    }
    serialize(inverted_index);
    return 0;
}