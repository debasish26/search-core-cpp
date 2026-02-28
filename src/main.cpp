#include <iostream>
#include "../include/tokenizer.h"
#include <fstream>
#include <vector>
#include<chrono>
using namespace std;

#define nl '\n'

int main() {

    ifstream file("../data/2MB.txt");
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

    cout << "Time: " << elapsed.count() << " ms" << nl;
    cout << "Token count: " << tokens.size() << nl;

    return 0;
}