#include<string>
#include <unordered_map>
#include<chrono>
#include <iostream>

using namespace std;
#define nl '\n'

void benchmarks(
    const string &filename,
    size_t tokens,
    const unordered_map<string,int> &frequencies,
    chrono::duration<double,milli> tokenizer_time,
    chrono::duration<double,milli> frequency_time
){

    cout << "================ HASH TABLE BENCHMARK ================" << nl << nl;

    cout << "Input File        : " << filename << nl;
    cout << "Tokens Processed  : " << tokens << nl;
    cout << "Unique Words      : " << frequencies.size() << nl;

    cout << nl;
    cout << "--------------- Tokenization ----------------" << nl;
    cout << "Tokenizer Time    : " << tokenizer_time.count() << " ms" << nl;

    cout << nl;
    cout << "--------------- Frequency Build -------------" << nl;
    cout << "Frequency Time    : " << frequency_time.count() << " ms" << nl;

    cout << nl;
    cout << "--------------- Hash Table Stats ------------" << nl;
    cout << "Bucket Count      : " << frequencies.bucket_count() << nl;
    cout << "Load Factor       : " << frequencies.load_factor() << nl;
    cout << "Max Load Factor   : " << frequencies.max_load_factor() << nl;

    cout << nl;
    cout << "======================================================" << nl;
}
