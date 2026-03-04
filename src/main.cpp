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

/*-------------------------------------------------------------------------*/
int main() {

    const string filename = "../test/huge.txt";
    ifstream file(filename);
    string s = "";
    string line;
    while(getline(file,line)){
        s += line;
        s += nl;
    }






















    /* Bench Marks Counting */

    auto start = chrono::high_resolution_clock::now();
    vector<string> tokens = tokenizer(s);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> tokenizer_time = end - start;


    auto freqstart = chrono::high_resolution_clock::now();
    unordered_map<string,int> frequencies = frequency(tokens);
    auto freqend = chrono::high_resolution_clock::now();

    chrono::duration<double,milli> frequency_time = freqend - freqstart;


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

    return 0;
}
