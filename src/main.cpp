#include <iostream>
#include "../include/tokenizer.h"
using namespace std;

#define nl '\n'

int main() {
    string s(1000000, 'a');
    vector<string> token = tokenizer(s);

    for(auto x:token){
        cout<<x<<" ";
    }
    cout<<nl;

    return 0;
}