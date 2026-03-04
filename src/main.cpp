// System Headers
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>

// User defines Headers
#include "../include/tokenizer.h"
#include "../include/frequency.h"

using namespace std;
#define nl '\n'


/* Global Variables */

// Inverted Index is a data structure use to find which files containes that specific word used in search engine,dbms
unordered_map<string,vector<pair<int,int>>>inverted_index;
vector<string>documents;

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
        cout<<"filename :"<<filename<<nl;
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

        // for(auto x:tokens){
        //     cout<<x<<" ";
        // }
        // cout<<nl;

        auto freqstart = chrono::high_resolution_clock::now();
        unordered_map<string,int> frequencies = frequency(tokens);
        auto freqend = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> tokenizer_time = end - start;
        chrono::duration<double,milli> frequency_time = freqend - freqstart;

        // need to calculate inverted_index
        // which will store text_word = {file,frequency}
        // the = {doc0,2},{doc1,3}
        // inverted_index[word].push_back({file,frequency})
        // loop over frequencies vector
        // store doc_id a number not filenames cause string takes more storage

        for(auto &word:frequencies){
            inverted_index[word.first].push_back({i,word.second});
        }

        auto& fr = frequencies;
        cout << nl;

        /* Bench Marks Displaying*/
        cout << "================ HASH TABLE BENCHMARK ================" << nl << nl;

        cout << "Input File        : " << filename << nl;
        cout << "Tokens Processed  : " << tokens.size() << nl;
        cout << "Unique Words      : " << fr.size() << nl;

        cout << nl;
        cout << "--------------- Tokenization ----------------" << nl;
        cout << "Tokenizer Time    : " << tokenizer_time.count() << " ms" << nl;

        cout << nl;
        cout << "--------------- Frequency Build -------------" << nl;
        cout << "Frequency Time    : " << frequency_time.count() << " ms" << nl;

        cout << nl;
        cout << "--------------- Hash Table Stats ------------" << nl;
        cout << "Bucket Count      : " << fr.bucket_count() << nl;
        cout << "Load Factor       : " << fr.load_factor() << nl;
        cout << "Max Load Factor   : " << fr.max_load_factor() << nl;

        cout << nl;
        cout << "======================================================" << nl;
    }

    for(auto &x:inverted_index){
        cout<<x.first<<" "<<"-->";
        for(auto &y:x.second){
            cout<<"{"<<documents[y.first]<<" "<<y.second<<"}"<<" ";
        }
        cout<<nl;
    }

    return 0;
}
