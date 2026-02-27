#include <iostream>
#include "../include/tokenizer.h"
#include <fstream>
#include <vector>
using namespace std;

#define nl '\n'

int main() {
    ifstream file("../data/article1.txt");
    string s;
    
    vector<string> tokens;
    while(getline(file,s)){
        vector<string> token = tokenizer(s);
        for(auto x:token){
            tokens.push_back(x);
        }
    }

    for(auto x: tokens){
        cout<<x<<" ";
    }
    cout<<nl;
    
    

    return 0;
}