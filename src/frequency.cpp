#include <iostream>
#include<unordered_map>
#include<vector>
#include<string>
using namespace std;

#define nl '\n'
using ll = long long;

unordered_map<string,int> frequency(vector<string> &tokens){
    unordered_map<string,int> fr;
    int n = (int)tokens.size();
    for(int i =0;i<n;i++){
        fr[tokens[i]]++;
    }

    return fr;
}

