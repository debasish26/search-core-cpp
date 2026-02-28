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

    ifstream file("../data/article1.txt");
    string s = "";
    string line;
    while(getline(file,line)){
        s += line;
        s += nl;
    }
    
    auto start = chrono::high_resolution_clock::now();
    vector<string> tokens = tokenizer(s);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed = end - start;

    
    // for(auto x: tokens){
    //     cout<<x<<" ";
    // }
    // cout<<nl;
    


    auto freqstart = chrono::high_resolution_clock::now();
    unordered_map<string,int>frequencies = frequency(tokens);
    auto freqend = chrono::high_resolution_clock::now();
    chrono::duration<double,milli>e = freqend - freqstart;

    for(auto i:frequencies){
        cout<<"["<<i.first<<" "<<i.second<<"]"<<" ";    
    }
    cout<<nl;



    cout<<"-- Bench Marks --"<<nl;
    cout<<nl;
    cout << "Time: " << elapsed.count() << " ms" << nl;
    cout << "Token count: " << tokens.size() << nl;

    cout<<nl;

    cout<<"Frequency Count: "<<e.count()<<" ms"<<nl;
    cout<<"Size of frequency vector: "<<frequencies.size()<<nl;
    cout<<nl;
    cout<<"-- ------------ --"<<nl;

    return 0;
}
